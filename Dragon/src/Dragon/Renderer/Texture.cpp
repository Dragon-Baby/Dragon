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

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, const std::string& directory)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path,directory);
		}

		DG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::vector<std::string>& faces)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTextureCube>(faces);
		}

		DG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::vector<std::string>& faces, const std::string& directory)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTextureCube>(faces);
		}

		DG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}