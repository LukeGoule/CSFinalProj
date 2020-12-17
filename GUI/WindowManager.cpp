#include "WindowManager.hpp"
#include "HurtMold.hpp"

#include "imgui_impl_dx10.hpp"
#include "imgui_impl_win32.hpp"
#include "TextEditor.hpp"

#include "Emulation.hpp"
#include "Instructions.hpp"
#include "Mappings.hpp"

#include <stdexcept>
#include <thread>
#include <chrono>

bool g_bEmulatorRunning = false;
Emulation Emulator;
TextEditor Editor;
std::string CodeString = "; This is an example program that this assembly language emulator can execute.\n; It will simply count to 200, then exit.\n\nMOV R0, #0; Clear R0\n\nloop:\n\tADD R0, R0, #1; Increment R0\n\tCMP R0, #200; Test R0 against the value 200\n\tOUT R0; Print R0\n\tBNE loop; Loop if not equal\n\nHALT";

std::string g_LastError = "";
int delayMS = 500;

void EmulationThread()
{
    while (true)
    {
        if (!g_bEmulatorRunning) continue;

        try 
        {
            Emulator.RunFile(CodeString, &g_bEmulatorRunning, &delayMS);
        }
        catch (const std::runtime_error& Error)
        {
            g_LastError = Error.what();
        }
        catch (const std::out_of_range& Error)
        {
            g_LastError = Error.what();
        }

        g_bEmulatorRunning = false;
    }
}

void WindowManager::Main() 
{
    TextEditor::LanguageDefinition EditorLanguage = TextEditor::LanguageDefinition::AQA_ASM();
    
    Editor.SetTabSize(4);
    Editor.SetShowWhitespaces(false);
    Editor.SetLanguageDefinition(EditorLanguage);
    Editor.SetText(CodeString);

    Emulator.Reset();

    *Emulator.m_pRegisters->R0() = 0;
    *Emulator.m_pRegisters->R1() = 0;
    *Emulator.m_pRegisters->R2() = 0;
    *Emulator.m_pRegisters->R3() = 0;
    *Emulator.m_pRegisters->R4() = 0;
    *Emulator.m_pRegisters->R5() = 0;
    *Emulator.m_pRegisters->R6() = 0;
    *Emulator.m_pRegisters->R7() = 0;
    *Emulator.m_pRegisters->R8() = 0;
    *Emulator.m_pRegisters->R9() = 0;
    *Emulator.m_pRegisters->R10() = 0;
    *Emulator.m_pRegisters->R11() = 0;

    Emulator.m_pRegisters->_IP = 0;
    Emulator.m_pRegisters->_F_EQ = 0;
    Emulator.m_pRegisters->_F_NE = 0;
    Emulator.m_pRegisters->_F_GT = 0;
    Emulator.m_pRegisters->_F_LT = 0;

    std::thread t0(EmulationThread);

    // Manually override the HALT instruction
    InstructionBindings[HALT] = [](INSTRUCTION_INPUTS) {
        
        g_bEmulatorRunning = false;
        g_LastError = "Processor halted.";
        
        return INSTRUCTION_EXECUTE_OK;
    };

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT && !this->m_bForceQuit)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX10_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(this->m_iWindowSizeX, this->m_iWindowSizeY));
        ImGui::Begin("##lololol", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        {
            ImGui::PushFont(this->m_pFontConsolas);
            {
                Editor.Render("Code Entry", ImVec2(this->m_iWindowSizeX - 30, this->m_iWindowSizeY - 300));
            }
            ImGui::PopFont();

            ImGui::Spacing();

            ImGui::PushFont(this->m_pFontVerdana);
            {
                if (ImGui::Button(g_bEmulatorRunning ? "STOP" : "RUN"))
                {
                    g_bEmulatorRunning = !g_bEmulatorRunning;
                }
            }
            ImGui::PopFont();

            ImGui::SameLine();
            ImGui::PushItemWidth(250);
            ImGui::LabelText("", "Last Error: \"%s\"", g_LastError.c_str());
            ImGui::SameLine();
            ImGui::PushItemWidth(250);
            ImGui::SliderInt("Delay (MS)", &delayMS, 0, 2000);
            ImGui::PopItemWidth();

            // Left frame
            ImGui::BeginChildFrame(82394809234, ImVec2((this->m_iWindowSizeX - 30) / 2, 205));
            {
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R0   %020llu", *Emulator.m_pRegisters->R0());
                ImGui::Text("R1   %020llu", *Emulator.m_pRegisters->R1());
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R2   %020llu", *Emulator.m_pRegisters->R2());
                ImGui::Text("R3   %020llu", *Emulator.m_pRegisters->R3());
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R4   %020llu", *Emulator.m_pRegisters->R4());
                ImGui::Text("R5   %020llu", *Emulator.m_pRegisters->R5());
               
                ImGui::Spacing();

                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "EQ   %020llu", Emulator.m_pRegisters->_F_EQ);
                ImGui::Text("NE   %020llu", Emulator.m_pRegisters->_F_NE);
                
                ImGui::Spacing();

                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "IP   %020llu", Emulator.m_pRegisters->_IP);
            }
            ImGui::EndChildFrame();

            ImGui::SameLine();

            // Right frame
            ImGui::BeginChildFrame(82394809235, ImVec2((this->m_iWindowSizeX - 30) / 2, 205));
            {
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R6   %020llu", *Emulator.m_pRegisters->R6());
                ImGui::Text("R7   %020llu", *Emulator.m_pRegisters->R7());
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R8   %020llu", *Emulator.m_pRegisters->R8());
                ImGui::Text("R9   %020llu", *Emulator.m_pRegisters->R9());
                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "R10  %020llu", *Emulator.m_pRegisters->R10());
                ImGui::Text("R11  %020llu", *Emulator.m_pRegisters->R11());
                
                ImGui::Spacing();

                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "GT   %020llu", Emulator.m_pRegisters->_F_GT);
                ImGui::Text("LT   %020llu", Emulator.m_pRegisters->_F_LT);

                ImGui::Spacing();

                ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1), "Eval `%s`", Emulator.CurrentInstruction.c_str());
            }
            ImGui::EndChildFrame();

            ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "Copyright (c) Luke Goule    Released under MIT License");
        }
        ImGui::End();
        
        CodeString = Editor.GetText();

        if (g_bEmulatorRunning) 
        {
            TextEditor::ErrorMarkers x;
            x.insert(std::make_pair<int, std::string>((int)Emulator.m_pRegisters->_IP, "Currently executing line."));
            Editor.SetErrorMarkers(x);
        }
        else
        {
            TextEditor::ErrorMarkers x;
            Editor.SetErrorMarkers(x);
        }

        static float clear_color[4] = { 0,0,0,0 };

        // Rendering
        ImGui::Render();
        this->m_pD3DDevice->OMSetRenderTargets(1, &this->m_pMainRenderTargetView, NULL);
        this->m_pD3DDevice->ClearRenderTargetView(this->m_pMainRenderTargetView, (float*)clear_color);
        ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

        // VSync On
        this->m_pSwapChain->Present(1, 0);

        // VSync Off
        // this->m_pSwapChainn->Present(0, 0);
    }
}

void WindowManager::Setup(const wchar_t* szWindowClassName, int iWidth, int iHeight, HINSTANCE hInstance) {

    this->m_iWindowSizeX = iWidth;
    this->m_iWindowSizeY = iHeight;
    this->m_szWindowName = szWindowClassName;

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
    
    RegisterClassEx(&wc);
    
    this->m_pWindowHandle = CreateWindow(wc.lpszClassName, this->m_szWindowName, 0, 100, 100, this->m_iWindowSizeX, this->m_iWindowSizeY, NULL, NULL, wc.hInstance, NULL);

    if (!this->m_pWindowHandle)
    {
        throw std::runtime_error{ "FATAL! Failed to initialise window!" };
    }

    SetWindowText(this->m_pWindowHandle, this->m_szWindowName);

    SetWindowLongPtrA(this->m_pWindowHandle, GWL_STYLE, (
        WS_OVERLAPPEDWINDOW                         // Normal window 
        ^ WS_MAXIMIZEBOX)                           // No maximize box
        ^ WS_SIZEBOX);                              // Disable resizing.

    if (!CreateDeviceD3D(this->m_pWindowHandle))
    {
        MessageBoxA(0, "FATAL! Failed to initialise DirectX", "FATAL ERROR", 0);
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        exit(-1);
    }


    ShowWindow(this->m_pWindowHandle, SW_SHOWDEFAULT);
    UpdateWindow(this->m_pWindowHandle);

    //SetClassLong(hwnd, GCL_HICON, (LONG)LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON1))); // set custom icon

    if (!CreateDeviceD3D(this->m_pWindowHandle))
    {
        CleanupDeviceD3D();
        exit(0);
    }
}

void WindowManager::SetupImGui() {
    // Sets up a neat looking dark mode for the ImGui instance.

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle* style = &ImGui::GetStyle();

    ImGui::StyleColorsDark();
    style->WindowPadding        = ImVec2(5, 5);
    style->WindowRounding       = 0.0f;
    style->FramePadding         = ImVec2(5, 5);
    style->FrameRounding        = 0.0f;
    style->ItemSpacing          = ImVec2(5, 5);
    style->ItemInnerSpacing     = ImVec2(10, 10);
    style->IndentSpacing        = 25.0f;
    style->ScrollbarSize        = 5.0f;
    style->ScrollbarRounding    = 0.0f;
    style->GrabMinSize          = 1.0f;
    style->GrabRounding         = 0.0f;

    auto C = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 255.f);
    auto C2 = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 255.f);

    style->Colors[ImGuiCol_Border]          = C;
    style->Colors[ImGuiCol_BorderShadow]    = C;
    style->Colors[ImGuiCol_WindowBg]        = C;
    style->Colors[ImGuiCol_FrameBgActive]   = C;
    style->Colors[ImGuiCol_FrameBgHovered]  = C;
    style->Colors[ImGuiCol_FrameBg]         = C;

    this->m_pFontConsolas   = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 13.f);
    this->m_pFontVerdana    = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 13.f);

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(this->m_pWindowHandle);
    ImGui_ImplDX10_Init(this->m_pD3DDevice);
}

void WindowManager::Cleanup() {
    ImGui_ImplDX10_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    this->CleanupDeviceD3D();

    DestroyWindow(this->m_pWindowHandle);
}

bool WindowManager::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
    if (D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &this->m_pSwapChain, &this->m_pD3DDevice) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void WindowManager::CleanupDeviceD3D()
{
    this->CleanupRenderTarget();
    if (this->m_pSwapChain) { this->m_pSwapChain->Release(); this->m_pSwapChain = NULL; }
    if (this->m_pD3DDevice) { this->m_pD3DDevice->Release(); this->m_pD3DDevice = NULL; }
}

void WindowManager::CreateRenderTarget()
{
    ID3D10Texture2D* pBackBuffer;
    this->m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    this->m_pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->m_pMainRenderTargetView);
    pBackBuffer->Release();
}

void WindowManager::CleanupRenderTarget()
{
    if (this->m_pMainRenderTargetView) { this->m_pMainRenderTargetView->Release(); this->m_pMainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Win32 message handler
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
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
