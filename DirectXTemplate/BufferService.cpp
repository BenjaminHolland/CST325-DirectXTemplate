#include "BufferService.h"
#include "CameraService.h";
#include "ScreenService.h"
#include "TimeService.h"
using namespace cst;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
cst::BufferService::BufferService()
{
	auto &screenService = ScreenService::current();
	screenService.with([&](ComPtr<ID3D11Device> device) {
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(TimeBufferStateType);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &_timeState;
		ThrowIfFailed(device->CreateBuffer(&bd, &sd, &_timeBuffer));
	});
	screenService.with([&](ComPtr<ID3D11Device> device) {
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(CameraBufferStateType);
		
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &_cameraState;
		ThrowIfFailed(device->CreateBuffer(&bd, &sd, &_cameraBuffer));
	});

	screenService.with([&](ComPtr<ID3D11Device> device) {
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(TransformBufferStateType);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &_transformState;
		ThrowIfFailed(device->CreateBuffer(&bd, &sd, &_transformBuffer));
	});
}

BufferService & cst::BufferService::current()
{
	static BufferService current;
	return current;
}

void cst::BufferService::update()
{
	auto &cameraService = CameraService::current();
	with([&](TimeBufferStateType &state, auto buffer) {
		auto &timeService = TimeService::current();
		timeService.with([&](XMUINT2 &discreteTime) {
			state.Frames = discreteTime;
		});
	});
	with([&](CameraBufferStateType &state, auto buffer) {
		cameraService.withPosition([&](auto &pos) {
			state.Position = pos;
		});
	});
	with([&](TransformBufferStateType &state, auto buffer) {
		cameraService.withProjection([&](auto proj) {
			state.Projection = proj;
		});
		cameraService.withView([&](auto view) {
			state.View = view;
		});
		XMStoreFloat4x4(&state.World, XMMatrixIdentity());
	});

}

void BufferService::with(function<void(TimeBufferStateType&, ComPtr<ID3D11Buffer>)> execute) {
	execute(_timeState, _timeBuffer);
}
void cst::BufferService::with(std::function<void(CameraBufferStateType&, Microsoft::WRL::ComPtr<ID3D11Buffer>)> execute)
{
	execute(_cameraState, _cameraBuffer);
}
void cst::BufferService::with(std::function<void(TransformBufferStateType&, Microsoft::WRL::ComPtr<ID3D11Buffer>)> execute) {
	execute(_transformState, _transformBuffer);
}