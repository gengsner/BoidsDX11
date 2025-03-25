#include "ImguiHandler.h"

ImguiHandler::ImguiHandler(HWND* hWnd, ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplWin32_Init(*hWnd);
	ImGui_ImplDX11_Init(aDevice, aContext);
}

void ImguiHandler::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiHandler::Render(const bool aRenderFlag)
{
	ImGui::Render();
	if(aRenderFlag)
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void ImguiHandler::Destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
