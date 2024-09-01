#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace CG {

    class Logger final {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger_; }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger_; }

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger_;
        static std::shared_ptr<spdlog::logger> sClientLogger_;
    };

} // namespace CG

// Core Logging Macros
#define CG_CORE_TRACE(...) CG::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CG_CORE_INFO(...) CG::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CG_CORE_WARN(...) CG::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CG_CORE_ERROR(...) CG::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CG_CORE_FATAL(...) CG::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define CCG_APP_TRACE(...) CG::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CCG_APP_INFO(...) CG::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CCG_APP_WARN(...) CG::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CCG_APP_ERROR(...) CG::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CCG_APP_FATAL(...) CG::Logger::GetClientLogger()->critical(__VA_ARGS__)
