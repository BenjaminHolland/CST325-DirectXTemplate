#include "WindowService.h"
using namespace cst;
using namespace sf;
using namespace std;
WindowService::WindowService():_window(VideoMode(800,600),"DirectX Template")
{

}

WindowService & WindowService::current()
{
	static WindowService current;
	return current;
}

void WindowService::with(function<void(Window&)> execute) {
	execute(_window);
}
