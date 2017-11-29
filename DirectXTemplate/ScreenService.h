#pragma once
#include "Standard.h"
namespace cst {
	class ScreenService {
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> _buffers;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencil;
		template <class T>
		void with(std::function<void(Microsoft::WRL::ComPtr<T>)> execute,Microsoft::WRL::ComPtr<T> &member);
		ScreenService();
	public:
		ScreenService(ScreenService const&) = delete;
		static ScreenService& current();
		void with(std::function<void(Microsoft::WRL::ComPtr<ID3D11Device>)> execute);
		void with(std::function<void(Microsoft::WRL::ComPtr<IDXGISwapChain>)> execute);
		void with(std::function<void(Microsoft::WRL::ComPtr<ID3D11DeviceContext>)> execute);
		void with(std::function<void(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>)> execute);
		void with(std::function<void(Microsoft::WRL::ComPtr<ID3D11DepthStencilView>)> execute);

	};
}