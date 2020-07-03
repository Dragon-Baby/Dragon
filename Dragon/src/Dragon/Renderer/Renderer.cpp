#include "dgpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>

namespace Dragon
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}


	void Renderer::BeginScene(bool DepthTest, bool StencilTest, bool Blend)
	{
		if (DepthTest)
			glEnable(GL_DEPTH_TEST);	
		if (StencilTest)
			glEnable(GL_STENCIL_TEST);
		if (Blend)
			glEnable(GL_BLEND);
		m_SceneData->ViewProjectionMatrix = glm::mat4(0.0f);
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}
}