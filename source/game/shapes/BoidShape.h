#pragma once
#include "graphics/Mesh.h"

class BoidShape : public Mesh
{

public:
	bool Initialize(ID3D11Device* device, const CommonUtilities::Matrix4x4<float>& aTransform, const bool aInstancedFlag = true);
};

