#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <vector>

/**
 * @brief User class representing a system user in the Tourmate system
 * 
 * This class encapsulates all user-related data and provides
 * methods for user authentication and management.
 */
class User {
private:
    int userId_;
    std::string username_;
    std::string password_;
    std::string role_;
    std::string lastLogin_;
    std::string accountStatus_;

public:
    // Constructors
    User();
    User(int userId, const std::string& username, const std::string& password,
         const std::string& role, const std::string& lastLogin,
         const std::string& accountStatus);

    // Destructor
    ~User();

    // Getters
    int getUserId() const;
    std::string getUsername() const;
    std::string getRole() const;
    std::string getLastLogin() const;
    std::string getAccountStatus() const;

    // Setters
    void setUserId(int id);
    void setUsername(const std::string& username);
    void setRole(const std::string& role);
    void setLastLogin(const std::string& lastLogin);
    void setAccountStatus(const std::string& status);

    // Validation methods
    bool isValidUsername(const std::string& username) const;
    bool isValidPassword(const std::string& password) const;
    bool isValidRole(const std::string& role) const;
    bool isValidAccountStatus(const std::string& status) const;
    bool validateUserData() const;

    // Authentication methods
    bool verifyPassword(const std::string& password) const;
    std::string encryptPassword(const std::string& password) const;
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    void setPassword(const std::string& password);
    bool authenticate(const std::string& username, const std::string& password) const;
    void updateLastLogin();

    // Permission methods
    bool isAdmin() const;
    bool isOperator() const;
    bool isViewer() const;
    bool canCreate() const;
    bool canUpdate() const;
    bool canDelete() const;
    bool canView() const;

    // Account status methods
    bool isActive() const;
    bool isInactive() const;
    bool isSuspended() const;
    void activateAccount();
    void deactivateAccount();
    void suspendAccount();

    // Utility methods
    void displayUser() const;
    std::string toString() const;
    std::string toCSV() const;
    bool fromCSV(const std::string& csvLine);

    // Comparison operators
    bool operator==(const User& other) const;
    bool operator!=(const User& other) const;
    bool operator<(const User& other) const;

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);

    // Static constants
    static const std::vector<std::string> VALID_ROLES;
    static const std::vector<std::string> VALID_ACCOUNT_STATUSES;
    static const int MIN_PASSWORD_LENGTH;
    static const int MAX_PASSWORD_LENGTH;
};

#endif // USER_H