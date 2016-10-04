project "imgui"

	SetupNativeDependencyProject()
		
	local version = "5.2"
	local repo = ""
	local license = ""
	
  kind "StaticLib"
  language "C++"
  files { "*.cpp" }
  includedirs {  }

  excludes
  {
  }

  configuration "Debug"
  	flags { "Symbols" }
    defines
    {
      "_DEBUG"
      , "MEMORY_DEBUGGING"
      ,"_HAS_ITERATOR_DEBUGGING=1"
      , "_SECURE_SCL=1"
      , "_ITERATOR_DEBUG_LEVEL=2"
    }

  configuration "Release"
  	flags { "Optimize" }