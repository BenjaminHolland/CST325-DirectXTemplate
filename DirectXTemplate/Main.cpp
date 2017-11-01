#include "EventLoop.h"
#include "Window.h"
#include "State.h"
#include "Graphics.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
	CoInitialize(NULL);
	HRESULT result;
	if (FAILED(result = Window::initialize(hInstance))) return result;
	if (FAILED(result = EventLoop::initialize())) return result;
	if (FAILED(result = State::initialize())) return result;
	if (FAILED(result = Graphics::initialize())) return result;
	ShowWindow(Window::handle(), SW_SHOW);
	EventLoop::run();
	return 0;
}