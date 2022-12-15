#include <Application.hpp>
#include <Debug.hpp>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>

#include <iostream>
#include <cstdint>


static GLFWwindow* g_Window = nullptr;
static float g_LastFrameTime = 0.0f;

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

    g_Window = glfwCreateWindow(1280, 720, "glfw-opengl4.5 template", nullptr, nullptr);
    ASSERT(g_Window, "GLFW failed to create a window!");
    glfwMakeContextCurrent(g_Window);
    glfwSwapInterval(1);
    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad!");

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

    ImGui_ImplGlfw_InitForOpenGL(g_Window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Application::OnCreate();

    // Main loop
    while (!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        float time = (float)glfwGetTime();
        float deltaTime = time - g_LastFrameTime;
        Application::OnUpdate(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Application::OnImGuiRender();

        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(g_Window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }

        glfwSwapBuffers(g_Window);
        g_LastFrameTime = time;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(g_Window);
    glfwTerminate();

    return 0;
}
