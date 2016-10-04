/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: File.h 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include <mbstring.h>
#include <string>

namespace Base
{
  namespace FileAccess
  {
    enum Type
    {
      Read,
      Write,
      Append
    };
  };

  struct File
  {
    File();
    File(FILE* file);
    ~File();

    void Open(const char* name, FileAccess::Type accessLevel);
    void Close(void);

    void Write(const char* format, ...);
    void Read(const char* format, ...);

    void SeekFromBeginOfFile(int offset);
    void SeekFromEndOfFile(int offset);
    void SeekByOffset(int offset);

    std::string GetLine(char delim);          // Retrieve one line delimited by delim
    std::string GetLine(const char* delims);  // Retrieve one line delimited by chars within delims

    bool Validate(void) const;
    static bool FileExists(const char* name);

    FILE* fp;
    char buffer[1024 * 32];
  };
}