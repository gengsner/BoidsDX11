#pragma once
#include <vector>
#include "Mesh.h"
#include "Texture.h"

struct Model
{
	Mesh mesh;
	ComPtr<ID3D11ShaderResourceView> environmentTexture;
	std::vector<Texture> textures;
	std::vector<Texture> normalMaps;
	std::vector<Texture> materials;
};