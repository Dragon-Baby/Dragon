#pragma once

#include "RenderCommand.h"
//#include "OrthographicCamera.h"
#include "Shader.h"

namespace Dragon
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}