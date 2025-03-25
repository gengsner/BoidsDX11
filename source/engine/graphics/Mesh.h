#pragma once
#include <wrl/client.h>
#include "commonUtilities/Matrix4x4.h"

using Microsoft::WRL::ComPtr;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


class Mesh
{
public:

struct Vertex
{
	CommonUtilities::Vector3<float> posítion;
	CommonUtilities::Vector3<float> normal;
	float u, v;
};

	Mesh();
	~Mesh();
	void Init(const CommonUtilities::Matrix4x4<float>& aTransform, const int aVertexCount, const int aIndexCount, const size_t aVertexSize = sizeof(Vertex));
	virtual bool Initialize(ID3D11Device*, const CommonUtilities::Matrix4x4<float>& aTransform) { aTransform;  return false; }
	void Render(ID3D11DeviceContext*, const bool aIndexedFlag = false);
	void RenderInstanced(ID3D11DeviceContext*, const UINT aCount, const bool aIndexedFlag = false);
	void SetTransform(const CommonUtilities::Matrix4x4<float>& aTransform);
	const CommonUtilities::Matrix4x4<float>& GetTransform();

protected:
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myInputLayout;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;

	CommonUtilities::Matrix4x4<float> myTransform;
	int myIndexCount = 0;
	int myVertexCount = 0;
	size_t myVertexSize;
};