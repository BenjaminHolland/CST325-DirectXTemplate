#pragma once
#include "Standard.h"
namespace cst {
	class CameraService {
	private:
		CameraService();
		DirectX::XMFLOAT3 _position;
		DirectX::XMFLOAT3 _target;
	public:
		CameraService(CameraService const&) = delete;
		static CameraService& current();
		void withPosition(std::function<void(DirectX::XMFLOAT3&)> execute);
		void withTarget(std::function<void(DirectX::XMFLOAT3&)> execute);
		void withProjection(std::function<void(DirectX::XMFLOAT4X4)> execute);
		void withView(std::function<void(DirectX::XMFLOAT4X4)> execute);
	

	};
}