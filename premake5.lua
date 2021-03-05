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
	pchsource "%{prj.name}/src/cgpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter {"configurations:Debug"}
		defines {"DEBUG"}
		symbols "On"

	filter {"configurations:Release"}
		defines {"NDEBUG"}
		optimize "On"
