#include "BoidSimulation.h"
#include "shapes/BoidShape.h"
#include "shapes/Cube.h"
#include "GraphicsEngine.h"
#include "Camera.h"

#include <imgui/imgui.h>
#include <string>

#include "Boid.h"
#include "commonUtilities/UtilityFunctions.h"
#include "commonUtilities/Quaternion.h"

using namespace CommonUtilities;
constexpr float IMGUI_SPACING = 200.f;
constexpr float MAX_BOIDS_PER_CELL = 50000;
constexpr float MIN_FRAME_TIME = 10.f;

BoidSimulation::~BoidSimulation()
{
	myBoidComputer.UnInit();
}

bool BoidSimulation::Init(GraphicsEngine* aGraphicsEngine, Camera* aCamera)
{
	myGraphicsEngine = aGraphicsEngine;
	myCamera = aCamera;

	CommonUtilities::Matrix4x4<float> transform;
	BoidShape boidShape;
	if (!boidShape.Initialize(myGraphicsEngine->GetDevice(), transform))
	{
		return false;
	}
	myBoidMesh = boidShape;

	if (!boidShape.Initialize(myGraphicsEngine->GetDevice(), transform, false))
	{
		return false;
	}
	myPlayer.mesh = boidShape;

	Cube cubeShape;
	if (!cubeShape.Initialize(myGraphicsEngine->GetDevice(), transform))
	{
		return false;
	}
	myCubeMesh = cubeShape;

	return true;
}

bool BoidSimulation::InitBoidComputer()
{
	int result = myBoidComputer.Init(*myGraphicsEngine);
	assert(result == 0);

	ResetSimulation();

	return result;
}

void BoidSimulation::ResetSimulation()
{
	myBoidComputer.InitBoidTransforms();

	myFPSHaltFlag = false;
	myAutoHaltFlag = false;

	Vector3<float> middle = mySimSettings.minPos + (mySimSettings.maxPos) * 0.5f;
	middle.z -= (mySimSettings.maxPos.z - mySimSettings.minPos.z) * 0.5f;

	Matrix4x4<float> m;
	m.SetTranslation(middle);
	myPlayer.transform = m;
	myCamera->SetTransform(m);
	myPlayer.velocity = 0.f;
}

void BoidSimulation::BeginFrame(const float aDeltaTime, const float aUnscaledDeltaTime)
{
	myDeltaTimeSum += aUnscaledDeltaTime;
	myFrameCountSum += aUnscaledDeltaTime;
	if (FPS_COLLECTION_TIME < myFrameCountSum)
	{
		myLastFPS = myFrameCountSum / (float)(myFrame - myLastFPSUpdateFrame);
		myLastFPS = FPS_COLLECTION_TIME / myLastFPS;
		myLastFPSUpdateFrame = myFrame;
		myFrameCountSum = 0.f;

		if (myLastFPS < MIN_FRAME_TIME)
			myFPSHaltFlag = true;
	}
	myDeltaTime = aDeltaTime;
	myFrame++;
}

const SimulationMessage BoidSimulation::UpdateSimulationSettings()
{
	SimulationMessage returnMsg = SimulationMessage::None;
	ImGui::Text("FPS"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(myLastFPS).c_str());

	ImGui::Text("Simulation"); ImGui::SameLine(IMGUI_SPACING);
	if (myDeltaTime == 0)
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Halted: delta time is '0'");
	else if (myAutoHaltFlag)
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Halted: invalid settings");
	else if (myFPSHaltFlag)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Halted: low fps. Try changing settings and restart simulation.");
	}
	else
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Running");

	if (ImGui::Button("Restart simulation (un-halt)"))
		returnMsg = SimulationMessage::Reset;

	ImGui::Text("BoidCount Count"); ImGui::SameLine(IMGUI_SPACING);
	ImVec4 boidTextColor = mySimSettings.boidCount > MAX_BOIDS ?
		ImVec4(1, 0, 0, 1) :
		ImVec4(0, 1, 0, 1);
	ImGui::TextColored(boidTextColor, std::to_string(mySimSettings.boidCount).c_str());

	auto color = mySimSettings.griddingOn ?
		(myCellCount == 0 || myCellCount > MAX_CELLS) ?
			ImVec4(1, 0, 0, 1) :
			ImVec4(0, 1, 0, 1) :
		ImVec4(1, 1, 0, 1);
	ImGui::Text("Cell Count"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(color, std::to_string(myCellCount).c_str());

	ImGui::Text("");
	bool clicked = myPlayer.controlled ?
		ImGui::Button("Control Camera") :
		ImGui::Button("Control Player");
	if (clicked)
	{
		myPlayer.controlled = !myPlayer.controlled;
		myPlayer.active = myPlayer.controlled;
	}
	ShowPlayerControls();

	if (ImGui::Button("Save Simulation Settings"))
	{
		returnMsg = SimulationMessage::Save;
		mySaveTimeStamp = myDeltaTimeSum;
	}

	if (mySaveTimeStamp + SAVE_TEXT_DISPLAY_TIME > (myDeltaTimeSum))
	{
		ImGui::SameLine(200.f);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Saving");
	}

	if (ImGui::CollapsingHeader("Boid Settings"))
	{
		ImGui::DragInt("BoidCount", &mySimSettings.boidCount, 10.f, 0, MAX_BOIDS);
		ImGui::DragFloat("Cohesion", &mySimSettings.cohesionFactor, 0.1f, 0, 100.f);
		ImGui::DragFloat("Separation", &mySimSettings.separationFactor, 0.1f, 0, 100.f);
		ImGui::DragFloat("Alignment", &mySimSettings.alignmentFactor, 0.1f, 0, 100.f);
		ImGui::DragFloat("Field Of View", &mySimSettings.fieldOfView, 1.f, 0.f, 360.f);
		ImGui::DragFloat("Visual Range", &mySimSettings.visualRange, 0.1f, mySimSettings.protectedRange, 100.f);
		ImGui::DragFloat("Protected Range", &mySimSettings.protectedRange, 0.1f, 0.1f, mySimSettings.visualRange);
		ImGui::DragFloat("Max Speed", &mySimSettings.maxSpeed, 0.1f, mySimSettings.minSpeed, 100.f);
		ImGui::DragFloat("Min Speed", &mySimSettings.minSpeed, 0.1f, 0.f, mySimSettings.maxSpeed);
		ImGui::DragFloat("Gravity", &mySimSettings.gravity, 0.1f, 0.f, 100.f);
	}
	if (ImGui::CollapsingHeader("Grid Settings"))
	{
		ImGui::Checkbox("Grid On", &mySimSettings.griddingOn);
		ImGui::DragFloat("Cell Size Mult", &mySimSettings.cellSizeMult, 0.1f, 1.f, 100.f);
		ImGui::DragFloat3("Min Pos", &mySimSettings.minPos.x, 0.1f, -10000.f, 0.f);
		ImGui::DragFloat3("Max Pos", &mySimSettings.maxPos.x, 0.1f, 0.f, 10000.f);
		ImGui::DragFloat("Turn Speed", &mySimSettings.turnSpeed, 0.1f, 0.1f, 100.f);
		ImGui::DragFloat("Turn Margin", &mySimSettings.turnMagin, 0.1f, 0.f, 100.f);
	}
	if (ImGui::CollapsingHeader("Graphics Settings"))
	{
		ImGui::Checkbox("Render Bounds", &myGraphicsSettings.renderBounds);
		ImGui::DragInt("Flock Size to fully set Alt Color", &myGraphicsSettings.flockSizeToFullyColor, 0.1f, 0, 10000);
		ImGui::DragFloat("View Distance", &myGraphicsSettings.viewDist, 0.1f, 0.f, 10000.f);
		if (ImGui::TreeNode("Objects Color"))
		{
			if (ImGui::TreeNode("Boid"))
			{
				ImGui::ColorPicker3("Boid Color", &myGraphicsSettings.boidColor.x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("BoidAlt"))
			{
				ImGui::ColorPicker3("Boid Alt Color", &myGraphicsSettings.boidAltColor.x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Bounds"))
			{
				ImGui::ColorPicker3("Bounds Color", &myGraphicsSettings.boundsColor.x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Player"))
			{
				ImGui::ColorPicker3("Player Color", &myGraphicsSettings.playerColor.x);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Clear Color"))
		{
			ImGui::ColorPicker3("Clear Color", &myGraphicsSettings.clearColor.x);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Direcional Light"))
		{
			ImGui::DragFloat3("Direction", &myGraphicsSettings.dirLight.dir.x, 0.03f, -1.f, 1.f);
			ImGui::ColorPicker3("Color", &myGraphicsSettings.dirLight.color.x);
			ImGui::DragFloat("Directional Light Intensity", &myGraphicsSettings.dirLight.intensity, 0.03f, 0.f, 1.f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("AmbientLight"))
		{
			ImGui::ColorPicker3("Color", &myGraphicsSettings.ambientLight.color.x);
			ImGui::DragFloat("Ambient Light Intensity", &myGraphicsSettings.ambientLight.intensity, 0.03f, 0.f, 1.f);
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Player Settings"))
	{
		ImGui::DragFloat("Boid Attraction", &myPlayerSettings.boidAttraction, 0.1f, -1000.f, 1000.f);
		ImGui::DragFloat("Max Velocity", &myPlayerSettings.maxVelocity, 0.1f, 0.f, 1000.f);
		ImGui::DragFloat("Acceleration", &myPlayerSettings.acceleration, 0.1f, 0.f, 1000.f);
		ImGui::DragFloat("Roll Acceleration", &myPlayerSettings.rollAcceleration, 0.001f, 0.f, 0.5f);
		ImGui::DragFloat("Mouse Sensitivity", &myPlayerSettings.mouseAcceleration, 0.001f, 0.f, 0.3f);
	}
	ImGui::Text("");
	return returnMsg;
}

void BoidSimulation::ShowPlayerControls()
{
	ImGui::Text("");
	if (!myPlayer.controlled) return;
	ImVec4 color(0.5f, 0.5f, 1, 1);
	ImGui::Text("Hide UI and start moving"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(color, "[ESC]");
	ImGui::Text("Adjust Speed"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(color, "[W], [S]");
	ImGui::Text("Roll"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(color, "[A], [D]");
	ImGui::Text("Pitch and Yaw"); ImGui::SameLine(IMGUI_SPACING);
	ImGui::TextColored(color, "Move Mouse");
	ImGui::Text("");
}

void BoidSimulation::UpdatePlayer(InputHandler& aInputHandler)
{
	ImGui::Text("~*~CONTROLS~*~");
	ImGui::Text("WASD");

	Vector3<float> input;
	auto delta = aInputHandler.GetMouseDeltaPos();

	float& yaw = myPlayer.rotVelocity.x;
	float& pitch = myPlayer.rotVelocity.y;
	float& roll = myPlayer.rotVelocity.z;
	float dampening = std::powf(0.1f, myDeltaTime);
	myPlayer.rotVelocity *= dampening;

	if (aInputHandler.GetKey('W')) input.y++;
	if (aInputHandler.GetKey('S')) input.y--;

	if (aInputHandler.GetKey('A')) input.x++;
	if (aInputHandler.GetKey('D')) input.x--;

	roll += input.x * myDeltaTime * myPlayerSettings.rollAcceleration;
	float m = myPlayerSettings.mouseAcceleration * 0.001f;
	pitch += (float)delta.y * m;
	yaw += (float)delta.x * m;

	Quaternion<float> q(yaw, pitch, roll);

	myPlayer.quaternion *= q;
	myPlayer.transform.SetRotation(myPlayer.quaternion.GetRotationMatrix4x4());

	myPlayer.velocity += input.y * myPlayerSettings.acceleration * myDeltaTime;
	myPlayer.velocity = Clamp<float>(myPlayer.velocity, 0, myPlayerSettings.maxVelocity);
	myPlayer.transform.SetTranslation(myPlayer.transform.GetTranslation() + myPlayer.transform.GetZ() * myPlayer.velocity * myDeltaTime);
}

void BoidSimulation::ApplySettings(const GraphicsSettings& aGraphicsSettings, const SimulationSettings& aBoidSettings, const PlayerSettings& aPlayerSettings)
{
	myGraphicsSettings = aGraphicsSettings;
	mySimSettings = aBoidSettings;
	myPlayerSettings = aPlayerSettings;
}

const Matrix4x4<float> BoidSimulation::GetPlayerFollowCameraTransform()
{
	myFollowCamera.quaternion.SLerp(myPlayer.quaternion, 0.08f);
	myFollowCamera.transform.SetRotation(myFollowCamera.quaternion.GetRotationMatrix4x4());

	myFollowCamera.velocity = Lerp<float>(myFollowCamera.velocity, myPlayer.velocity, 0.08f);

	Vector3<float> camPos = myPlayer.transform.GetTranslation() - myFollowCamera.transform.GetZ() * (7.f + myFollowCamera.velocity * 0.02f) + myPlayer.transform.GetY() * 1.7f;
	myFollowCamera.transform.SetTranslation(camPos);
	return myFollowCamera.transform;
}

void BoidSimulation::UpdateFrameBuffer()
{
	FrameBufferData& frameBufferData = myGraphicsEngine->GetFrameBufferData();

	frameBufferData.worldToClipMatrix = myCamera->GetWorldToClipMatrix();
	frameBufferData.lightDir = myGraphicsSettings.dirLight.dir.GetNormalized();
	frameBufferData.dirLightColor = myGraphicsSettings.dirLight.color;
	frameBufferData.dirLightIntensity = myGraphicsSettings.dirLight.intensity;

	frameBufferData.ambientLightColor = myGraphicsSettings.ambientLight.color;
	frameBufferData.ambientLightIntensity = myGraphicsSettings.ambientLight.intensity;

	frameBufferData.boidColor = myGraphicsSettings.boidColor;
	frameBufferData.boidAltColor = myGraphicsSettings.boidAltColor;
	frameBufferData.flockSizeToFullyColor = myGraphicsSettings.flockSizeToFullyColor;
	frameBufferData.boundsColor = myGraphicsSettings.boundsColor;
	frameBufferData.playerColor = myGraphicsSettings.playerColor;

	frameBufferData.fogColor = myGraphicsSettings.clearColor;
	frameBufferData.fogDist = myGraphicsSettings.viewDist;

	frameBufferData.camPos = myCamera->GetPos();
	frameBufferData.deltaTime = myDeltaTime;

	frameBufferData.cohesionFactor = mySimSettings.cohesionFactor;
	frameBufferData.separationFactor = mySimSettings.separationFactor;
	frameBufferData.alignmentFactor = mySimSettings.alignmentFactor;
	frameBufferData.fieldOfViewPercent = mySimSettings.fieldOfView / 360.f;
	frameBufferData.visualRangeSqr = mySimSettings.visualRange * mySimSettings.visualRange;
	frameBufferData.protectedRangeSqr = mySimSettings.protectedRange * mySimSettings.protectedRange;
	frameBufferData.gravity = mySimSettings.gravity;

	frameBufferData.maxSpeed = mySimSettings.maxSpeed;
	frameBufferData.minSpeed = mySimSettings.minSpeed;
	frameBufferData.turnMargin = mySimSettings.turnMagin;
	frameBufferData.turnSpeed = mySimSettings.turnSpeed;

	frameBufferData.minPos = mySimSettings.minPos;
	frameBufferData.maxPos = mySimSettings.maxPos;

	frameBufferData.playerAttraction = myPlayerSettings.boidAttraction;
	frameBufferData.playerPosition = myPlayer.transform.GetTranslation();
	frameBufferData.playerFuturePosition = myPlayer.transform.GetTranslation() + myPlayer.transform.GetZ() * max(0.1f, myPlayer.velocity);

	auto cubeSize = mySimSettings.maxPos - mySimSettings.minPos;

	float cellSize = mySimSettings.visualRange * mySimSettings.cellSizeMult;

	frameBufferData.cellSize = cellSize;

	frameBufferData.gridDims = {
		(unsigned int)(ceil(cubeSize.x / cellSize)),
		(unsigned int)(ceil(cubeSize.y / cellSize)),
		(unsigned int)(ceil(cubeSize.z / cellSize)) };

	myCellCount = (frameBufferData.gridDims.x * frameBufferData.gridDims.y * frameBufferData.gridDims.z);
	frameBufferData.cellCount = myCellCount;

	frameBufferData.boidCount = mySimSettings.boidCount;
	auto cubePos = (mySimSettings.minPos + mySimSettings.maxPos) * 0.5f;

	bool invalidSettings = (
		cubeSize.x <= 0
		|| cubeSize.y <= 0
		|| cubeSize.z <= 0
		|| myCellCount == 0
		|| myCellCount > MAX_CELLS
		|| mySimSettings.boidCount > MAX_BOIDS
		|| (mySimSettings.griddingOn && mySimSettings.boidCount / myCellCount > MAX_BOIDS_PER_CELL)
		|| (!mySimSettings.griddingOn && mySimSettings.boidCount > MAX_BOIDS_PER_CELL)
		);

	myAutoHaltFlag = invalidSettings;

	if (!myAutoHaltFlag)
	{
		myCubeMesh.SetTransform({
			cubeSize.x, 0, 0, 0,
			0, cubeSize.y, 0, 0,
			0, 0, cubeSize.z, 0,
			cubePos.x, cubePos.y, cubePos.z, 1,
			});
	}
}

void BoidSimulation::SimulateGPU()
{
	if (myAutoHaltFlag || myFPSHaltFlag || myDeltaTime == 0)
		return;

	if (mySimSettings.griddingOn)
	{
		myBoidComputer.RunBoidsGPUGridded(mySimSettings.boidCount, myCellCount);
	}
	else
	{
		myBoidComputer.RunBoidsGPU(mySimSettings.boidCount);
		myBoidComputer.SwapBuffers();
	}
}

void BoidSimulation::Render()
{
	if (myGraphicsSettings.renderBounds)
		myGraphicsEngine->RenderNoDepth(&myCubeMesh);

	if (myPlayer.active)
	{
		myPlayer.mesh.SetTransform(myPlayer.transform);
		myGraphicsEngine->Render(&myPlayer.mesh);
	}

	myBoidComputer.BindStructuredBuffer();
	myGraphicsEngine->RenderBoids(&myBoidMesh, min(MAX_BOIDS, (UINT)mySimSettings.boidCount));
	myBoidComputer.UnbindStructuredBuffer();
}

const bool BoidSimulation::GetPlayerControlled() const
{
	return myPlayer.controlled;
}

const Vector3<float> BoidSimulation::GetClearColor() const
{
	return myGraphicsSettings.clearColor;
}

const SimulationSettings& BoidSimulation::GetSimulationSettings() const
{
	return mySimSettings;
}

const GraphicsSettings& BoidSimulation::GetGraphicsSettings() const
{
	return myGraphicsSettings;
}

const PlayerSettings& BoidSimulation::GetPlayerSettings() const
{
	return myPlayerSettings;
}