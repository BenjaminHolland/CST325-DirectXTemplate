#include "ScreenService.h"
#include "WindowService.h"
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace cst;
using namespace std;
using namespace sf;

ScreenService::ScreenService()
{
	auto &windowService = WindowService::current();

	D3D_FEATURE_LEVEL required_level = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL actual_level;
	ThrowIfFailed(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &required_level, 1, D3D11_SDK_VERSION, &_device, &actual_level, &_context));
	
	windowService.with([this](Window& window) {
		ComPtr<IDXGIFactory> factory;
		ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), &factory));
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferCount = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Width = 800;
		scd.BufferDesc.Height = 600;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.OutputWindow = window.getSystemHandle();
		scd.Windowed = TRUE;
		ThrowIfFailed(factory->CreateSwapChain(_device.Get(), &scd, &_buffers));
	});

	ComPtr<ID3D11Texture2D> buffer;
	ThrowIfFailed(_buffers->GetBuffer(0, __uuidof(ID3D11Texture2D), &buffer));
	ThrowIfFailed(_device->CreateRenderTargetView(buffer.Get(), NULL, &_renderTarget));

	
	
	D3D11_TEXTURE2D_DESC dd = {};
	dd.Width = 800;
	dd.Height = 600;
	dd.ArraySize = 1;
	dd.MipLevels = 1;
	dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dd.SampleDesc = { 1,0 };
	dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ThrowIfFailed(_device->CreateTexture2D(&dd, NULL, &buffer));


	D3D11_DEPTH_STENCIL_VIEW_DESC dsd = {};
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsd.Texture2D.MipSlice = 0;
	ThrowIfFailed(_device->CreateDepthStencilView(buffer.Get(), &dsd, &_depthStencil));

}

ScreenService & cst::ScreenService::current()
{
	static ScreenService current;
	return current;
}
template <class T>
void cst::ScreenService::with(std::function<void(Microsoft::WRL::ComPtr<T>)> execute,ComPtr<T> &member) {
	execute(member);
}

void cst::ScreenService::with(std::function<void(Microsoft::WRL::ComPtr<ID3D11Device>)> execute)
{
	with<ID3D11Device>(execute, _device);
}
void cst::ScreenService::with(function<void(ComPtr<IDXGISwapChain>)> execute) {
	with<IDXGISwapChain>(execute, _buffers);
}
void cst::ScreenService::with(function<void(ComPtr<ID3D11DeviceContext>)> execute) {
	with<ID3D11DeviceContext>(execute, _context);
}
void cst::ScreenService::with(function<void(ComPtr<ID3D11RenderTargetView>)> execute) {
	with<ID3D11RenderTargetView>(execute, _renderTarget);
}
void cst::ScreenService::with(function<void(ComPtr<ID3D11DepthStencilView>)> execute) {
	with<ID3D11DepthStencilView>(execute, _depthStencil);
}

