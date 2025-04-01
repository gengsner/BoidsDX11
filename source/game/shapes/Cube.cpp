#include "Cube.h"
#include <d3d11.h>
#include <fstream>

#define VERTICES 36
#define INDICES 0

using namespace CommonUtilities;

bool Cube::Initialize(ID3D11Device* device, const Matrix4x4<float>& aTransform)
{
	HRESULT result;
	Init(aTransform, VERTICES, INDICES);

	myTransform = aTransform;

	float size = 0.5f;

	Vector3<float> p0({ size, size, size });
	Vector3<float> p1({ size, size, -size });
	Vector3<float> p2({ -size, size, -size });
	Vector3<float> p3({ -size, size, size });
	Vector3<float> p4({ size, -size, size });
	Vector3<float> p5({ size, -size, -size });
	Vector3<float> p6({ -size, -size, -size });
	Vector3<float> p7({ -size, -size, size });

	Vector3<float> normalRight	{1,0,0};
	Vector3<float> normalLeft	{-1,0,0};
	Vector3<float> normalTop	{0,1,0};
	Vector3<float> normalBottom	{0,-1,0};
	Vector3<float> normalFront	{0,0,1};
	Vector3<float> normalBack	{0,0,-1};

	myVertexCount = 36;

	Vertex vertices[36] = {
		{ p0, normalFront }, { p1, normalFront }, { p4, normalFront },
		{ p1, normalFront }, { p5, normalFront }, { p4, normalFront },

		{ p2, normalBack }, { p3, normalBack }, { p6, normalBack },
		{ p3, normalBack }, { p7, normalBack }, { p6, normalBack },

		{ p3, normalLeft }, { p0, normalLeft }, { p7, normalLeft },
		{ p0, normalLeft }, { p4, normalLeft }, { p7, normalLeft },

		{ p1, normalRight }, { p2, normalRight }, { p5, normalRight },
		{ p2, normalRight }, { p6, normalRight }, { p5, normalRight },

		{ p0, normalTop }, { p2, normalTop }, { p1, normalTop },
		{ p0, normalTop }, { p3, normalTop }, { p2, normalTop },

		{ p4, normalBottom }, { p5, normalBottom }, { p6, normalBottom },
		{ p4, normalBottom }, { p6, normalBottom }, { p7, normalBottom }
	};

	{
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
		vsFile.open("../dependencies/Cube_VS.cso", std::ios::binary);
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