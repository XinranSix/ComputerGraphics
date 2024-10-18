#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class Logger final {
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger() { return sLogger_; }

private:
    static std::shared_ptr<spdlog::logger> sLogger_;
};

// Client Logging Macros
#define TRACE(...) Logger::GetLogger()->trace(__VA_ARGS__)
#define INFO(...) Logger::GetLogger()->info(__VA_ARGS__)
#define WARN(...) Logger::GetLogger()->warn(__VA_ARGS__)
#define ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)
#define FATAL(...) Logger::GetLogger()->critical(__VA_ARGS__)
