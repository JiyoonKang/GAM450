/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugAssist.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file   DebugAssist.cpp
\project Bring the Paint
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch/precompiled.h"
#include "DebugAssist.h"

#include <stdio.h>
#include <cstdarg>
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#define RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define WHITE FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY

#define SET_TEXT_COLOR(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x)

/******************************************************************************/
/*!
\brief
Handles any error signals. Sends out a message box that explains what the
error is. Also outputs the error to the debug command window.

\returns
returns a true boolean to signify that the error was handled correctly.
*/
/******************************************************************************/
bool SignalErrorHandler(const char* expression, const char* file, int line, const char* formatMessage, ...)
{
	const int bufferSize = 1024;
	char FinalMessage[bufferSize];

	char name[128];
	FileName(name, file);

	//MAKES THE ERROR TEXT PRINT OUT AS RED ON THE CONSOLE
	SET_TEXT_COLOR(RED);

	//PRINT THE ERROR TO THE COMMAND CONSOLE
	std::cout << "ERROR:" << std::endl;
	std::cout << "File: " << name << std::endl;
	std::cout << "Line: " << "(" << line << ") " << std::endl;

	if (formatMessage)
		std::cout << formatMessage << std::endl;

	else
		std::cout << "NO ERROR MESSAGE" << std::endl;

	//RESETS THE TEXT TO WHITE
	SET_TEXT_COLOR(WHITE);

	//FORMAT THE POP UP WINDOW MESSAGE
	int offset = sprintf_s(FinalMessage, "File: %s \n \nLine: %d \n \nError: ", name, line);
	if (formatMessage != NULL)
	{
		va_list args;
		va_start(args, formatMessage);
		vsnprintf_s(FinalMessage + offset, bufferSize - offset, _TRUNCATE, formatMessage, args);
		va_end(args);
	}
	else
	{
		strcpy_s(FinalMessage + offset, bufferSize - offset, "No error message.");
	}

	//CREATE THE POP UP WINDOW WITH THE SPECIFIED MESSAGE
	MessageBoxA(NULL, FinalMessage, "Error", 0);

	return true;
}

/******************************************************************************/
/*!
\brief
Handles any warning signals. Outputs the warning to the debug command window.

\returns
returns a true boolean to signify that the warning was handled correctly.
*/
/******************************************************************************/
bool SignalWarningHandler(const char* expression, const char* file, int line, const char* formatMessage, ...)
{
	//SET THE COLOR FOR WARNING TEXT TO YELLOW
	SET_TEXT_COLOR(YELLOW);

	std::cout << "WARNING: " << std::endl;

	char name[128];

	//Extract the file name from path
	FileName(name, file);

	//Print warning message
	std::cout << "File: " << name << std::endl;
	std::cout << "Line: " << "(" << line << ") " << std::endl;

	if (formatMessage)
		std::cout << formatMessage << std::endl;

	else
		std::cout << "NO WARNING MESSAGE" << std::endl;

	//RESET TEXT COLOR TO WHITE
	SET_TEXT_COLOR(WHITE);
	return true;
}

/******************************************************************************/
/*!
\brief
Prints out text to the debug window

\returns
void
*/
/******************************************************************************/
void PrintText(const char* msg, ...)
{
	const int bufferSize = 1024;
	char FinalMessage[bufferSize];

	if (msg)
	{
		va_list args;
		va_start(args, msg);
		vsnprintf_s(FinalMessage, bufferSize, _TRUNCATE, msg, args);
		va_end(args);
		std::cout << FinalMessage << std::endl;
	}

	else
		std::cout << "NO MESSAGE" << std::endl;
}

/******************************************************************************/
/*!
\brief
Extracts the file name and extension from a full path to a file

\param dest
Destination buffer for the file name

\param file
The complete path to the specified file

\returns
void
*/
/******************************************************************************/
void FileName(char * dest, const char* file)
{
	char fileName[123];
	char ext[5];
	_splitpath_s(file, NULL, 0, NULL, 0, fileName, 123, ext, 5);

	strcpy_s(dest, 128, fileName);
	strcat_s(dest, 128, ext);
}
