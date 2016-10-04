project "SkyeAwesomium"

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
		"../SkyeAwesomium",
		Core.includedirs
	}

	deps
	{
		"Base",
		"Graphics",
		
	}

	defines
	{
		Core.defines,
		-- Defines
	}

	libdirs
	{
		depsdir .. "Awesomium/lib",
		Core.libdirs,
		-- Libs
	}

  postbuildcommands 
  {
		"xcopy $(SolutionDir)..\\..\\deps\\Awesomium\\bin\\\*.dll $(OutDir) /f /y",
		"xcopy $(SolutionDir)..\\..\\deps\\Awesomium\\bin\\\*.exe $(OutDir) /f /y",
		"xcopy $(SolutionDir)..\\..\\deps\\Awesomium\\bin\\\*.pdb $(OutDir) /f /y",
		"xcopy $(SolutionDir)..\\..\\deps\\Awesomium\\bin\\\*.pak $(OutDir) /f /y",
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