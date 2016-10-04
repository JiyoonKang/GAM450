/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugOutput.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
// author: Jacques Newman
// Date Created: 5/13/2015
// All content © 2015 DigiPen (USA) Corporation, all rights reserved.

#include "pch/precompiled.h"
#include "DebugOutput.h"
#include <windows.h>

void DebugOutput::MessageBoxOk(const std::string& body, const std::string& title)
{
    MessageBox(nullptr, body.c_str(), title.c_str(), MB_OK);
}

int DebugOutput::MessageBoxOkCancel(const std::string& question, const std::string& title)
{
    return (MessageBox(nullptr, question.c_str(), title.c_str(), MB_OKCANCEL));
}

void DebugOutput::Print(const std::string& str)
{
    //OutputDebugString(str.c_str());
}

void DebugOutput::PrintLn(const std::string& str)
{
    Print(str + "\n");
}