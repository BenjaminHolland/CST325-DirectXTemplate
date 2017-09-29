#include "Window.h"
#include "Input.h"
#include <windowsx.h>
namespace Window {
	HWND Handle;
}
HRESULT Window::Init(HINSTANCE hInstance)
{

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Window::OnMessage;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpszClassName = "MainWindow";
	if (RegisterClassEx(&wc) == 0) return E_FAIL;
	Window::Handle = CreateWindow(
		"MainWindow", "CurrentWindow",
		WS_OVERLAPPEDWINDOW, 0, 0,
		Window::InitialWidth, Window::InitialHeight,
		NULL, NULL, hInstance, NULL);
	if (INVALID_HANDLE_VALUE == Window::Handle) return E_FAIL;
	ShowWindow(Window::Handle, SW_SHOW);
	UpdateWindow(Window::Handle);
	return S_OK;
}

LRESULT CALLBACK Window::OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
		Map messages we want to handle to their handlers. HANDLE_MSG is defined in the
		windowsx.h header. 
		To add handle additional messages, there's an easy but messy way, and a hard but nice way:
		Easy But Messy:
			create a `case` statement that handles the message you want to handle.
			This is fast and easy, but makes this function long and difficult to read when you're 
			handling more than half a dozen messages.
		Hard but nice:
			search in windowsx.h for the message you want to handle. You should find a line
			#define HANDLE_XXX, where XXX is the message you want to handle. Directly above
			that line you will find the signature of the function HANDLE_MSG is expecting.
			Write a function with that signature, and add a HANDLE_MSG statement using that
			function. 

			While this procedure is a bit more difficult, it allows you a lot more flexibility
			in how you structure your code, and is a lot more readable for larger message sets.
			Note that the functions you add need not be in the Input namespace. 
	*/
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Input::OnMouseLeftButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, Input::OnMouseLeftButtonUp);
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, Input::OnMouseRightButtonDown);
		HANDLE_MSG(hWnd, WM_RBUTTONUP, Input::OnMouseRightButtonUp);
		HANDLE_MSG(hWnd, WM_KEYUP, Input::OnKeyboardKeyUp);
		HANDLE_MSG(hWnd, WM_KEYDOWN, Input::OnKeyboardKeyDown);
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
