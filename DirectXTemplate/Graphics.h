#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
class Graphics {
private:
	struct CameraState {
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
	struct LightState {
		DirectX::XMVECTOR Position;
		DirectX::XMVECTOR Color;
	};

	static bool _is_initialized;
	static ID3D11Device* Device;
	static ID3D11DeviceContext* ImmediateContext;
	static IDXGISwapChain* SwapChain;
	static ID3D11RenderTargetView* BackBufferRenderTargetView;
	static ID3D11DepthStencilView* BackBufferDepthStencilView;
	
	static ID3D11VertexShader* BasicVertexShader;
	static ID3D11InputLayout* BasicVertexShaderLayout;
	static ID3D11PixelShader* BasicPixelShader;

	static CameraState CameraStateData;
	static LightState LightStateData;
	static ID3D11Buffer* CameraBuffer;
	static ID3D11Buffer* LightBuffer;


	static ID3D11Buffer* QuadBuffer;
	static HRESULT _init_device_basics();
	static HRESULT _init_back_buffer();
	static HRESULT _init_pipeline();
	static HRESULT _init_vertex_shader();
	static HRESULT _init_pixel_shader();
	static HRESULT _init_camera();
	static HRESULT _init_light();
	static HRESULT _init_quad();
	static void _update();
public:
	static HRESULT initialize();
	static inline bool is_initialized() { return _is_initialized; }
	
	static void render();
};