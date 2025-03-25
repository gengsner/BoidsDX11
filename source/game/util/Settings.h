#pragma once
#include "util/SettingsStructs.h"

struct LoadedWindowSettings
{
	bool fullscreen = false;
	int width = 1980;
	int height = 1020;
};

namespace Settings
{
	LoadedWindowSettings GetWindowSettings();

	void SaveBoidSimulationSettings(const SimulationSettings& aSimulationSettings,
									const GraphicsSettings& aGraphicsSettings,
									const PlayerSettings& aPlayerSettings);

	void LoadBoidSimulationSettings(SimulationSettings& aOutSimulationSettings,
								GraphicsSettings& aOutGraphicsSettings,
								PlayerSettings& aOutPlayerSettings);
};

