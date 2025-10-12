#include "User.h"
#include "Utilities.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <functional>

// Static constants definition
const std::vector<std::string> User::VALID_ROLES = {"Admin", "Operator", "Viewer"};
const std::vector<std::string> User::VALID_ACCOUNT_STATUSES = {"Active", "Inactive", "Suspended"};
const int User::MIN_PASSWORD_LENGTH = 6;
const int User::MAX_PASSWORD_LENGTH = 50;

// Default constructor
User::User() 
    : userId_(0), username_(""), password_(""), role_("Viewer"),
      lastLogin_(""), accountStatus_("Active") {
}

// Parameterized constructor
User::User(int userId, const std::string& username, const std::string& password,
          const std::string& role, const std::string& lastLogin,
          const std::string& accountStatus)
    : userId_(userId), username_(username), password_(password), role_(role),
      lastLogin_(lastLogin), accountStatus_(accountStatus) {
}

// Destructor
User::~User() {
    // Clear sensitive data
    password_.clear();
}

// Getters implementation
int User::getUserId() const { return userId_; }
std::string User::getUsername() const { return username_; }
std::string User::getRole() const { return role_; }
std::string User::getLastLogin() const { return lastLogin_; }
std::string User::getAccountStatus() const { return accountStatus_; }

// Setters implementation
void User::setUserId(int id) { userId_ = id; }
void User::setUsername(const std::string& username) { username_ = username; }
void User::setRole(const std::string& role) { role_ = role; }
void User::setLastLogin(const std::string& loginTime) { lastLogin_ = loginTime; }
void User::setAccountStatus(const std::string& status) { accountStatus_ = status; }

// Authentication methods
bool User::verifyPassword(const std::string& password) const {
    return encryptPassword(password) == password_;
}

std::string User::encryptPassword(const std::string& password) const {
    // Simple hash function for demo purposes
    // In production, use proper hashing like bcrypt or SHA-256 with salt
    std::hash<std::string> hasher;
    size_t hashValue = hasher(password + "tourmate_salt");
    return TourmateUtils::intToString(static_cast<int>(hashValue));
}

bool User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyPassword(oldPassword)) {
        return false;
    }
    
    if (!isValidPassword(newPassword)) {
        return false;
    }
    
    password_ = encryptPassword(newPassword);
    return true;
}

void User::setPassword(const std::string& password) {
    password_ = encryptPassword(password);
}

bool User::authenticate(const std::string& username, const std::string& password) const {
    return username_ == username && verifyPassword(password) && isActive();
}

// Validation methods
bool User::isValidUsername(const std::string& username) const {
    if (username.empty() || username.length() < 3 || username.length() > 20) {
        return false;
    }
    
    // Check for valid characters (alphanumeric and underscore)
    for (char c : username) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    // Must start with a letter
    return std::isalpha(username[0]);
}

bool User::isValidPassword(const std::string& password) const {
    if (password.length() < MIN_PASSWORD_LENGTH || password.length() > MAX_PASSWORD_LENGTH) {
        return false;
    }
    
    // Check for at least one uppercase, one lowercase, and one digit
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        if (std::islower(c)) hasLower = true;
        if (std::isdigit(c)) hasDigit = true;
    }
    
    return hasUpper && hasLower && hasDigit;
}

bool User::isValidRole(const std::string& role) const {
    return std::find(VALID_ROLES.begin(), VALID_ROLES.end(), role) != VALID_ROLES.end();
}

bool User::isValidAccountStatus(const std::string& status) const {
    return std::find(VALID_ACCOUNT_STATUSES.begin(), VALID_ACCOUNT_STATUSES.end(), status) 
           != VALID_ACCOUNT_STATUSES.end();
}

bool User::validateUserData() const {
    return userId_ > 0 &&
           isValidUsername(username_) &&
           !password_.empty() &&
           isValidRole(role_) &&
           isValidAccountStatus(accountStatus_);
}

// Role-based permissions
bool User::isAdmin() const { return role_ == "Admin"; }
bool User::isOperator() const { return role_ == "Operator"; }
bool User::isViewer() const { return role_ == "Viewer"; }

bool User::canCreate() const { return isAdmin() || isOperator(); }
bool User::canUpdate() const { return isAdmin() || isOperator(); }
bool User::canDelete() const { return isAdmin(); }
bool User::canView() const { return true; } // All roles can view

// Account status management
bool User::isActive() const { return accountStatus_ == "Active"; }
bool User::isInactive() const { return accountStatus_ == "Inactive"; }
bool User::isSuspended() const { return accountStatus_ == "Suspended"; }

void User::activateAccount() { accountStatus_ = "Active"; }
void User::deactivateAccount() { accountStatus_ = "Inactive"; }
void User::suspendAccount() { accountStatus_ = "Suspended"; }

// Session management
void User::updateLastLogin() {
    lastLogin_ = TourmateUtils::getCurrentDateTime();
}

// Utility methods
void User::displayUser() const {
    std::cout << "==================================================\n";
    std::cout << "                 USER DETAILS\n";
    std::cout << "==================================================\n";
    std::cout << "User ID         : " << userId_ << "\n";
    std::cout << "Username        : " << username_ << "\n";
    std::cout << "Role            : " << role_ << "\n";
    std::cout << "Account Status  : " << accountStatus_ << "\n";
    std::cout << "Last Login      : " << (lastLogin_.empty() ? "Never" : lastLogin_) << "\n";
    std::cout << "Permissions     :\n";
    std::cout << "  - Can Create  : " << (canCreate() ? "Yes" : "No") << "\n";
    std::cout << "  - Can Update  : " << (canUpdate() ? "Yes" : "No") << "\n";
    std::cout << "  - Can Delete  : " << (canDelete() ? "Yes" : "No") << "\n";
    std::cout << "  - Can View    : " << (canView() ? "Yes" : "No") << "\n";
    std::cout << "==================================================\n";
}

std::string User::toString() const {
    std::ostringstream oss;
    oss << "User[ID=" << userId_ 
        << ", Username=" << username_
        << ", Role=" << role_
        << ", Status=" << accountStatus_
        << ", LastLogin=" << (lastLogin_.empty() ? "Never" : lastLogin_) << "]";
    return oss.str();
}

std::string User::toCSV() const {
    std::vector<std::string> fields = {
        TourmateUtils::intToString(userId_),
        username_,
        password_, // Store encrypted password
        role_,
        lastLogin_,
        accountStatus_
    };
    return TourmateUtils::createCSVLine(fields);
}

bool User::fromCSV(const std::string& csvLine) {
    std::vector<std::string> fields = TourmateUtils::parseCSVLine(csvLine);
    
    if (fields.size() != 6) {
        return false;
    }
    
    try {
        userId_ = TourmateUtils::stringToInt(fields[0]);
        username_ = TourmateUtils::trim(fields[1]);
        password_ = TourmateUtils::trim(fields[2]); // Already encrypted
        role_ = TourmateUtils::trim(fields[3]);
        lastLogin_ = TourmateUtils::trim(fields[4]);
        accountStatus_ = TourmateUtils::trim(fields[5]);
        
        return validateUserData();
    } catch (...) {
        return false;
    }
}

// Comparison operators
bool User::operator==(const User& other) const {
    return userId_ == other.userId_;
}

bool User::operator!=(const User& other) const {
    return !(*this == other);
}

bool User::operator<(const User& other) const {
    return userId_ < other.userId_;
}

// Stream operators
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.toString();
    return os;
}

std::istream& operator>>(std::istream& is, User& user) {
    std::string line;
    std::getline(is, line);
    user.fromCSV(line);
    return is;
}