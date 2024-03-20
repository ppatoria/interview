#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

struct Logger {
    std::shared_ptr<spdlog::logger> console;

    Logger()
    {
        console = spdlog::stdout_color_mt("console");
    }

    std::shared_ptr<spdlog::logger> get()
    {
        return spdlog::get("console");
        auto logger = spdlog::get("console");
    }
};

#endif // LOGGER_H
