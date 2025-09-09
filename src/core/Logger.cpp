#include "Logger.hpp"

namespace {

// contenitore per le risorse condivise (mutex + file)
struct State {
    std::mutex mtx;
    std::ofstream file;
};

State& state() {
    static State s;
    return s;
}

const char* level_name(LogLevel lvl) {
    switch(lvl) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warn:     return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Fatal:    return "FATAL";
    }
    return "?";
}

// colore ANSI per il livello
const char* level_color(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::Trace:    return "\x1b[37m";  // gray
        case LogLevel::Debug:    return "\x1b[36m";  // cyan
        case LogLevel::Info:     return "\x1b[32m";  // green
        case LogLevel::Warn:     return "\x1b[33m";  // yellow
        case LogLevel::Error:    return "\x1b[31m";  // red
        case LogLevel::Fatal:    return "\x1b[41m";  // red bg
    }
    return "\x1b[0m";
}

std::string now_timestamp() {
    using namespace std::chrono;
    const auto tp = system_clock::now();
    const auto t  = system_clock::to_time_t(tp);
    const auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

    std::tm tm;
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

namespace Log {
// applica configurazione a runtime
bool init() {
    auto& s = state();
    std::lock_guard<std::mutex> lock(s.mtx);

    // TODO: open file ecc.
    return true;
}

void shutdown() {
    // TODO: cleanup logging/writing queued entries
}

void log_sink(LogLevel level, std::string_view msg){    
    auto& s = state();
    const std::string ts = now_timestamp();

    std::ostringstream line; 
    line << '[' << ts << "] [" << level_name(level) << "] " << msg << '\n';
    const std::string plain = line.str();

    std::lock_guard<std::mutex> lock(s.mtx);

    std::ostream& out = (level >= LogLevel::Warn) ? std::cerr : std::cout;
    if (LOGGER_USE_COLORS) {
        out << level_color(level) << '[' << ts << "] [" << level_name(level) << "] "
            << msg << "\x1b[0m\n";
    } else {
        out << plain;
    }
    out.flush();

    // TODO: make file writes in batches
}
}