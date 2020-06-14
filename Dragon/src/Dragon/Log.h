#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Dragon
{
	class DRAGON_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log maros
#define DG_CORE_TRACE(...)		::Dragon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DG_CORE_INFO(...)		::Dragon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DG_CORE_WARN(...)		::Dragon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DG_CORE_ERROR(...)		::Dragon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DG_CORE_FATAL(...)		::Dragon::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define DG_TRACE(...)			::Dragon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DG_INFO(...)			::Dragon::Log::GetClientLogger()->info(__VA_ARGS__)
#define DG_WARN(...)			::Dragon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DG_ERROR(...)			::Dragon::Log::GetClientLogger()->error(__VA_ARGS__)
#define DG_FATAL(...)			::Dragon::Log::GetClientLogger()->fatal(__VA_ARGS__)