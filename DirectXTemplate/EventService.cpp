#include "EventService.h"
#include "WindowService.h"
#include "CameraService.h"
using namespace sf;
using namespace cst;
using namespace DirectX;
EventService::EventService()
{
}
EventService & EventService::current()
{
	static EventService current;
	return current;
}

void cst::EventService::update()
{
	WindowService &windowService = WindowService::current();

	Event event;
	windowService.with([&event](Window& window){
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed: {
				auto &cameraService = CameraService::current();
				if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A) {
					cameraService.withAngle([&](float &angle) {
						angle -= 0.1f;
					});
				}
				if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {
					cameraService.withAngle([&](float &angle) {
						angle += 0.1f;
					});
				}if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
					cameraService.withPosition([&](XMFLOAT3 &pos) {
						cameraService.withFacing([&](XMFLOAT3 &facing) {
					
							pos.x += facing.x*1.f;
							pos.y += facing.y*1.f;
							pos.z += facing.z*1.0f;
					
						});
					});
				}if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
					cameraService.withPosition([&](XMFLOAT3 &pos) {
						cameraService.withFacing([&](XMFLOAT3 &facing) {

							pos.x -= facing.x*1.f;
							pos.y -= facing.y*1.f;
							pos.z -= facing.z*1.f;

						});
					});
				}
				cameraService.withPosition([&](XMFLOAT3 &pos) {
					if (pos.x > 25) {
						pos.x = -25;
					}
					if (pos.x < -25) {
						pos.x = 25;
					}
					if (pos.z > 25) {
						pos.z = -25;
					}
					if (pos.z < -25) {
						pos.z = 25;
					}
				});
			};

			default:
				break;
			}
		}
	});
}

