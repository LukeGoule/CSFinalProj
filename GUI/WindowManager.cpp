#include "WindowManager.hpp"

#include "imgui_impl_dx10.hpp"
#include "imgui_impl_win32.hpp"
#include "ImGui_CustomExtensions.hpp"

#include "Emulation.hpp"
#include "Instructions.hpp"
#include "../shared/Mappings.hpp"
#include "DefaultProgram.hpp"
#include "EmulationManager.hpp"

#include "Menu.hpp"
#include "Localisation.hpp"
#include "Colours.hpp"
#include "Fonts.hpp"

#include "resource.h"

#include <stdexcept>
#include <thread>
#include <chrono>

#pragma warning(disable: 4311 4302)

void WindowManager::Main() 
{
#pragma region WINDOWMANAGER_MAIN_DO_ONCE
    TextEditor::LanguageDefinition EditorLanguage = TextEditor::LanguageDefinition::AQA_ASM();
    
    EmulationManager::NewState();

    // Configuration for the text editor extension.
    m_Editor.SetTabSize(4);
    m_Editor.SetShowWhitespaces(false);
    m_Editor.SetLanguageDefinition(EditorLanguage);
    m_Editor.SetText(GET_EMULATOR_CODE);

    // Manually override the HALT instruction. Makes good use of macros for readable code.
    // This is a lambda function, though, which isn't good practice. Works though.
    // The new HALT instruction will stop the emulator thread from doing anything until the user
    // clicks "Run" again.
    InstructionBindings[INSTR_HALT] = [](INSTRUCTION_INPUTS) 
    {
        SET_EMULATOR_RUNNING(false);
        SET_EMULATOR_ERROR(Localisation.HaltProcessor);

        return INSTRUCTION_EXECUTE_OK;
    };
    
    // Start the emulator's own thread.
    EmulationManager::Launch();

    Fonts::Load();

    // Initialise the static menu tree & load all of the resources for the tree's items.
    MenuSystem::SetInstance(this);
    MenuSystem::ConstructMenuTree();
    MenuSystem::LoadComponents(); // resource loading etc.

    MSG pWndProcMsg;
    ZeroMemory(&pWndProcMsg, sizeof(pWndProcMsg));

#pragma endregion
    

#pragma region WINDOWMANAGER_MAIN_LOOP
    // This loop should run 60 times a second, or more if the user's monitor supports it (or less even).
    // It will only quit when the application receives the WM_QUIT message from Windows or m_bForceQuit is set
    // to true. Any other way of quitting could be unsafe if file operations are happening.
    // SHOULD being the key word. There is a chance it is rendering much more than it needs to be because of the 
    // window message handling.

    while (pWndProcMsg.message != WM_QUIT && !m_bForceQuit)
    {
        // Windows related stuff for handling events relating to the actual window.
        // This will handle anything like painting, resizing, minimising&maximising etc etc.
        if (PeekMessage(&pWndProcMsg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&pWndProcMsg);
            DispatchMessage(&pWndProcMsg);
            continue;
        }

        // Checks if the DX object has been invalidated, update if so. 
        // Clears the previous render state ready to draw again.
        ImGui_ImplDX10_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Our menu
        DrawImGui();
        
        // Update the code the emulator instance can see if the user changes the visible code.
        // They need to be synchronised.
        // TODO: Rescan the code on code change or specific key press. (SHIFT+S ?)
        if (m_Editor.IsTextChanged())
            SET_EMULATOR_CODE(m_Editor.GetText());

        if (GET_EMULATOR_RUNNING) 
        {
            // Set a red "error marker" on the current / last executed line.
            // A future change may be to recode the error system in ImGuiTextEdit to be a new class which defines something like "CurrentLine"
            // that doesn't have an inner message saying "error" constantly.
            TextEditor::ErrorMarkers ClearedErrorMarkers;
            ClearedErrorMarkers.insert(std::make_pair<int, std::string>((int)REGISTERS->_IP, _c Localisation.CurrentExecLine));
            m_Editor.SetErrorMarkers(ClearedErrorMarkers);
        }
        else
        {
            // Empty list of "error markers" so no red things linger on the screen.
            TextEditor::ErrorMarkers ClearedErrorMarkers;
            m_Editor.SetErrorMarkers(ClearedErrorMarkers);
        }

        // Clear the DirectX framebuffer to purple.
        // If the user can see this then something has gone wrong during the initialisation process.
        static float clear_color[4] = { 1.f, 0.f, 1.f, 1.f };

        // Render the ImGui draw lists etc, the window.
        ImGui::Render();

        // Clear the previous framebuffer. This isn't strictly necessary since everything is drawn over the previous frame but is good practice.
        m_pD3DDevice->OMSetRenderTargets(1, &m_pMainRenderTargetView, NULL);
        m_pD3DDevice->ClearRenderTargetView(m_pMainRenderTargetView, (float*)clear_color);
    
        // Draw ImGui's draw data onto DirectX.
        ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

        // Must be the last draw-related call. Also determines if VSync is enabled.
        m_pSwapChain->Present((UINT)m_bVsync, 0);
    }
#pragma endregion
}

void WindowManager::DrawImGui()
{
    // The ImGui window must be drawn at -1,-1 otherwise the user can see the surrounding border because of ImGui's way of rendering.
    ImGui::SetNextWindowPos(ImVec2(-1, -1));
    ImGui::SetNextWindowSize(ImVec2((float)m_iWindowSizeX, (float)m_iWindowSizeY));

    // This is where the actual inner window drawing happens. The `##` in front of the name means it is purely a tag. It will not render an actual window title.
    // The flags `NoResize` and `NoTitleBar` do as you may expect. This is done because we want the user to only see the inner content of the ImGui "window".
    ImGui::Begin("##MainRenderImGui", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    {
        MenuSystem::DrawTree();
    }
    ImGui::End();

    // Compile-time determination as to whether to include this code. There is no need for it if we aren't compiling in debug mode.
#if _DEBUG
    ImGui::SetNextWindowPos(ImVec2((float)m_iWindowSizeX + 1.f, 0.f));
    ImGui::SetNextWindowSize(ImVec2((float)m_iDebugExtraWidth - 7.f, (float)m_iWindowSizeY));
    ImGui::Begin("##DebugScreen", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PushFont(m_pFontConsolas);
    {
        ImGui::Text("Debug Extension");
    }
    ImGui::PopFont();
    ImGui::End();
#endif
}

void WindowManager::Setup(const wchar_t* szWindowClassName, int iWidth, int iHeight, HINSTANCE hInstance)
{
    m_iWindowSizeX = iWidth;
    m_iWindowSizeY = iHeight;
    m_szWindowName = szWindowClassName;

    // Create a new Window class structure with the data we want (size, style, etc).
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WindowManager::WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL,
        NULL,
        NULL,
        NULL,
        szWindowClassName,
        NULL
    };
    
    // Register this window class within Windows.
    // The window hasn't yet actually been created.
    RegisterClassEx(&wc);
    
    int iInitialiseWidthX = m_iWindowSizeX,
        iInitialiseWidthY = m_iWindowSizeY;

#if _DEBUG // Add some width to the window if compiling a debug build.
    iInitialiseWidthX += m_iDebugExtraWidth + 10;
#endif

    // Get ourselves a handle of our window. Give Windows kernel everything it needs to generate a new window instance that we can modify.
    // No proper layered window styling is set here, as can be seen VhereV
    m_hWindowHandle = CreateWindow(wc.lpszClassName, m_szWindowName, NULL, 100, 100, iInitialiseWidthX, iInitialiseWidthY, NULL, NULL, wc.hInstance, NULL);

    // In the case that Windows decided to not give us a Window for once, or we gave it bad data, show an error to the user.
    if (!m_hWindowHandle)
    {
        // Show the error message to the user.
        MessageBoxA(0, Localisation.Error.WindowInit, Localisation.Error.Fatal, MB_ICONERROR);
        abort(); // This is not a safe way to exit the application. Fix me.
        return;
    }

    // This sets the title of the window.
    SetWindowText(m_hWindowHandle, m_szWindowName);

    // Finally define what we want the style of the window to be by setting a variable using a window wrapper.
    SetWindowLongPtrA(m_hWindowHandle, GWL_STYLE, (
        WS_OVERLAPPEDWINDOW                         // Normal window 
        ^ WS_MAXIMIZEBOX)                           // No maximize box
        ^ WS_SIZEBOX);                              // Disable resizing.

    // This is meant to set the window icon that shows at the top left corner on Windows, but it fails to function as intended strangely.
    // What's even stranger to me is that the task bar's icon for this application renders as intended?
    SetClassLong(m_hWindowHandle, GCLP_HICON, (LONG)LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ICON1)));

    // Since the window has been setup successfully, we can create and linke a new DirectX instance to it for accelerated graphics rendering.
    if (!CreateDeviceD3D())
    {
        // On the case that DirectX failed to load, show an error. This can happen if
        // the user hasn't installed DirectX properly or there is an error with the DirectX
        // drivers. This doesn't (seem to) happen often. Requires much more testing on more systems.
        MessageBoxA(0, Localisation.Error.DXInit, Localisation.Error.Fatal, 0);
        
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        
        exit(-1);
        return;
    }

    ShowWindow(m_hWindowHandle, SW_SHOWDEFAULT);
    UpdateWindow(m_hWindowHandle);
}

void WindowManager::SetupImGui() 
{
    // Create the ImGui static instance.
    ImGui::CreateContext();
    
    // This next huge block of code just sets all the variables ImGui will use for colouration and general styling.
    // TODO: Load absolutely everything here from a resource file so it can be modified without recompiling the 
    // application.
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding        = ImVec2(5.f, 5.f);
    style->WindowRounding       = 0.0f;
    style->FramePadding         = ImVec2(5.f, 5.f);
    style->FrameRounding        = 0.0f;
    style->ItemSpacing          = ImVec2(5.f, 5.f);
    style->ItemInnerSpacing     = ImVec2(10.f, 10.f);
    style->IndentSpacing        = 25.0f;
    style->ScrollbarSize        = 5.0f;
    style->ScrollbarRounding    = 0.0f;
    style->GrabMinSize          = 1.0f;
    style->GrabRounding         = 0.0f;

    style->Colors[ImGuiCol_Border]          = Colours.FullDark;
    style->Colors[ImGuiCol_BorderShadow]    = Colours.FullDark;
    style->Colors[ImGuiCol_WindowBg]        = Colours.FullDark;
    style->Colors[ImGuiCol_FrameBgActive]   = Colours.Lighter;
    style->Colors[ImGuiCol_FrameBgHovered]  = Colours.Lighter;
    style->Colors[ImGuiCol_FrameBg]         = Colours.Dark;
    
    style->Colors[ImGuiCol_Button]          = Colours.Buttons.Default;
    style->Colors[ImGuiCol_ButtonHovered]   = Colours.Buttons.Hovered;
    style->Colors[ImGuiCol_ButtonActive]    = Colours.Buttons.Active;

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(m_hWindowHandle);
    ImGui_ImplDX10_Init(m_pD3DDevice);
}

void WindowManager::Cleanup() 
{
    // Allow the thread to finish this cycle and quit.
    SET_EMULATOR_RUNNING(false);
    SET_EMULATOR_PAUSED(false);
    SET_EMULATOR_FULLEXIT(true);
    
    Sleep(EMULATOR_DAEMON_EXIT_TIMER);

    // Join it with the main thread so it finishes safely.
    EMULATOR_DAEMON.join();

    ImGui_ImplDX10_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();

    DestroyWindow(m_hWindowHandle);
}

bool WindowManager::CreateDeviceD3D()
{
    // Setup the DX swap chain, this defines what window it renders to, how fast, at what depth etc.
    DXGI_SWAP_CHAIN_DESC SwapChainSettings;
    ZeroMemory(&SwapChainSettings, sizeof(SwapChainSettings));
    SwapChainSettings.BufferCount                          = 2;
    SwapChainSettings.BufferDesc.Width                     = 0;
    SwapChainSettings.BufferDesc.Height                    = 0;
    SwapChainSettings.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainSettings.BufferDesc.RefreshRate.Numerator     = m_iRefreshRate;
    SwapChainSettings.BufferDesc.RefreshRate.Denominator   = 1;
    SwapChainSettings.Flags                                = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainSettings.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainSettings.OutputWindow                         = m_hWindowHandle;
    SwapChainSettings.SampleDesc.Count                     = 1;
    SwapChainSettings.SampleDesc.Quality                   = 0;
    SwapChainSettings.Windowed                             = TRUE;
    
    // https://devblogs.microsoft.com/directx/dxgi-flip-model/
    SwapChainSettings.SwapEffect                           = /*DXGI_SWAP_EFFECT_DISCARD*/ DXGI_SWAP_EFFECT_FLIP_DISCARD;

    UINT createDeviceFlags = NULL;

#ifdef _DEBUG
    m_bD3DDebug = true;
#endif

    if (m_bD3DDebug)
    {
        createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
    }

    if (D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &SwapChainSettings, &m_pSwapChain, &m_pD3DDevice) != S_OK)
    {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void WindowManager::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
    if (m_pD3DDevice) { m_pD3DDevice->Release(); m_pD3DDevice = NULL; }
}

void WindowManager::CreateRenderTarget()
{
    ID3D10Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    if (!pBackBuffer)
    {
        throw std::runtime_error{ Localisation.Error.Fatal };
        return;
    }

    m_pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
    pBackBuffer->Release();
}

void WindowManager::CleanupRenderTarget()
{
    if (m_pMainRenderTargetView) 
    { 
        m_pMainRenderTargetView->Release(); 
        m_pMainRenderTargetView = NULL; 
    }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// This code was taken from imgui_impl_win32 and modified for my own purposes.
LRESULT WINAPI WindowManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (WindowManager::get().m_pD3DDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            WindowManager::get().CleanupRenderTarget();
            WindowManager::get().m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            WindowManager::get().CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        // When the user closes the application, make sure the emulator thread can exit.
        // If we don't do this then the program can be executing in the background, using resources it
        // definitely should not be.
        SET_EMULATOR_RUNNING(false);
        SET_EMULATOR_PAUSED(false);
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}