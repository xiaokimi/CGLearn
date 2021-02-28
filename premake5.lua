workspace "CGLearn"
	architecture "x64"
	configurations 
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CGSandBox"
	location "CGSandBox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cgpch.h"
	pchsource "CGSandBox/src/cgpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++17"

	filter {"configurations:Debug"}
		defines {"DEBUG"}
		symbols "On"

	filter {"configurations:Release"}
		defines {"NDEBUG"}
		optimize "On"
