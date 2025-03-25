#pragma once
#include <Windows.h>
#include <functional>
#include <string>

struct sWindowConfig
{
    std::wstring applicationName;
    HWND* hWnd = nullptr;
    HINSTANCE hInstance = nullptr;
    int width = 1280;
    int height = 720;
    int nCmdSow = 0;
};

class WindowHandler
{
public:
	bool Init(sWindowConfig& aWindowConfig);
	HWND* GetWindow();
    int GetHeight() { return myHeight; }
    int GetWidth() { return myWidth; }

private:
    HWND* myHWnd = nullptr;
    HINSTANCE myHInstance = nullptr;
    int myWidth = 1280;
    int myHeight = 720;
};

