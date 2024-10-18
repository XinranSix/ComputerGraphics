#include "log/log.h"

std::shared_ptr<spdlog::logger> Logger::sLogger_;

void Logger::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    sLogger_ = spdlog::stdout_color_mt("Shader Toy");
    sLogger_->set_level(spdlog::level::trace);
}
