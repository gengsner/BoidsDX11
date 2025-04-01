#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ComputeShader;
struct ID3D11Buffer;
struct ID3D11UnorderedAccessView;
struct ID3D11ShaderResourceView;
class GraphicsEngine;
struct Boid;

typedef unsigned int UINT;

class BoidComputer
{
public:
	int Init(GraphicsEngine& aGraphicsEngine);
	void InitBoidTransforms();
	void RunBoidsGPUGridded(const UINT aBoidCount, const UINT aCellCount);
	void RunBoidsGPU(const UINT aBoidCount);
	void SwapBuffers();
	void BindStructuredBuffer();
	void UnbindStructuredBuffer();
	void UnInit();

private:
	void RunComputeShader(ID3D11ComputeShader* aComputeShader,
		UINT aSRVSlot, UINT aSRVCount, ID3D11ShaderResourceView** someSRV,
		UINT aUAVSlot, UINT aUAVCount, ID3D11UnorderedAccessView** someUAV,
		UINT X, UINT Y, UINT Z);

	Boid* DebugMapBoids(ID3D11Buffer** aBufffer);
	unsigned int* DebugMapCounts(ID3D11Buffer** aBufffer);

	ID3D11Device* gEDevice = nullptr;
	ID3D11DeviceContext* gEContext = nullptr;

	//Grid_CS
	ID3D11ComputeShader* countCS = nullptr;
	ID3D11ComputeShader* sumCS = nullptr;
	ID3D11ComputeShader* sweepCS = nullptr;
	ID3D11ComputeShader* blockSumCS = nullptr;
	ID3D11ComputeShader* groupBlockSumCS = nullptr;
	ID3D11ComputeShader* clearCS = nullptr;
	ID3D11ComputeShader* copyCS = nullptr;
	ID3D11ComputeShader* sortBoidsCS = nullptr;

	//Boid_CS
	ID3D11ComputeShader* initBoidCS = nullptr;
	ID3D11ComputeShader* runBoidCS = nullptr;
	ID3D11ComputeShader* runBoidGriddedCS = nullptr;

	ID3D11Buffer* sumBuffer = nullptr;
	ID3D11Buffer* unsortedSumBuffer = nullptr;
	ID3D11Buffer* sortingStageBuffer = nullptr;
	ID3D11UnorderedAccessView* uavSumBuffer = nullptr;
	ID3D11UnorderedAccessView* uavUnsortedSumBuffer = nullptr;

	ID3D11Buffer* boidsIn = nullptr;
	ID3D11UnorderedAccessView* uavBoidsIn = nullptr;

	ID3D11Buffer* boidsOut = nullptr;
	ID3D11ShaderResourceView* srvBoidsOut = nullptr;
	ID3D11UnorderedAccessView* uavBoidsOut = nullptr;
};

