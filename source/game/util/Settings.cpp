#include "Settings.h"
#include <nlohmann/json.hpp>
#include <fstream>

const std::string windowSettingsFile = "windowSettings.json";
const std::string simulationSettingsFile = "simulationSettings.json";
const std::string savedSimulationsSettingsFile = "../premake/Settings/simulationSettings.json";

LoadedWindowSettings Settings::GetWindowSettings()
{
	LoadedWindowSettings windowSettings;
	std::ifstream ifStream(windowSettingsFile);
	if (!ifStream)
		return windowSettings;

	nlohmann::json data;
	ifStream >> data;

	windowSettings.fullscreen = data["window"]["fullscreen"];
	windowSettings.width = data["window"]["width"];
	windowSettings.height = data["window"]["height"];

	return windowSettings;
}

void Settings::SaveBoidSimulationSettings(const SimulationSettings& aSimulationSettings, const GraphicsSettings& aGraphicsSettings, const PlayerSettings& aPlayerSettings)
{
	const auto& s = aSimulationSettings;
	const auto& g = aGraphicsSettings;
	const auto& p = aPlayerSettings;

	nlohmann::json settings =
	{
		//simulation
		{"boidCount", s.boidCount},
		{"griddingOn", s.griddingOn},
		{"cellSizeMult", s.cellSizeMult},
		{"gravity", s.gravity},
		{"visualRange", s.visualRange},
		{"protectedRange", s.protectedRange},
		{"fieldOfView", s.fieldOfView},
		{"cohesionFactor", s.cohesionFactor},
		{"separationFactor", s.separationFactor},
		{"alignmentFactor", s.alignmentFactor},
		{"maxSpeed", s.maxSpeed},
		{"minSpeed", s.minSpeed},
		{"turnSpeed", s.turnSpeed},
		{"turnMagin", s.turnMagin},
		{"minPos", { s.minPos.x, s.minPos.y, s.minPos.z }},
		{"maxPos", { s.maxPos.x, s.maxPos.y, s.maxPos.z }},
		//player
		{"boidAttraction", p.boidAttraction },
		{"maxVelocity", p.maxVelocity },
		{"acceleration", p.acceleration },
		{"rollAcceleration", p.rollAcceleration },
		{"mouseAcceleration", p.mouseAcceleration },
		//graphics
		{"renderBounds", g.renderBounds},
		{"dirLightDir",{ g.dirLight.dir.x, g.dirLight.dir.y, g.dirLight.dir.z }},
		{"dirLightColor",{ g.dirLight.color.x, g.dirLight.color.y, g.dirLight.color.z }},
		{"dirLightIntensity", g.dirLight.intensity},
		{"ambientLightColor",{ g.ambientLight.color.x, g.ambientLight.color.y, g.ambientLight.color.z }},
		{"ambientLightIntensity", g.ambientLight.intensity},
		{"boidColor",{ g.boidColor.x, g.boidColor.y, g.boidColor.z }},
		{"boidAltColor",{ g.boidAltColor.x, g.boidAltColor.y, g.boidAltColor.z }},
		{"boundsColor",{ g.boundsColor.x, g.boundsColor.y, g.boundsColor.z }},
		{"playerColor",{ g.playerColor.x, g.playerColor.y, g.playerColor.z }},
		{"clearColor",{ g.clearColor.x, g.clearColor.y, g.clearColor.z }},
		{"viewDist", g.viewDist }
	};

	{
		std::ofstream o(simulationSettingsFile);
		o << settings;
	}

	{
		std::ofstream o(savedSimulationsSettingsFile);
		o << settings;
	}
}

void Settings::LoadBoidSimulationSettings(SimulationSettings& aOutSimulationSettings, GraphicsSettings& aOutGraphicsSettings, PlayerSettings& aOutPlayerSettings)
{
	std::ifstream ifStream(simulationSettingsFile);
	if (!ifStream)
		return;

	nlohmann::json data;
	ifStream >> data;

	auto& s = aOutSimulationSettings;
	auto& g = aOutGraphicsSettings;
	auto& p = aOutPlayerSettings;
	//simulation
	s.boidCount = data["boidCount"];
	s.griddingOn = data["griddingOn"];
	s.cellSizeMult = data["cellSizeMult"];
	s.gravity = data["gravity"];
	s.visualRange = data["visualRange"];
	s.protectedRange = data["protectedRange"];
	s.fieldOfView = data["fieldOfView"];
	s.cohesionFactor = data["cohesionFactor"];
	s.separationFactor = data["separationFactor"];
	s.alignmentFactor = data["alignmentFactor"];
	s.maxSpeed = data["maxSpeed"];
	s.minSpeed = data["minSpeed"];
	s.turnSpeed = data["turnSpeed"];
	s.turnMagin = data["turnMagin"];
	s.minPos = { data["minPos"][0], data["minPos"][1], data["minPos"][2] };
	s.maxPos = { data["maxPos"][0], data["maxPos"][1], data["maxPos"][2] };
	s.maxPos = { data["maxPos"][0], data["maxPos"][1], data["maxPos"][2] };

	//player
	p.boidAttraction = data["boidAttraction"];
	p.maxVelocity = data["maxVelocity"];
	p.acceleration = data["acceleration"];
	p.rollAcceleration = data["rollAcceleration"];
	p.mouseAcceleration = data["mouseAcceleration"];

	//graphics
	g.renderBounds = data["renderBounds"];
	g.dirLight.dir = { data["dirLightDir"][0], data["dirLightDir"][1], data["dirLightDir"][2] };
	g.dirLight.color = { data["dirLightColor"][0], data["dirLightColor"][1], data["dirLightColor"][2] };
	g.dirLight.intensity = data["dirLightIntensity"];
	g.ambientLight.color = { data["ambientLightColor"][0], data["ambientLightColor"][1], data["ambientLightColor"][2] };
	g.ambientLight.intensity = data["ambientLightIntensity"];
	g.boidColor = { data["boidColor"][0], data["boidColor"][1], data["boidColor"][2] };
	g.boidAltColor = { data["boidAltColor"][0], data["boidAltColor"][1], data["boidAltColor"][2] };
	g.boundsColor = { data["boundsColor"][0], data["boundsColor"][1], data["boundsColor"][2] };
	g.playerColor = { data["playerColor"][0], data["playerColor"][1], data["playerColor"][2] };
	g.clearColor = { data["clearColor"][0], data["clearColor"][1], data["clearColor"][2] };
	g.viewDist = data["viewDist"];
}