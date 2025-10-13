#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include "User.h"
#include <vector>
#include <string>
#include <memory>

/**
 * @brief AuthenticationManager class for handling user authentication and session management
 * 
 * This class provides comprehensive user authentication services including:
 * - User login/logout
 * - Session management
 * - User registration
 * - Password policies
 * - Role-based access control
 * - File-based user storage
 */
class AuthenticationManager {
private:
    std::vector<User> users_;
    std::shared_ptr<User> currentUser_;
    std::string userDataFile_;
    int maxLoginAttempts_;
    int sessionTimeoutMinutes_;
    std::string currentSessionId_;
    std::string sessionStartTime_;

public:
    // Constructor and Destructor
    AuthenticationManager();
    AuthenticationManager(const std::string& userDataFile);
    ~AuthenticationManager();

    // Core Authentication Methods
    bool login(const std::string& username, const std::string& password);
    bool logout();
    bool isLoggedIn() const;
    std::shared_ptr<User> getCurrentUser() const;
    
    // User Management Methods
    bool registerUser(const std::string& username, const std::string& password, 
                     const std::string& role = "Viewer");
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    bool deleteUser(const std::string& username);
    bool updateUserRole(const std::string& username, const std::string& newRole);
    bool updateUserStatus(const std::string& username, const std::string& newStatus);
    
    // Session Management
    std::string generateSessionId();
    bool isSessionValid() const;
    void refreshSession();
    std::string getSessionInfo() const;
    int getSessionRemainingMinutes() const;
    
    // User Lookup and Validation
    User* findUser(const std::string& username);
    bool userExists(const std::string& username) const;
    bool validateCredentials(const std::string& username, const std::string& password) const;
    
    // Password Policy Management
    bool isPasswordStrong(const std::string& password) const;
    std::vector<std::string> getPasswordRequirements() const;
    bool enforcePasswordPolicy(const std::string& password, std::string& errorMessage) const;
    
    // File Operations
    bool loadUsersFromFile();
    bool saveUsersToFile() const;
    bool backupUserData() const;
    bool restoreUserData(const std::string& backupFile);
    
    // Administrative Functions
    std::vector<User> getAllUsers() const;
    std::vector<User> getUsersByRole(const std::string& role) const;
    std::vector<User> getActiveUsers() const;
    int getTotalUserCount() const;
    int getActiveUserCount() const;
    
    // Security and Audit
    void logLoginAttempt(const std::string& username, bool success);
    std::vector<std::string> getLoginHistory() const;
    bool isAccountLocked(const std::string& username) const;
    void unlockAccount(const std::string& username);
    
    // Permission Checking
    bool hasPermission(const std::string& operation) const;
    bool canCreateUsers() const;
    bool canDeleteUsers() const;
    bool canModifyUsers() const;
    bool canViewUsers() const;
    
    // Utility Methods
    void displayCurrentUser() const;
    void displayAllUsers() const;
    void displayUserStatistics() const;
    std::string toString() const;
    
    // Default Users Creation
    void createDefaultUsers();
    void resetToDefaults();
    
    // Constants
    static const int DEFAULT_MAX_LOGIN_ATTEMPTS;
    static const int DEFAULT_SESSION_TIMEOUT;
    static const std::string DEFAULT_USER_FILE;
    static const std::vector<std::string> VALID_OPERATIONS;
};

#endif // AUTHENTICATION_MANAGER_H