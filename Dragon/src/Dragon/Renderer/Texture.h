#pragma once

#include <string>
#include <memory>

namespace Dragon
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::string GetType() const = 0;
		virtual void SetType(std::string type)  = 0;
		virtual std::string GetPath() const = 0;
		virtual void SetPath(std::string path)  = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetTextureID() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const std::string& path);
		static std::shared_ptr<Texture2D> Create(const std::string& path, const std::string& directory);
	};

	class TextureCube : public Texture
	{
	public:
		static std::shared_ptr<TextureCube> Create(const std::vector<std::string>& faces);
		static std::shared_ptr<TextureCube> Create(const std::vector<std::string>& faces, const std::string& directory);
	};
}
