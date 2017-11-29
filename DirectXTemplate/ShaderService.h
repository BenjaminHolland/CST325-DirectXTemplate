#pragma once
#include "Standard.h"

#include <unordered_map>
#include <string>
namespace cst {
	class ShaderService {
	private:
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11VertexShader>> _vs;
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> _ps;
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11InputLayout>> _layouts;
		
		ShaderService();
	public:
		ShaderService(ShaderService const&) = delete;
		static ShaderService& current();
		void with(std::string shaderName,std::function<void(Microsoft::WRL::ComPtr<ID3D11VertexShader>)> execute);
		void with(std::string shaderName,std::function<void(Microsoft::WRL::ComPtr<ID3D11PixelShader>)> execute);
		void with(std::string layoutName, std::function<void(Microsoft::WRL::ComPtr<ID3D11InputLayout>)> execute);
		

	};
}