include "../../premake/extensions.lua"

workspace "game"
	location "../../"
	startproject "game"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

include "../../premake/common.lua"
include (dirs.engine)
include (dirs.external)
-------------------------------------------------------------
project "game"
	location (dirs.localfiles)
	dependson { "engine", "external" }
		
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	debugdir ("%{dirs.bin}")
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}")
	enableunitybuild "On"

	links {
		"kernel32",
		"user32",
		"gdi32",
		"winspool",
		"comdlg32",
		"advapi32",
		"shell32",
		"ole32",
		"oleaut32",
		"uuid",
		"odbc32",
		"odbccp32"
	}

	includedirs { 
		dirs.engine,
		dirs.external,
		dirs.game
	}

	files {
		"**.h",
		"**.cpp",
		"**.hpp",
		"../external/**.h",
		"../external/**.cpp",
	}

	libdirs { 
		dirs.dependencies
	}
	
	defines {
		"_CONSOLE"
	}

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		links {
			"engine_Debug"
		}

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		links {
			"engine_Release"
		}

	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

		links {
			"engine_Retail"
		}

	filter "system:windows"
		kind "WindowedApp" 
		-- entrypoint "wWinMainCRTStartup" 
		symbols "On"		
		systemversion "latest"
		warnings "Extra"

		fatalwarnings { "All" }
		flags { 
			"MultiProcessorCompile"
		}