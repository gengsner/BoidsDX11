#include "Mesh.h"
#include <d3d11.h>
#include <fstream>

Mesh::Mesh() = default;
Mesh::~Mesh() = default;


void Mesh::Init(const CommonUtilities::Matrix4x4<float>& aTransform, const int aVertexCount, const int aIndexCount, const size_t aVertexSize)
{
	myTransform = aTransform;
	myVertexCount = aVertexCount;
	myIndexCount = aIndexCount;
	myVertexSize = aVertexSize;
}

void Mesh::Render(ID3D11DeviceContext* context, const bool aIndexedFlag)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(myInputLayout.Get());
	unsigned int stride = (unsigned int)myVertexSize;
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(myVertexShader.Get(), nullptr, 0);
	context->PSSetShader(myPixelShader.Get(), nullptr, 0);
	aIndexedFlag ?
		context->DrawIndexed(myIndexCount, 0, 0) :
		context->Draw(myVertexCount, 0);
}

void Mesh::RenderInstanced(ID3D11DeviceContext* context, const UINT aCount, const bool aIndexedFlag)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(myInputLayout.Get());
	unsigned int stride = (unsigned int)myVertexSize;
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(myVertexShader.Get(), nullptr, 0);
	context->PSSetShader(myPixelShader.Get(), nullptr, 0);
	aIndexedFlag ?
		context->DrawIndexedInstanced(myIndexCount, aCount, 0, 0, 0) :
		context->DrawInstanced(myVertexCount, aCount, 0, 0);
}

void Mesh::SetTransform(const CommonUtilities::Matrix4x4<float>& aTransform)
{
	myTransform = aTransform;
}

const CommonUtilities::Matrix4x4<float>& Mesh::GetTransform()
{
	return myTransform;
}
