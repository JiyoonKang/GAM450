project "Audio"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "pch/precompiled.h"
	pchsource "pch/precompiled.cpp"

	files
	{
		-- local assignment files
		"**.h",
		"**.cpp",
		"**.lua"
	}

	includedirs
	{
		"../Audio",
		"../Base",
		"../Core",
		Core.includedirs,
	}

	deps
	{
  		"Base",
  		"Core",
  		"fmodL_vc",
  		"fmodstudioL_vc",
		Core.deps
	}

	defines
	{
		-- Preproccesor defines
		"AUDIO",
	}

	libdirs
	{
		-- more directories to look for lib files
		depsdir .. "/fmod/lib",
		Core.libdirs,
	}

  postbuildcommands 
  {
  	"xcopy $(SolutionDir)..\\..\\deps\\fmod\\dll\\\*.dll $(OutDir) /f /y"
    -- Command line to be run after building (you can use macros)
  }

	configuration "windows"
		includedirs( Core.msvcincludedirs )
		defines( Core.msvcdefines )
		links( Core.msvclinks )

	configuration "Debug"
		defines
		{
			"DEBUG",
			"_DEBUG",
			"MEMORY_DEBUGGING",
			"_HAS_ITERATOR_DEBUGGING=1",
			"_SECURE_SCL=1",
			"_ITERATOR_DEBUG_LEVEL=2"
		}

		flags { "Symbols" }
    libdirs
    {
      Core.libdirs
    }

	configuration "Release"
		defines {"NDEBUG", "RELEASE"}
		flags {"Optimize"}
    libdirs
    {
      Core.libdirs
    }