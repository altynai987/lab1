#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <climits>

const std::string MENU_PROMPT = "Введите номер действия: ";
std::string getString(const std::string& prompt, std::istream& in = std::cin);
std::string getNonEmptyString(const std::string& prompt, std::istream& in = std::cin);
int getInt(const std::string& prompt, int minValue = INT_MIN, int maxValue = INT_MAX,
    std::istream& in = std::cin);
double getDouble(const std::string& prompt, double minValue, double maxValue,
    std::istream& in = std::cin);
double getPositiveDouble(const std::string& prompt, std::istream& in = std::cin);
bool loadInputsFromLog(const std::string& filename, int& count);

#endif