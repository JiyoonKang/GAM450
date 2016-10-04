#pragma once

#include <string>
#include <sys/stat.h>

namespace Lua
{
    class FileInfo
    {
        public:
            FileInfo(std::string name) : m_FileInfoName(name)
            {
                // Grab the time that the file was last modified from
                // the system and store it to see if we need to 
                // reload the file later.
                struct _stat buf;
                _stat(name.c_str(), &buf);
                m_ModTime = buf.st_mtime;
            }

            ~FileInfo() {}

            virtual void Load() {}
            virtual void Reload() {}
            virtual void Unload() {}

            inline void SetLastModTime(time_t modTime) { m_ModTime = modTime; }
            inline time_t GetLastModTime() { return m_ModTime; }
            inline void SetFileInfoName(std::string& name) { m_FileInfoName = name; }
            inline std::string& GetFileInfoName() { return m_FileInfoName; }

        private:
            std::string m_FileInfoName;
            time_t m_ModTime;
    };
};