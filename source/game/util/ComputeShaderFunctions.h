/*
 * Portions of this code are from a DirectX 11 Compute Shader sample.
 * Link: https://github.com/walbourn/directx-sdk-samples/blob/main/BasicCompute11/
 * Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License.
 */

#pragma once
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif

static HRESULT FindDXSDKShaderFileCch(WCHAR* aDestinationPath, int aSize, LPCWSTR aFileName)
{
	if (!aFileName || aFileName[0] == 0 || !aDestinationPath || aSize < 10)
		return E_INVALIDARG;

	WCHAR strExePath[MAX_PATH] = { 0 };
	WCHAR strExeName[MAX_PATH] = { 0 };
	WCHAR* strLastSlash = nullptr;
	GetModuleFileName(nullptr, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);
		*strLastSlash = 0;
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	wcscpy_s(aDestinationPath, aSize, aFileName);
	if (GetFileAttributes(aDestinationPath) != 0xFFFFFFFF)
		return S_OK;

	swprintf_s(aDestinationPath, aSize, L"%s\\..\\..\\%s\\%s", strExePath, strExeName, aFileName);
	if (GetFileAttributes(aDestinationPath) != 0xFFFFFFFF)
		return S_OK;

	wcscpy_s(aDestinationPath, aSize, aFileName);

	return E_FAIL;
}

static HRESULT CreateComputeShader(LPCWSTR aSourceFile, LPCSTR aFunctionName, ID3D11Device* aDevice, ID3D11ComputeShader** aOutShader)
{
	if (!aDevice || !aOutShader)
		return E_INVALIDARG;

	WCHAR str[MAX_PATH];
	HRESULT hr = FindDXSDKShaderFileCch(str, MAX_PATH, aSourceFile);
	if (FAILED(hr))
		return hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	const D3D_SHADER_MACRO defines[] =
	{
		nullptr, nullptr
	};

	LPCSTR pProfile = (aDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

	ID3DBlob* pErrorBlob = nullptr;
	ID3DBlob* pBlob = nullptr;
	hr = D3DCompileFromFile(str, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, aFunctionName, pProfile,
		dwShaderFlags, 0, &pBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		SAFE_RELEASE(pErrorBlob);
		SAFE_RELEASE(pBlob);

		return hr;
	}

	hr = aDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, aOutShader);

	SAFE_RELEASE(pErrorBlob);
	SAFE_RELEASE(pBlob);
	return hr;
}

static ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* aDevice, ID3D11DeviceContext* aImmediateContext, ID3D11Buffer* aBuffer)
{
	ID3D11Buffer* debugbuf = nullptr;

	D3D11_BUFFER_DESC desc = {};
	aBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	if (SUCCEEDED(aDevice->CreateBuffer(&desc, nullptr, &debugbuf)))
	{
		aImmediateContext->CopyResource(debugbuf, aBuffer);
	}

	return debugbuf;
}

static HRESULT CreateStructuredBuffer(ID3D11Device* aDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** aBufferOut)
{
	*aBufferOut = nullptr;

	D3D11_BUFFER_DESC desc = {};
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = uElementSize * uCount;
	desc.StructureByteStride = uElementSize;


	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return aDevice->CreateBuffer(&desc, &InitData, aBufferOut);
	}
	else
		return aDevice->CreateBuffer(&desc, nullptr, aBufferOut);
}

static HRESULT CreateConstantBuffer(ID3D11Device* aDevice, UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** aBufferOut)
{
	*aBufferOut = nullptr;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = ((uElementSize * uCount + 15) / 16) * 16;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return aDevice->CreateBuffer(&desc, &InitData, aBufferOut);
	}
	else
		return aDevice->CreateBuffer(&desc, nullptr, aBufferOut);
}

static HRESULT CreateBufferSRV(ID3D11Device* aDevice, ID3D11Buffer* aBuffer, ID3D11ShaderResourceView** aSRVOut)
{
	D3D11_BUFFER_DESC descBuf = {};
	aBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else
		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
		}
		else
		{
			return E_INVALIDARG;
		}

	return aDevice->CreateShaderResourceView(aBuffer, &desc, aSRVOut);
}

static HRESULT CreateBufferUAV(ID3D11Device* aDevice, ID3D11Buffer* aBuffer, ID3D11UnorderedAccessView** aUAVOut)
{
	D3D11_BUFFER_DESC descBuf = {};
	aBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else
		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
		}
		else
		{
			return E_INVALIDARG;
		}

	return aDevice->CreateUnorderedAccessView(aBuffer, &desc, aUAVOut);
}