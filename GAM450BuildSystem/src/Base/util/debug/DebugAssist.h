/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugAssist.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   DebugAssist.h
\project Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once


#	if defined(_DEBUG) 
#		define DEBUG_ASSIST_ENABLED 1
#	else
#		define DEBUG_ASSIST_ENABLED 0
#	endif
#include <cstdio>


bool SignalErrorHandler(const char* expression, const char* file, int line, const char* formatMessage = 0, ...);
void PrintText(const char* msg, ...);
bool SignalWarningHandler(const char* expression, const char* file, int line, const char* formatMessage = 0, ...);
void FileName(char* dest, const char* file);


#if DEBUG_ASSIST_ENABLED


#ifndef ErrorIf
#define ErrorIf(exp, ...) \
{																	\
	if(exp)															\
		SignalErrorHandler(#exp, __FILE__, __LINE__, __VA_ARGS__);	\
}
#endif

#ifndef WarningIf
#define WarningIf(exp, ...) \
{ \
	if(exp) \
	SignalWarningHandler(#exp, __FILE__, __LINE__, __VA_ARGS__); \
}
#endif

#ifndef PrintOut
#define PrintOut(...) \
{ \
	PrintText(__VA_ARGS__); \
}
#endif

#ifndef Warning
#define Warning(...) \
{ \
	SignalWarningHandler(0, __FILE__, __LINE__, __VA_ARGS__); \
}
#endif

#ifndef DebugError
#define DebugError(...) \
{ \
	SignalErrorHandler(0, __FILE__, __LINE__, __VA_ARGS__); \
}
#endif


#ifndef ErrorHandler
#define ErrorHandler(exp, ... )									\
{																\
	HRESULT hr = (exp);											\
	if(FAILED(hr))												\
	SignalErrorHandler(#exp, __FILE__, __LINE__, __VA_ARGS__);	\
} 


#endif // !ErrorHandler



#else

#ifndef ErrorIf
#define ErrorIf(...) ((void) 0)
#endif

#ifndef WarningIf
#define WarningIf(...) ((void) 0)
#endif

#ifndef PrintOut
#define PrintOut(...) ((void) 0)
#endif

#ifndef Warning
#define Warning(...) ((void) 0)
#endif

#ifndef DebugError
#define DebugError(...) ((void) 0)
#endif

#ifndef ErrorHandler
#define ErrorHandler(exp, ... ) (exp)
#endif // !ErrorHandler

#endif

#ifndef SafeRelease
#define SafeRelease(x) { if(x) {x->Release(); x = 0;} }
#endif

#ifndef SafeDelete
#define SafeDelete(x) { delete x; x = 0; }
#endif

#ifndef SafeDeleteArr
#define SafeDeleteArr(x) { delete [] x; x = 0; }
#endif