#include "Texture.h"

#include <d3d11.h>

Texture::Texture() = default;	

Texture::~Texture() = default;

bool Texture::Initialize(ID3D11Device* device, unsigned char* rgbaPixels, int width, int height, ID3D11DeviceContext* context, const bool aForceSRGB)
{
	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	if (aForceSRGB)
	{
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	else
	{
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.MipLevels = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(device->CreateTexture2D(&desc, nullptr, &texture)))
		return false;

	HRESULT hr = device->CreateShaderResourceView(texture, NULL, &myShaderResourceView);
	if (FAILED(hr))
	{
		return false;
	}

	context->UpdateSubresource(texture, 0, nullptr, (void*)rgbaPixels, width * 4, width* height * 4);
	context->GenerateMips(myShaderResourceView.Get());

	texture->Release();

	return true;
}

bool Texture::Bind(ID3D11DeviceContext* context, int slot)
{
	context->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());

	return true;
}
