#include "Application.hpp"
#include <imgui.h>
#include <imgui_internal.h>


void Application::OnCreate()
{
    Debug::Trace("Window created");
}

void Application::OnDestroy()
{
    Debug::Trace("Window destroyed");
}

void Application::OnUpdate(float deltaTime)
{
    // Called once per frame
}

void Application::OnImGuiRender()
{
    // Called once per frame within the context of an 'ImGuiRender'

    bool open = true;
    ImGui::ShowDemoWindow(&open);
}

void Application::OnWindowResize(int width, int height)
{
    Debug::Trace("Window resized: {}x{}", width, height);
}

void Application::OnWindowClose()
{
    Debug::Trace("Window closed");
}

void Application::OnKeyPressed(int key)
{
    Debug::Trace("Key pressed: {}", key);
}

void Application::OnKeyReleased(int key)
{
    Debug::Trace("Key released: {}", key);
}

void Application::OnMouseButtonPressed(int button)
{
    Debug::Trace("MouseButton pressed: {}", button);
}

void Application::OnMouseButtonReleased(int button)
{
    Debug::Trace("MouseButton released: {}", button);
}

void Application::OnMouseScrolled(float x, float y)
{
    Debug::Trace("Mouse scrolled: {}x{}", x, y);
}

void Application::OnMouseMoved(float x, float y)
{
    Debug::Trace("Mouse moved: {}x{}", x, y);
}
