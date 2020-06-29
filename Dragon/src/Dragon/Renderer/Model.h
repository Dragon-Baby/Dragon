#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

namespace Dragon
{
	class Model
	{
	public:
		Model(std::string const& path, bool gamma = false);

		void Draw(std::shared_ptr<Shader>& shader);

	private:
		void LoadModel(std::string const& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	private:
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		bool gammaCorrection;
	};
}
