#pragma once

#ifdef DG_PLATFORM_WINDOWS

extern Dragon::Application* Dragon::CreateApplication();

int main(int argc, char** argv)
{
	Dragon::Log::Init();
	DG_CORE_WARN("Initialized Log!");
	DG_INFO("Hello!");
	
	auto app = Dragon::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
