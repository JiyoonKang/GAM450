project "Lua"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "pch/precompiled.h"
	pchsource "pch/precompiled.cpp"

	files
	{
		-- local assignment files
		"*.lua",
		"**.h",
		"**.cpp",
    "*.link"
	}

	includedirs
	{
		"../Lua",
		"../Base",
		"../Core",
		Core.includedirs
	}

	deps
	{

	}

	defines
	{
		Core.defines,
		-- Defines
	}

	libdirs
	{
		Core.libdirs,
		-- Libs
	}

  postbuildcommands 
  {
  }

	configuration "windows"
		includedirs( Core.msvcincludedirs );
		defines( Core.msvcdefines );
		links( Core.msvclinks );

	configuration "Debug"
		defines
		{
			"_DEBUG"
			, "MEMORY_DEBUGGING"
			,"_HAS_ITERATOR_DEBUGGING=1"
			, "_SECURE_SCL=1"
			, "_ITERATOR_DEBUG_LEVEL=2"
		}

		flags { "Symbols" }

	configuration "Release"
		defines {"NDEBUG", "RELEASE"}
		flags {"Optimize"}