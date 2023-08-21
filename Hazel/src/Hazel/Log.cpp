#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {
	// 在类外部定义静态变量，分配存储空间
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// 日志样式：颜色 时间戳 LoggerID 日志内容
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		// 使用工厂函数创建记录器, mt表示多线程
		// 会注册到 spdlog 的全局注册表中, "HAZEL"作为其 ID
		// https://github.com/gabime/spdlog/wiki/2.-Creating-loggers
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		// 设置日志级别。trace 是最低级别，用于追踪（跟踪）详细的调试信息
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}