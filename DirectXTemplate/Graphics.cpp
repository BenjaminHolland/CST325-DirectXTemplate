#include "Graphics.h"
#include "Window.h"
#include "Vertex.h"
#include "State.h"
#include <d3dcompiler.h>
#include <d3d11.h>

ID3D11Device* Graphics::Device;
ID3D11DeviceContext* Graphics::ImmediateContext;
IDXGISwapChain* Graphics::SwapChain;
ID3D11RenderTargetView* Graphics::BackBufferRenderTargetView;
ID3D11DepthStencilView* Graphics::BackBufferDepthStencilView;

ID3D11VertexShader* Graphics::BasicVertexShader;
ID3D11InputLayout* Graphics::BasicVertexShaderLayout;
ID3D11PixelShader* Graphics::BasicPixelShader;

ID3D11Buffer* Graphics::CameraBuffer;
Graphics::CameraState Graphics::CameraStateData;

ID3D11Buffer* Graphics::LightBuffer;
Graphics::LightState Graphics::LightStateData;

ID3D11Buffer* Graphics::QuadBuffer;

HRESULT Graphics::_init_vertex_shader() {
	HRESULT result;
	ID3DBlob* vs_shader_blob;
	if (FAILED(result = D3DReadFileToBlob(L"BasicVertexShader.cso", &vs_shader_blob)))
		return result;
	D3D11_INPUT_ELEMENT_DESC input_layout[]{
		D3D11_INPUT_ELEMENT_DESC{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		D3D11_INPUT_ELEMENT_DESC{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
		D3D11_INPUT_ELEMENT_DESC{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
		D3D11_INPUT_ELEMENT_DESC{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	if (FAILED(result = Device->CreateVertexShader(vs_shader_blob->GetBufferPointer(), vs_shader_blob->GetBufferSize(), NULL, &Graphics::BasicVertexShader))) {
		vs_shader_blob->Release();
		return result;
	}
	if (FAILED(result = Device->CreateInputLayout(input_layout, 4, vs_shader_blob->GetBufferPointer(), vs_shader_blob->GetBufferSize(), &Graphics::BasicVertexShaderLayout))) {
		vs_shader_blob->Release();
		return result;
	}
	vs_shader_blob->Release();
	return result;
}

HRESULT Graphics::_init_pixel_shader()
{
	HRESULT result;
	ID3DBlob* ps_shader_blob;
	if (FAILED(result = D3DReadFileToBlob(L"PixelShader.cso", &ps_shader_blob)))
		return result;
	if (FAILED(result = Device->CreatePixelShader(ps_shader_blob->GetBufferPointer(), ps_shader_blob->GetBufferSize(), NULL, &Graphics::BasicPixelShader))) {
		ps_shader_blob->Release();
		return result;
	}
	ps_shader_blob->Release();
	return result;

}

HRESULT Graphics::_init_camera()
{
	HRESULT result;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = sizeof(Graphics::CameraState);

	if (FAILED(result = Device->CreateBuffer(&bd, NULL, &Graphics::CameraBuffer)))
		return result;
	return S_OK;
}

HRESULT Graphics::_init_light()
{
	HRESULT result;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = sizeof(Graphics::LightState);

	if (FAILED(result = Device->CreateBuffer(&bd, NULL, &Graphics::LightBuffer)))
		return result;
	return S_OK;
}


HRESULT Graphics::_init_quad()
{
	HRESULT result;
	DirectX::XMFLOAT4 pos[]{
		DirectX::XMFLOAT4{ -0.5,-0.5,0.5f,1 },
		DirectX::XMFLOAT4{ 0.5,-0.5,0.5f,1 },
		DirectX::XMFLOAT4{ 0.5,0.5,0.5f,1 },
		DirectX::XMFLOAT4{ -0.5,0.5,0.5f,1 },
	};

	DirectX::XMFLOAT2 tex[]{
		DirectX::XMFLOAT2{ 0,0 },
		DirectX::XMFLOAT2{ 1,0 },
		DirectX::XMFLOAT2{ 1,1 },
		DirectX::XMFLOAT2{ 0,1 },
	};

	DirectX::XMFLOAT4 color{ 1.0f,0.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 normal{ 0.0f,0.0f,1.0f,0.0f };

	Vertex verts[]{
		Vertex{
			pos[0],
			color,
			normal,
			tex[0]
		},
		Vertex{
			pos[1],
			color,
			normal,
			tex[1]
		},
		Vertex{
			pos[2],
			color,
			normal,
			tex[2]
		},
		Vertex{
			pos[3],
			color,
			normal,
			tex[3]
		}
	};
	Vertex final_verts[]{
		verts[0],verts[3],verts[1],verts[1],verts[3],verts[2]
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(Vertex) * 6;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = final_verts;
	if (FAILED(result = Device->CreateBuffer(&bd, &sd, &Graphics::QuadBuffer))) return result;
	return S_OK;
}

void Graphics::_update()
{
	CameraStateData.View = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&State::Camera::Position),
		DirectX::XMLoadFloat3(&State::Camera::Target),
		DirectX::XMLoadFloat3(&State::Camera::Up)
	);
	CameraStateData.Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(State::Camera::FieldOfView), 800 / 600.0f, State::Camera::NearPlane, State::Camera::FarPlane);
	CameraStateData.World = DirectX::XMMatrixIdentity();

	LightStateData.Position = DirectX::XMLoadFloat3(&State::Light::Position);
	LightStateData.Color = DirectX::XMLoadFloat3(&State::Light::Color);

}

HRESULT Graphics::_init_back_buffer() {
	HRESULT result;
	ID3D11Texture2D* back_buffer;
	ID3D11Texture2D* ds_buffer;
	D3D11_TEXTURE2D_DESC back_buffer_desc;

	if (FAILED(result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)))
		return result;
	back_buffer->GetDesc(&back_buffer_desc);

	result = Device->CreateRenderTargetView(back_buffer, NULL, &Graphics::BackBufferRenderTargetView);
	back_buffer->Release();
	if (FAILED(result))
		return result;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = back_buffer_desc.Width;
	descDepth.Height = back_buffer_desc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	if (FAILED(result = Device->CreateTexture2D(&descDepth, NULL, &ds_buffer)))
		return result;

	result = Device->CreateDepthStencilView(ds_buffer, NULL, &Graphics::BackBufferDepthStencilView);
	ds_buffer->Release();
	if (FAILED(result))
		return result;
	return S_OK;
}

HRESULT Graphics::_init_device_basics()
{
	RECT client_rect;
	GetClientRect(Window::handle(), &client_rect);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Height = client_rect.bottom - client_rect.top;
	scd.BufferDesc.Width = client_rect.right - client_rect.left;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.OutputWindow = Window::handle();
	scd.Windowed = true;

	D3D_FEATURE_LEVEL acceptable_feature_levels[]{ D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };
	D3D_DRIVER_TYPE acceptable_drivers[]{ D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP };
	D3D11_CREATE_DEVICE_FLAG flags = (D3D11_CREATE_DEVICE_FLAG)(
		D3D11_CREATE_DEVICE_DEBUGGABLE|
		D3D11_CREATE_DEVICE_DEBUG);

	HRESULT result;
	D3D_FEATURE_LEVEL actual_feature_level;
	for (D3D_DRIVER_TYPE driver : acceptable_drivers) {
		result = D3D11CreateDeviceAndSwapChain(NULL, driver, NULL, flags, acceptable_feature_levels, 1, D3D11_SDK_VERSION, &scd,
			&Graphics::SwapChain, &Graphics::Device, &actual_feature_level, &Graphics::ImmediateContext);
		if (SUCCEEDED(result)) return S_OK;
	}

	return result;
}
HRESULT Graphics::_init_pipeline()
{
	ImmediateContext->OMSetRenderTargets(1, &BackBufferRenderTargetView, BackBufferDepthStencilView);
	//ImmediateContext->OMSetRenderTargets(1, &BackBufferRenderTargetView, NULL);
	ImmediateContext->VSSetShader(BasicVertexShader, NULL, 0);
	ImmediateContext->PSSetShader(BasicPixelShader, NULL, 0);

	ID3D11Buffer* buffers[]{ CameraBuffer,LightBuffer };
	ImmediateContext->VSSetConstantBuffers(0, 2, buffers);
	ImmediateContext->PSSetConstantBuffers(0, 2, buffers);
	ImmediateContext->IASetInputLayout(BasicVertexShaderLayout);
	UINT strides, offsets;
	strides = sizeof(Vertex);
	offsets = 0;
	ImmediateContext->IASetVertexBuffers(0, 1, &QuadBuffer, &strides, &offsets);
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Height = 600;
	vp.Width = 800;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;

	ImmediateContext->RSSetViewports(1, &vp);
	return S_OK;
}

HRESULT Graphics::initialize()
{
	HRESULT result;
	if (FAILED(result = Graphics::_init_device_basics()))
		return result;
	if (FAILED(result = Graphics::_init_back_buffer()))
		return result;
	if (FAILED(result = Graphics::_init_vertex_shader()))
		return result;

	if (FAILED(result = Graphics::_init_pixel_shader()))
		return result;
	if (FAILED(result = Graphics::_init_camera()))
		return result;
	if (FAILED(result = Graphics::_init_light()))
		return result;
	if (FAILED(result = Graphics::_init_quad())) {
		return result;
	}
	if (FAILED(result = Graphics::_init_pipeline()))
		return result;
	return S_OK;
}


void Graphics::render()
{
	_update();
	ImmediateContext->UpdateSubresource(CameraBuffer, 0, NULL, &CameraStateData, 0, 0);
	ImmediateContext->UpdateSubresource(LightBuffer, 0, NULL, &LightStateData, 0, 0);

	ImmediateContext->ClearDepthStencilView(BackBufferDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	float clear_color[]{ 0.1f,0.4f,0.6f,1.0f };
	ImmediateContext->ClearRenderTargetView(BackBufferRenderTargetView, clear_color);

	CameraStateData.World = DirectX::XMMatrixIdentity();
	ImmediateContext->UpdateSubresource(CameraBuffer, 0, NULL, &CameraStateData, 0, 0);
	ImmediateContext->Draw(6, 0);
	
	for (int i = 0; i < 10; i++) {
		CameraStateData.World = DirectX::XMMatrixTranslation(i*0.1, i*0.1, i);
		ImmediateContext->UpdateSubresource(CameraBuffer, 0, NULL, &CameraStateData, 0, 0);
		ImmediateContext->Draw(6, 0);

	}
	

	SwapChain->Present(1, NULL);
	


}
