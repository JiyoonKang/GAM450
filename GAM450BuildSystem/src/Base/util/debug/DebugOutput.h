/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: DebugOutput.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
// author: Jacques Newman
// Date Created: 5/13/2015
// All content © 2015 DigiPen (USA) Corporation, all rights reserved.
#pragma once
#include <string>

namespace DebugOutput
{
    void MessageBoxOk(const std::string & body, const std::string & title = "The Paint");
    int MessageBoxOkCancel(const std::string & question, const std::string & title = "The Paint");
    void Print(const std::string & str);
    void PrintLn(const std::string & str);
};

