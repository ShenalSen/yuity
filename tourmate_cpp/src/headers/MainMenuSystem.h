#ifndef MAINMENUSYSTEM_H
#define MAINMENUSYSTEM_H

#include "AuthenticationManager.h"
#include "VehicleManager.h"
#include "SalesManager.h"
#include "Utilities.h"
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <vector>

/**
 * @class MainMenuSystem
 * @brief Unified main menu system integrating all Tourmate modules
 * 
 * This class provides a comprehensive console-based interface that integrates
 * authentication, vehicle management, and sales management modules with 
 * role-based access control and professional user experience.
 * 
 * Features:
 * - Role-based menu customization (Admin/Operator/Viewer)
 * - Seamless module integration and navigation
 * - Professional console interface with branding
 * - Session management and timeout handling
 * - System statistics and dashboard
 * - Comprehensive help and documentation
 * - Error handling and user feedback
 * - Auto-save and data persistence
 * 
 * @author Tourmate Development Team
 * @version 1.0
 * @date 2024
 */
class MainMenuSystem {
public:
    /**
     * @enum MenuState
     * @brief Represents different states of the menu system
     */
    enum class MenuState {
        LOGIN,           ///< User needs to login
        MAIN_MENU,       ///< Main dashboard menu
        VEHICLE_MENU,    ///< Vehicle management submenu
        SALES_MENU,      ///< Sales management submenu
        USER_MENU,       ///< User management submenu
        REPORTS_MENU,    ///< Reports and analytics submenu
        SETTINGS_MENU,   ///< System settings submenu
        HELP_MENU,       ///< Help and documentation
        EXITING          ///< System shutdown
    };
    
    /**
     * @struct SystemStats
     * @brief System-wide statistics for dashboard display
     */
    struct SystemStats {
        int totalUsers;
        int activeUsers;
        int totalVehicles;
        int availableVehicles;
        int totalCustomers;
        int vipCustomers;
        int totalBookings;
        int activeBookings;
        int completedTrips;
        double todayRevenue;
        double monthlyRevenue;
        std::string lastBackup;
        std::string systemUptime;
    };
    
    /**
     * @struct UserSession
     * @brief Current user session information
     */
    struct UserSession {
        std::string username;
        std::string role;
        std::chrono::system_clock::time_point loginTime;
        std::chrono::system_clock::time_point lastActivity;
        int operationsCount;
        std::string currentModule;
        bool isActive;
    };

private:
    // Core system managers
    std::shared_ptr<AuthenticationManager> authManager;
    std::shared_ptr<VehicleManager> vehicleManager;
    std::shared_ptr<SalesManager> salesManager;
    
    // Menu system state
    MenuState currentState;
    UserSession currentSession;
    SystemStats systemStats;
    
    // Configuration
    static const int SESSION_TIMEOUT_MINUTES = 30;
    static const int MENU_WIDTH = 80;
    static const std::string SYSTEM_VERSION;
    static const std::string SYSTEM_TITLE;
    
    // Console utilities
    void clearScreen();
    void pauseForUser();
    void displayHeader(const std::string& title);
    void displayFooter();
    void displaySeparator(char character = '=');
    void displayCentered(const std::string& text, int width = MENU_WIDTH);
    
    // Input utilities
    void clearInputBuffer();
    std::string getStringInput(const std::string& prompt);
    int getIntInput(const std::string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX);
    double getDoubleInput(const std::string& prompt, double minVal = -DBL_MAX, double maxVal = DBL_MAX);
    bool getYesNoInput(const std::string& prompt);
    char getCharInput(const std::string& prompt);
    
    // Session management
    void initializeSession();
    void updateLastActivity();
    bool checkSessionTimeout();
    void displaySessionInfo();
    void logUserAction(const std::string& action);
    
    // System statistics
    void updateSystemStats();
    void displayDashboard();
    void displaySystemStatus();
    void displayQuickStats();
    
    // Menu display methods
    void displayMainMenu();
    void displayVehicleMenu();
    void displaySalesMenu();
    void displayUserMenu();
    void displayReportsMenu();
    void displaySettingsMenu();
    void displayHelpMenu();
    
    // Menu handlers
    void handleLoginMenu();
    void handleMainMenu();
    void handleVehicleMenu();
    void handleSalesMenu();
    void handleUserMenu();
    void handleReportsMenu();
    void handleSettingsMenu();
    void handleHelpMenu();
    
    // Vehicle management integration
    void vehicleQuickView();
    void vehicleAddNew();
    void vehicleSearch();
    void vehicleReports();
    void vehicleMaintenance();
    
    // Sales management integration
    void salesQuickView();
    void salesCreateBooking();
    void salesManageBookings();
    void salesCustomerManagement();
    void salesReports();
    void salesAnalytics();
    
    // User management
    void userAccountSettings();
    void userChangePassword();
    void userViewProfile();
    void userManageUsers(); // Admin only
    void userViewAuditLog(); // Admin only
    
    // Reports and analytics
    void reportsOverview();
    void reportsVehicleUtilization();
    void reportsSalesAnalytics();
    void reportsCustomerInsights();
    void reportsFinancialSummary();
    void reportsExportData();
    
    // System settings
    void settingsSystemConfig();
    void settingsDataBackup();
    void settingsDataRestore();
    void settingsUserPreferences();
    void settingsAboutSystem();
    
    // Help and documentation
    void helpUserGuide();
    void helpFeatureGuide();
    void helpTroubleshooting();
    void helpContactSupport();
    void helpKeyboardShortcuts();
    
    // Utility methods
    bool hasPermission(const std::string& permission);
    void displayPermissionDenied();
    void displayOperationSuccess(const std::string& message);
    void displayOperationError(const std::string& message);
    void displayWarning(const std::string& message);
    void displayInfo(const std::string& message);
    
    // Role-based menu filtering
    std::vector<std::string> getAvailableMenuOptions(MenuState state);
    bool isMenuOptionAvailable(const std::string& option);
    
    // System initialization and cleanup
    void initializeDataFiles();
    void performSystemChecks();
    void cleanupOnExit();
    void saveSystemState();
    
public:
    /**
     * @brief Constructor
     * Initializes the main menu system with all required managers
     */
    MainMenuSystem();
    
    /**
     * @brief Destructor
     * Cleans up resources and saves system state
     */
    ~MainMenuSystem();
    
    /**
     * @brief Initialize the system and prepare for operation
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Run the main menu system
     * This is the primary entry point that starts the interactive menu system
     * @return Exit code (0 for success, non-zero for error)
     */
    int run();
    
    /**
     * @brief Handle system shutdown
     * Performs cleanup and saves state before exit
     */
    void shutdown();
    
    /**
     * @brief Display welcome screen
     */
    void displayWelcomeScreen();
    
    /**
     * @brief Display system information
     */
    void displaySystemInfo();
    
    /**
     * @brief Get current system statistics
     * @return SystemStats structure with current statistics
     */
    SystemStats getSystemStats() const;
    
    /**
     * @brief Get current user session information
     * @return UserSession structure with session details
     */
    UserSession getCurrentSession() const;
    
    /**
     * @brief Check if user has specific permission
     * @param permission Permission string to check
     * @return true if user has permission, false otherwise
     */
    bool checkPermission(const std::string& permission) const;
    
    /**
     * @brief Navigate to specific menu state
     * @param state Target menu state
     */
    void navigateToMenu(MenuState state);
    
    /**
     * @brief Display branded header for all screens
     * @param subtitle Optional subtitle to display
     */
    void displayBrandedHeader(const std::string& subtitle = "");
    
    /**
     * @brief Handle emergency system operations
     * For critical system functions and emergency procedures
     */
    void handleEmergencyOperations();
    
    /**
     * @brief Export system data for backup
     * @param exportPath Path where to export data
     * @return true if export successful, false otherwise
     */
    bool exportSystemData(const std::string& exportPath);
    
    /**
     * @brief Import system data from backup
     * @param importPath Path from where to import data
     * @return true if import successful, false otherwise
     */
    bool importSystemData(const std::string& importPath);
    
    /**
     * @brief Get system version information
     * @return Version string
     */
    static std::string getVersion();
    
    /**
     * @brief Validate system integrity
     * @return true if system is in good state, false if issues found
     */
    bool validateSystemIntegrity();
};

// Static member initialization
const std::string MainMenuSystem::SYSTEM_VERSION = "1.0.0";
const std::string MainMenuSystem::SYSTEM_TITLE = "TOURMATE VEHICLE MANAGEMENT SYSTEM";

#endif // MAINMENUSYSTEM_H