#include <string>
#include "log_level.h"

class LevelToString {
public:
    static std::string to_string(log_level level) {
        switch (level) {
        case log_level::TRACE: return "TRACE";
        case log_level::DEBUG: return "DEBUG";
        case log_level::INFO:  return "INFO";
        case log_level::WARN:  return "WARN";
        case log_level::ERROR: return "ERROR";
        case log_level::FATAL: return "FATAL";
        default: return "UNKNOWN";
        }
    }
};