#include "dgpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Dragon
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		DG_CORE_ASSERT(windowHandle, "Window handle is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DG_CORE_ASSERT(status, "Failed to initailize GLAD!");

		DG_CORE_INFO("OpenGL info:");
		DG_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		DG_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		DG_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}