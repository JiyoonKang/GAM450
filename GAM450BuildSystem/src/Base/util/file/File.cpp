/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: File.cpp 
 * Author: Judy Cheng
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "File.h"
#include "util/Utilities.h"

#include <stdio.h>
#include <stdarg.h>

namespace Base
{
  File::File() : fp(nullptr) {}

  File::File(FILE* fileptr) : fp(fileptr) {}

  File::~File() 
  {
    Close();
  }

  void File::Open(const char* name, FileAccess::Type access)
  {
    DISABLE_WARNING(4996)
    switch (access)
    {
    case FileAccess::Read:
      fp = fopen(name, "r");
      break;
    case FileAccess::Write:
      fp = fopen(name, "w");
      break;
    case FileAccess::Append:
      fp = fopen(name, "a");
      break;
    }
    END_DISABLE()

    ErrorIf2(!Validate(), "FileIO", "File pointer is null!");
  }

  void File::Close()
  {
    if (fp)
    {
      fclose(fp);
    }
  }

  void File::Write(const char* format, ...)
  {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
  }

  void File::Read(const char* format, ...)
  {
    va_list args;
    va_start(args, format);
    vfscanf(fp, format, args);
    va_end(args);
  }

  void File::SeekFromBeginOfFile(int offset)
  {
    fseek(fp, offset, SEEK_SET);
  }

  void File::SeekFromEndOfFile(int offset)
  {
    fseek(fp, offset, SEEK_END);
  }

  void File::SeekByOffset(int offset)
  {
    fseek(fp, offset, SEEK_CUR);
  }

  bool File::Validate(void) const
  {
    return fp && !feof(fp) ? true : false;
  }

  std::string File::GetLine(char delim)
  {
    std::string temp;
    char c;

    for(;;)
    {
      c = static_cast<char>(fgetc(fp));

      if(feof( fp ))
        break;
      else if(c == delim)
        break;

      temp += c;
    }

    return std::move( temp );
  }

  std::string File::GetLine(const char* delims)
  {
    std::string temp;

    char c;

    for(;;)
    {
      c = static_cast<char>(fgetc(fp));

      if (feof( fp ))
        break;
      for (const char *temp_delim = delims; *temp_delim; ++temp_delim)
        if (c == *temp_delim)
          return std::move( temp );
      temp += c;
    }

    return std::move( temp );
  }

  bool File::FileExists(const char* name)
  {
    FILE* file;
    bool exists = !fopen_s(&file, name, "r");
    if (exists)
      fclose(file);
    return exists;
  }
}