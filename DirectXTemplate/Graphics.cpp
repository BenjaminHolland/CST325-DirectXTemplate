#include "Graphics.h"
#include <d3dcompiler.h>
#include <array>
#include <iostream>
#include <fstream>
#include <xnamath.h>
#include <vector>
namespace Graphics {
	D3D_FEATURE_LEVEL FeatureLevel;
	
	ID3D11Device* Device;
	ID3D11DeviceContext* ImmediateContext;
	ID3D11RenderTargetView* BackBufferRenderTargetView;
	ID3D11PixelShader* BasicPixelShader;
	ID3D11VertexShader* BasicVertexShader;
	IDXGISwapChain* SwapChain;
	ID3D11InputLayout* VertexShaderLayout;
	ID3D11Buffer* TriangleVBuffer;
	ID3D11Buffer* TimeConstantBuffer;

	DWORD Width;
	DWORD Height;
	
	std::array<float, 4> _BackgroundColor = { 0.0f,0.0f,1.0f,1.0f };
	std::array<D3D_DRIVER_TYPE,3> _DriversToTry={
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	std::array<D3D_FEATURE_LEVEL,3> _FeatureLevels{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	struct {
		float Time;
		float pad[3];
	} TimeBuffer;

	HRESULT InitTimeBuffer() {
		D3D11_BUFFER_DESC vbd;
		
		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vbd.ByteWidth = 16;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA idata;
		ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
		idata.pSysMem = &TimeBuffer;
		return Device->CreateBuffer(&vbd, &idata, &TimeConstantBuffer);
	}

	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT3 Col;
	};
	HRESULT InitTriangle() {

		std::array<Vertex, 4> quad{
			Vertex({XMFLOAT3(-0.5f,-0.5f,0.5f),XMFLOAT3(1.0f,1.0f,1.0f)}),
			Vertex({XMFLOAT3(0.5f,-0.5f,0.5f),XMFLOAT3(0.6f,0.6f,0.6f)}),
			Vertex({XMFLOAT3(0.5f,0.5f,0.5f),XMFLOAT3(0.3f,0.3f,0.3f)}),
			Vertex({XMFLOAT3(-0.5f,0.5f,0.5f),XMFLOAT3(0.1f,0.1f,0.1f)})
		};

		std::vector<Vertex> model;
		model.push_back(quad[0]);
		model.push_back(quad[3]);
		model.push_back(quad[1]);
		model.push_back(quad[1]);
		model.push_back(quad[3]);
		model.push_back(quad[2]);

		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = sizeof(Vertex)*model.size();
		vbd.CPUAccessFlags = 0;
		
		D3D11_SUBRESOURCE_DATA idata;
		ZeroMemory(&idata, sizeof(D3D11_SUBRESOURCE_DATA));
		idata.pSysMem = model.data();

		HRESULT hr = Device->CreateBuffer(&vbd, &idata, &TriangleVBuffer);
		if (FAILED(hr)) return hr;
		return S_OK;
	}
	void InitPipeline() {
		ImmediateContext->OMSetRenderTargets(1, &BackBufferRenderTargetView, NULL);
		ImmediateContext->VSSetShader(BasicVertexShader, NULL, 0);
		ImmediateContext->PSSetShader(BasicPixelShader, NULL, 0);
		
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)Width;
		vp.Height = (FLOAT)Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		ImmediateContext->RSSetViewports(1, &vp);
		
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ImmediateContext->IASetVertexBuffers(0, 1, &TriangleVBuffer,&stride, &offset);
		ImmediateContext->IASetInputLayout(VertexShaderLayout);
		ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	}
	HRESULT InitPixelShader() {
		HRESULT hr;
		std::ifstream stream;
		stream.open("BasicPixelShader.cso",std::ios::binary);
		if (!stream.good())
			return E_FAIL;
		stream.seekg(0, std::ios::end);
		size_t fileSize = stream.tellg();
		char* ps_data = new char[fileSize];
		stream.seekg(0, std::ios::beg);
		stream.read(ps_data, fileSize);
		stream.close();

		hr = Device->CreatePixelShader(ps_data, fileSize, NULL, &BasicPixelShader);
		return hr;
	}
	HRESULT InitVertexShader() {
		HRESULT hr;
		std::ifstream stream;
		stream.open("BasicVertexShader.cso", std::ios::binary);
		if (!stream.good())
			return E_FAIL;
		stream.seekg(0, std::ios::end);
		size_t size = stream.tellg();
		char* data = new char[size];
		stream.seekg(0, std::ios::beg);
		stream.read(data, size);
		stream.close();

		hr = Device->CreateVertexShader(data, size, NULL, &BasicVertexShader);
		D3D11_INPUT_ELEMENT_DESC triangle_vert_desc[]{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
	
		hr = Device->CreateInputLayout(triangle_vert_desc, 2, data, size, &VertexShaderLayout);
		return hr;
	}
	HRESULT InitBackBufferRenderTargetView() {
		HRESULT hr;
		ID3D11Texture2D* pBackBuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr)) return hr;
		hr = Device->CreateRenderTargetView(pBackBuffer, NULL, &BackBufferRenderTargetView);
		pBackBuffer->Release();
		return hr;
	}
	HRESULT InitDeviceAndSwapChain(HWND hWnd) {
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		DWORD width = clientRect.right - clientRect.left;
		DWORD height = clientRect.bottom - clientRect.top;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.SampleDesc.Count = 1;
		scd.Windowed = TRUE;
		scd.OutputWindow = hWnd;

		HRESULT hr;
		for (size_t idx=0;idx<_DriversToTry.size();idx++){
			hr = D3D11CreateDeviceAndSwapChain(
				NULL, _DriversToTry[idx], NULL, NULL,
				_FeatureLevels.data(), _FeatureLevels.size(),
				D3D11_SDK_VERSION, &scd,
				&SwapChain, &Device, &FeatureLevel, &ImmediateContext);
			if (SUCCEEDED(hr)) break;
		}
		return hr;
	}
}

HRESULT Graphics::Init(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	Width = clientRect.right - clientRect.left;
	Height = clientRect.bottom - clientRect.top;
	HRESULT hr;
	hr=InitDeviceAndSwapChain(hWnd);
	if (FAILED(hr)) 
		return hr;
	hr = InitBackBufferRenderTargetView();
	if (FAILED(hr))
		return hr;
	hr = InitPixelShader();
	if (FAILED(hr)) 
		return hr;
	hr = InitVertexShader();
	if (FAILED(hr))
		return hr;
	hr = InitTriangle();
	if (FAILED(hr))
		return hr;
	hr = InitTimeBuffer();
	if (FAILED(hr))
		return hr;
	InitPipeline();
	return S_OK;
}

void Graphics::Render()
{
	TimeBuffer.Time += 0.01;
	ImmediateContext->UpdateSubresource(TimeConstantBuffer, 0, 0, &TimeBuffer, 0, 0);
	ImmediateContext->VSSetConstantBuffers(0, 1, &TimeConstantBuffer);
	ImmediateContext->PSSetConstantBuffers(0, 1, &TimeConstantBuffer);
	ImmediateContext->ClearRenderTargetView(BackBufferRenderTargetView, _BackgroundColor.data());
	ImmediateContext->Draw(6, 0);
		

	SwapChain->Present(0, 0);
}
