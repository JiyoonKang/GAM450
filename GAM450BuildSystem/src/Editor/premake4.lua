project "Editor"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "pch/precompiled.h"
	pchsource "pch/precompiled.cpp"

	files
	{
		-- local assignment files
		"*.lua", 
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		"../Editor",
		"$(DXSDK_DIR)Include",
		Core.includedirs
	}

	deps
	{
		"Base",
		"Core",
		"Lua",
		"Audio",
		"Graphics",
		"Physics",
		"GameLogic",
		"lua",
		"dxtk",
		"fmodL_vc",
		"fmodStudioL_vc",
		"fw1fontwrapper",
		Core.deps
	}

	defines
	{
		Core.defines,
		-- Defines
	}

	libdirs
	{
		depsdir .. "/fw1fontwrapper/lib",
		"$(DXSDK_DIR)/Lib/x86",
		Core.libdirs,
		-- Libs
	}

  postbuildcommands 
  {
    "xcopy $(SolutionDir)..\\..\\deps\\fmod\\dll\\\*.dll $(OutDir) /f /y",
    "xcopy $(SolutionDir)..\\..\\deps\\fw1fontwrapper\\dll\\\*.dll $(OutDir) /f /y"
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