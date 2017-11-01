#include "State.h"

bool State::_is_initialized = false;
HRESULT State::initialize()
{
	_is_initialized = true;
	return S_OK;
}

void State::update()
{
	State::Camera::update();
	State::Light::update();
	State::Time::update();
}


DirectX::XMFLOAT3 State::Camera::Position = DirectX::XMFLOAT3{ 0.0,0.0,-20.0f };
DirectX::XMFLOAT3 State::Camera::Target=DirectX::XMFLOAT3{ 0.0,0.0,1.0f };
DirectX::XMFLOAT3 State::Camera::Up = DirectX::XMFLOAT3{ 0.0,1.0f,0.0f };
float State::Camera::NearPlane=0.01f;
float State::Camera::FarPlane=10000.0f;
float State::Camera::FieldOfView = 30.0f;

HRESULT State::Camera::initialize()
{
	return S_OK;
}

void State::Camera::update()
{
	
}

DirectX::XMFLOAT3 State::Light::Position = DirectX::XMFLOAT3{ 0,0,-10.0f };
DirectX::XMFLOAT3 State::Light::Color= DirectX::XMFLOAT3{ 1.0f,1.0f,0.0f };
HRESULT State::Light::initialize()
{
	return S_OK;
}
float time = 0;
void State::Light::update()
{
	time += 0.01f;
	float z = 10 * sinf(time);
	Position.z = z;

}
unsigned int State::Time::Frame = 0;
void State::Time::update()
{
	State::Time::Frame += 1;
}
