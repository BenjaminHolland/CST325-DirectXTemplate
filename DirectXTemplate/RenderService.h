#pragma once
#include "Standard.h"
namespace cst {
	class RenderService {
	private:
		RenderService();
		Microsoft::WRL::ComPtr<ID3D11BlendState> defaultBlendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> defaultRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> defaultDepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> defaultSamplerState;
	public:
		RenderService(RenderService const&) = delete;
		static RenderService & current();
		void update();

	};
}