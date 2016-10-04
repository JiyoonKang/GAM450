/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Function.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

namespace Base
{
  Function::Function() :m_sig(), m_callHelper(nullptr), m_context() {}

  Function::Function(const Function& rhs) 
    : m_sig(rhs.m_sig), m_callHelper(rhs.m_callHelper), m_context() {}

  Function::Function(void(*fn)(void), void(*helper)(Variable*, void*, Variable*, size_t))
    : m_sig(fn), m_callHelper(helper) {}

  void Function::Clear()
  {
    m_callHelper = nullptr;
    m_sig = FunctionSignature();
    m_context = Variable();
  }

  Function& Function::operator=(const Function& rhs)
  {
    if (this != &rhs)
    {
      m_sig = rhs.m_sig;
      m_callHelper = rhs.m_callHelper;
    }
    return *this;
  }

  void Function::operator()(Variable& ret) const
  {
    m_callHelper(&ret, m_context.GetData(), nullptr, 0);
  }

  void Function::operator()() const
  {
    m_callHelper(nullptr, m_context.GetData(), nullptr, 0);
  }

  void Function::operator()(Variable& ret, Variable* args, size_t argCount) const
  {
    m_callHelper(&ret, m_context.GetData(), args, argCount);
  }

  void Function::operator()(Variable& ret, Variable* args, size_t argCount)
  {
    m_callHelper(&ret, m_context.GetData(), args, argCount);
  }
}