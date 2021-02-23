#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialtion Log!");

	int a = 5;
	HZ_INFO("Hello! Var={0}", a);


	Hazel::Application* app = Hazel::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif // HZ_PLATFORM_WINDOWS


