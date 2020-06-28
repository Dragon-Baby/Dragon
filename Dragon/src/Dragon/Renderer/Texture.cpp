#include "dgpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dragon
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			DG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		DG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}