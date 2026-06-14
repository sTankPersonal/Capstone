#pragma once

#include <crow.h>
#include <iostream>
#include <ctime>

class FilteredCrowLogger : public crow::ILogHandler {
public:
    void log(std::string message, crow::LogLevel level) override {
        if (message.rfind("Request: ", 0) == 0 || message.rfind("Response: ", 0) == 0)
            return;

        const char* levelStr = "INFO    ";
        switch (level) {
            case crow::LogLevel::Debug:    levelStr = "DEBUG   "; break;
            case crow::LogLevel::Warning:  levelStr = "WARNING "; break;
            case crow::LogLevel::Error:    levelStr = "ERROR   "; break;
            case crow::LogLevel::Critical: levelStr = "CRITICAL"; break;
            default: break;
        }

        std::time_t t = std::time(nullptr);
        std::tm tm{};
        gmtime_r(&t, &tm);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

        std::cerr << "(" << buf << ") [" << levelStr << "] " << message << "\n";
    }
};

struct LoggingMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);
};
