#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>

class Logger {
private:
    std::ofstream logFile;
    std::string filename;

public:
    Logger();
    ~Logger();

    void open(const std::string& baseName);
    void close();
    std::ofstream& getStream();
    bool isOpen() const;
};

#endif