#include "pch.h"
#include "InputHandler.h"

namespace CommonUtilities
{
	InputHandler::InputHandler() :
		myKeys(),
		myPreviousKeys(),
		myMousePos(),
		myPreviousMousePos(),
		myMouseWheelDelta(),
		myMouseState()
	{
	}

	void InputHandler::Update()
	{
		myPreviousKeys = myKeys;
		myPreviousMousePos = myMousePos;
		myMouseWheelDelta = 0;
	}

	bool InputHandler::UpdateEvents(UINT aMsg, WPARAM aWparam, LPARAM lParam)
	{
		bool flag = true;
		switch (aMsg)
		{
		case WM_KEYDOWN:
		{
			myKeys[aWparam] = true;
			break;
		}
		case WM_KEYUP:
		{
			myKeys[aWparam] = false;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			myKeys[VK_LBUTTON] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			myKeys[VK_LBUTTON] = false;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			myKeys[VK_RBUTTON] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			myKeys[VK_RBUTTON] = false;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			myKeys[VK_MBUTTON] = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			myKeys[VK_MBUTTON] = false;
			break;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			myMousePos = { xPos, yPos };
			break;
		}
		case WM_MOUSEWHEEL:
		{
			myMouseWheelDelta = GET_WHEEL_DELTA_WPARAM(aWparam) / WHEEL_DELTA;
			break;
		}
		default:
		{
			flag = false;
			break;
		}
		}

		return flag;
	}

	bool InputHandler::GetKey(const int aKeyCode) const
	{
		return myKeys[aKeyCode];
	}

	bool InputHandler::GetKeyDown(const int aKeyCode) const
	{
		return myKeys[aKeyCode] && !myPreviousKeys[aKeyCode];
	}

	bool InputHandler::GetKeyRelease(int aKeyCode) const
	{
		return !myKeys[aKeyCode] && myPreviousKeys[aKeyCode];
	}

	int InputHandler::GetMouseWheelDelta() const
	{
		return myMouseWheelDelta;
	}

	POINT InputHandler::GetMousePos() const
	{
		return myMousePos;
	}

	POINT InputHandler::GetMouseDeltaPos() const
	{
		return { myMousePos.x - myPreviousMousePos.x, myMousePos.y - myPreviousMousePos.y };
	}

	void InputHandler::HandleMouse(const HWND& hWnd)
	{
		if (hWnd == GetActiveWindow())
		{
			switch (myMouseState)
			{
			case MouseState::Centered:
				ForceHideCursor();
				CenterMouse(hWnd);
				[[fallthrough]];
			case MouseState::Captured:
				CaptureMouse(hWnd);
				return;
			default:
				break;
			}
		}
		ForceShowCursor();
		ReleaseMouse(hWnd);
	}

	void InputHandler::SetMouseState(const MouseState aMouseState)
	{
		myMouseState = aMouseState;
	}

	void InputHandler::CenterMouse(const HWND& hWnd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		POINT pt{ int((rect.left + rect.right) * 0.5f), int((rect.top + rect.bottom) * 0.5f) };
		myPreviousMousePos = { pt.x, pt.y };
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void InputHandler::SetMousePos(const POINT& aPoint)
	{
		SetCursorPos(aPoint.x + -myMousePos.x, aPoint.y - myMousePos.y);
		myMousePos = { aPoint.x, aPoint.y };
	}

	void InputHandler::CaptureMouse(const HWND& hWnd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		POINT upperLeft{ rect.left,rect.top };
		POINT botRight{ rect.right,rect.bottom };

		MapWindowPoints(hWnd, nullptr, &upperLeft, 1);
		MapWindowPoints(hWnd, nullptr, &botRight, 1);

		rect.left = upperLeft.x;
		rect.top = upperLeft.y;
		rect.right = botRight.x;
		rect.bottom = botRight.y;

		ClipCursor(&rect);
	}
	void InputHandler::ReleaseMouse(const HWND&)
	{
		ClipCursor(NULL);
	}
	
	void InputHandler::ForceShowCursor()
	{
		while (ShowCursor(TRUE) < 0);
	}
	void InputHandler::ForceHideCursor()
	{
		while (ShowCursor(FALSE) >= 0);
	}
}