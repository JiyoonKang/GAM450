/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FileSystem.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/

#include <Windows.h>
#include <string>
#include <vector>

namespace Base
{
	namespace FileSystem
	{
		std::vector<std::string> GetFilesNameFromDirectory(std::string directory);

		std::string OpenFile(char* filter = "All Files (*.*)\0*.*\0", HWND owner = nullptr);

		std::string SaveFile(char* filter = "All Files (*.*)\0*.*\0", HWND owner = nullptr);

		std::string GetFilenameFromPath(std::string filepath);

		std::string RemoveFileExtension(const std::string& file);

		std::string RemoveFileExtensionPath(const std::string& file);

		std::string GetFileExtension(const std::string& file);
	}
}