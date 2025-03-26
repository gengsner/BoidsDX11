#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include "hlsl/CBuffer.h"
#include <string>

using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11SamplerState;

class Camera;
class Mesh;
class Texture;

struct RenderTarget
{
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
};

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	bool Init(HWND windowHandle, const bool aFullscreenFlag);
	FrameBufferData& GetFrameBufferData();
	void PrepareFrame(const Vector3<float>& aClearColor);
	void Render(Mesh* aMesh);
	void RenderNoDepth(Mesh* aMesh);
	void RenderBoids(Mesh* aMesh, const UINT aBoidCount);
	void EndFrame();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();


private:
	
	FrameBufferData myFrameBufferData = {};

	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myContext;
	ComPtr<IDXGISwapChain> mySwapChain;
	ComPtr<ID3D11RenderTargetView> myBackBuffer;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11DepthStencilView> myDepthBuffer;

	ComPtr<ID3D11SamplerState> mySamplerState;

	RenderTarget myWaterReflectionRenderTarget;
	ComPtr<ID3D11RasterizerState> myFrontFaceCullingRasterizerState;

	CommonUtilities::Vector2<int> myResolution;

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
};