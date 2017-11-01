#pragma once
#include <Windows.h>
class Window {
private:
	static HWND _handle;
	static LRESULT CALLBACK _on_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool _is_initialized;
public:
	static HRESULT initialize(HINSTANCE hInstance);
	static inline HWND handle() { return _handle; }
	static inline bool is_initialized() { return _is_initialized; }


};
