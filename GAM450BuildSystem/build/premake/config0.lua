-- 
-- Keeps the build settings. Each developer should keep a local modified
-- version of this file so there are no conflicts between the different
-- developer build configurations.
--

config = {}

Core = {}
Core.libdirs = {}
Core.deps = {}

-- global config
Core.inlcudedirs = {}
Core.defines = {}
Core.links = {}

-- msw configs
Core.msvcincludedirs = {}
Core.msvcdefines = {}
Core.msvclinks = {}


Core.msvcincludedirs = 
{

}

Core.msvcdefines = 
{
  "_CRT_SECURE_NO_WARNINGS",
  "_CRT_SECURE_NO_DEPRECATE",
  "_SCL_SECURE_NO_WARNINGS",
  "WIN32",
}

Core.msvclinks = 
{ 
  "kernel32",
  "user32",
  "gdi32",
  "comdlg32",
  "winspool",
  "winmm",
  "shell32",
  "comctl32",
  "ole32",
  "uuid",
  "rpcrt4",
  "advapi32",
}

Core.includedirs = 
{
  -- deps folders
  depsdir .. "/boost_1_58",
  depsdir .. "/fmod/include",
  depsdir .. "/lua52",
  depsdir .. "/xinput",
  depsdir .. "/LuaBridge",
  depsdir .. "/imgui",
  depsdir .. "/Awesomium/include",
  depsdir,
  srcdir
}

Core.deps = 
{
  "d3d11",
  "d3dcompiler",
}

Core.libdirs = 
{
  --"$(DXSDK_DIR)/lib/x86"
    depsdir .. "/fmod/lib",
    depsdir .. "/boost_1_58/lib",
	depsdir .. "/Awesomium/lib",
}

Core.defines = 
{
  "WIN32"
}