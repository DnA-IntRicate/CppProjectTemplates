#include "Application.hpp"
#include <imgui.h>
#include <imgui_internal.h>


void Application::OnCreate()
{
}

void Application::OnDestroy()
{
}

void Application::OnUpdate(float deltaTime)
{
}

void Application::OnImGuiRender()
{
    bool open = true;
    ImGui::ShowDemoWindow(&open);
}

void Application::OnWindowResize(int width, int height)
{
    Debug::Trace("Window resized: {}x{}", width, height);
}

void Application::OnWindowClose()
{
}

void Application::OnKeyPressed(int key)
{
}

void Application::OnKeyReleased(int key)
{
}

void Application::OnMouseButtonPressed(int button)
{
}

void Application::OnMouseButtonReleased(int button)
{
}

void Application::OnMouseScrolled(float x, float y)
{
}

void Application::OnMouseMoved(float x, float y)
{
}
