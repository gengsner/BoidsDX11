#pragma once
#include "BoidComputer.h"
#include "util/SettingsStructs.h"
#include "SimulationStructs.h"

class GraphicsEngine;
class Camera;

namespace CommonUtilities
{
	class InputHandler;
}

constexpr float SAVE_TEXT_DISPLAY_TIME = 1.2f;
constexpr float FPS_COLLECTION_TIME = 1.f;

using namespace CommonUtilities;

enum class SimulationMessage
{
	None,
	Save,
	Reset
};

class BoidSimulation
{
public:
	~BoidSimulation();
	bool Init(GraphicsEngine* aGraphicsEngine, Camera* aCamera);
	void ApplySettings(const GraphicsSettings& aGraphicsSettings, const SimulationSettings& aBoidSettings, const PlayerSettings& aPlayerSettings);
	bool InitBoidComputer();
	void ResetSimulation();
	void BeginFrame(const float aDeltaTime, const float aUnscaledDeltaTime);
	const SimulationMessage UpdateSimulationSettings();
	void ShowPlayerControls();
	void UpdatePlayer(InputHandler& aInputHandler);
	void UpdateFrameBuffer();
	void SimulateGPU();
	void Render();

	//Getters
	const bool GetPlayerControlled() const;
	const Vector3<float> GetClearColor() const;
	const Matrix4x4<float> GetPlayerFollowCameraTransform();
	const SimulationSettings& GetSimulationSettings() const;
	const GraphicsSettings& GetGraphicsSettings() const;
	const PlayerSettings& GetPlayerSettings() const;

private:
	Model myBoidModel;
	Model myCubeModel;
	SimulationSettings mySimSettings;
	GraphicsSettings myGraphicsSettings;
	PlayerSettings myPlayerSettings;

	Player myPlayer;
	BoidComputer myBoidComputer;
	FollowCamera myFollowCamera;
		
	GraphicsEngine* myGraphicsEngine = nullptr;
	Camera* myCamera = nullptr;
	
	float myDeltaTimeSum = 0;
	float myFrameCountSum = 0;
	float myDeltaTime = 0;
	float myLastFPS = 0.f;
	uint64_t myLastFPSUpdateFrame = 0;
	uint64_t myFrame = 0;
	unsigned int myCellCount = 0;
	float mySaveTimeStamp = -SAVE_TEXT_DISPLAY_TIME;
	bool myAutoHaltFlag = false;
	bool myFPSHaltFlag = false;
	bool myRestartSimulationFlag = false;
};

