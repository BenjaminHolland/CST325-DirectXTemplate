#pragma once
#include "Standard.h"
namespace cst {
	struct TransformBufferStateType {
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
	};
	struct CameraBufferStateType {
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Target;
		float pad[2];
	};
	class BufferService {
	private:
		BufferService();
		TransformBufferStateType _transformState;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _transformBuffer;

		CameraBufferStateType _cameraState;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _cameraBuffer;

	public:
		BufferService(BufferService const&) = delete;
		static BufferService& current();
		void update();
		void with(std::function<void(TransformBufferStateType&, Microsoft::WRL::ComPtr<ID3D11Buffer>)> execute);
		void with(std::function<void(CameraBufferStateType&, Microsoft::WRL::ComPtr<ID3D11Buffer>)> execute);

	};
}