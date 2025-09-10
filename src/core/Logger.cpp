#include "Logger.hpp"
#include "assert.hpp"

#include <sstream>
#include <iomanip>
#include <chrono>

namespace RV::Core::Log {

namespace {

const char* level_name(Level lvl) {
    switch(lvl) {
        case Level::Trace: return "TRACE";
        case Level::Debug: return "DEBUG";
        case Level::Info:  return "INFO";
        case Level::Warn:  return "WARN";
        case Level::Error: return "ERROR";
        case Level::Fatal: return "FATAL";
    }
    return "?";
}

const char* level_color(Level lvl) {
    switch (lvl) {
        case Level::Trace: return "\x1b[37m";  // gray
        case Level::Debug: return "\x1b[36m";  // cyan
        case Level::Info:  return "\x1b[32m";  // green
        case Level::Warn:  return "\x1b[33m";  // yellow
        case Level::Error: return "\x1b[31m";  // red
        case Level::Fatal: return "\x1b[41m";  // red bg
    }
    return "\x1b[0m";
}

std::string now_timestamp() {
    using namespace std::chrono;
    const auto tp = system_clock::now();
    const auto t  = system_clock::to_time_t(tp);
    const auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

    std::tm tm{};
#if defined(WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S") << '.'
        << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}

}

Logger::Logger() = default;   
Logger::~Logger() = default;

Logger& Logger::instance() {
    static Logger s;
    return s;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(mtx);
    if (file.is_open()) file.flush(), file.close();
}

void Logger::log(Level level, std::string_view msg) {
    const std::string ts = now_timestamp();

    std::ostringstream line;
    line << '[' << ts << "] [" << level_name(level) << "] " << msg << '\n';
    const std::string plain = line.str();

    std::lock_guard<std::mutex> lock(mtx);

    std::ostream& out = (level >= Level::Warn) ? std::cerr : std::cout;
#if LOGGER_USE_COLORS
    out << level_color(level) << '[' << ts << "] [" << level_name(level) << "] "
        << msg << "\x1b[0m\n";
#else
    out << plain;
#endif
    out.flush();

    if (level == Level::Fatal) rv_debug_break();
}

}

// Hook per le assert (già usato da assert.hpp)
void report_assertion_failure(const char* expr, const char* msg, const char* file, int line) {
    LOG_FATAL("Assertion Failure: {}, message: {}, {}:{}", expr, (msg ? msg : ""), file, line);
}
