#include "WindowHandler.h"

LRESULT CALLBACK WndProc(HWND hWmD, UINT message, WPARAM wParam, LPARAM lParam);


bool WindowHandler::Init(sWindowConfig& aWindowConfig)
{
	myHWnd = aWindowConfig.hWnd;
	myHInstance = aWindowConfig.hInstance;
	myWidth = aWindowConfig.width;
	myHeight = aWindowConfig.width;

	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = myHInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = aWindowConfig.applicationName.c_str();
	RegisterClassExW(&wcex);

	myHWnd = new HWND();
	*myHWnd = CreateWindow(aWindowConfig.applicationName.c_str(), aWindowConfig.applicationName.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, aWindowConfig.width, aWindowConfig.height, nullptr, nullptr, aWindowConfig.hInstance, nullptr);

	if (!myHWnd)
	{
		return false;
	}

	ShowWindow(*myHWnd, aWindowConfig.nCmdSow);
	UpdateWindow(*myHWnd);
	return true;
}

HWND* WindowHandler::GetWindow()
{
	return myHWnd;
}
