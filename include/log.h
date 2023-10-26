#pragma once
#include "config.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

namespace ULC{
enum class LogLevel {
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
public:
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  void log(LogLevel level, const std::string &message,
           const std::string &module = "Main") {
    switch (level) {
    case LogLevel::INFO:
      spdlog::info("[{}] {}", module, message);
      break;
    case LogLevel::WARN:
      spdlog::warn("[{}] {}", module, message);
      break;
    case LogLevel::ERROR:
      spdlog::error("[{}] {}", module, message);
      break;
    case LogLevel::FATAL:
      spdlog::critical("[{}] {}", module, message);
      break;
    }
    if (level == LogLevel::FATAL) {
      std::abort();
    }
  }

  void logd(LogLevel level, const std::string &message,
            const std::string &module = "Main") {
      spdlog::debug("[{}] {}", module, message);
  }

  void info(const std::string &message, const std::string &module = "Main") {
    log(LogLevel::INFO, message, module);
  }

  void warn(const std::string &message, const std::string &module = "Main") {
    log(LogLevel::WARN, message, module);
  }

  void error(const std::string &message, const std::string &module = "Main") {
    log(LogLevel::ERROR, message, module);
  }

  void fatal(const std::string &message, const std::string &module = "Main") {
    log(LogLevel::FATAL, message, module);
  }

private:
  Logger() {}
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
};
}