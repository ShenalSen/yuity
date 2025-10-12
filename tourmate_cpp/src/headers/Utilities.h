#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

/**
 * @brief Utility functions for the Tourmate Vehicle Management System
 * 
 * This header contains common utility functions used across the system
 * for string manipulation, validation, file operations, and formatting.
 */
namespace TourmateUtils {

    // String manipulation functions
    std::string trim(const std::string& str);
    std::string toLowerCase(const std::string& str);
    std::string toUpperCase(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    bool contains(const std::string& str, const std::string& substring);
    std::string replace(const std::string& str, const std::string& from, const std::string& to);

    // Validation functions
    bool isValidEmail(const std::string& email);
    bool isValidDate(const std::string& date);
    bool isValidTime(const std::string& time);
    bool isNumeric(const std::string& str);
    bool isAlphabetic(const std::string& str);
    bool isEmpty(const std::string& str);

    // Number conversion functions
    int stringToInt(const std::string& str);
    double stringToDouble(const std::string& str);
    std::string intToString(int value);
    std::string doubleToString(double value);

    // Date and time functions
    std::string getCurrentDate();
    std::string getCurrentTime();
    std::string getCurrentDateTime();
    std::string formatDate(const std::string& date);

    // CSV parsing functions
    std::vector<std::string> parseCSVLine(const std::string& line);
    std::string createCSVLine(const std::vector<std::string>& fields);
    bool writeCSVFile(const std::string& filename, const std::vector<std::vector<std::string>>& data);
    bool readCSVFile(const std::string& filename, std::vector<std::vector<std::string>>& data);

    // Console utility functions
    void clearScreen();
    void pauseScreen();
    void printSeparator(char character = '=', int length = 50);
    void printCentered(const std::string& text, int width = 50);
    int getIntInput(const std::string& prompt, int minValue, int maxValue);
    double getDoubleInput(const std::string& prompt, double minValue, double maxValue);
    std::string getStringInput(const std::string& prompt, bool allowEmpty);

    // Error handling utilities
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void logInfo(const std::string& message);
    void showErrorMessage(const std::string& title, const std::string& message);
    void showSuccessMessage(const std::string& message);

    // Data formatting functions
    std::string formatCurrency(double amount);
    std::string formatPercentage(double value);
    std::string formatFileSize(long bytes);

} // namespace TourmateUtils

#endif // UTILITIES_H