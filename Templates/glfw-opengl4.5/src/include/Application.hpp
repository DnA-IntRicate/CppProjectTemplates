#pragma once
#include "Debug.hpp"


class Application
{
public:
    static void OnCreate();
    static void OnDestroy();
    static void OnUpdate(float deltaTime);
    static void OnImGuiRender();

    static void OnWindowResize(int width, int height);
    static void OnWindowClose();

    static void OnKeyPressed(int key);
    static void OnKeyReleased(int key);

    static void OnMouseButtonPressed(int button);
    static void OnMouseButtonReleased(int button);

    static void OnMouseScrolled(float x, float y);
    static void OnMouseMoved(float x, float y);
};
