#include "BoidComputer.h"
#include <stdio.h>
#include <crtdbg.h>
#include "util/ComputeShaderFunctions.h"
#include "commonUtilities/Vector2.h"
#include "Boid.h"
#include <cstring>
#include "GraphicsEngine.h"
#include "hlsl/ComputeShaderDefines.h"
#include <unordered_map>

using namespace CommonUtilities;

int BoidComputer::Init(GraphicsEngine& aGraphicsEngine)
{
	gEDevice = aGraphicsEngine.GetDevice();
	gEContext = aGraphicsEngine.GetContext();

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Boid_CS.hlsl", "main", gEDevice, &runBoidCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Boid_CS.hlsl", "mainGridded", gEDevice, &runBoidGriddedCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Boid_CS.hlsl", "init", gEDevice, &initBoidCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "count", gEDevice, &countCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "sum", gEDevice, &sumCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "sweepPrefixSum", gEDevice, &sweepCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "blockSum", gEDevice, &blockSumCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "groupBlockSum", gEDevice, &groupBlockSumCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "copy", gEDevice, &copyCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "sort", gEDevice, &sortBoidsCS)))
		return 1;

	if (FAILED(CreateComputeShader(L"../source/engine/hlsl/Grid_CS.hlsl", "clear", gEDevice, &clearCS)))
		return 1;

	CreateStructuredBuffer(gEDevice, sizeof(unsigned int), MAX_CELLS, nullptr, &countBuffer);
	CreateBufferUAV(gEDevice, countBuffer, &uavCountBuffer);

	CreateStructuredBuffer(gEDevice, sizeof(unsigned int), MAX_CELLS, nullptr, &tempCountBuffer);
	CreateBufferUAV(gEDevice, tempCountBuffer, &uavTempCountBuffer);

	std::array<UINT, 2> iterInit = { 1, DOUBLE_THREAD_GROUP_SIZE };
	CreateConstantBuffer(gEDevice, sizeof(unsigned int), 2, &iterInit, &groupSumIteration);

	CreateStructuredBuffer(gEDevice, sizeof(Boid), MAX_BOIDS, nullptr, &boidsIn);
	CreateBufferUAV(gEDevice, boidsIn, &uavBoidsIn);

	CreateStructuredBuffer(gEDevice, sizeof(Boid), MAX_BOIDS, nullptr, &boidsOut);
	CreateBufferUAV(gEDevice, boidsOut, &uavBoidsOut);
	CreateBufferSRV(gEDevice, boidsOut, &srvBoidsOut);
	
	return 0;
}

void BoidComputer::InitBoidTransforms()
{
	ID3D11UnorderedAccessView* aUAVViews[2] = { uavBoidsIn, uavBoidsOut };
	RunComputeShader(initBoidCS, 0, 0, nullptr, 0, 2, aUAVViews,
		(MAX_BOIDS + THREAD_GROUP_SIZE - 1) / THREAD_GROUP_SIZE, 1, 1);

	gEContext->CSSetConstantBuffers(1, 1, &groupSumIteration);
}


void BoidComputer::RunBoidsGPUGridded(const UINT aBoidCount, const UINT aCellCount)
{
	ID3D11UnorderedAccessView* aUAVViews[4] = { uavBoidsIn, uavBoidsOut, uavCountBuffer, uavTempCountBuffer/*, uavGroupSumIterationBuffer*/ };

	UINT threadGroupCell = (aCellCount + THREAD_GROUP_SIZE - 1) / THREAD_GROUP_SIZE;
	threadGroupCell;
	UINT halfThreadGroupCell = (aCellCount + DOUBLE_THREAD_GROUP_SIZE - 1) / DOUBLE_THREAD_GROUP_SIZE;
	UINT threadGroupBoid = (aBoidCount + THREAD_GROUP_SIZE - 1) / THREAD_GROUP_SIZE;

	gEContext->CSSetUnorderedAccessViews(0, 4, aUAVViews, nullptr);

	gEContext->CSSetShader(clearCS, nullptr, 0);
	gEContext->Dispatch(halfThreadGroupCell, 1, 1);

	gEContext->CSSetShader(countCS, nullptr, 0);
	gEContext->Dispatch(threadGroupBoid, 1, 1);

#define PARALLEL_SUM
#ifdef PARALLEL_SUM	
#define PARALLEL_BLOCK
#ifdef PARALLEL_BLOCK
	UINT coverage = 1;
	UINT dispatch = halfThreadGroupCell;
	float groupsToHandle = (float)aCellCount;
	bool multipleDispatches = DOUBLE_THREAD_GROUP_SIZE < (aCellCount / 2);

	gEContext->CSSetShader(sweepCS, nullptr, 0);
	while (coverage < (aCellCount / 2))
	{
		gEContext->Dispatch(dispatch, 1, 1);
		coverage *= DOUBLE_THREAD_GROUP_SIZE;

		if (coverage < (aCellCount / 2))
		{
			groupsToHandle = groupsToHandle / (float)DOUBLE_THREAD_GROUP_SIZE;
			dispatch = ((UINT)groupsToHandle + DOUBLE_THREAD_GROUP_SIZE - 1) / DOUBLE_THREAD_GROUP_SIZE;
			UINT nextCoverage = coverage * coverage;
			if (nextCoverage == 0)
			{
				assert(dispatch == 1);	
			}
			std::array<UINT, 2> iterInit = { coverage, nextCoverage };
			gEContext->UpdateSubresource(groupSumIteration, 0, nullptr, &iterInit, 0, 0);
		}
	}
	
	gEContext->CSSetShader(groupBlockSumCS, nullptr, 0);
	if (multipleDispatches)
	{
		coverage /= DOUBLE_THREAD_GROUP_SIZE;
	}
	while (coverage > 1)
	{
		UINT lastCoverage = coverage;
		coverage /= DOUBLE_THREAD_GROUP_SIZE;
		std::array<UINT, 2> iterInit = { coverage, lastCoverage };
		gEContext->UpdateSubresource(groupSumIteration, 0, nullptr, &iterInit, 0, 0);
		groupsToHandle *= DOUBLE_THREAD_GROUP_SIZE;
		dispatch = ((UINT)(groupsToHandle - 1.f) + DOUBLE_THREAD_GROUP_SIZE - 1) / DOUBLE_THREAD_GROUP_SIZE;
		gEContext->Dispatch(dispatch, 1, 1);
	}
#else
	gEContext->CSSetShader(sweepCS, nullptr, 0);
	gEContext->Dispatch(halfThreadGroupCell, 1, 1);
	gEContext->CSSetShader(blockSumCS, nullptr, 0);
	gEContext->Dispatch(1, 1, 1);
#endif
#else
	gEContext->CSSetShader(sumCS, nullptr, 0);
	gEContext->Dispatch(1, 1, 1);
#endif // PARALLEL_SUM

	gEContext->CSSetShader(copyCS, nullptr, 0);
	gEContext->Dispatch(threadGroupCell, 1, 1);

	gEContext->CSSetShader(sortBoidsCS, nullptr, 0);
	gEContext->Dispatch(threadGroupBoid, 1, 1);

	gEContext->CSSetShader(runBoidGriddedCS, nullptr, 0);
	gEContext->Dispatch(threadGroupBoid, 1, 1);

	gEContext->CSSetShader(nullptr, nullptr, 0);
	ID3D11UnorderedAccessView* uavNull[4] = { nullptr, nullptr, nullptr, nullptr };
	gEContext->CSSetUnorderedAccessViews(0, 4, uavNull, nullptr);
}

void BoidComputer::RunBoidsGPU(const UINT aBoidCount)
{
	ID3D11UnorderedAccessView* aUAVViews[3] = { uavBoidsIn, uavBoidsOut, uavCountBuffer };
	RunComputeShader(runBoidCS, 0, 0, nullptr, 0, 3, aUAVViews,
		(aBoidCount + THREAD_GROUP_SIZE - 1) / THREAD_GROUP_SIZE, 1, 1);
}

void BoidComputer::SwapBuffers()
{
	std::swap(uavBoidsIn, uavBoidsOut);

}

void BoidComputer::BindStructuredBuffer()
{
	gEContext->VSSetShaderResources(0, 1, &srvBoidsOut);
}

void BoidComputer::UnbindStructuredBuffer()
{
	ID3D11ShaderResourceView* srvNull[1] = { nullptr };
	gEContext->VSSetShaderResources(0, 1, srvNull);
}

void BoidComputer::RunComputeShader(ID3D11ComputeShader* aComputeShader, UINT aSRVSlot, UINT aSRVCount, ID3D11ShaderResourceView** someSRV, UINT aUAVSlot, UINT aUAVCount, ID3D11UnorderedAccessView** someUAV, UINT X, UINT Y, UINT Z)
{
	gEContext->CSSetShader(aComputeShader, nullptr, 0);
	gEContext->CSSetShaderResources(aSRVSlot, aSRVCount, someSRV);
	gEContext->CSSetUnorderedAccessViews(aUAVSlot, aUAVCount, someUAV, nullptr);

	gEContext->Dispatch(X, Y, Z);

	gEContext->CSSetShader(nullptr, nullptr, 0);

	ID3D11ShaderResourceView* nullSRV = nullptr;
	for (UINT slot = aSRVSlot; slot < aSRVSlot + aSRVCount; slot++)
	{
		gEContext->CSSetShaderResources(slot, 1, &nullSRV);
	}

	ID3D11UnorderedAccessView* nullUAV = nullptr;
	for (UINT slot = aUAVSlot; slot < aUAVSlot + aUAVCount; slot++)
	{
		gEContext->CSSetUnorderedAccessViews(slot, 1, &nullUAV, nullptr);
	}
}

unsigned int* BoidComputer::DebugMapCounts(ID3D11Buffer** aBufffer)
{
	ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(gEDevice, gEContext, *aBufffer);
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	unsigned int* p;

	gEContext->Map(debugbuf, 0, D3D11_MAP_READ, 0, &MappedResource);

	p = (unsigned int*)MappedResource.pData;

	gEContext->Unmap(debugbuf, 0);
	SAFE_RELEASE(debugbuf);

	return p;
}

Boid* BoidComputer::DebugMapBoids(ID3D11Buffer** aBufffer)
{
	{
		__debugbreak();
		ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(gEDevice, gEContext, *aBufffer);
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		Boid* p;

		gEContext->Map(debugbuf, 0, D3D11_MAP_READ, 0, &MappedResource);

		p = (Boid*)MappedResource.pData;

		gEContext->Unmap(debugbuf, 0);
		SAFE_RELEASE(debugbuf);
		return p;
	}
}

void BoidComputer::UnInit()
{
	SAFE_RELEASE(boidsIn);
	SAFE_RELEASE(boidsOut);
	SAFE_RELEASE(countBuffer);
	SAFE_RELEASE(tempCountBuffer);
	SAFE_RELEASE(groupSumIteration);

	SAFE_RELEASE(uavBoidsIn);
	SAFE_RELEASE(uavBoidsOut);
	SAFE_RELEASE(srvBoidsOut);
	SAFE_RELEASE(uavCountBuffer);
	SAFE_RELEASE(uavTempCountBuffer);

	SAFE_RELEASE(runBoidCS);
	SAFE_RELEASE(runBoidGriddedCS);
	SAFE_RELEASE(initBoidCS);
	SAFE_RELEASE(clearCS);
	SAFE_RELEASE(countCS);
	SAFE_RELEASE(sumCS);
	SAFE_RELEASE(sortBoidsCS);
	SAFE_RELEASE(sweepCS);
	SAFE_RELEASE(blockSumCS);
	SAFE_RELEASE(copyCS);
	SAFE_RELEASE(groupBlockSumCS);
}