/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Function.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "FuncSignature.h"
#include "Base/util/Utilities.h"
#include "Base\modules\introspection\variable\Variable.h"
#include "Base\util\Macros.h"

namespace Base
{
  //////////////////////////////////////////////////////////////////////////
  // STATIC FUNCTION WITH A RETURN VALUE
  class Variable;

  template <typename FuncType, FuncType FuncPtr, typename R>
  void Call(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 0, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (*FuncPtr)();
  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1>
  void Call(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 1, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (*FuncPtr)(args[0].GetValue<A1>());
  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2>
  void Call(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 2, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>());
  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2, typename A3>
  void Call(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 3, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>(),
      args[2].GetValue<A3>());
  }

  //////////////////////////////////////////////////////////////////////////
  // CALL STATIC FUNCTION WITHOUT A RETURN VALUE

  template <typename FuncType, FuncType FuncPtr>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 0, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)();
  }

  template <typename FuncType, FuncType FuncPtr, typename A1>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 1, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)(args[0].GetValue<A1>());
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 2, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>());
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 3, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>(), args[2].GetValue<A3>());
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3, typename A4>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 4, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>(), args[2].GetValue<A3>(), args[3].GetValue<A4>());
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3, typename A4, typename A5>
  void CallVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 5, "FunctionBinding", "Wrong overload!");
    (*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>(), args[2].GetValue<A3>(), args[3].GetValue<A4>(), args[4].GetValue<A5>());
  }

  //////////////////////////////////////////////////////////////////////////
  // FUNCTION WITH A RETURN VALUE

  template <typename FuncType, FuncType FuncPtr, typename R, typename C>
  void CallMethod(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 0, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (((C*)context)->*FuncPtr)();
  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename C, typename A1>
  void CallMethod(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 1, "FunctionBinding", "Wrong overload!");

    ret->GetValue<R>() = (((C*)context)->*FuncPtr)(args[0].GetValue<A1>());
  }

  //////////////////////////////////////////////////////////////////////////
  // FUNCTION WITH NO RETURN VALUE

  template <typename FuncType, FuncType FuncPtr, typename C>
  void CallMethodVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 0, "FunctionBinding", "Wrong overload!");

    (((C*)context)->*FuncPtr)();
  }

  template <typename FuncType, FuncType FuncPtr, typename C, typename A1>
  void CallMethodVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 1, "FunctionBinding", "Wrong overload!");

    (((C*)context)->*FuncPtr)(args[0].GetValue<A1>());
  }

  template <typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2>
  void CallMethodVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 2, "FunctionBinding", "Wrong overload!");

    (((C*)context)->*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>());
  }

  template <typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2, typename A3>
  void CallMethodVoid(Variable* ret, void* context, Variable* args, size_t argCount)
  {
    ErrorIf2(argCount != 3, "FunctionBinding", "Wrong overload!");

    (((C*)context)->*FuncPtr)(args[0].GetValue<A1>(), args[1].GetValue<A2>(), args[2].GetValue<A3>());
  }

  //////////////////////////////////////////////////////////////////////////

  class Function
  {
  public:
    Function();
    Function(const Function& rhs);
    Function& operator=(const Function& rhs);

    const FunctionSignature* Signature() const { return &m_sig; }

    // Binding 
    template<typename C>
    void Bind(C& c)
    {
      m_context = &c;
    }

    template<typename C>
    void Bind(C* c)
    {
      m_context = c;
    }

    void ExplicitBind(const TypeInfo* type, void* data)
    {
      m_context = Variable(type, data);
    }

    void ForceBind(void* data)
    {
      if (m_sig.GetContext())
        m_context = Variable(m_sig.GetContext(), data);
    }

    operator bool()
    {
      return m_callHelper != nullptr;
    }

    void Clear(void);

    // Get the context (owner) of the function
    inline Variable& Context() { return m_context; }
    const Variable& Context() const { return m_context; }
    // If it is a class function (has an owner) then it is a method
    bool IsMethod() const { return m_sig.GetContext() ? true : false; }

    //////////////////////////////////////////////////////////////////////////
    // STATIC FUNCTION WITH A RETURN VALUE

    template<typename R>
    Function(R(*fn)(void), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename R, typename A1>
    Function(R(*fn)(A1), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename R, typename A1, typename A2>
    Function(R(*fn)(A1, A2), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename R, typename A1, typename A2, typename A3>
    Function(R(*fn)(A1, A2, A3), void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // STATIC FUNCTION WITH NO RETURN VALUE

    Function(void(*fn)(void), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename A1>
    Function(void(*fn)(A1), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename A1, typename A2>
    Function(void(*fn)(A1, A2), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename A1, typename A2, typename A3>
    Function(void(*fn)(A1, A2, A3), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename A1, typename A2, typename A3, typename A4>
    Function(void(*fn)(A1, A2, A3, A4), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    Function(void(*fn)(A1, A2, A3, A4, A5), void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // CLASS METHODS WITH A RETURN VALUE, NON-CONST

    template<typename R, typename C>
    Function(R(C::*fn)(void), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename R, typename C, typename A1>
    Function(R(C::*fn)(A1), void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // CLASS METHODS WITH NO RETURN VALUE, NON-CONST

    template<typename C>
    Function(void (C::*fn)(void), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename C, typename A1>
    Function(void (C::*fn)(A1), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename C, typename A1, typename A2>
    Function(void (C::*fn)(A1, A2), void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename C, typename A1, typename A2, typename A3>
    Function(void (C::*fn)(A1, A2, A3), void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // CLASS METHOD WITH A RETURN VALUE, CONST

    template<typename R, typename C>
    Function(R(C::*fn)(void) const, void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename R, typename C, typename A1>
    Function(R(C::*fn)(A1) const, void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // CLASS METHOD WITH NO RETURN VALUE, CONST

    template<typename C>
    Function(void (C::*fn)(void) const, void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename C, typename A1>
    Function(void (C::*fn)(A1) const, void(*helper)(Variable*, void*, Variable*, size_t));

    template<typename C, typename A1, typename A2>
    Function(void (C::*fn)(A1, A2) const, void(*helper)(Variable*, void*, Variable*, size_t));

    //////////////////////////////////////////////////////////////////////////
    // OPERATORS

    void operator()(Variable& ret, Variable* args, size_t argCount) const;

    void operator()(Variable& ret, Variable* args, size_t argCount);

    void operator()(Variable& ret) const;

    template <typename A1>
    void operator()(Variable& ret, A1 arg1) const;

    template <typename A1, typename A2>
    void operator()(Variable& ret, A1 arg1, A2 arg2) const;

    template <typename A1, typename A2, typename A3>
    void operator()(Variable& ret, A1 arg1, A2 arg2, A3 arg3) const;

    template <typename A1, typename A2, typename A3, typename A4>
    void operator()(Variable& ret, A1 arg1, A2 arg2, A3 arg3, A4 arg4) const;

    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    void operator()(Variable& ret, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5) const;

    void operator()(void) const;

    template <typename A1>
    void operator()(A1 arg1) const;

    template <typename A1, typename A2>
    void operator()(A1 arg1, A2 arg2) const;

    template <typename A1, typename A2, typename A3>
    void operator()(A1 arg1, A2 arg2, A3 arg3) const;

    template <typename A1, typename A2, typename A3, typename A4>
    void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const;

    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5) const;

  private:
    Variable m_context;
    FunctionSignature m_sig;

    void(*m_callHelper)(Variable*, void*, Variable*, size_t);
  };

#define HP void (*helper)(Variable*, void*, Variable*, size_t)
#define PE :m_sig(fn), m_callHelper(helper) {}

  //////////////////////////////////////////////////////////////////////////
  // STATIC FUNCTIONS WITH A RETURN VALUE

  template<typename R>
  Function::Function(R(*fn)(void), HP) PE;

  template<typename R, typename A1>
  Function::Function(R(*fn)(A1), HP) PE;

  template<typename R, typename A1, typename A2>
  Function::Function(R(*fn)(A1, A2), HP) PE;

  template<typename R, typename A1, typename A2, typename A3>
  Function::Function(R(*fn)(A1, A2, A3), HP) PE;

  //////////////////////////////////////////////////////////////////////////
  // STATIC FUNCTIONS WITH NO RETURN VALUE

  template<typename A1>
  Function::Function(void(*fn)(A1), HP) PE;

  template<typename A1, typename A2>
  Function::Function(void(*fn)(A1, A2), HP) PE;

  template<typename A1, typename A2, typename A3>
  Function::Function(void(*fn)(A1, A2, A3), HP) PE;

  template<typename A1, typename A2, typename A3, typename A4>
  Function::Function(void(*fn)(A1, A2, A3, A4), HP) PE;

  template<typename A1, typename A2, typename A3, typename A4, typename A5>
  Function::Function(void(*fn)(A1, A2, A3, A4, A5), HP) PE;

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH RETURN VALUE, NON CONST

  template<typename R, typename C>
  Function::Function(R(C::*fn)(void), HP) PE;

  template<typename R, typename C, typename A1>
  Function::Function(R(C::*fn)(A1), HP) PE;

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITHOUT RETURN VALUE, NON CONST

  template<typename C>
  Function::Function(void (C::*fn)(void), HP) PE;

  template<typename C, typename A1>
  Function::Function(void (C::*fn)(A1), HP) PE;

  template<typename C, typename A1, typename A2>
  Function::Function(void (C::*fn)(A1, A2), HP) PE;

  template<typename C, typename A1, typename A2, typename A3>
  Function::Function(void (C::*fn)(A1, A2, A3), HP) PE;

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH RETURN VALUE, CONST

  template<typename R, typename C>
  Function::Function(R(C::*fn)(void) const, HP) PE;

  template<typename R, typename C, typename A1>
  Function::Function(R(C::*fn)(A1) const, HP) PE;

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITHOUT RETURN VALUE, CONST

  template<typename C>
  Function::Function(void (C::*fn)(void) const, HP) PE;

  template<typename C, typename A1>
  Function::Function(void (C::*fn)(A1) const, HP) PE;

  template<typename C, typename A1, typename A2>
  Function::Function(void (C::*fn)(A1, A2) const, HP) PE;

#undef HP
#undef PE

  // FUNCTION BUILDERS 

  //////////////////////////////////////////////////////////////////////////
  // STATIC FUNCTION WITH A RETURN VALUE

#define TS(...) template<typename FuncType, FuncType FuncPtr, ##__VA_ARGS__>
#define RV(...) Function(fn, &Call<FuncType, FuncPtr, ##__VA_ARGS__> )

  TS(typename R)
  Function BuildFunction(R(*fn)(void))
  {
    return RV(R);
  }

  TS(typename R, typename A1)
  Function BuildFunction(R(*fn)(A1))
  {
     return RV(R, A1);
  }

  TS(typename R, typename A1, typename A2)
  Function BuildFunction(R(*fn)(A1, A2))
  {
     return RV(R, A1, A2);
  }

  template<typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2, typename A3>
  Function BuildFunction(R(*fn)(A1, A2, A3))
  {
    return Function(fn, &Call<FuncType, FuncPtr, R, A1, A2, A3>);
  }

  //////////////////////////////////////////////////////////////////////////
  // STATIC FUNCTION WITH NO RETURN VALUE

#undef RV
#define RV(...) Function(fn, &CallVoid<FuncType, FuncPtr, ##__VA_ARGS__> )

  template<typename FuncType, FuncType FuncPtr>
  Function BuildFunction(void(*fn)(void))
  {
    return Function(fn, &CallVoid<FuncType, FuncPtr>);
  }

  TS(typename A1)
  Function BuildFunction(void(*fn)(A1))
  {
    return RV(A1);
  }

  TS(typename A1, typename A2)
  Function BuildFunction(void(*fn)(A1, A2))
  {
    return RV(A1, A2);
  }

  TS(typename A1, typename A2, typename A3)
  Function BuildFunction(void(*fn)(A1, A2, A3))
  {
    return RV(A1, A2, A3);
  }

  TS(typename A1, typename A2, typename A3, typename A4)
  Function BuildFunction(void(*fn)(A1, A2, A3, A4))
  {
    return RV(A1, A2, A3, A4);
  }

  TS(typename A1, typename A2, typename A3, typename A4, typename A5)
  Function BuildFunction(void(*fn)(A1, A2, A3, A4, A5))
  {
      return RV(A1, A2, A3, A4, A5);
  }

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH A RETURN VALUE, NON-CONST

#undef RV
#define RV(...) Function(fn, &CallMethod<FuncType, FuncPtr, ##__VA_ARGS__> )

  TS(typename R, typename C)
  Function BuildFunction(R(C::*fn)(void))
  {
    return RV(R, C);
  }

  TS(typename R, typename C, typename A1)
  Function BuildFunction(R(C::*fn)(A1))
  {
    return RV(R, C, A1);
  }

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH RETURN VALUE, CONST

  TS(typename R, typename C)
  Function BuildFunction(R(C::*fn)(void) const)
  {
    return RV(R, C);
  }

  TS(typename R, typename C, typename A1)
  Function BuildFunction(R(C::*fn)(A1) const)
  {
    return RV(R, C, A1);
  }

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH NO RETURN VALUE, NON-CONST

#undef RV
#define RV(...) Function(fn, &CallMethodVoid<FuncType, FuncPtr, ##__VA_ARGS__> )

  TS(typename C)
  Function BuildFunction(void (C::*fn)(void))
  {
    return RV(C);
  }

  template<typename FuncType, FuncType FuncPtr, typename C, typename A1>
  Function BuildFunction(void (C::*fn)(A1))
  {
    return Function(fn, &CallMethodVoid<FuncType, FuncPtr, C, A1>);
  }

  template<typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2>
  Function BuildFunction(void (C::*fn)(A1, A2))
  {
    return Function(fn, &CallMethodVoid<FuncType, FuncPtr, C, A1, A2>);
  }

  template<typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2, typename A3>
  Function BuildFunction(void (C::*fn)(A1, A2, A3))
  {
    return Function(fn, &CallMethodVoid<FuncType, FuncPtr, C, A1, A2, A3>);
  }

  //////////////////////////////////////////////////////////////////////////
  // CLASS METHODS WITH NO RETURN VALUE, CONST

#undef RV
#define RV(...) Function(fn, &CallMethodVoid<FuncType, FuncPtr, ##__VA_ARGS__> )

  TS(typename C)
  Function BuildFunction(void (C::*fn)(void) const)
  {
    return RV(C);
  }

  TS(typename C, typename A1)
  Function BuildFunction(void (C::*fn)(A1) const)
  {
    return RV(C, A1);
  }

  TS(typename C, typename A1, typename A2)
  Function BuildFunction(void (C::*fn)(A1, A2) const)
  {
     return RV(C, A1, A2);
  }

#undef TS
#undef RV

  // CALLING FUNCTIONS 

  //////////////////////////////////////////////////////////////////////////
  // FUNCTIONS WITH A RETURN VALUE

  template<typename A1>
  void Function::operator()(Variable& ret, A1 arg1) const
  {
    ErrorIf2(m_sig.ArgCount() != 1, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[1];

    new (argStack)Variable(arg1);

    m_callHelper(&ret, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2>
  void Function::operator()(Variable& ret, A1 arg1, A2 arg2) const
  {
    ErrorIf2(m_sig.ArgCount() != 1, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[2];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);

    m_callHelper(&ret, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2, typename A3>
  void Function::operator()(Variable& ret, A1 arg1, A2 arg2, A3 arg3) const
  {
    ErrorIf2(m_sig.ArgCount() != 1, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[3];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);
    new (argStack + 2) Variable(arg3);

    m_callHelper(&ret, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  //////////////////////////////////////////////////////////////////////////
  // FUNCTION WITH NO RETURN VALUE

  template<typename A1>
  void Function::operator()(A1 arg1) const
  {
    ErrorIf2(m_sig.ArgCount() != 1, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[1];

    new (argStack)Variable(arg1);

    m_callHelper(NULL, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2>
  void Function::operator()(A1 arg1, A2 arg2) const
  {
    ErrorIf2(m_sig.ArgCount() != 2, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");
    ErrorIf2(m_sig.GetArg(1) != GET_TYPE(A2), "Bound Function calling", "Argument type error!");

    Variable argStack[2];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);

    m_callHelper(NULL, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2, typename A3>
  void Function::operator()(A1 arg1, A2 arg2, A3 arg3) const
  {
    ErrorIf2(m_sig.ArgCount() != 3, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[3];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);
    new (argStack + 2) Variable(arg3);

    m_callHelper(NULL, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2, typename A3, typename A4>
  void Function::operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const
  {
    ErrorIf2(m_sig.ArgCount() != 4, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[4];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);
    new (argStack + 2) Variable(arg3);
    new (argStack + 3) Variable(arg4);

    m_callHelper(NULL, m_context.GetData(), argStack, m_sig.ArgCount());
  }

  template<typename A1, typename A2, typename A3, typename A4, typename A5>
  void Function::operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5) const
  {
    ErrorIf2(m_sig.ArgCount() != 5, "Bound Function calling", "Wrong Overload!");
    ErrorIf2(m_sig.GetArg(0) != GET_TYPE(A1), "Bound Function calling", "Argument type error!");

    Variable argStack[5];

    new (argStack)Variable(arg1);
    new (argStack + 1) Variable(arg2);
    new (argStack + 2) Variable(arg3);
    new (argStack + 3) Variable(arg4);
    new (argStack + 4) Variable(arg5);

    m_callHelper(NULL, m_context.GetData(), argStack, m_sig.ArgCount());
  }
}