#pragma once

#ifdef DG_PLATFORM_WINDOWS
	#ifdef  DG_BUILD_DLL
		#define DRAGON_API __declspec(dllexport)
	#else
		#define DRAGON_API __declspec(dllimport)
	#endif
#else
	#error Dragon only supports Windows!
#endif

#ifdef DG_DEBUG
	#define DG_ENABLE_ASSERTS
#endif

#ifdef DG_ENABLE_ASSERTS
	#define DG_ASSERT(x, ...){if(!(x)){DG_ERROR("Assertion Failed : {0}", __VA_ARGS__);__debugbreak();}}
	#define DG_CORE_ASSERT(x, ...){if(!(x)){DG_CORE_ERROR("Assertion Failed : {0}", __VA_ARGS__);__debugbreak();}}
#else
	#define DG_ASSERT(x, ...)
	#define DG_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
