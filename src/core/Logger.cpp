#include "Logger.hpp"

namespace {

struct State {
    std::mutex mtx;
    LogLevel runtime_min = kBuildMinLevel;
    bool use_colors = true;
    bool also_file = true;
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
        case LogLevel::Crit: return "CRITICAL";
        case LogLevel::Off:      return "OFF";
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
        case LogLevel::Crit: return "\x1b[41m";  // red bg
        case LogLevel::Off:      return "\x1b[0m";    
    }
    return "\x1b[0m";
}

std::string now_timestamp() {
    using namespace std::chrono;
    const auto tp = system_clock::now();
    const auto t  = system_clock::to_time_t(tp);
    const auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

    std::tm tm;
    #if defined(WIN32)          // thread safety
        localtime_s(&tm, &t);   
    #else
        localtime_r(&t, &tm);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S") << '.'
        << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}

inline bool enabled(LogLevel lvl) {
  return static_cast<int>(lvl) >= static_cast<int>(state().runtime_min) && lvl != LogLevel::Off;
}
}

namespace Log {

// applica configurazione a runtime
void init(const Config& cfg) {
    auto& s = state();
    std::lock_guard<std::mutex> lock(s.mtx);
    s.use_colors = cfg.useColors;
    s.also_file = cfg.alsoFile;

    if (s.also_file) {
        if (s.file.is_open()) s.file.close();
        s.file.open(cfg.filePath, std::ios::out | std::ios::trunc); // sovrascrive
    }
}

void shutdown() {
    auto& s = state();
    std::lock_guard<std::mutex> lock(s.mtx);
    if (s.file.is_open()) s.file.close();
}

LogLevel min_level() {
    auto& s = state();
    std::lock_guard<std::mutex> lock(s.mtx);
    return s.runtime_min;
}

void log_sink(LogLevel level, std::string_view msg){
    if (!enabled(level)) return;
    
    auto& s = state();
    const std::string ts = now_timestamp();
    const char* name = level_name(level);

    std::ostringstream line; 
    line << '[' << ts << "] [" << name << "] " << msg << '\n';
    const std::string plain = line.str();

    std::lock_guard<std::mutex> lock(s.mtx);

    std::ostream& out = (level >= LogLevel::Warn) ? std::cerr : std::cout;
    if (s.use_colors) {
        out << level_color(level) << '[' << ts << "] [" << name << "] "
            << msg << "\x1b[0m\n";
    } else {
        out << plain;
    }
    out.flush();

    if (s.also_file && s.file.is_open()) {
        s.file << plain;
        s.file.flush();
    }
}
}