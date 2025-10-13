#include "AuthenticationManager.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <random>

// Static constants definition
const int AuthenticationManager::DEFAULT_MAX_LOGIN_ATTEMPTS = 3;
const int AuthenticationManager::DEFAULT_SESSION_TIMEOUT = 30;
const std::string AuthenticationManager::DEFAULT_USER_FILE = "data/users.csv";
const std::vector<std::string> AuthenticationManager::VALID_OPERATIONS = {
    "CREATE_USER", "DELETE_USER", "MODIFY_USER", "VIEW_USER",
    "CREATE_VEHICLE", "DELETE_VEHICLE", "MODIFY_VEHICLE", "VIEW_VEHICLE",
    "CREATE_SALE", "DELETE_SALE", "MODIFY_SALE", "VIEW_SALE"
};

// Constructor and Destructor
AuthenticationManager::AuthenticationManager() 
    : userDataFile_(DEFAULT_USER_FILE), maxLoginAttempts_(DEFAULT_MAX_LOGIN_ATTEMPTS),
      sessionTimeoutMinutes_(DEFAULT_SESSION_TIMEOUT), currentUser_(nullptr),
      currentSessionId_(""), sessionStartTime_("") {
    loadUsersFromFile();
    if (users_.empty()) {
        createDefaultUsers();
    }
}

AuthenticationManager::AuthenticationManager(const std::string& userDataFile)
    : userDataFile_(userDataFile), maxLoginAttempts_(DEFAULT_MAX_LOGIN_ATTEMPTS),
      sessionTimeoutMinutes_(DEFAULT_SESSION_TIMEOUT), currentUser_(nullptr),
      currentSessionId_(""), sessionStartTime_("") {
    loadUsersFromFile();
    if (users_.empty()) {
        createDefaultUsers();
    }
}

AuthenticationManager::~AuthenticationManager() {
    if (isLoggedIn()) {
        logout();
    }
    saveUsersToFile();
}

// Core Authentication Methods
bool AuthenticationManager::login(const std::string& username, const std::string& password) {
    // Check if already logged in
    if (isLoggedIn()) {
        TourmateUtils::logWarning("User already logged in. Please logout first.");
        return false;
    }
    
    // Find user
    User* user = findUser(username);
    if (!user) {
        logLoginAttempt(username, false);
        TourmateUtils::logError("User not found: " + username);
        return false;
    }
    
    // Check account status
    if (!user->isActive()) {
        logLoginAttempt(username, false);
        TourmateUtils::logError("Account is not active: " + username);
        return false;
    }
    
    // Validate credentials
    if (!user->authenticate(username, password)) {
        logLoginAttempt(username, false);
        TourmateUtils::logError("Invalid credentials for user: " + username);
        return false;
    }
    
    // Successful login
    currentUser_ = std::make_shared<User>(*user);
    currentUser_->updateLastLogin();
    currentSessionId_ = generateSessionId();
    sessionStartTime_ = TourmateUtils::getCurrentDateTime();
    
    // Update user in the vector
    auto it = std::find_if(users_.begin(), users_.end(),
        [&username](const User& u) { return u.getUsername() == username; });
    if (it != users_.end()) {
        it->updateLastLogin();
    }
    
    logLoginAttempt(username, true);
    TourmateUtils::logInfo("User logged in successfully: " + username);
    return true;
}

bool AuthenticationManager::logout() {
    if (!isLoggedIn()) {
        TourmateUtils::logWarning("No user is currently logged in.");
        return false;
    }
    
    std::string username = currentUser_->getUsername();
    currentUser_.reset();
    currentSessionId_.clear();
    sessionStartTime_.clear();
    
    TourmateUtils::logInfo("User logged out successfully: " + username);
    return true;
}

bool AuthenticationManager::isLoggedIn() const {
    return currentUser_ != nullptr && isSessionValid();
}

std::shared_ptr<User> AuthenticationManager::getCurrentUser() const {
    if (isLoggedIn()) {
        return currentUser_;
    }
    return nullptr;
}

// User Management Methods
bool AuthenticationManager::registerUser(const std::string& username, const std::string& password, 
                                       const std::string& role) {
    // Check if current user has permission
    if (isLoggedIn() && !canCreateUsers()) {
        TourmateUtils::logError("Insufficient permissions to create users");
        return false;
    }
    
    // Validate username
    if (userExists(username)) {
        TourmateUtils::logError("Username already exists: " + username);
        return false;
    }
    
    // Validate password strength
    std::string errorMessage;
    if (!enforcePasswordPolicy(password, errorMessage)) {
        TourmateUtils::logError("Password policy violation: " + errorMessage);
        return false;
    }
    
    // Create new user
    int newUserId = users_.empty() ? 1 : users_.back().getUserId() + 1;
    User newUser(newUserId, username, password, role, "", "Active");
    
    if (!newUser.validateUserData()) {
        TourmateUtils::logError("Invalid user data");
        return false;
    }
    
    users_.push_back(newUser);
    saveUsersToFile();
    
    TourmateUtils::logInfo("User registered successfully: " + username);
    return true;
}

bool AuthenticationManager::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!isLoggedIn()) {
        TourmateUtils::logError("User not logged in");
        return false;
    }
    
    // Validate new password
    std::string errorMessage;
    if (!enforcePasswordPolicy(newPassword, errorMessage)) {
        TourmateUtils::logError("Password policy violation: " + errorMessage);
        return false;
    }
    
    // Change password for current user
    if (!currentUser_->changePassword(oldPassword, newPassword)) {
        TourmateUtils::logError("Failed to change password - incorrect old password");
        return false;
    }
    
    // Update in users vector
    auto it = std::find_if(users_.begin(), users_.end(),
        [this](const User& u) { return u.getUserId() == currentUser_->getUserId(); });
    if (it != users_.end()) {
        it->changePassword(oldPassword, newPassword);
    }
    
    saveUsersToFile();
    TourmateUtils::logInfo("Password changed successfully");
    return true;
}

bool AuthenticationManager::deleteUser(const std::string& username) {
    if (!isLoggedIn() || !canDeleteUsers()) {
        TourmateUtils::logError("Insufficient permissions to delete users");
        return false;
    }
    
    if (currentUser_->getUsername() == username) {
        TourmateUtils::logError("Cannot delete currently logged in user");
        return false;
    }
    
    auto it = std::find_if(users_.begin(), users_.end(),
        [&username](const User& u) { return u.getUsername() == username; });
    
    if (it == users_.end()) {
        TourmateUtils::logError("User not found: " + username);
        return false;
    }
    
    users_.erase(it);
    saveUsersToFile();
    
    TourmateUtils::logInfo("User deleted successfully: " + username);
    return true;
}

bool AuthenticationManager::updateUserRole(const std::string& username, const std::string& newRole) {
    if (!isLoggedIn() || !canModifyUsers()) {
        TourmateUtils::logError("Insufficient permissions to modify users");
        return false;
    }
    
    User* user = findUser(username);
    if (!user) {
        TourmateUtils::logError("User not found: " + username);
        return false;
    }
    
    if (!user->isValidRole(newRole)) {
        TourmateUtils::logError("Invalid role: " + newRole);
        return false;
    }
    
    user->setRole(newRole);
    saveUsersToFile();
    
    TourmateUtils::logInfo("User role updated successfully: " + username + " -> " + newRole);
    return true;
}

bool AuthenticationManager::updateUserStatus(const std::string& username, const std::string& newStatus) {
    if (!isLoggedIn() || !canModifyUsers()) {
        TourmateUtils::logError("Insufficient permissions to modify users");
        return false;
    }
    
    User* user = findUser(username);
    if (!user) {
        TourmateUtils::logError("User not found: " + username);
        return false;
    }
    
    if (!user->isValidAccountStatus(newStatus)) {
        TourmateUtils::logError("Invalid status: " + newStatus);
        return false;
    }
    
    user->setAccountStatus(newStatus);
    saveUsersToFile();
    
    TourmateUtils::logInfo("User status updated successfully: " + username + " -> " + newStatus);
    return true;
}

// Session Management
std::string AuthenticationManager::generateSessionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000, 9999999);
    
    std::string timestamp = TourmateUtils::getCurrentDateTime();
    std::string randomNum = TourmateUtils::intToString(dis(gen));
    
    return "SID_" + timestamp + "_" + randomNum;
}

bool AuthenticationManager::isSessionValid() const {
    if (currentSessionId_.empty() || sessionStartTime_.empty()) {
        return false;
    }
    
    // Simple session validation - in production, use proper datetime comparison
    return true;
}

void AuthenticationManager::refreshSession() {
    if (isLoggedIn()) {
        sessionStartTime_ = TourmateUtils::getCurrentDateTime();
        TourmateUtils::logInfo("Session refreshed for user: " + currentUser_->getUsername());
    }
}

std::string AuthenticationManager::getSessionInfo() const {
    if (!isLoggedIn()) {
        return "No active session";
    }
    
    std::ostringstream oss;
    oss << "Session ID: " << currentSessionId_ << "\n";
    oss << "User: " << currentUser_->getUsername() << "\n";
    oss << "Role: " << currentUser_->getRole() << "\n";
    oss << "Login Time: " << sessionStartTime_ << "\n";
    oss << "Status: Active";
    
    return oss.str();
}

int AuthenticationManager::getSessionRemainingMinutes() const {
    if (!isLoggedIn()) {
        return 0;
    }
    
    // Simplified - return full timeout for demo
    return sessionTimeoutMinutes_;
}

// User Lookup and Validation
User* AuthenticationManager::findUser(const std::string& username) {
    auto it = std::find_if(users_.begin(), users_.end(),
        [&username](const User& u) { return u.getUsername() == username; });
    
    return (it != users_.end()) ? &(*it) : nullptr;
}

bool AuthenticationManager::userExists(const std::string& username) const {
    return std::find_if(users_.begin(), users_.end(),
        [&username](const User& u) { return u.getUsername() == username; }) != users_.end();
}

bool AuthenticationManager::validateCredentials(const std::string& username, const std::string& password) const {
    auto it = std::find_if(users_.begin(), users_.end(),
        [&username](const User& u) { return u.getUsername() == username; });
    
    return (it != users_.end()) ? it->verifyPassword(password) : false;
}

// Password Policy Management
bool AuthenticationManager::isPasswordStrong(const std::string& password) const {
    return password.length() >= 6 &&
           std::any_of(password.begin(), password.end(), ::isupper) &&
           std::any_of(password.begin(), password.end(), ::islower) &&
           std::any_of(password.begin(), password.end(), ::isdigit);
}

std::vector<std::string> AuthenticationManager::getPasswordRequirements() const {
    return {
        "Minimum 6 characters",
        "At least one uppercase letter",
        "At least one lowercase letter",
        "At least one digit"
    };
}

bool AuthenticationManager::enforcePasswordPolicy(const std::string& password, std::string& errorMessage) const {
    if (password.length() < 6) {
        errorMessage = "Password must be at least 6 characters long";
        return false;
    }
    
    if (!std::any_of(password.begin(), password.end(), ::isupper)) {
        errorMessage = "Password must contain at least one uppercase letter";
        return false;
    }
    
    if (!std::any_of(password.begin(), password.end(), ::islower)) {
        errorMessage = "Password must contain at least one lowercase letter";
        return false;
    }
    
    if (!std::any_of(password.begin(), password.end(), ::isdigit)) {
        errorMessage = "Password must contain at least one digit";
        return false;
    }
    
    return true;
}

// File Operations
bool AuthenticationManager::loadUsersFromFile() {
    std::vector<std::vector<std::string>> data;
    if (!TourmateUtils::readCSVFile(userDataFile_, data)) {
        TourmateUtils::logWarning("Could not load users from file: " + userDataFile_);
        return false;
    }
    
    users_.clear();
    for (const auto& row : data) {
        if (row.size() >= 6) {
            User user;
            std::string csvLine = TourmateUtils::createCSVLine(row);
            if (user.fromCSV(csvLine)) {
                users_.push_back(user);
            }
        }
    }
    
    TourmateUtils::logInfo("Loaded " + TourmateUtils::intToString(users_.size()) + " users from file");
    return true;
}

bool AuthenticationManager::saveUsersToFile() const {
    std::vector<std::vector<std::string>> data;
    
    for (const auto& user : users_) {
        std::string csvLine = user.toCSV();
        std::vector<std::string> fields = TourmateUtils::parseCSVLine(csvLine);
        data.push_back(fields);
    }
    
    if (!TourmateUtils::writeCSVFile(userDataFile_, data)) {
        TourmateUtils::logError("Failed to save users to file: " + userDataFile_);
        return false;
    }
    
    return true;
}

bool AuthenticationManager::backupUserData() const {
    std::string backupFile = userDataFile_ + ".backup." + TourmateUtils::getCurrentDate();
    
    std::vector<std::vector<std::string>> data;
    if (!TourmateUtils::readCSVFile(userDataFile_, data)) {
        return false;
    }
    
    return TourmateUtils::writeCSVFile(backupFile, data);
}

bool AuthenticationManager::restoreUserData(const std::string& backupFile) {
    if (!isLoggedIn() || !currentUser_->isAdmin()) {
        TourmateUtils::logError("Admin privileges required for data restoration");
        return false;
    }
    
    // Create backup of current data first
    backupUserData();
    
    std::vector<std::vector<std::string>> data;
    if (!TourmateUtils::readCSVFile(backupFile, data)) {
        TourmateUtils::logError("Failed to read backup file: " + backupFile);
        return false;
    }
    
    return TourmateUtils::writeCSVFile(userDataFile_, data);
}

// Administrative Functions
std::vector<User> AuthenticationManager::getAllUsers() const {
    return users_;
}

std::vector<User> AuthenticationManager::getUsersByRole(const std::string& role) const {
    std::vector<User> result;
    std::copy_if(users_.begin(), users_.end(), std::back_inserter(result),
        [&role](const User& u) { return u.getRole() == role; });
    return result;
}

std::vector<User> AuthenticationManager::getActiveUsers() const {
    std::vector<User> result;
    std::copy_if(users_.begin(), users_.end(), std::back_inserter(result),
        [](const User& u) { return u.isActive(); });
    return result;
}

int AuthenticationManager::getTotalUserCount() const {
    return static_cast<int>(users_.size());
}

int AuthenticationManager::getActiveUserCount() const {
    return static_cast<int>(getActiveUsers().size());
}

// Security and Audit
void AuthenticationManager::logLoginAttempt(const std::string& username, bool success) {
    std::string logMessage = "Login attempt - User: " + username + 
                           ", Success: " + (success ? "Yes" : "No") + 
                           ", Time: " + TourmateUtils::getCurrentDateTime();
    
    if (success) {
        TourmateUtils::logInfo(logMessage);
    } else {
        TourmateUtils::logWarning(logMessage);
    }
}

std::vector<std::string> AuthenticationManager::getLoginHistory() const {
    // Simplified implementation - in production, maintain proper log files
    return {"Login history feature - implementation pending"};
}

bool AuthenticationManager::isAccountLocked(const std::string& username) const {
    User* user = const_cast<AuthenticationManager*>(this)->findUser(username);
    return user ? user->isSuspended() : false;
}

void AuthenticationManager::unlockAccount(const std::string& username) {
    if (!isLoggedIn() || !currentUser_->isAdmin()) {
        TourmateUtils::logError("Admin privileges required to unlock accounts");
        return;
    }
    
    User* user = findUser(username);
    if (user) {
        user->activateAccount();
        saveUsersToFile();
        TourmateUtils::logInfo("Account unlocked: " + username);
    }
}

// Permission Checking
bool AuthenticationManager::hasPermission(const std::string& operation) const {
    if (!isLoggedIn()) {
        return false;
    }
    
    if (currentUser_->isAdmin()) {
        return true; // Admin has all permissions
    }
    
    if (currentUser_->isOperator()) {
        // Operators can do most operations except delete users
        return operation != "DELETE_USER";
    }
    
    // Viewers can only view
    return operation.find("VIEW_") == 0;
}

bool AuthenticationManager::canCreateUsers() const {
    return isLoggedIn() && (currentUser_->isAdmin() || currentUser_->isOperator());
}

bool AuthenticationManager::canDeleteUsers() const {
    return isLoggedIn() && currentUser_->isAdmin();
}

bool AuthenticationManager::canModifyUsers() const {
    return isLoggedIn() && (currentUser_->isAdmin() || currentUser_->isOperator());
}

bool AuthenticationManager::canViewUsers() const {
    return isLoggedIn();
}

// Utility Methods
void AuthenticationManager::displayCurrentUser() const {
    if (!isLoggedIn()) {
        std::cout << "No user is currently logged in.\n";
        return;
    }
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "           CURRENT USER SESSION\n";
    std::cout << std::string(50, '=') << "\n";
    currentUser_->displayUser();
    std::cout << "Session ID       : " << currentSessionId_ << "\n";
    std::cout << "Login Time       : " << sessionStartTime_ << "\n";
    std::cout << "Session Timeout  : " << sessionTimeoutMinutes_ << " minutes\n";
    std::cout << std::string(50, '=') << "\n";
}

void AuthenticationManager::displayAllUsers() const {
    if (!isLoggedIn() || !canViewUsers()) {
        TourmateUtils::logError("Insufficient permissions to view users");
        return;
    }
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "                     ALL USERS\n";
    std::cout << std::string(80, '=') << "\n";
    
    for (const auto& user : users_) {
        user.displayUser();
        std::cout << "\n";
    }
    
    std::cout << "Total Users: " << getTotalUserCount() << "\n";
    std::cout << "Active Users: " << getActiveUserCount() << "\n";
    std::cout << std::string(80, '=') << "\n";
}

void AuthenticationManager::displayUserStatistics() const {
    if (!isLoggedIn() || !canViewUsers()) {
        TourmateUtils::logError("Insufficient permissions to view statistics");
        return;
    }
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "            USER STATISTICS\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "Total Users      : " << getTotalUserCount() << "\n";
    std::cout << "Active Users     : " << getActiveUserCount() << "\n";
    std::cout << "Admins           : " << getUsersByRole("Admin").size() << "\n";
    std::cout << "Operators        : " << getUsersByRole("Operator").size() << "\n";
    std::cout << "Viewers          : " << getUsersByRole("Viewer").size() << "\n";
    std::cout << std::string(50, '=') << "\n";
}

std::string AuthenticationManager::toString() const {
    std::ostringstream oss;
    oss << "AuthenticationManager[Users=" << getTotalUserCount()
        << ", Active=" << getActiveUserCount()
        << ", LoggedIn=" << (isLoggedIn() ? currentUser_->getUsername() : "None")
        << ", SessionValid=" << (isSessionValid() ? "Yes" : "No") << "]";
    return oss.str();
}

// Default Users Creation
void AuthenticationManager::createDefaultUsers() {
    TourmateUtils::logInfo("Creating default users...");
    
    // Create admin user
    User admin(1, "admin", "Admin123", "Admin", "", "Active");
    users_.push_back(admin);
    
    // Create operator user
    User operator1(2, "operator", "Oper123", "Operator", "", "Active");
    users_.push_back(operator1);
    
    // Create viewer user
    User viewer(3, "viewer", "View123", "Viewer", "", "Active");
    users_.push_back(viewer);
    
    saveUsersToFile();
    
    TourmateUtils::logInfo("Default users created successfully");
    std::cout << "\n=== DEFAULT USERS CREATED ===\n";
    std::cout << "Admin    - Username: admin    Password: Admin123\n";
    std::cout << "Operator - Username: operator Password: Oper123\n";
    std::cout << "Viewer   - Username: viewer   Password: View123\n";
    std::cout << "=============================\n";
}

void AuthenticationManager::resetToDefaults() {
    if (!isLoggedIn() || !currentUser_->isAdmin()) {
        TourmateUtils::logError("Admin privileges required to reset to defaults");
        return;
    }
    
    users_.clear();
    createDefaultUsers();
    TourmateUtils::logInfo("System reset to default users");
}