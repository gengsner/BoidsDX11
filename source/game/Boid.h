#pragma once
#include <commonUtilities/vector3.h>

struct Boid
{
	CommonUtilities::Vector3<float> pos{};
	unsigned int cellIndex;
	CommonUtilities::Vector3<float> vel{};
	unsigned int neighbours;
};

constexpr unsigned int MAX_BOIDS = 10000000;
constexpr unsigned int MAX_CELLS = 100000000;