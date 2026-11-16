#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>

Logger::Logger() {}

Logger::~Logger() {
    close();
}

void Logger::open(const std::string& baseName) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::tm tm_now;
    localtime_s(&tm_now, &time_t_now);

    std::ostringstream oss;
    oss << baseName << "_"
        << std::put_time(&tm_now, "%d_%m_%Y_%H_%M_%S")
        << ".txt";

    filename = oss.str();
    logFile.open(filename);

    if (!logFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть лог-файл " << filename << std::endl;
    }
    else {
        std::cout << "Логирование в файл: " << filename << std::endl;
    }
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::ofstream& Logger::getStream() {
    return logFile;
}

bool Logger::isOpen() const {
    return logFile.is_open();
}