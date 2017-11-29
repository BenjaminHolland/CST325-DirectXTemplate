#include "CameraService.h"
using namespace cst;
using namespace std;
using namespace DirectX;
cst::CameraService::CameraService() :_position(0, 1, -1), _target(0, 1, 1)
{
}
CameraService & cst::CameraService::current()
{
	static CameraService current;
	return current;
}

void cst::CameraService::withPosition(std::function<void(DirectX::XMFLOAT3&)> execute)
{
	execute(_position);
}
void cst::CameraService::withTarget(std::function<void(DirectX::XMFLOAT3&)> execute) {
	execute(_target);
}
void cst::CameraService::withProjection(std::function<void(DirectX::XMFLOAT4X4)> execute) {
	XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0/600.0, 0.01f, 1000.0f));
	execute(projection);
}
void cst::CameraService::withView(std::function<void(DirectX::XMFLOAT4X4)> execute) {
	XMVECTOR pos, target;
	pos=XMLoadFloat3(&_position);
	target = XMLoadFloat3(&_target);
	XMFLOAT4X4 view;
	DirectX::XMStoreFloat4x4(&view, XMMatrixLookAtLH(pos, target, { 0,1,0,1 }));
	execute(view);
}
