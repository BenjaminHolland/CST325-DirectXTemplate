#include "EventLoop.h"
#include "State.h"
#include "Graphics.h"
bool EventLoop::_is_initialized = false;
bool EventLoop::_continue = true;
HRESULT EventLoop::initialize()
{
	_is_initialized = true;
	return S_OK;
}

void EventLoop::run()
{
	if (!_is_initialized) return;
	MSG msg;
	while (_continue) {
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				_continue = false;
				break;
			}
		}
		State::update();
		Graphics::render();
	}
}

