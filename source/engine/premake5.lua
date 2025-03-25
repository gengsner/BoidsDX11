include "../../premake/common.lua"

project "engine"
location (dirs.localfiles)

    language "C++"
	cppdialect "C++17"

	targetdir (dirs.dependencies)
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}")
	
	enableunitybuild "On"

	pchheader "stdafx.h"
	pchsource "stdafx.cpp"
	forceincludes  { "stdafx.h" }

	files {
		"**.h",
		"**.cpp",
		"**.hlsl",
		"**.hlsli",
	}

	includedirs {
		dirs.engine
	}

	libdirs { dirs.dependencies }

	links {
		"d3d11.lib"
	}

	defines {"_CONSOLE"}

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		sdlchecks "true"
		fatalwarnings { "All" }

		flags { 
			"MultiProcessorCompile"
		}
	
	-- enableunitybuild "On"

	shadermodel("5.0")

	filter("files:**PS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Pixel")

	filter("files:**VS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Vertex")

	filter("files:**CS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Compute")
