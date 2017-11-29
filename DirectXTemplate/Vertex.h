#pragma once
#include "Standard.h"
struct Vertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 Texture;
};
extern D3D11_INPUT_ELEMENT_DESC VERTEX_FORMAT[];