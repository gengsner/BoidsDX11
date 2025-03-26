#include "GraphicsEngine.h"
#include "Mesh.h"
#include "Camera.h"

#define REPORT_DX_WARNINGS

GraphicsEngine::GraphicsEngine() = default;
GraphicsEngine::~GraphicsEngine()
{}


bool GraphicsEngine::Init(HWND windowHandle, const bool aFullscreenFlag)
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !aFullscreenFlag;

	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL flOut;
	static const D3D_FEATURE_LEVEL flvl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		flvl,
		sizeof(flvl) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		&flOut,
		&myContext
	);

	if (SUCCEEDED(result))
	{
		assert(flOut >= D3D_FEATURE_LEVEL_11_0);
	}

	ID3D11Texture2D* backBufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();
	myResolution = { static_cast<int>(textureDesc.Width), static_cast<int>(textureDesc.Height) };

	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), nullptr);

	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(textureDesc.Width);
	viewport.Height = static_cast<float>(textureDesc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	myContext->RSSetViewports(1, &viewport);

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescription.ByteWidth = sizeof(FrameBufferData);
	result = myDevice->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}
	bufferDescription.ByteWidth = sizeof(ObjectBufferData);
	result = myDevice->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}

	{
		ID3D11Texture2D* depthBufferTexture;
		D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
		depthBufferDesc.Width = static_cast<unsigned int>(textureDesc.Width);
		depthBufferDesc.Height = static_cast<unsigned int>(textureDesc.Height);
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferTexture);
		if (FAILED(result))
		{
			return false;
		}
		result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
		if (FAILED(result))
		{
			return false;
		}
		depthBufferTexture->Release();
	}

	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerState);
		if (FAILED(result))
		{
			return false;
		}
	}

	myFrameBufferData.resolution = myResolution;

	return true;
}

FrameBufferData& GraphicsEngine::GetFrameBufferData()
{
	return myFrameBufferData;
}

void GraphicsEngine::PrepareFrame(const Vector3<float>& aClearColor)
{
	myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &myFrameBufferData, sizeof(FrameBufferData));
	myContext->Unmap(myFrameBuffer.Get(), 0);

	myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	myContext->CSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	myContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());

	float color[4] = {aClearColor.x, aClearColor.y, aClearColor.z, 1.0f };
	myContext->ClearRenderTargetView(myBackBuffer.Get(), color);
	myContext->ClearDepthStencilView(
		myDepthBuffer.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}

void GraphicsEngine::Render(Mesh* aMesh)
{
	myContext->OMSetRenderTargets(
		1,
		myBackBuffer.GetAddressOf(),
		myDepthBuffer.Get()
	);
	ObjectBufferData objectBufferData = {};
	objectBufferData.modelToWorldMatrix = aMesh->GetTransform();
	myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &objectBufferData, sizeof(ObjectBufferData));
	myContext->Unmap(myObjectBuffer.Get(), 0);
	myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	aMesh->Render(myContext.Get());
}

void GraphicsEngine::RenderNoDepth(Mesh* aMesh)
{
	myContext->OMSetRenderTargets(
		1,
		myBackBuffer.GetAddressOf(),
		nullptr
	);
	ObjectBufferData objectBufferData = {};
	objectBufferData.modelToWorldMatrix = aMesh->GetTransform();
	myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &objectBufferData, sizeof(ObjectBufferData));
	myContext->Unmap(myObjectBuffer.Get(), 0);
	myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
	aMesh->Render(myContext.Get());
}

void GraphicsEngine::RenderBoids(Mesh* aMesh, const UINT aBoidCount)
{
	myContext->OMSetRenderTargets(
		1,
		myBackBuffer.GetAddressOf(),
		myDepthBuffer.Get()
	);
	myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	aMesh->RenderInstanced(myContext.Get(), aBoidCount);
}

void GraphicsEngine::EndFrame()
{
	mySwapChain->Present(1, 0);
}

ID3D11Device* GraphicsEngine::GetDevice()
{
	return *myDevice.GetAddressOf();
}

ID3D11DeviceContext* GraphicsEngine::GetContext()
{
	return *myContext.GetAddressOf();
}