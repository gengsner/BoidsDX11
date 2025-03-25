#include "BoidShape.h"
#include <d3d11.h>
#include <fstream>

#define VERTICES 12
#define INDICES 0

using namespace CommonUtilities;

bool BoidShape::Initialize(ID3D11Device* device, const CommonUtilities::Matrix4x4<float>& aTransform, const bool aInstancedFlag)
{
	HRESULT result;
	Init(aTransform, VERTICES, INDICES);

	myTransform = aTransform;
	float size = 0.3f;

	Vector3<float> p0({ 0, 0, size * 2.f });
	Vector3<float> p1({ 0, size * 0.5f, -size });
	Vector3<float> p2({ size, -size, -size });
	Vector3<float> p3({ -size, -size, -size });

	Vector3<float> faceNorm0 = (p0 - p1).Cross(p0 - p2).GetNormalized();
	Vector3<float> faceNorm1 = (p0 - p2).Cross(p0 - p3).GetNormalized();
	Vector3<float> faceNorm2 = (p0 - p3).Cross(p0 - p1).GetNormalized();
	Vector3<float> faceNorm3 = (p1 - p3).Cross(p1 - p2).GetNormalized();

	Vertex vertices[VERTICES] = {
	{ p0, faceNorm0 }, { p1, faceNorm0 }, { p2, faceNorm0 },
	{ p0, faceNorm1 }, { p2, faceNorm1 }, { p3, faceNorm1 },
	{ p0, faceNorm2 }, { p3, faceNorm2 }, { p1, faceNorm2 },
	{ p1, faceNorm3 }, { p3, faceNorm3 }, { p2, faceNorm3 }
	};

	{
		// Create vertex buffer
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexData = { 0 };
		vertexData.pSysMem = vertices;
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	std::string vsData;
	{
		std::ifstream vsFile;
		if(aInstancedFlag)
			vsFile.open("../dependencies/BoidInstance_VS.cso", std::ios::binary);
		else
			vsFile.open("../dependencies/Player_VS.cso", std::ios::binary);
		vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		result = device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader);
		if (FAILED(result))
		{
			return false;
		}
		vsFile.close();
		std::ifstream psFile;

		psFile.open("../dependencies/Boid_PS.cso", std::ios::binary);

		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		result = device->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader);
		if (FAILED(result))
		{
			return false;
		}
		psFile.close();
	}
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		result = device->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &myInputLayout);
		if (FAILED(result))
			return false;
	}
	return true;
}

#undef INDICES
#undef VERTICES