#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "stdafx.h"
#include <Windows.h>
#include "commonUtilities/InputHandler.h"
#include "commonUtilities/Timer.h"
#include "util/Settings.h"
#include "GraphicsEngine.h"
#include "Camera.h"
#include "BoidSimulation.h"
#include "WindowHandler.h"
#include "util/ImguiHandler.h"

LRESULT CALLBACK WndProc(HWND hWmD, UINT message, WPARAM wParam, LPARAM lParam);

constexpr wchar_t APPLICATION_NAME[6] = L"boids";

CommonUtilities::InputHandler inputHandler;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int nCmdShow)
{
	LoadedWindowSettings windowSettings = Settings::GetWindowSettings();

	sWindowConfig winConf;
	winConf.applicationName = APPLICATION_NAME;
	winConf.width = windowSettings.width;
	winConf.height = windowSettings.height;
	winConf.hInstance = hInstance;
	winConf.nCmdSow = nCmdShow;

	WindowHandler windowHandler;
	windowHandler.Init(winConf);

	GraphicsEngine graphicsEngine;
	graphicsEngine.Init(*windowHandler.GetWindow(), windowSettings.fullscreen);

	Camera camera;
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	camera.Init({ (float)desktop.right, (float)desktop.bottom }, 90.f, { 0.f, 0.f, 0.f });
	CommonUtilities::Timer timer;

	BoidSimulation boidSim;
	{
		if (!boidSim.Init(&graphicsEngine, &camera))
			return 0;
		GraphicsSettings graphicsSettings;
		SimulationSettings simSettings;
		PlayerSettings playerSettings;
		Settings::LoadBoidSimulationSettings(simSettings, graphicsSettings, playerSettings);
		boidSim.ApplySettings(graphicsSettings, simSettings, playerSettings);
		boidSim.UpdateFrameBuffer();
		graphicsEngine.PrepareFrame(boidSim.GetClearColor());
		boidSim.InitBoidComputer();
	}

	float timeScale = 1.f;
	bool renderImgui = true;

	ImguiHandler imguiHandler(windowHandler.GetWindow(), graphicsEngine.GetDevice(), graphicsEngine.GetContext());

	MSG msg = {};
	bool shouldRun = true;
	while (shouldRun)
	{
		inputHandler.HandleMouse(*windowHandler.GetWindow());
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}

		imguiHandler.NewFrame();
		ImGui::Begin("Settings (ESC to hide)");
		ImGui::DragFloat("TimeScale", &timeScale, 0.05f, 0, 10.f);
		timer.Update();
		float scaledDeltaTime = timer.GetDeltaTime() * timeScale;

		if (renderImgui)
			inputHandler.SetMouseState(MouseState::None);

		boidSim.BeginFrame(scaledDeltaTime, timer.GetDeltaTime());

		switch (boidSim.UpdateSimulationSettings())
		{
		case (SimulationMessage::Save):
			Settings::SaveBoidSimulationSettings(boidSim.GetSimulationSettings(), boidSim.GetGraphicsSettings(), boidSim.GetPlayerSettings());
			break;
		case (SimulationMessage::Reset):
			boidSim.ResetSimulation();
			break;
		}

		if (boidSim.GetPlayerControlled())
		{
			if (!renderImgui)
			{
				inputHandler.SetMouseState(MouseState::Centered);
				boidSim.UpdatePlayer(inputHandler);
			}
			camera.SetTransform(boidSim.GetPlayerFollowCameraTransform());
		}
		else
		{
			camera.Update(timer.GetDeltaTime(), inputHandler);
		}
		boidSim.UpdateFrameBuffer();

		graphicsEngine.PrepareFrame(boidSim.GetClearColor());
		boidSim.SimulateGPU();
		boidSim.Render();

		if (ImGui::Button("Quit"))
			shouldRun = false;
		ImGui::End();

		if (inputHandler.GetKeyDown(VK_ESCAPE))
			renderImgui = !renderImgui;
		imguiHandler.Render(renderImgui);

		inputHandler.Update();
		graphicsEngine.EndFrame();
	}

	imguiHandler.Destroy();
	return (int)msg.wParam;
}

WNDPROC oriWndProc = NULL;

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)
		|| inputHandler.UpdateEvents(message, wParam, lParam))
		return S_OK;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
