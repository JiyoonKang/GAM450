/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FuncSignature.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "FuncSignature.h"

namespace Base
{
  FunctionSignature::FunctionSignature() : m_ret(NULL), m_args(NULL), m_argCount(0) {}

  FunctionSignature::FunctionSignature(const FunctionSignature& rhs)
    : m_ret(rhs.m_ret), m_args(rhs.m_args), m_argCount(rhs.m_argCount) {}

  FunctionSignature::FunctionSignature(void(*)(void)) 
    : m_ret(NULL), m_args(NULL), m_argCount(0), m_context(NULL) {}

  FunctionSignature& FunctionSignature::operator=(const FunctionSignature& rhs)
  {
    if (this != &rhs)
    {
      m_ret = rhs.m_ret;
      m_args = rhs.m_args;
      m_argCount = rhs.m_argCount;
      m_context = rhs.m_context;
    }
    return *this;
  }
}