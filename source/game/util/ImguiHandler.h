#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "WindowHandler.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImguiHandler
{
public:
	ImguiHandler(HWND* hWnd, ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	void NewFrame();
	void Render(const bool aRenderFlag = true);
	void Destroy();
};