project "Base"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "pch/precompiled.h"
	pchsource "pch/precompiled.cpp"

	files
	{
		-- local assignment files
		"**.h",
		"**.cpp",
	}

	includedirs
	{

  	"../Base",
		Core.includedirs
	}

	deps
	{
		--Core.deps

	}

	defines
  {
    --Core.defines,
    "BASE",
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
			"DEBUG"
			, "_DEBUG"
			, "MEMORY_DEBUGGING"
			,"_HAS_ITERATOR_DEBUGGING=1"
			, "_SECURE_SCL=1"
			, "_ITERATOR_DEBUG_LEVEL=2"
		}

		flags { "Symbols" }

	configuration "Release"
		defines {"NDEBUG", "RELEASE"}
		flags {"Optimize"}