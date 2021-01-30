#pragma once

#include <Windows.h>
#include <iostream>

#include <d3d10_1.h>
#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")
#define DIRECTINPUT_VERSION 0x0800

#include "Singleton.hpp"
#include "imgui/imgui.h"
#include "TextEditor.hpp"

class WindowManager 
	: public Singleton<WindowManager> 
{
public:
	
	// Contains the main loop.
	void Main();

	// Draws ImGui every frame.
	void DrawImGui();
	
	// Create the window instance and call the DX setup methods.
	void Setup(const wchar_t* szWindowClassName, int iWidth, int iHeight, HINSTANCE hInstance);

	// Load a new ImGui instance and set it's style.
	void SetupImGui();

	// Call when exiting, safely closes all open handles.
	void Cleanup();

	// Creates a new DX device and connects it to the new window.
	bool CreateDeviceD3D();

	// Destroys the DX device for when the application closes.
	void CleanupDeviceD3D();

	// Creates a new "render target" which the DX will render to, linked to back buffer
	void CreateRenderTarget();

	// Safely remove the render target.
	// Used when the buffer size updates or freezes temporarily. (call CreateRenderTarget immediately after).
	void CleanupRenderTarget();

	// Handles messages sent by windows. Usually user commands such as resizing. Static function, annoyingly.
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND					m_hWindowHandle			= NULL;
	ID3D10Device*			m_pD3DDevice			= NULL;
	IDXGISwapChain*			m_pSwapChain			= NULL;
	ID3D10RenderTargetView* m_pMainRenderTargetView = NULL;
	bool					m_bForceQuit			= false,
							m_bVsync				= true,
							m_bD3DDebug				= false;

	ImFont*					m_pFontConsolas			= nullptr,
						   *m_pFontVerdana			= nullptr,
						   *m_pFontVerdana15px		= nullptr;

	const wchar_t*			m_szWindowName;
	int						m_iWindowSizeX = 0, 
							m_iWindowSizeY = 0,
							m_iDebugExtraWidth = 250,
							m_iRefreshRate = 60;

	TextEditor m_Editor;
};

