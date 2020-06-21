#include "dgpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dragon
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}