workspace "Dragon"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
    "Debug",
    "Release",
    "Dist"
    }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Dragon/vendor/GLFW/include"
IncludeDir["GLAD"] = "Dragon/vendor/GLAD/include"
IncludeDir["ImGui"] = "Dragon/vendor/ImGui"
IncludeDir["glm"] = "Dragon/vendor/glm"

include "Dragon/vendor/GLFW"
include "Dragon/vendor/GLAD"
include "Dragon/vendor/ImGui"

project "Dragon"
    location "Dragon"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dgpch.h"
    pchsource "Dragon/src/dgpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.hpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS",
            "DG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

    filter "configurations:Debug"
        defines "DG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "DG_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "DG_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "Dragon/src",
        "Dragon/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Dragon"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS"
        }

       

    filter "configurations:Debug"
        defines "DG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "DG_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "DG_DIST"
        runtime "Release"
        optimize "On"