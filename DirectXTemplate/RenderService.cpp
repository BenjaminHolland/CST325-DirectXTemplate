#include "RenderService.h"
#include "ScreenService.h"
#include "ShaderService.h"
#include "ModelService.h"
#include "BufferService.h"
#include "TextureService.h"
#include "CameraService.h"
#include "Vertex.h"
#include <vector>
using namespace std;
using namespace cst;
using namespace Microsoft::WRL;
using namespace DirectX;
cst::RenderService::RenderService()
{
	auto &screenService = ScreenService::current();

	screenService.with([this](ComPtr<ID3D11Device> device) {
		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		device->CreateBlendState(&blendStateDesc, &defaultBlendState);

	});
	screenService.with([&](ComPtr<ID3D11Device> device) {
		D3D11_SAMPLER_DESC sd = {};
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;
		ThrowIfFailed(device->CreateSamplerState(&sd, &defaultSamplerState));
	});
	screenService.with([this](ComPtr<ID3D11Device> device) {
		D3D11_RASTERIZER_DESC			RS_CW, RS_Wire;

		RS_CW.AntialiasedLineEnable = FALSE;
		RS_CW.CullMode = D3D11_CULL_BACK;
		RS_CW.DepthBias = 0;
		RS_CW.DepthBiasClamp = 0.0f;
		RS_CW.DepthClipEnable = false;
		RS_CW.FillMode = D3D11_FILL_SOLID;
		RS_CW.FrontCounterClockwise =false;
		RS_CW.MultisampleEnable = FALSE;
		RS_CW.ScissorEnable = false;
		RS_CW.SlopeScaledDepthBias = 0.0f;
		device->CreateRasterizerState(&RS_CW, &defaultRasterizerState);

	});
	
	screenService.with([this](ComPtr<ID3D11Device> device) {
		D3D11_DEPTH_STENCIL_DESC		DS_ON;
		DS_ON.DepthEnable = true;
		DS_ON.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DS_ON.DepthFunc = D3D11_COMPARISON_LESS;
		// Stencil test parameters
		DS_ON.StencilEnable = true;
		DS_ON.StencilReadMask = 0xFF;
		DS_ON.StencilWriteMask = 0xFF;
		// Stencil operations if pixel is front-facing
		DS_ON.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DS_ON.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		DS_ON.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DS_ON.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing
		DS_ON.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DS_ON.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		DS_ON.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DS_ON.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		device->CreateDepthStencilState(&DS_ON, &defaultDepthStencilState);
	});

	screenService.with([&](ComPtr<ID3D11DeviceContext> context) {
		auto &shaderService = ShaderService::current();
		auto &modelService = ModelService::current();
		auto &bufferService = BufferService::current();
		auto &textureService = TextureService::current();
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		context->RSSetViewports(1, &vp);
		context->OMSetDepthStencilState(defaultDepthStencilState.Get(), 1);
		context->OMSetBlendState(defaultBlendState.Get(), DirectX::Colors::Transparent, 0xffffffff);
		context->RSSetState(defaultRasterizerState.Get());
		screenService.with([this, &screenService, &context](ComPtr<ID3D11RenderTargetView> renderTarget) {
			screenService.with([this, &context, &renderTarget](ComPtr<ID3D11DepthStencilView> depthStencil) {
				ID3D11RenderTargetView* views[] = { renderTarget.Get() };
				context->OMSetRenderTargets(1, views, depthStencil.Get());
			});
		});
		
		shaderService.with("PNCT", [this, &context](ComPtr<ID3D11InputLayout> layout) {
			context->IASetInputLayout(layout.Get());
		});
		vector<ID3D11Buffer*> buffers;
		
		bufferService.with([&](TransformBufferStateType &state, auto buffer) {
			buffers.push_back(buffer.Get());
		});
		bufferService.with([&](CameraBufferStateType &state, auto buffer) {
			buffers.push_back(buffer.Get());
		});
		bufferService.with([&](TimeBufferStateType &state, auto buffer) {
			buffers.push_back(buffer.Get());
		});
		shaderService.with("WVP", [&](ComPtr<ID3D11VertexShader> shader) {
			context->VSSetShader(shader.Get(), NULL, 0);
			context->VSSetConstantBuffers(0, buffers.size(), buffers.data());
		});

		modelService.with("Teapot", [&](ModelInfo model) {
			
			shaderService.with("Object", [&](ComPtr<ID3D11PixelShader> shader) {
				context->PSSetShader(shader.Get(), NULL, 0);
				context->PSSetConstantBuffers(0, buffers.size(), buffers.data());
				
				vector<ID3D11SamplerState*> samplers;
				samplers.push_back(defaultSamplerState.Get());
				context->PSSetSamplers(0, 1, samplers.data());
				
				vector<ID3D11ShaderResourceView*> resources;
				textureService.withResource("Noise1", [&](ComPtr<ID3D11ShaderResourceView> resource) {
					resources.push_back(resource.Get());
				});

				context->PSSetShaderResources(0, 1, resources.data());
			});
			unsigned int strides = sizeof(Vertex);
			unsigned int offsets = 0;
			ID3D11Buffer* buffers[] = { model.Buffer.Get() };

			context->IASetVertexBuffers(0, 1, buffers, &strides, &offsets);
		});
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
		
	});

	
}

RenderService & cst::RenderService::current()
{
	static RenderService current;
	return current;
}

void cst::RenderService::update()
{
	auto &ss = ScreenService::current();
	auto &bs = BufferService::current();
	auto &modelService = ModelService::current();
	auto &textureService = TextureService::current();
	auto &shaderService = ShaderService::current();
	auto &cameraService = CameraService::current();
	ss.with([&](ComPtr<ID3D11DeviceContext> context) {
		bs.with([&](CameraBufferStateType &state, auto buffer) {
			context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
		});
		bs.with([&](TransformBufferStateType &state, auto buffer) {
			context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
		});
		bs.with([&](TimeBufferStateType &state, auto buffer) {
			context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
		});
		ss.with([&](ComPtr<ID3D11RenderTargetView> renderTarget) {
			context->ClearRenderTargetView(renderTarget.Get(), DirectX::Colors::AliceBlue);
		});
		ss.with([&](ComPtr<ID3D11DepthStencilView> depthStencil) {
			context->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH , 1.0f, 0);
		});
		
		
		modelService.with("Teapot", [&](ModelInfo model) {
			
			bs.with([&](TransformBufferStateType &state, auto buffer) {
				XMStoreFloat4x4(&state.World, XMMatrixIdentity());
		
				context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
			});
			vector<ID3D11ShaderResourceView*> resources;
			textureService.withResource("Noise1", [&](ComPtr<ID3D11ShaderResourceView> resource) {
				resources.push_back(resource.Get());
			});
			context->PSSetShaderResources(0, 1, resources.data());

			shaderService.with("Object", [&](ComPtr < ID3D11PixelShader> shader) {
				context->PSSetShader(shader.Get(), NULL, 0);
			});


			unsigned int strides = sizeof(Vertex);
			unsigned int offsets = 0;
			ID3D11Buffer* buffers[] = { model.Buffer.Get() };
			context->IASetVertexBuffers(0, 1, buffers, &strides, &offsets);
			context->Draw(model.VertexCount,0);
		});
		modelService.with("Skybox", [&](ModelInfo model) {
			
			bs.with([&](TransformBufferStateType &state, auto buffer) {
				cameraService.withPosition([&](XMFLOAT3 pos) {
					XMStoreFloat4x4(&state.World, XMMatrixTranslation(pos.x, pos.y, pos.z));
				
				});
				context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
			});
			vector<ID3D11ShaderResourceView*> resources;
			textureService.withResource("Skybox", [&](ComPtr<ID3D11ShaderResourceView> resource) {
				resources.push_back(resource.Get());
			});
			context->PSSetShaderResources(0, 1, resources.data());

			shaderService.with("Skybox", [&](ComPtr < ID3D11PixelShader> shader) {
				context->PSSetShader(shader.Get(), NULL, 0);
			});


			unsigned int strides = sizeof(Vertex);
			unsigned int offsets = 0;
			ID3D11Buffer* buffers[] = { model.Buffer.Get() };
			context->IASetVertexBuffers(0, 1, buffers, &strides, &offsets);
			context->Draw(model.VertexCount, 0);
		});

		/*modelService.with("Skybox", [this, &context,&bs,&textureService](ModelInfo model) {
			bs.with([&](TransformBufferStateType &state, auto buffer) {
				context->UpdateSubresource(buffer.Get(), 0, NULL, &state, 0, 0);
			});
			

			vector<ID3D11ShaderResourceView*> resources;
			textureService.withResource("Skybox", [&](ComPtr<ID3D11ShaderResourceView> resource) {
				resources.push_back(resource.Get());
			});
			context->PSSetShaderResources(0, 1, resources.data());
			unsigned int strides = sizeof(Vertex);
			unsigned int offsets = 0;
			ID3D11Buffer* buffers[] = { model.Buffer.Get() };

			context->IASetVertexBuffers(0, 1, buffers, &strides, &offsets);
			context->Draw(model.VertexCount,0);
		});*/

	});
	ss.with([this](ComPtr<IDXGISwapChain> screen) {
		screen->Present(1, 0);
	});
}
