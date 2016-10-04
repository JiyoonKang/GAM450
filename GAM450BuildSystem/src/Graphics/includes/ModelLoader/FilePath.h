/******************************************************************************/
/*!
\file  FilePath.h
\project  CS460
\author Jorge Antonio Robles

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <string>

const unsigned FileMark = 'skye';
const unsigned MarkAnimation = 'anim';
const unsigned MarkMesh = 'mesh';
const unsigned MarkSkel = 'skel';
const unsigned MarkType = 'type';
const std::string BeanExt = ".skye";

class FilePath
{
	public:
	
		FilePath();
		FilePath(std::string file);
		FilePath& operator=(std::string file);
		void SetFilePath(std::string file);
		std::string GetFilePathWithNewExtension(std::string newExtension);
	
		//Extension of file which may be empty includes the period
		//such as ".png", ".txt"
		std::string Extension;
		//The root filename of the file without the extension or path
		//and in lower case For "C:\Data\FileName.txt" "filename"
		std::string FileName;
		//The path the file is locate at
		//For"C:\Data\FileName.txt" "C:\data\"
		std::string FullDirectory;
		//The full path including the filename
		//For "C:\Data\FileName.txt" "c:\data\filename.txt"
		std::string FullPath;

};