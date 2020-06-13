workspace "Dragon"
    architecture "x64"

    configurations
    {
    "Debug",
    "Release",
    "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Dragon"
    location "Dragon"
    kind "SharedLib"
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
      "%{prj.name}/vender/spdlog/include"
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

    postbuildcommands
    {
       ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
        "Dragon/vender/spdlog/include",
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