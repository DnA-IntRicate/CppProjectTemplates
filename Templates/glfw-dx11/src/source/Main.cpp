#include <Application.hpp>
#include <Debug.hpp>

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <ostream>
#include <fstream>
#include <cstdint>

static GLFWwindow* g_Window = nullptr;
static float g_LastFrameTime = 0.0f;

static ID3D11Device* g_Device;
static ID3D11DeviceContext* g_DeviceContext;
static IDXGISwapChain* g_SwapChain;
static ID3D11RenderTargetView* g_RenderTargetView;
static ID3D11Buffer* g_VertexBuffer;
static ID3D11Buffer* g_IndexBuffer;

static ID3D11InputLayout* g_VertexBufferLayout;
static ID3D11VertexShader* g_VertexShader;
static ID3D11PixelShader* g_PixelShader;

struct Vertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT4 Color;
};


constexpr static void GLFWErrorCallback(int error_code, const char* description) noexcept
{
    Debug::Critical("GLFW Error ({0}) - ", error_code, description);
}

static void CacheShader(ID3D10Blob* shaderBlob, const std::string& fileName)
{
    std::ofstream out(fileName, std::ios::out | std::ios::binary);
    if (out.is_open())
    {
        out.write((char*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
        out.flush();
        out.close();
    }
}

static void GetOrCompileShaders()
{
    ID3D10Blob* vertexShaderBlob = nullptr;
    ID3D10Blob* pixelShaderBlob = nullptr;

    if (std::filesystem::exists("SHADER_VERTEX.cache"))
    {
        std::ifstream in("SHADER_VERTEX.cache", std::ios::in | std::ios::binary);
        if (in.is_open())
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            in.seekg(0, std::ios::beg);

            D3DCreateBlob(size, &vertexShaderBlob);
            in.read((char*)vertexShaderBlob->GetBufferPointer(), size);
            in.close();
        }
    }
    else
    {
        HRESULT hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0, &vertexShaderBlob, nullptr);
        if (FAILED(hr))
            std::cout << "VertexShader failed to compile!";
    }

    g_Device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &g_VertexShader);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0  },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    g_Device->CreateInputLayout(inputElementDesc.data(), (UINT)inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(), &g_VertexBufferLayout);

    if (std::filesystem::exists("SHADER_PIXEL.cache"))
    {
        std::ifstream in("SHADER_PIXEL.cache", std::ios::in | std::ios::binary);
        if (in.is_open())
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            in.seekg(0, std::ios::beg);

            D3DCreateBlob(size, &pixelShaderBlob);
            in.read((char*)pixelShaderBlob->GetBufferPointer(), size);
            in.close();
        }
    }
    else
    {
        HRESULT hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0, &pixelShaderBlob, nullptr);
        if (FAILED(hr))
            std::cout << "PixelShader failed to compile!";
    }

    g_Device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &g_PixelShader);

    if (!std::filesystem::exists("SHADER_VERTEX.cache"))
        CacheShader(vertexShaderBlob, "SHADER_VERTEX.cache");

    if (!std::filesystem::exists("SHADER_PIXEL.cache"))
        CacheShader(pixelShaderBlob, "SHADER_PIXEL.cache");
}

static void InitDx11()
{
    HWND hWnd = glfwGetWin32Window(g_Window);

    int width, height;
    glfwGetFramebufferSize(g_Window, &width, &height);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    std::vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_11_0 };

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, featureLevels.data(),
        (UINT)featureLevels.size(), D3D11_SDK_VERSION, &swapChainDesc, &g_SwapChain, &g_Device, &featureLevel, &g_DeviceContext);

    ID3D11Texture2D* backBuffer;
    g_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    g_Device->CreateRenderTargetView(backBuffer, nullptr, &g_RenderTargetView);
    backBuffer->Release();

    // Get the actual back buffer size
    D3D11_TEXTURE2D_DESC backBufferDesc;
    backBuffer->GetDesc(&backBufferDesc);
    width = backBufferDesc.Width;
    height = backBufferDesc.Height;

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    g_DeviceContext->RSSetViewports(1, &viewport);

    std::vector<Vertex> triangleVertices =
    {
        { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },  // Top Left
        { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },  // Top Right
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Bottom Right    
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }   // Bottom Left
    };

    std::vector<UINT> triangleIndices = { 0, 1, 2, 2, 3, 0 };

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * triangleVertices.size(); 
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = triangleVertices.data();
    g_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_VertexBuffer);

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(UINT) * triangleIndices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexBufferData;
    ZeroMemory(&indexBufferData, sizeof(indexBufferData));
    indexBufferData.pSysMem = triangleIndices.data();
    g_Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &g_IndexBuffer);

    GetOrCompileShaders();
}

static void DoDx11Stuff()
{
    // Set the InputLayout and shaders
    g_DeviceContext->IASetInputLayout(g_VertexBufferLayout);
    g_DeviceContext->VSSetShader(g_VertexShader, nullptr, 0);
    g_DeviceContext->PSSetShader(g_PixelShader, nullptr, 0);

    // Set the vertex buffer and index buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
    g_DeviceContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Draw the indexed triangle
    g_DeviceContext->DrawIndexed(6, 0, 0);
}

int main(int argc, char** argv)
{
    Debug::Initialize();

    ASSERT(glfwInit() == GLFW_TRUE, "GLFW initialisation failed!");
    Debug::Warn("GLFW initialisation succeeded!");
    glfwDefaultWindowHints();
    glfwSetErrorCallback((GLFWerrorfun)GLFWErrorCallback);

    g_Window = glfwCreateWindow(1280, 720, "glfw-dx11 template", nullptr, nullptr);
    ASSERT(g_Window, "GLFW failed to create a window!");
    glfwMakeContextCurrent(g_Window);

    InitDx11();

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

    ImGui_ImplGlfw_InitForOther(g_Window, true);
    ImGui_ImplDX11_Init(g_Device, g_DeviceContext);

    Application::OnCreate();

    while (!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        float time = (float)glfwGetTime();
        float deltaTime = time - g_LastFrameTime;
        Application::OnUpdate(deltaTime);

        const FLOAT clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);
        g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, clearColor);
        g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DoDx11Stuff();
        Application::OnImGuiRender();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_SwapChain->Present(0, 0);
        g_LastFrameTime = time;
    }

    glfwDestroyWindow(g_Window);
    glfwTerminate();

    return 0;
}
