workspace "glfw-dx11"
    architecture "x86_64"
    startproject "glfw-dx11"

    configurations
    {
        "Debug",
        "Release"
    }

    solutionitems
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    defines
    {
        "_CRT_SECURE_NO_DEPRECATE",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "Off"
        defines "_WIN32"

    filter "configurations:Debug"
        defines "_DEBUG"
        runtime "Debug"
        symbols "Full"

    filter "configurations:Release"
        defines "_RELEASE"
        runtime "Release"
        symbols "Off"
        optimize "Full"

        flags
        {
            "NoBufferSecurityCheck",
            "NoRuntimeChecks",
            "LinkTimeOptimization",
            "NoIncrementalLink"
        }

    project "glfw-dx11"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"

        debugdir (OUT_DIR)
        targetdir (OUT_DIR)
        objdir (INT_DIR)

        files
        {
            "src/include/**.h",
            "src/include/**.hpp",
            "src/source/**.cpp"
        }

        includedirs
        {
            "src/include",
            "../Vendor/glfw/include",
            "../Vendor/imgui",
            "../Vendor/imgui/backends",
            "../Vendor/spdlog/include"
        }

        libdirs
        {
            OUT_DIR
        }

        links
        {
            "glfw",
            "spdlog",
            "D3d11.dll"
        }

        defines
        {
            "GLFW_INCLUDE_NONE"
        }

    group "Vendor"
        include "../Vendor/glfw"
        include "../Vendor/spdlog"
    group ""
