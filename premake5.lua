workspace "Dragon"
    architecture "x64"

    configurations
    {
    "Debug",
    "Release",
    "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Dragon/vendor/GLFW/include"

include "Dragon/vendor/GLFW"

project "Dragon"
    location "Dragon"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dgpch.h"
    pchsource "Dragon/src/dgpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS",
            "DG_BUILD_DLL"
        }

    

    filter "configurations:Debug"
        defines "DG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DG_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "DG_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Dragon/vendor/spdlog/include",
        "Dragon/src"
    }

    links
    {
        "Dragon"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS"
        }

       

    filter "configurations:Debug"
        defines "DG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DG_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "DG_DIST"
        optimize "On"