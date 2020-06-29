#pragma once

#include "Dragon/Renderer/Texture.h"

namespace Dragon
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const std::string& path, const std::string& directory);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual std::string GetType() const override { return m_Type; }
		virtual void SetType(std::string type) override { m_Type = type; }
		virtual std::string GetPath() const override { return m_Path; }
		virtual void SetPath(std::string path) override { m_Path = path; }
		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		std::string m_Type;
	};
}