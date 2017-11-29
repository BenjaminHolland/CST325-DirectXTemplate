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
				cameraService.withTarget([&](auto &target) {
					cameraService.withPosition([&](auto &pos) {
						XMVECTOR from = XMLoadFloat3(&pos);
						XMVECTOR to = XMLoadFloat3(&target);
						XMVECTOR facing = to-from;
						facing = XMVector3Normalize(facing);
						facing.m128_f32[3] = 0;

						if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A) {
							facing = XMVector4Transform(facing, XMMatrixRotationY(-0.05f));
							auto newTarget = from+facing;
							XMStoreFloat3(&target, newTarget);
						}
						if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D) {

							facing = XMVector4Transform(facing, XMMatrixRotationY(0.05f));
							auto newTarget = from + facing;
							XMStoreFloat3(&target, newTarget);
						}if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
							XMVECTOR newPos = from + facing * 2;
							XMVECTOR newTar = to + facing * 2;
							XMStoreFloat3(&target, newTar);
							XMStoreFloat3(&pos, newPos);
						}if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
							XMVECTOR newPos = from - facing * 2;
							XMVECTOR newTar = to - facing * 2;
							XMStoreFloat3(&target, newTar);
							XMStoreFloat3(&pos, newPos);
						}
					});
				});
			};

			default:
				break;
			}
		}
	});
}

