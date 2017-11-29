#pragma once
#include "Standard.h"
#include <unordered_map>

#include <string>
namespace cst {
	class TextureService {
	private:
		TextureService();
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> _textures;
	public:
		TextureService(TextureService const&) = delete;
		static TextureService& current();
		void withResource(std::string textureName, std::function<void(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>)> execute);
	};
}