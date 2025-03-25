#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <WinUser.h>
#include <bitset>

enum class MouseState
{
	None,
	Captured,
	Centered
};

namespace CommonUtilities
{
	class InputHandler
	{
	public:
		InputHandler();

		void Update();
		bool UpdateEvents(UINT aMsg, WPARAM aWparam, LPARAM lParam);

		bool GetKey(const int aKeyCode) const;
		bool GetKeyDown(const int aKeyCode) const;
		bool GetKeyRelease(int aKeyCode) const;

		int GetMouseWheelDelta() const;
		POINT GetMousePos() const;
		POINT GetMouseDeltaPos() const;

		void HandleMouse(const HWND& hWnd);
		void SetMouseState(const MouseState aMouseState);

		void CenterMouse(const HWND& hWnd);
		void SetMousePos(const POINT& aPoint);
		void CaptureMouse(const HWND& hWnd);
		void ReleaseMouse(const HWND& hWnd);
		void ForceShowCursor();
		void ForceHideCursor();

	private:
		std::bitset<265> myKeys;
		std::bitset<265> myPreviousKeys;

		int myMouseWheelDelta;
		POINT myMousePos;
		POINT myPreviousMousePos;
		MouseState myMouseState;		
	};
}