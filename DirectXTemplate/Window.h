#pragma once
#include <Windows.h>
namespace Window {

	//The initial width and height of the window. These shouldn't be used for any per-frame calculations, since the window is resizeable.
	const DWORD InitialWidth = 800;
	const DWORD InitialHeight = 600;
	
	//A handle we can use to do stuff to our window.
	//Only valid if preceeded by a single successful call to Init(HINSTANCE).
	//(Don't try to use this without calling Init first)
	extern HWND Handle;

	//Initializes and shows the window. 
	HRESULT Init(HINSTANCE hInstance);
	
	//The messaging processing function for our window.
	//Renamed from WndProc for readability.
	LRESULT CALLBACK OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}