#pragma once

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, unsigned char* rgbaPixels, int width, int height, ID3D11DeviceContext* context, const bool aForceSRGB);
	bool Bind(ID3D11DeviceContext* context, int slot);

private:
	ComPtr<ID3D11ShaderResourceView> myShaderResourceView;
};

