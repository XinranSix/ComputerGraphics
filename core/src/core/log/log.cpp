#include "core/log/log.h"

namespace CG {
    std::shared_ptr<spdlog::logger> Logger::sCoreLogger_;
    std::shared_ptr<spdlog::logger> Logger::sClientLogger_;

    void Logger::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        sCoreLogger_ = spdlog::stdout_color_mt("CORE");
        sCoreLogger_->set_level(spdlog::level::trace);

        sClientLogger_ = spdlog::stdout_color_mt("APP");
        sClientLogger_->set_level(spdlog::level::trace);
    }

} // namespace CG
