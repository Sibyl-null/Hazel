#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	// Hazel::Log::GetCoreLogger()->warn("init log");
	// Hazel::Log::GetClientLogger()->info("hello");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif