#pragma once
#include <Windows.h>
namespace Input {
	/*
		In general, each method here handles a different windows message. 
		The mapping from these functions to the windows messages is done in 
		Window::OnMessage.The instructions for adding additional handlers are 
		also in the documentation for that function.
	*/
	
	void OnMouseMoved(HWND hwnd, int x, int y, UINT keyFlags);
	void OnMouseLeftButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnMouseLeftButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void OnMouseRightButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnMouseRightButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void OnKeyboardKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void OnKeyboardKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
}