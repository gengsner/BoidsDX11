#pragma once
#include "Mesh.h"

class Cube : public Mesh
{
public:
	bool Initialize(ID3D11Device* device, const CommonUtilities::Matrix4x4<float>& aTransform) override;
};

