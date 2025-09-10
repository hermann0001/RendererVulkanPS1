#pragma once

#include <string>
#include <string_view>
#include <cstdarg>
#include <mutex>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <format>
#include "defines.hpp"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#define LOGGER_USE_COLORS 1

// disable debug and trace logging for release build
#if RV_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

namespace RV::Core::Log {
enum class Level : uint8_t { Fatal, Error,  Warn, Info, Debug, Trace };

class Logger {
public:
  static Logger& instance();
  void shutdown();

  // Unico entry-point: stile {} (std::format)
  template <typename... Args>
  inline void logf(Level level, std::format_string<Args...> fmt, Args&&... args) {
    log(level, std::format(fmt, std::forward<Args>(args)...));
  }
  void log(Level level, std::string_view msg);

private:
  Logger();
  ~Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  mutable std::mutex mtx;
  std::ofstream file;
};
}

// Macro comode (oneline + short-circuit compile-time)

// logs a fatal-level message
#define LOG_FATAL(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Fatal, __VA_ARGS__)

#ifndef LOG_ERROR
// logs an error-level message
#define LOG_ERROR(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Error, __VA_ARGS__)
#endif

#if LOG_WARN_ENABLED == 1
// log a warning-level message
#define LOG_WARN(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Warn, __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_INFO_ENABLED == 1
// log a info-level message
#define LOG_INFO(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Info, __VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_DEBUG_ENABLED == 1
// log a debug-level message
#define LOG_DEBUG(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_TRACE_ENABLED == 1
// log a debug-level message
#define LOG_TRACE(...)  ::RV::Core::Log::Logger::instance().logf(::RV::Core::Log::Level::Trace, __VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif