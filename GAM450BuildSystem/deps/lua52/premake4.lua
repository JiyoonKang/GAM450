project "lua52"

	SetupNativeDependencyProject()

	local version = "5.2"
	local repo = ""
	local license = ""

  kind "StaticLib"
  language "C"
  files { "src/*.c",
            "src/*.link" }
  includedirs { "include" }
  --linkoptions { "lj_vm.link" }

  configuration "Debug"
  	flags { "Symbols" }

  configuration "Release"
  	flags { "Optimize" }

