#include "Window.h"


HWND Window::_handle =(HWND)INVALID_HANDLE_VALUE;
bool Window::_is_initialized = false;
LRESULT Window::_on_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

HRESULT Window::initialize(HINSTANCE hInstance)
{
	if (_is_initialized) return S_OK;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Window::_on_message;
	wc.lpszClassName = "MainWindow";

	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	if (RegisterClassEx(&wc) == 0) return E_FAIL;
	
	_handle = CreateWindow("MainWindow", "Main Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
	if (_handle == INVALID_HANDLE_VALUE) return E_FAIL;
	_is_initialized = true;
	return S_OK;
}
