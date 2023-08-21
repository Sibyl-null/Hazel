#pragma once
#include "Core.h"
#include <spdlog/spdlog.h>
#include <memory>

namespace Hazel {
	class HAZEL_API Log
	{
	private:
		// 类内部声明静态变量表明属于整个类（未定义）
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}