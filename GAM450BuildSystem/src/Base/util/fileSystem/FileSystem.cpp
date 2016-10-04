/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FileSystem.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/

#include "pch/precompiled.h"
#include "FileSystem.h"
#include <commdlg.h>

namespace Base
{
	namespace FileSystem
	{
		std::vector<std::string> GetFilesNameFromDirectory(std::string directory)
		{
			std::vector<std::string> filenamelist;
			WIN32_FIND_DATA fileData;
			HANDLE position;

			if (directory.back() != '\\' && directory.back() != '/')
				directory.append("/");

			// Get first file in directory and check if successful
			position = FindFirstFile((directory + "*").c_str(), &fileData);
			if (position == INVALID_HANDLE_VALUE)
				return filenamelist;

			do
			{
				// Skip if file is just a directory symbol
				std::string name(fileData.cFileName);
				if (name == "." || name == "..")
					continue;

				// Make sure to add directory back to name
				name = directory + name;
				filenamelist.push_back(name);
			} while (FindNextFile(position, &fileData) != 0);

			FindClose(position);
			return filenamelist;
		}

		std::string OpenFile(char* filter, HWND owner)
		{
			OPENFILENAME of;
			char fileName[MAX_PATH] = "";
			ZeroMemory(&of, sizeof(of));

			char workingDirectory[MAX_PATH] = "";
			GetCurrentDirectory(MAX_PATH, workingDirectory);

			of.lStructSize = sizeof(OPENFILENAME);
			of.hwndOwner = owner;
			of.lpstrFilter = filter;
			of.lpstrFile = fileName;
			of.nMaxFile = MAX_PATH;
			of.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

			std::string fileNameStr;

			if (GetOpenFileName(&of))
				fileNameStr = fileName;
			else
				fileNameStr = "*InvalidName*";

			SetCurrentDirectory(workingDirectory);

			return fileNameStr;
		}

		std::string SaveFile(char* filter, HWND owner)
		{
			OPENFILENAME sf;
			char fileName[MAX_PATH] = "";
			ZeroMemory(&sf, sizeof(sf));

			char workingDirectory[MAX_PATH] = "";
			GetCurrentDirectory(MAX_PATH, workingDirectory);

			sf.lStructSize = sizeof(OPENFILENAME);
			sf.hwndOwner = owner;
			sf.lpstrFilter = filter;
			sf.lpstrFile = fileName;
			sf.nMaxFile = MAX_PATH;
			sf.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
			sf.lpstrDefExt = "";

			std::string fileNameStr;

			if (GetSaveFileName(&sf))
				fileNameStr = fileName;

			SetCurrentDirectory(workingDirectory);

			return fileNameStr;
		}

		std::string GetFilenameFromPath(std::string filepath)
		{
			return filepath.substr(filepath.find_last_of("/\\") + 1);
		}

		std::string RemoveFileExtension(const std::string& file)
		{
			std::string filename = file;
			unsigned int index = filename.rfind(".");
			if (index != std::string::npos)
				filename.erase(filename.rfind("."));
			return filename;
		}

		std::string RemoveFileExtensionPath(const std::string& file)
		{
			return RemoveFileExtension(GetFilenameFromPath(file));
		}

		std::string GetFileExtension(const std::string& file)
		{
			std::string extension = file;
			unsigned int index = extension.rfind(".");

			if (index != std::string::npos)
				extension.erase(0, index + 1);

			return extension;
		}
	}
}