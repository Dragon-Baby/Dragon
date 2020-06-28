#include "dgpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dragon
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}


	void Renderer::BeginScene()
	{
		m_SceneData->ViewProjectionMatrix = glm::mat4(0.0f);
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}