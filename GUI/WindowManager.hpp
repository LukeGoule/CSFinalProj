#pragma once

#include <Windows.h>
#include <iostream>

#include <d3d10_1.h>
#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")
#define DIRECTINPUT_VERSION 0x0800

#include "Singleton.hpp"
#include "imgui/imgui.h"

class WindowManager 
	: public Singleton<WindowManager> 
{
public:

	void Main();

	void Setup(const wchar_t* szWindowClassName, int iWidth, int iHeight, HINSTANCE hInstance);
	void SetupImGui();

	void Cleanup();

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ID3D10Device* m_pD3DDevice = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D10RenderTargetView* m_pMainRenderTargetView = NULL;
	bool					m_bForceQuit = false;

	ImFont* m_pFontConsolas;
	ImFont* m_pFontVerdana;
	ImFont* m_pFontHurtMold;

	HWND m_pWindowHandle;
	const wchar_t* m_szWindowName;
	int m_iWindowSizeX = 0, m_iWindowSizeY = 0;

	ImVec4 C = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 255.f);
	ImVec4 C2 = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 255.f);
};

