#pragma once
#include <Windows.h>
#include <DirectXMath.h>
class State {
private:
	static bool _is_initialized;
	
public:
	static HRESULT initialize();
	static inline bool is_initialized() { return _is_initialized; }
	static void update();
	
	class Camera {
	public:
		static HRESULT initialize();
		static void update();
		static DirectX::XMFLOAT3 Position;
		static DirectX::XMFLOAT3 Target;
		static DirectX::XMFLOAT3 Up;
		static float NearPlane;
		static float FarPlane;
		static float FieldOfView;
	};
	class Light {
	public:
		static HRESULT initialize();
		static void update();
		static DirectX::XMFLOAT3 Position;
		static DirectX::XMFLOAT3 Color;
	};
};