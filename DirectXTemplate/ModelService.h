#pragma once
#include <unordered_map>
#include <string>
#include "Standard.h"
namespace cst {
	struct ModelInfo {
		Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
		size_t VertexCount;
	};
	class ModelService {
	private:
		std::unordered_map<std::string, ModelInfo> _models;
		ModelService();
	public:
		ModelService(ModelService const&) = delete;
		static ModelService& current();
		void with(std::string modelName, std::function<void(ModelInfo)> execute);
	};
}