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
#if RENDERER_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif


enum class LogLevel : uint8_t { Fatal, Error,  Warn, Info, Debug, Trace };

namespace Log {

bool init();
void shutdown();
LogLevel min_level();

void log_sink(LogLevel level, std::string_view msg);

// Unico entry-point: stile {} (std::format)
template <typename... Args>
inline void logf(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
  log_sink(level, std::format(fmt, std::forward<Args>(args)...));
}
}

// Macro comode (oneline + short-circuit compile-time)

// logs a fatal-level message
#define LOG_FATAL(...)  ::Log::logf(LogLevel::Fatal, __VA_ARGS__)

#ifndef LOG_ERROR
// logs an error-level message
#define LOG_ERROR(...)  ::Log::logf(LogLevel::Error, __VA_ARGS__)
#endif

#if LOG_WARN_ENABLED == 1
// log a warning-level message
#define LOG_WARN(...)  ::Log::logf(LogLevel::Warn, __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_INFO_ENABLED == 1
// log a info-level message
#define LOG_INFO(...)  ::Log::logf(LogLevel::Info, __VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_DEBUG_ENABLED == 1
// log a debug-level message
#define LOG_DEBUG(...)  ::Log::logf(LogLevel::Debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_TRACE_ENABLED == 1
// log a debug-level message
#define LOG_TRACE(...)  ::Log::logf(LogLevel::Trace, __VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif