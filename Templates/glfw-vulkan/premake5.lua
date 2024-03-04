function NormalizePath(path)
    result = path:gsub("\\", "/")

    return result
end

VULKAN_SDK = NormalizePath(os.getenv("VULKAN_SDK"))

workspace "glfw-vulkan"
    architecture "x86_64"
    startproject "glfw-vulkan"

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
        "_CRT_NONSTDC_NO_WARNINGS",
        "_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS"
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

    project "glfw-vulkan"
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
            "../Vendor/imgui/backends/imgui_impl_vulkan.h",
            "../Vendor/imgui/backends/imgui_impl_vulkan.cpp",
            "../Vendor/glfw/src/vulkan.c"
        }

        includedirs
        {
            "src/include",
            "../Vendor/glfw/include",
            "../Vendor/imgui",
            "../Vendor/imgui/backends",
            "../Vendor/spdlog/include",

            "%{VULKAN_SDK}/Include",
            "%{VULKAN_SDK}/Source/SPIRV-Reflect/include"
        }

        libdirs
        {
            OUT_DIR
        }

        bindirs
        {
            OUT_DIR
        }

        links
        {
            "glfw",
            "spdlog",

            "%{VULKAN_SDK}/Lib/volk.lib"
        }

        defines
        {
            "IMGUI_DEFINE_MATH_OPERATORS",
            "IMGUI_IMPL_VULKAN_USE_VOLK",
            "GLM_FORCE_RADIANS",
            "GLM_FORCE_DEPTH_ZERO_TO_ONE"
        }

        filter "system:windows"
            cppdialect "C++20"

            defines
            {
                "VK_USE_PLATFORM_WIN32_KHR"
            }

        filter "system:linux"
            cppdialect "gnu++20"

            defines
            {
                "VK_USE_PLATFORM_XCB_KHR"
            }

        filter "system:macosx"
            cppdialect "C++latest"

            defines
            {
                "VK_USE_PLATFORM_MACOS_MVK"
            }

        filter "configurations:Debug"
            links
            {
                "%{VULKAN_SDK}/Lib/shaderc_sharedd.lib",
                "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib",
                "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib",
                "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll"
            }

        filter "configurations:Release"
            links
            {
                "%{VULKAN_SDK}/Lib/shaderc_shared.lib",
                "%{VULKAN_SDK}/Lib/spirv-cross-core.lib",
                "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib",
                "%{VULKAN_SDK}/Bin/shaderc_shared.dll"
            }

    group "Vendor"
        include "../Vendor/glfw"
        include "../Vendor/spdlog"
    group ""
