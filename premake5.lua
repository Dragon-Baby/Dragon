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
IncludeDir["stb_image"] = "Dragon/vendor/stb_image"

include "Dragon/vendor/GLFW"
include "Dragon/vendor/GLAD"
include "Dragon/vendor/ImGui"

project "Dragon"
    location "Dragon"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dgpch.h"
    pchsource "Dragon/src/dgpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines
    {
    	"_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
	}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS",
            "DG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "DG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DG_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        systemversion "latest"

        defines
        {
            "DG_PLATFORM_WINDOWS"
        }

       

    filter "configurations:Debug"
        defines "DG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DG_DIST"
        runtime "Release"
        optimize "on"