#include "ShaderService.h"
#include "ScreenService.h"
#include "Vertex.h"
using namespace std;
using namespace cst;
using namespace Microsoft::WRL;
using namespace DirectX;
cst::ShaderService::ShaderService() :_vs(), _ps(), _layouts()
{
	auto &screenService = cst::ScreenService::current();
	screenService.with([this, &screenService](ComPtr<ID3D11Device> device) {
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		if (FAILED(D3DCompileFromFile(L"VertexShader.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", NULL,NULL, &shaderBlob, &errorBlob))) {
			printf("%s\n", (char*)errorBlob->GetBufferPointer());
			throw exception((char*)errorBlob->GetBufferPointer(),errorBlob->GetBufferSize());
		}
		ComPtr<ID3D11VertexShader> vs;
		ComPtr<ID3D11InputLayout> layout;
		ComPtr<ID3D11PixelShader> ps;
		ThrowIfFailed(device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vs));
		ThrowIfFailed(device->CreateInputLayout(VERTEX_FORMAT, 4, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &layout));
		if (FAILED(D3DCompileFromFile(L"PixelShader.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", NULL, NULL, &shaderBlob, &errorBlob))) {
			throw exception((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &ps));
		_vs.insert({ "Main",vs });
		_layouts.insert({ "Main",layout });
		_ps.insert({ "Main",ps });
	});

}

ShaderService & cst::ShaderService::current()
{
	static ShaderService current;
	return current;
}

void cst::ShaderService::with( std::string shaderName,std::function<void(Microsoft::WRL::ComPtr<ID3D11VertexShader>)> execute)
{
	execute(_vs.at(shaderName));
}
void cst::ShaderService::with(std::string shaderName,std::function<void(Microsoft::WRL::ComPtr<ID3D11PixelShader>)> execute ) {
	execute(_ps.at(shaderName));
}
void cst::ShaderService::with(std::string layoutName,std::function<void(Microsoft::WRL::ComPtr<ID3D11InputLayout>)> execute) {
	execute(_layouts.at(layoutName));
}