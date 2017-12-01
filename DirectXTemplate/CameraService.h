#pragma once
#include "Standard.h"
namespace cst {
	class CameraService {
	private:
		CameraService();
		DirectX::XMFLOAT3 _position;
		float _angle = 0;
	public:
		CameraService(CameraService const&) = delete;
		static CameraService& current();
		void withPosition(std::function<void(DirectX::XMFLOAT3&)> execute);
		void withFacing(std::function<void(DirectX::XMFLOAT3)> execute);
			void withAngle(std::function<void(float&)> execute);
		void withProjection(std::function<void(DirectX::XMFLOAT4X4)> execute);
		void withView(std::function<void(DirectX::XMFLOAT4X4)> execute);
	

	};
}