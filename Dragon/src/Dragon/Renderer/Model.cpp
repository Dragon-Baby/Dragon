#include "dgpch.h"

#include "Model.h"

namespace Dragon
{
	Model::Model(std::string const& path, bool gamma)
		:gammaCorrection(gamma)
	{
		LoadModel(path);
	}
	void Model::Draw(std::shared_ptr<Shader>& shader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Draw(shader);
	}
	void Model::LoadModel(std::string const& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		//检查错误
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			DG_CORE_ERROR("Error::Assimp::{0}", importer.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
	}
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture2D>> textures;

		//遍历顶点
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			//位置
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			//法线
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			//纹理坐标
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		//遍历面
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//处理材质
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//diffuse
		std::vector<std::shared_ptr<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//specular
		std::vector<std::shared_ptr<Texture2D>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//法线贴图
		std::vector<std::shared_ptr<Texture2D>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		return Mesh(vertices, indices, textures);
	}
	std::vector<std::shared_ptr<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<std::shared_ptr<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < m_Textures.size(); j++)
			{
				if (std::strcmp(m_Textures[j]->GetPath().data(), str.C_Str()) == 0)
				{
					textures.push_back(m_Textures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				std::shared_ptr<Texture2D> texture;
				texture = Texture2D::Create(str.C_Str());
				texture->SetType(typeName);
				texture->SetPath(str.C_Str());
				textures.push_back(texture);
			}
		}
		return textures;
	}
}