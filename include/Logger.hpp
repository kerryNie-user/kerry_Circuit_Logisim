#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <iomanip>

enum class LogLevel {
    ERROR = 0,
    INFO  = 1,
    DEBUG = 2
};

class Logger {
public:
    static void setLevel(LogLevel level) {
        currentLevel = level;
    }

    static void error(const std::string& msg) {
        if (currentLevel >= LogLevel::ERROR) {
            std::cerr << "\033[91m" << "[ERROR] " << msg << "\033[0m" << std::endl;
        }
    }

    static void info(const std::string& msg) {
        if (currentLevel >= LogLevel::INFO) {
            std::clog << "\033[35m" << "[INFO] " << msg << "\033[0m" << std::endl;
        }
    }

    static void debug(const std::string& msg) {
        if (currentLevel >= LogLevel::DEBUG) {
            std::clog << "\033[36m" << "[DEBUG] " << msg << "\033[0m" << std::endl;
        }
    }

    static void list(const std::vector<std::string>& msg) {
        for (int i = 0; i < msg.size(); i++) {
            std::cout << "[" << i << "] " << "\033[32m" << msg[i] << "\033[0m" << std::endl;
        }
    }

    static void logTheProgress(int progress, int total, const std::string& msg, const std::string& done_msg) {
        if (progress < total - 1) {
            double percent = 100.0 * progress / total;
            std::clog << "\r[" << std::setw(3) << std::setfill(' ') << (int)percent << "%] " << msg << std::flush;
        } else {
            std::clog << "\r[" << 100 << "%] " << done_msg << std::endl;
        }
    }

    static LogLevel getCurrentLevel() {
        return currentLevel;
    }
private:
    static LogLevel currentLevel;
};

#endif