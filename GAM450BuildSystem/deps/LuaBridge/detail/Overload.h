#ifndef LUA_OVERLOAD_H
#define LUA_OVERLOAD_H

namespace detail
{
    template<class T> struct remove_all_impl { typedef T type; };
    template<class T> using remove_all = remove_all_impl<typename std::remove_cv<T>::type>;
    template<class T> struct remove_all_impl<T*> : remove_all<T>{};
    template<class T> struct remove_all_impl<T&> : remove_all<T>{};
    template<class T> struct remove_all_impl<T&&> : remove_all<T>{};
    template<class T> struct remove_all_impl<T[]> : remove_all<T>{};
    template<class T, int n> struct remove_all_impl<T[n]> : remove_all<T>{};

    template<typename T>
    struct CheckStack
    {
        static bool Check(lua_State *L, int index)
        {
            bool ret = Userdata::IsClass(L, index, ClassInfo<T>::getClassKey(), true) == 1;
            return ret;
        }
    };

    template<>
    struct CheckStack<bool>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isboolean(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<int>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isnumber(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<double>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isnumber(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<float>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isnumber(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<unsigned>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isnumber(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<const char *>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isstring(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<char>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isstring(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<char *>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isstring(L, index) == 1;
        }
    };

    template<>
    struct CheckStack<std::string>
    {
        static bool Check(lua_State *L, int index)
        {
            return lua_isstring(L, index) == 1;
        }
    };

    template<typename... Args>
    struct storedArgs{};

    template<std::size_t... indices>
    struct _indices{};

    template<std::size_t N, std::size_t... indices>
    struct _indicesBuilder : _indicesBuilder < N - 1, N - 1, indices... > {};

    template<std::size_t... indices>
    struct _indicesBuilder < 0, indices... >
    {
        using type = _indices <indices... >;
    };

    template<typename... Args, std::size_t... N>
    bool CheckArgs(lua_State *L, detail::storedArgs<Args...>,
        detail::_indices<N...>, bool isClass = true)
    {
        return AllTrue::Collect(detail::CheckStack<
                detail::remove_all<Args>::type>::Check(L, N + 1 + (int)(isClass))...);
    }
}

template<typename T, int id, bool isClass = true>
struct ChainCall
{
    static std::vector<int(*)(lua_State*)> &Get()
    {
        static std::vector<int(*)(lua_State*)> functions;
        return functions;
    }

    static void push(int(*func)(lua_State*))
    {
        Get().push_back(func);
    }

    static int call(lua_State *L)
    {
        for (auto function : Get())
        {
            int ret = function(L);
            if (ret >= 0)
                return ret;
        }

        luaL_error(L, "No overloaded function found!");

        return -1;
    }
};

struct AllTrue
{
    static bool Collect()
    {
        return true;
    }

    template<typename Arg, typename... Args>
    static bool Collect(Arg a, Args... args)
    {
        return a && Collect(args...);
    }
};

template<class T, int id, typename MemFn, typename... Args>
struct OverloadedMethodCall
{
    static MemFn& Get()
    {
        static MemFn fnPtr;

        return fnPtr;
    }

    static int call(lua_State *L)
    {
        int size = sizeof...(Args);

        // Check the number of arguments, break out if it is different from what's
        // expected
        if (lua_gettop(L) - 1 != size)
        {
            return -1;
        }

        // Call Function to check each argument (detail is the luabridge::detail namespace in FuncTraits.h)
        bool ret = CheckArgs(L, detail::storedArgs<Args...>(), 
            typename detail::_indicesBuilder<sizeof...(Args)>::type());

        // We are here now

        if (ret == false)
            return -1;

        //Grab the function to call the member function
        auto func = CFunc::CallMemberFunctionHelper<MemFn, 
            FuncTraits<MemFn>::isConstMemberFunction>::getFunc();

        // Re-assign upvalue to the proper function
        new (lua_newuserdata(L, sizeof(MemFn))) MemFn(Get());
        lua_replace(L, lua_upvalueindex(1));

        //Call the function
        return func(L);
    }
};

template<int id, typename FP, typename... Args>
struct OverloadedFunctionCall
{
    static FP& Get()
    {
        static FP fnPtr;

        return fnPtr;
    }

    static int call(lua_State *L)
    {
        int size = sizeof...(Args);

        // Check the number of arguments, break out if it is different from what's
        // expected
        if (lua_gettop(L) != size)
        {
            return -1;
        }

        // Call Function to check each argument (detail is the luabridge::detail namespace in FuncTraits.h)
        bool ret = CheckArgs(L, detail::storedArgs<Args...>(),
            typename detail::_indicesBuilder<sizeof...(Args)>::type(), false);

        // We are here now

        if (ret == false)
            return -1;

        //Grab the function to call the member function
        auto func = CFunc::Call<FP>::f;

        // Re-assign upvalue to the proper function
        new (lua_newuserdata(L, sizeof(FP))) FP(Get());
        lua_replace(L, lua_upvalueindex(1));

        //Call the function
        return func(L);
    }
};

#endif