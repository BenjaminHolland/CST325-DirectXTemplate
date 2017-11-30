#include "EventService.h"
#include "WindowService.h"
#include "RenderService.h"
#include "BufferService.h"
#include "TimeService.h"
using namespace std;
using namespace sf;
using namespace cst;
int main() {
	CoInitialize(NULL);
	WindowService& windowService = WindowService::current();
	
	windowService.with([&](Window& window) {
		EventService& eventService = EventService::current();
		RenderService& renderService = RenderService::current();
		auto &bufferService = BufferService::current();
		auto &timeService = TimeService::current();
		while (window.isOpen()) {
			timeService.update();
			eventService.update();
			bufferService.update();
			renderService.update();
		}
	});
	

}