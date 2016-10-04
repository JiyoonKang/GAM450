/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Utilities.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once

void OpenConsole();
void PrintError(const char *exp, const char *file, int line, const char *msg, ...);

#define DEBUG_BREAK() \
  __debugbreak()

#ifdef _DEBUG

#define FORCEERROR(MODULE, FORMAT, ...) \
  DISABLE_WARNING(4127); \
  do { \
  PrintError( "Forced Error", __FILE__, __LINE__, FORMAT, __VA_ARGS__ ); \
  _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, MODULE, "Forced Error\n"##FORMAT, __VA_ARGS__); \
  _CrtDbgBreak(); \
  } while(0) \
  END_DISABLE();

#define ErrorIf2( EXP, MODULE, FORMAT, ... ) \
  DISABLE_WARNING(4127); \
  do { \
    if (EXP) \
    { \
      PrintError(#EXP, __FILE__, __LINE__, FORMAT, __VA_ARGS__); \
      _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, MODULE, #EXP##"\n"##FORMAT, __VA_ARGS__); \
      _CrtDbgBreak(); \
    } \
  } while(0) \
  END_DISABLE();

#else
#define ErrorIf2(EXP, MODULE, FORMAT, ...) \
  DISABLE_WARNING(4127); \
  do { \
    if (EXP) \
    { \
      PrintError(#EXP, __FILE__, __LINE__, FORMAT, __VA_ARGS__); \
      _CrtDbgBreak(); \
    } \
  } while (0) \
  END_DISABLE();
#endif
