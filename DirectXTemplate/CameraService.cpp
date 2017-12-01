#include "CameraService.h"
using namespace cst;
using namespace std;
using namespace DirectX;
cst::CameraService::CameraService() :_position(0, 3, -1), _angle(0)
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
void CameraService::withAngle(std::function<void(float&)> execute) {
	execute(_angle);
}
void CameraService::withFacing(std::function<void(XMFLOAT3)> execute) {
	XMVECTOR tv = XMVECTOR({ 0,0,1,0 });
	tv = XMVector4Transform(tv, XMMatrixRotationY(_angle));
	
	XMFLOAT3 facing;
	XMStoreFloat3(&facing, tv);
	execute(facing);

}
void cst::CameraService::withProjection(std::function<void(DirectX::XMFLOAT4X4)> execute) {
	XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0/600.0, 0.01f, 1000.0f));
	execute(projection);
}
void cst::CameraService::withView(std::function<void(DirectX::XMFLOAT4X4)> execute) {
	XMVECTOR pos, target;
	pos=XMLoadFloat3(&_position);
	target = XMVECTOR({ 0,0, 0.01f, 0 });
	target = XMVector4Transform(target, XMMatrixRotationY(_angle));
	target = pos + target;
	XMFLOAT4X4 view;
	DirectX::XMStoreFloat4x4(&view, XMMatrixLookAtLH(pos, target, { 0,1,0,1 }));
	execute(view);
}
