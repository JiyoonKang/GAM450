/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FuncSignature.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

namespace Base
{
  class TypeInfo;

  class FunctionSignature
  {
  public:
    FunctionSignature();
    FunctionSignature(const FunctionSignature& rhs);
    FunctionSignature& operator=(const FunctionSignature& rhs);

    inline unsigned int ArgCount() const { return m_argCount; }
    inline const TypeInfo* GetRet() const { return m_ret; }
    inline const TypeInfo* GetArg(unsigned int index) const { return m_args[index]; }
    inline const TypeInfo* GetContext() const { return m_context; }

    //////////////////////////////////////////////////////////////////////////
    // STATIC FUNCTIONS WITH A RETURN VALUE

    // No arguments
    template<typename R>
    FunctionSignature(R(*fn)(void)) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(NULL), m_args(NULL) {}

    // One argument
    template<typename R, typename A1>
    FunctionSignature(R(*fn)(A1)) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    
    template<typename R, typename A1, typename A2>
    FunctionSignature(R(*fn)(A1, A2)) : m_ret(GET_TYPE(R)), m_argCount(2), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2) };
      m_args = args;
    }

    // Three arguments
    template<typename R, typename A1, typename A2, typename A3>
    FunctionSignature(R(*fn)(A1, A2, A3)) : m_ret(GET_TYPE(R)), m_argCount(3), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3) };
      m_args = args;
    }

    //////////////////////////////////////////////////////////////////////////
    // STATIC FUNCTIONS WITH NO RETURN VALUE-

    FunctionSignature(void(*)(void));

    // One argument
    template<typename A1>
    FunctionSignature(void(*fn)(A1)) : m_ret(NULL), m_argCount(1), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    // Two arguments
    template<typename A1, typename A2>
    FunctionSignature(void(*fn)(A1, A2)) : m_ret(NULL), m_argCount(2), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2) };
      m_args = args;
    }

    // Three arguments
    template<typename A1, typename A2, typename A3>
    FunctionSignature(void(*fn)(A1, A2, A3)) : m_ret(NULL), m_argCount(3), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3) };
      m_args = args;
    }

    // Four arguments
    template<typename A1, typename A2, typename A3, typename A4>
    FunctionSignature(void(*fn)(A1, A2, A3, A4)) : m_ret(NULL), m_argCount(4), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3), GET_TYPE(A4) };
      m_args = args;
    }

    // Five arguments
    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    FunctionSignature(void(*fn)(A1, A2, A3, A4, A5)) : m_ret(NULL), m_argCount(5), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3), GET_TYPE(A4), GET_TYPE(A5) };
      m_args = args;
    }

    //////////////////////////////////////////////////////////////////////////
    // FUNCTIONS WITH A RETURN VALUE, NON-CONST

    template <typename C, typename R>
    FunctionSignature(R(C::*)(void)) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    // One argument
    template <typename C, typename R, typename A1>
    FunctionSignature(R(C::*)(A1)) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    //////////////////////////////////////////////////////////////////////////
    // FUNCTIONS WITH NO RETURN VALUE, NON-CONST

    template <typename C>
    FunctionSignature(void (C::*)(void)) : m_ret(NULL), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    // One argument
    template <typename C, typename A1>
    FunctionSignature(void (C::*)(A1)) : m_ret(NULL), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    // Two arguments
    template <typename C, typename A1, typename A2>
    FunctionSignature(void (C::*)(A1, A2)) : m_ret(NULL), m_argCount(2), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2) };
      m_args = args;
    }

    // Three arguments
    template <typename C, typename A1, typename A2, typename A3>
    FunctionSignature(void (C::*)(A1, A2, A3)) : m_ret(NULL), m_argCount(3), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3) };
      m_args = args;
    }

    //////////////////////////////////////////////////////////////////////////
    // FUNCTION WITH A RETURN VALUE, CONST

    template <typename C, typename R>
    FunctionSignature(R(C::*)(void) const) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    // One argument
    template <typename C, typename R, typename A1>
    FunctionSignature(R(C::*)(A1) const) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    //////////////////////////////////////////////////////////////////////////
    // FUNCTION WITH NO RETURN VALUE, CONST

    template <typename C>
    FunctionSignature(void (C::*)(void) const) : m_ret(NULL), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    template <typename C, typename A1>
    FunctionSignature(void (C::*)(A1) const) : m_ret(NULL), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

  private:
    const TypeInfo* m_ret;
    const TypeInfo** m_args;
    const TypeInfo* m_context;

    unsigned int m_argCount;
  };
}