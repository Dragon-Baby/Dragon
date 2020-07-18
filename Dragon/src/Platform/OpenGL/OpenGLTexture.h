#pragma once

#include "Dragon/Renderer/Texture.h"

namespace Dragon
{
	struct ImGuiTexture
	{
		unsigned char* data;
		int width;
		int height;
		int channels;
	};

	ImGuiTexture LoadImGuiTexture(const std::string& path);
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
		virtual uint32_t GetTextureID() const override { return m_RendererID; }
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		std::string m_Type;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::vector<std::string>& faces);
		OpenGLTextureCube(const std::vector<std::string>& faces, const std::string& directory);

		virtual ~OpenGLTextureCube();
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual std::string GetType() const override { return m_Type; }
		virtual void SetType(std::string type) override { m_Type = type; }
		virtual std::string GetPath() const override { return m_Path; }
		virtual void SetPath(std::string path) override { m_Path = path; }
		virtual void Bind(uint32_t slot = 0) const override;
		virtual uint32_t GetTextureID() const override { return m_RendererID; }
	
	private:
		uint32_t m_RendererID;
		std::string m_Path;
		uint32_t m_Width, m_Height;
		std::string m_Type;
	};
}