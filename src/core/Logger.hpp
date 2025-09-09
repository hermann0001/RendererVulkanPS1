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

enum class LogLevel : uint8_t { Trace, Debug,  Info, Warn, Error, Crit, Off};

#ifndef LOG_COMPILETIME_MIN_LEVEL
    #if RENDERER_DEBUG
        #define LOG_COMPILETIME_MIN_LEVEL 0
    #elif RENDERER_RELEASE
        #define LOG_COMPILETIME_MIN_LEVEL 5
    #else 
        #define LOG_COMPILETIME_MIN_LEVEL 2
    #endif
#endif

constexpr LogLevel kBuildMinLevel = 
  (LOG_COMPILETIME_MIN_LEVEL <= 0) ? LogLevel::Trace :
  (LOG_COMPILETIME_MIN_LEVEL == 1) ? LogLevel::Debug :
  (LOG_COMPILETIME_MIN_LEVEL == 2) ? LogLevel::Info  :
  (LOG_COMPILETIME_MIN_LEVEL == 3) ? LogLevel::Warn  :
  (LOG_COMPILETIME_MIN_LEVEL == 4) ? LogLevel::Error :
  (LOG_COMPILETIME_MIN_LEVEL == 5) ? LogLevel::Crit  : LogLevel::Off;

namespace Log {
struct Config {
    bool        useColors = true;
    bool        alsoFile = false;
    std::string filePath = "renderer.log";
};

void init(const Config& cfg = {});
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
#if LOG_COMPILETIME_MIN_LEVEL <= 0
#define LOG_TRACE(...)  ::Log::logf(LogLevel::Trace, __VA_ARGS__)
#else
#define LOG_TRACE(...)  ((void)0)
#endif

#if LOG_COMPILETIME_MIN_LEVEL <= 1
#define LOG_DEBUG(...)  ::Log::logf(LogLevel::Debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)  ((void)0)
#endif

#if LOG_COMPILETIME_MIN_LEVEL <= 2
#define LOG_INFO(...)  ::Log::logf(LogLevel::Info, __VA_ARGS__)
#else
#define LOG_INFO(...)  ((void)0)
#endif

#if LOG_COMPILETIME_MIN_LEVEL <= 3
#define LOG_WARN(...)  ::Log::logf(LogLevel::Warn, __VA_ARGS__)
#else
#define LOG_WARN(...)  ((void)0)
#endif

#if LOG_COMPILETIME_MIN_LEVEL <= 4
#define LOG_ERROR(...)  ::Log::logf(LogLevel::Error, __VA_ARGS__)
#else
#define LOG_ERROR(...)  ((void)0)
#endif

#if LOG_COMPILETIME_MIN_LEVEL <= 5
#define LOG_CRIT(...)  ::Log::logf(LogLevel::Crit, __VA_ARGS__)
#else
#define LOG_CRIT(...)  ((void)0)
#endif

// VK_CHECEK
#define VK_CHECK(expr) do { \
    VkResult _vk_res = (expr); \
    if (_vk_res != VK_SUCCESS) { \
        LOG_ERRORF("Vulkan error {} at {}:{}", static_cast<int>(_vk_res), __FILE__, __LINE__); \
        std::terminate(); \
    } \
} while(0)
