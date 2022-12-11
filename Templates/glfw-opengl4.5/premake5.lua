workspace "glfw-opengl4.5"
    architecture "x86_64"
    startproject "glfw-opengl4.5"

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

    project "glfw-opengl4.5"
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
            "src/source/**.cpp",

            "../Vendor/imgui/*.h",
            "../Vendor/imgui/*.cpp",
            "../Vendor/imgui/backends/imgui_impl_glfw.cpp",
            "../Vendor/imgui/backends/imgui_impl_glfw.h",
            "../Vendor/imgui/backends/imgui_impl_opengl3.h",
            "../Vendor/imgui/backends/imgui_impl_opengl3_loader.h",
            "../Vendor/imgui/backends/imgui_impl_opengl3.cpp",

            "../Vendor/glad/4.5/**.h",
            "../Vendor/glad/4.5/**.c"
        }

        includedirs
        {
            "src/include",
            "../Vendor/glfw/include",
            "../Vendor/imgui",
            "../Vendor/imgui/backends",
            "../Vendor/spdlog/include",
            "../Vendor/glad/4.5/include"
        }

        libdirs
        {
            OUT_DIR
        }

        links
        {
            "glfw",
            "spdlog",
            "opengl32.dll"
        }

        defines
        {
            "GLFW_INCLUDE_NONE"
        }

    group "Vendor"
        include "../Vendor/glfw"
        include "../Vendor/spdlog"
    group ""
