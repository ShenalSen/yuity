#include "src/headers/AuthenticationManager.h"
#include "src/headers/Utilities.h"
#include <iostream>
#include <string>

void displayMenu() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "          TOURMATE AUTHENTICATION SYSTEM\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1.  Login\n";
    std::cout << "2.  Logout\n";
    std::cout << "3.  Register New User\n";
    std::cout << "4.  Change Password\n";
    std::cout << "5.  View Current User\n";
    std::cout << "6.  View All Users\n";
    std::cout << "7.  View User Statistics\n";
    std::cout << "8.  Update User Role\n";
    std::cout << "9.  Update User Status\n";
    std::cout << "10. Delete User\n";
    std::cout << "11. Session Information\n";
    std::cout << "12. Password Requirements\n";
    std::cout << "13. Reset to Default Users\n";
    std::cout << "0.  Exit\n";
    std::cout << std::string(60, '=') << "\n";
}

void testLogin(AuthenticationManager& auth) {
    std::cout << "\n=== USER LOGIN ===\n";
    
    if (auth.isLoggedIn()) {
        std::cout << "A user is already logged in. Please logout first.\n";
        return;
    }
    
    std::string username = TourmateUtils::getStringInput("Enter username: ", false);
    std::string password = TourmateUtils::getStringInput("Enter password: ", false);
    
    if (auth.login(username, password)) {
        TourmateUtils::showSuccessMessage("Login successful!");
        auth.displayCurrentUser();
    } else {
        TourmateUtils::showErrorMessage("Login Failed", "Invalid credentials or account issues");
    }
}

void testLogout(AuthenticationManager& auth) {
    std::cout << "\n=== USER LOGOUT ===\n";
    
    if (!auth.isLoggedIn()) {
        std::cout << "No user is currently logged in.\n";
        return;
    }
    
    if (auth.logout()) {
        TourmateUtils::showSuccessMessage("Logout successful!");
    } else {
        TourmateUtils::showErrorMessage("Logout Failed", "Could not logout user");
    }
}

void testRegistration(AuthenticationManager& auth) {
    std::cout << "\n=== USER REGISTRATION ===\n";
    
    if (!auth.isLoggedIn()) {
        std::cout << "Please login first to register new users.\n";
        return;
    }
    
    if (!auth.canCreateUsers()) {
        TourmateUtils::showErrorMessage("Permission Denied", "You don't have permission to create users");
        return;
    }
    
    std::string username = TourmateUtils::getStringInput("Enter new username: ", false);
    std::string password = TourmateUtils::getStringInput("Enter password: ", false);
    
    std::cout << "\nAvailable roles: Admin, Operator, Viewer\n";
    std::string role = TourmateUtils::getStringInput("Enter role (default: Viewer): ", true);
    if (role.empty()) role = "Viewer";
    
    if (auth.registerUser(username, password, role)) {
        TourmateUtils::showSuccessMessage("User registered successfully!");
    } else {
        TourmateUtils::showErrorMessage("Registration Failed", "Could not create user");
    }
}

void testPasswordChange(AuthenticationManager& auth) {
    std::cout << "\n=== CHANGE PASSWORD ===\n";
    
    if (!auth.isLoggedIn()) {
        std::cout << "Please login first to change password.\n";
        return;
    }
    
    std::string oldPassword = TourmateUtils::getStringInput("Enter current password: ", false);
    std::string newPassword = TourmateUtils::getStringInput("Enter new password: ", false);
    
    if (auth.changePassword(oldPassword, newPassword)) {
        TourmateUtils::showSuccessMessage("Password changed successfully!");
    } else {
        TourmateUtils::showErrorMessage("Password Change Failed", "Invalid current password or weak new password");
    }
}

void testUpdateUserRole(AuthenticationManager& auth) {
    std::cout << "\n=== UPDATE USER ROLE ===\n";
    
    if (!auth.isLoggedIn() || !auth.canModifyUsers()) {
        TourmateUtils::showErrorMessage("Permission Denied", "Insufficient privileges");
        return;
    }
    
    std::string username = TourmateUtils::getStringInput("Enter username to update: ", false);
    std::cout << "\nAvailable roles: Admin, Operator, Viewer\n";
    std::string newRole = TourmateUtils::getStringInput("Enter new role: ", false);
    
    if (auth.updateUserRole(username, newRole)) {
        TourmateUtils::showSuccessMessage("User role updated successfully!");
    } else {
        TourmateUtils::showErrorMessage("Update Failed", "Could not update user role");
    }
}

void testUpdateUserStatus(AuthenticationManager& auth) {
    std::cout << "\n=== UPDATE USER STATUS ===\n";
    
    if (!auth.isLoggedIn() || !auth.canModifyUsers()) {
        TourmateUtils::showErrorMessage("Permission Denied", "Insufficient privileges");
        return;
    }
    
    std::string username = TourmateUtils::getStringInput("Enter username to update: ", false);
    std::cout << "\nAvailable statuses: Active, Inactive, Suspended\n";
    std::string newStatus = TourmateUtils::getStringInput("Enter new status: ", false);
    
    if (auth.updateUserStatus(username, newStatus)) {
        TourmateUtils::showSuccessMessage("User status updated successfully!");
    } else {
        TourmateUtils::showErrorMessage("Update Failed", "Could not update user status");
    }
}

void testDeleteUser(AuthenticationManager& auth) {
    std::cout << "\n=== DELETE USER ===\n";
    
    if (!auth.isLoggedIn() || !auth.canDeleteUsers()) {
        TourmateUtils::showErrorMessage("Permission Denied", "Admin privileges required");
        return;
    }
    
    std::string username = TourmateUtils::getStringInput("Enter username to delete: ", false);
    
    std::cout << "Are you sure you want to delete user '" << username << "'? (yes/no): ";
    std::string confirmation = TourmateUtils::getStringInput("", false);
    
    if (TourmateUtils::toLowerCase(confirmation) == "yes") {
        if (auth.deleteUser(username)) {
            TourmateUtils::showSuccessMessage("User deleted successfully!");
        } else {
            TourmateUtils::showErrorMessage("Delete Failed", "Could not delete user");
        }
    } else {
        std::cout << "Delete operation cancelled.\n";
    }
}

void showPasswordRequirements(AuthenticationManager& auth) {
    std::cout << "\n=== PASSWORD REQUIREMENTS ===\n";
    auto requirements = auth.getPasswordRequirements();
    
    for (size_t i = 0; i < requirements.size(); ++i) {
        std::cout << (i + 1) << ". " << requirements[i] << "\n";
    }
    
    std::cout << "\nExample: MyPass123\n";
}

int main() {
    TourmateUtils::clearScreen();
    std::cout << "=== TOURMATE AUTHENTICATION MODULE TEST ===\n";
    std::cout << "Initializing Authentication Manager...\n";
    
    AuthenticationManager auth;
    
    std::cout << "\nAuthentication Manager initialized successfully!\n";
    std::cout << "Default users have been created.\n";
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        
        choice = TourmateUtils::getIntInput("Enter your choice (0-13): ", 0, 13);
        
        switch (choice) {
            case 1:
                testLogin(auth);
                break;
            case 2:
                testLogout(auth);
                break;
            case 3:
                testRegistration(auth);
                break;
            case 4:
                testPasswordChange(auth);
                break;
            case 5:
                auth.displayCurrentUser();
                break;
            case 6:
                auth.displayAllUsers();
                break;
            case 7:
                auth.displayUserStatistics();
                break;
            case 8:
                testUpdateUserRole(auth);
                break;
            case 9:
                testUpdateUserStatus(auth);
                break;
            case 10:
                testDeleteUser(auth);
                break;
            case 11:
                std::cout << "\n=== SESSION INFORMATION ===\n";
                std::cout << auth.getSessionInfo() << "\n";
                break;
            case 12:
                showPasswordRequirements(auth);
                break;
            case 13:
                if (auth.isLoggedIn() && auth.getCurrentUser()->isAdmin()) {
                    auth.resetToDefaults();
                    TourmateUtils::showSuccessMessage("System reset to default users!");
                } else {
                    TourmateUtils::showErrorMessage("Permission Denied", "Admin privileges required");
                }
                break;
            case 0:
                if (auth.isLoggedIn()) {
                    auth.logout();
                }
                std::cout << "\nThank you for using Tourmate Authentication System!\n";
                running = false;
                break;
            default:
                TourmateUtils::showErrorMessage("Invalid Choice", "Please select a valid option");
                break;
        }
        
        if (running && choice != 0) {
            TourmateUtils::pauseScreen();
        }
    }
    
    return 0;
}