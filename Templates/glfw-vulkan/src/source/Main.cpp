#include <Application.hpp>
#include <Debug.hpp>

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

// #include <glm>

#include <iostream>
#include <ostream>
#include <fstream>
#include <cstdint>

static GLFWwindow* g_Window = nullptr;
static float g_LastFrameTime = 0.0f;

//struct Vertex
//{
//    DirectX::XMFLOAT3 Position;
//    DirectX::XMFLOAT4 Color;
//};


constexpr static void GLFWErrorCallback(int error_code, const char* description) noexcept
{
    Debug::Critical("GLFW Error ({0}) - ", error_code, description);
}

int main(int argc, char** argv)
{
    Debug::Initialize();

    ASSERT(glfwInit() == GLFW_TRUE, "GLFW initialisation failed!");
    Debug::Warn("GLFW initialisation succeeded!");
    glfwDefaultWindowHints();
    glfwSetErrorCallback((GLFWerrorfun)GLFWErrorCallback);

    g_Window = glfwCreateWindow(1280, 720, "glfw-vulkan template", nullptr, nullptr);
    ASSERT(g_Window, "GLFW failed to create a window!");
    glfwMakeContextCurrent(g_Window);

  //  InitDx11();

    // GLFW event callbacks 
    glfwSetWindowSizeCallback(g_Window, [](GLFWwindow* window, int width, int height)
        {
            Application::OnWindowResize(width, height);
        });

    glfwSetWindowCloseCallback(g_Window, [](GLFWwindow* window)
        {
            Application::OnWindowClose();
        });

    glfwSetKeyCallback(g_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:    Application::OnKeyPressed(key); break;
                case GLFW_RELEASE:  Application::OnKeyReleased(key); break;
                    // TODO: Add OnKeyRepeated
            }
        });

    glfwSetMouseButtonCallback(g_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:    Application::OnMouseButtonPressed(button); break;
                case GLFW_RELEASE:  Application::OnMouseButtonReleased(button); break;
                    // TODO: Add OnMouseButtonRepeated
            }
        });

    glfwSetScrollCallback(g_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            Application::OnMouseScrolled((float)xoffset, (float)yoffset);
        });

    glfwSetCursorPosCallback(g_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            Application::OnMouseMoved((float)xpos, (float)ypos);
        });

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForVulkan(g_Window, true);
//    ImGui_ImplDX11_Init(g_Device, g_DeviceContext);

//    Application::OnCreate();

    while (!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        float time = (float)glfwGetTime();
        float deltaTime = time - g_LastFrameTime;
        Application::OnUpdate(deltaTime);

    //    ImGui_ImplDX11_NewFrame();
    //    ImGui_ImplGlfw_NewFrame();
    //    ImGui::NewFrame();

    //    DoDx11Stuff();
    //    Application::OnImGuiRender();

    //    ImGui::Render();
    //    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    //    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //    {
    //        GLFWwindow* backupContext = glfwGetCurrentContext();
    //        ImGui::UpdatePlatformWindows();
    //        ImGui::RenderPlatformWindowsDefault();
    //    }

    //    g_SwapChain->Present(0, 0);
        g_LastFrameTime = time;
    }

    glfwDestroyWindow(g_Window);
    glfwTerminate();

    return 0;
}
