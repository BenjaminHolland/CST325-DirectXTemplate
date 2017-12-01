#include "TextureService.h"
#include "ScreenService.h"
#include <DXTK\WICTextureLoader.h>
using namespace std;
using namespace cst;
using namespace Microsoft::WRL;
cst::TextureService::TextureService()
{
	auto &screenService = ScreenService::current();
	screenService.with([&](ComPtr<ID3D11Device> device) {
		ComPtr<ID3D11ShaderResourceView> buffer;
		DirectX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(device.Get(), L"Noise1.png",NULL,&buffer));
		_textures.insert({ "Noise1",buffer });
	});
	screenService.with([&](ComPtr<ID3D11Device> device) {
		ComPtr<ID3D11ShaderResourceView> buffer;
		DirectX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(device.Get(), L"skybox_texture.jpg", NULL, &buffer));
		_textures.insert({ "Skybox",buffer });
	});
}
void TextureService::withResource(string name, function<void(ComPtr<ID3D11ShaderResourceView>)> execute) {
	execute(_textures.at(name));
}
TextureService & cst::TextureService::current()
{
	static TextureService current;
	return current;
}
