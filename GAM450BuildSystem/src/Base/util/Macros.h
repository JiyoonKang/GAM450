/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Macros.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

#define STRINGIZE( _ ) #_

#define GET_ENUM(NAME) \
  (&Base::Enum##NAME)

#define GET_OFFSET_OF(TYPE, MEMBER) \
  ((unsigned)(&((((TYPE *)NULL))->MEMBER)))

// Special data types
#define TYPE_REGISTER(T) \
  Base::IntrospectionManager::Get()->RegisterType<Base::RemoveQualifiers<T>::type>(sizeof(T), #T, STRINGIZE(__##T##_MT), false)

// Plain old data types
#define TYPE_REGISTER_POD(T) \
  Base::IntrospectionManager::Get()->RegisterType<Base::RemoveQualifiers<T>::type>(sizeof(T), #T, STRINGIZE(__##T##_MT), true)

#define TYPE_REGISTER_PTR(T) \
  Base::IntrospectionManager::Get()->RegisterType<Base::RemoveQualifiers<T>::type>(sizeof(T), #T, GET_TYPE(RemovePtr<RemoveQualifiers<T>::type>::type)->LuaMetaTable(), true)

#define GET_TYPE(T) \
  Base::IntrospectionManager::Get()->GetType<Base::RemoveQualifiers<T>::type>()

#define GET_TYPE_OF(OBJECT) \
  Base::IntrospectionManager::Get()->GetType<Base::RemoveQualifiers<decltype(OBJECT)>::type>()

#define GET_TYPE_OF_MEMBER(T, MEMBER) \
  Base::RemoveQualifiers<Base::StripMemberness<decltype(&T::MEMBER)>::type>::type

#define GET_STR_TYPE(STR) \
  Base::IntrospectionManager::Get()->GetType(STR)

#define TYPE_ADD_MEMBER(T, MEMBER, ...) \
  ((Base::TypeInfo *)GET_TYPE(T))->AddMember(GET_TYPE(GET_TYPE_OF_MEMBER(T, MEMBER)), #MEMBER, GET_OFFSET_OF(T, MEMBER), __VA_ARGS__)

#define TYPE_SET_SERIALIZER(T, SERIALIZER) \
  ((Base::TypeInfo *)GET_TYPE(T))->SetSerializer(SERIALIZER)

#define TYPE_SET_DESERIALIZER(T, DESERIALIZER) \
  ((Base::TypeInfo *)GET_TYPE(T))->SetDeserializer(DESERIALIZER)

#define BUILD_FUNCTION(FN) \
  Base::BuildFunction<decltype(&FN), &FN>(&FN)

#define BIND_FUNCTION(L, FN) \
  static Base::Function FN##Function = BUILD_FUNCTION(FN); \
  Core::Lua::BindFunctionToLua(L, &FN, #FN##Function)

// Explicit bind
#define BIND_FUNCTION_EX(L, FN, NAME, ...) \
  static Base::Function NAME##Function = BUILD_FUNCTION(FN); \
  Core::Lua::BindFunctionToLua(L, &NAME##Function, #NAME, __VA_ARGS__)

#define BIND_FUNCTION_TABLE(L, FN, NAME, TABLE) \
  static Base::Function TABLE##NAME##Function = BUILD_FUNCTION(FN); \
  Core::Lua::BindFunctionToLua(L, &TABLE##NAME##Function, #NAME, #TABLE)

#define CREATE_TABLE(L, NAME) \
  Core::Lua::CreateNewGTable(L, #NAME)


#define TYPE_SET_TO_LUA(T, CB)\
  ((Base::TypeInfo*)GET_TYPE(T))->SetToLua(CB)

#define TYPE_SET_FROM_LUA(T, CB)\
  ((Base::TypeInfo*)GET_TYPE(T))->SetFromLua(CB)

// Add a pointer by an integral offset (bytes)
#define PTR_ADD(PTR, OFFSET) \
  ((char *)(PTR)+(OFFSET))

// Subtract a pointer by an integral offset (bytes)
#define PTR_SUB(PTR, OFFSET) \
  ((char *)(PTR)-(OFFSET))

// Use END_DISABLE in the end
#define DISABLE_WARNING(WARN_ID) \
  __pragma(warning(push)) \
  __pragma(warning(disable : WARN_ID))

// Disables a warning without the push call
#define PERM_DSIABLE_WARNING(WARN_ID) \
  __pragma(warning(disable : WARN_ID))

#define END_DISABLE() \
  __pragma(warning(pop))