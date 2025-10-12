#include "Utilities.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <regex>

namespace TourmateUtils {

// String manipulation functions
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

bool isEmpty(const std::string& str) {
    return trim(str).empty();
}

bool contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) return "";
    
    std::ostringstream oss;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) oss << delimiter;
        oss << strings[i];
    }
    
    return oss.str();
}

std::string replace(const std::string& str, const std::string& oldSubstr, const std::string& newSubstr) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(oldSubstr, pos)) != std::string::npos) {
        result.replace(pos, oldSubstr.length(), newSubstr);
        pos += newSubstr.length();
    }
    
    return result;
}

// Type conversion functions
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

int stringToInt(const std::string& str) {
    std::istringstream iss(trim(str));
    int value;
    iss >> value;
    
    if (iss.fail() || !iss.eof()) {
        throw std::invalid_argument("Invalid integer string: " + str);
    }
    
    return value;
}

double stringToDouble(const std::string& str) {
    std::istringstream iss(trim(str));
    double value;
    iss >> value;
    
    if (iss.fail() || !iss.eof()) {
        throw std::invalid_argument("Invalid double string: " + str);
    }
    
    return value;
}

// Validation functions
bool isValidEmail(const std::string& email) {
    // Simple email validation using regex
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailRegex);
}

bool isValidDate(const std::string& date) {
    // Validate date format: YYYY-MM-DD
    std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, dateRegex)) {
        return false;
    }
    
    // Basic range validation
    std::vector<std::string> parts = split(date, '-');
    if (parts.size() != 3) return false;
    
    try {
        int year = stringToInt(parts[0]);
        int month = stringToInt(parts[1]);
        int day = stringToInt(parts[2]);
        
        return year >= 2000 && year <= 2030 &&
               month >= 1 && month <= 12 &&
               day >= 1 && day <= 31;
    } catch (...) {
        return false;
    }
}

bool isValidTime(const std::string& time) {
    // Validate time format: HH:MM
    std::regex timeRegex(R"(\d{2}:\d{2})");
    if (!std::regex_match(time, timeRegex)) {
        return false;
    }
    
    std::vector<std::string> parts = split(time, ':');
    if (parts.size() != 2) return false;
    
    try {
        int hour = stringToInt(parts[0]);
        int minute = stringToInt(parts[1]);
        
        return hour >= 0 && hour <= 23 &&
               minute >= 0 && minute <= 59;
    } catch (...) {
        return false;
    }
}

bool isNumeric(const std::string& str) {
    if (isEmpty(str)) return false;
    
    for (char c : str) {
        if (!std::isdigit(c) && c != '.' && c != '-' && c != '+') {
            return false;
        }
    }
    
    return true;
}

bool isAlphabetic(const std::string& str) {
    if (isEmpty(str)) return false;
    
    for (char c : str) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    
    return true;
}

// Date and time functions
std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(4) << (localTime->tm_year + 1900) << "-"
        << std::setw(2) << (localTime->tm_mon + 1) << "-"
        << std::setw(2) << localTime->tm_mday;
    
    return oss.str();
}

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << localTime->tm_hour << ":"
        << std::setw(2) << localTime->tm_min;
    
    return oss.str();
}

std::string getCurrentDateTime() {
    return getCurrentDate() + " " + getCurrentTime();
}

std::string formatDate(const std::string& date) {
    if (!isValidDate(date)) return date;
    
    std::vector<std::string> parts = split(date, '-');
    return parts[2] + "/" + parts[1] + "/" + parts[0]; // DD/MM/YYYY
}

// CSV functions
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    
    while (std::getline(ss, field, ',')) {
        // Remove quotes if present
        if (!field.empty() && field.front() == '"' && field.back() == '"') {
            field = field.substr(1, field.length() - 2);
        }
        fields.push_back(field);
    }
    
    return fields;
}

std::string createCSVLine(const std::vector<std::string>& fields) {
    std::ostringstream oss;
    
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) oss << ",";
        
        std::string field = fields[i];
        // Escape commas and quotes
        if (contains(field, ",") || contains(field, "\"") || contains(field, "\n")) {
            field = replace(field, "\"", "\"\"");
            oss << "\"" << field << "\"";
        } else {
            oss << field;
        }
    }
    
    return oss.str();
}

bool writeCSVFile(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& row : data) {
        file << createCSVLine(row) << "\n";
    }
    
    file.close();
    return true;
}

bool readCSVFile(const std::string& filename, std::vector<std::vector<std::string>>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    data.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            data.push_back(parseCSVLine(line));
        }
    }
    
    file.close();
    return true;
}

// Console utilities
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void printSeparator(char character, int length) {
    std::cout << std::string(length, character) << std::endl;
}

void printCentered(const std::string& text, int width) {
    int padding = (width - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::endl;
}

int getIntInput(const std::string& prompt, int minValue, int maxValue) {
    int value;
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        try {
            value = stringToInt(input);
            if (value >= minValue && value <= maxValue) {
                return value;
            }
            std::cout << "Please enter a value between " << minValue << " and " << maxValue << ".\n";
        } catch (...) {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

double getDoubleInput(const std::string& prompt, double minValue, double maxValue) {
    double value;
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        try {
            value = stringToDouble(input);
            if (value >= minValue && value <= maxValue) {
                return value;
            }
            std::cout << "Please enter a value between " << minValue << " and " << maxValue << ".\n";
        } catch (...) {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

std::string getStringInput(const std::string& prompt, bool allowEmpty) {
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (!allowEmpty && isEmpty(input)) {
            std::cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        
        return trim(input);
    }
}

// Error handling
void logError(const std::string& message) {
    std::cerr << "[ERROR] " << getCurrentDateTime() << " - " << message << std::endl;
}

void logWarning(const std::string& message) {
    std::cout << "[WARNING] " << getCurrentDateTime() << " - " << message << std::endl;
}

void logInfo(const std::string& message) {
    std::cout << "[INFO] " << getCurrentDateTime() << " - " << message << std::endl;
}

void showErrorMessage(const std::string& title, const std::string& message) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ERROR: " << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << message << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void showSuccessMessage(const std::string& message) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "SUCCESS: " << message << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

// Data formatting
std::string formatCurrency(double amount) {
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

std::string formatPercentage(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << (value * 100) << "%";
    return oss.str();
}

std::string formatFileSize(long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    double size = static_cast<double>(bytes);
    int unitIndex = 0;
    
    while (size >= 1024 && unitIndex < 3) {
        size /= 1024;
        unitIndex++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return oss.str();
}

} // namespace TourmateUtils