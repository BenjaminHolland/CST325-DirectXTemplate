#include <Windows.h>
#include "Program.h"
#include "Window.h"
#include "Graphics.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
	HRESULT hr;
	hr=Window::Init(hInstance);
	if (FAILED(hr)) return hr;
	hr = Graphics::Init(Window::Handle);
	if (FAILED(hr)) return hr;
	hr = Program::RunMessageLoop(hInstance);
	return hr;
}