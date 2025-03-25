include "../../premake/common.lua"

project "external"
location (dirs.localfiles)
		
	language "C++"
	cppdialect "C++17"

	targetdir (dirs.dependencies)
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}")

	files {
		"**.h",
		"**.hpp",
		"**.inl",
		"**.c",
		"**.cpp",
	}

	includedirs {
		".",
		"pix",
		"imgui/"
	}

	includedirs {
		dirs.external
	}

	libdirs { dirs.dependencies }

	defines {"_CONSOLE"}

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		

	filter "configurations:Release"
		defines {"_RELEASE"}
		runtime "Release"
		optimize "on"

	filter "configurations:Retail"
		defines {"_RETAIL"}
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"

		flags { 
			"MultiProcessorCompile"
		}