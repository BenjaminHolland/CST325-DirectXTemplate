#pragma once
#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
namespace Graphics {

	HRESULT Init(HWND hwnd);
	void Render();
}