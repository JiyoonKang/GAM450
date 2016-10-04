/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Qualifiers.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

namespace Base
{
  template <typename T>
  struct StripMemberness;

  template <typename T, typename U>
  struct StripMemberness<T U::*>
  {
    typedef T type;
  };

  template <typename T>
  struct RemoveQualifiers
  {
    typedef T type;
  };

  template <typename T>
  struct RemoveQualifiers<const T>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<T&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<const T&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<T&&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<const T*>
  {
    typedef typename RemoveQualifiers<T*>::type type;
  };

  template <typename T>
  struct RemovePtr
  {
    typedef T type;
  };

  template <typename T>
  struct RemovePtr<T*>
  {
    typedef typename RemovePtr<T>::type type;
  };
}