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
