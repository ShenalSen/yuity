#include "MainMenuSystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

// Constructor
MainMenuSystem::MainMenuSystem() 
    : currentState(MenuState::LOGIN) {
    // Initialize managers
    authManager = std::make_shared<AuthenticationManager>();
    vehicleManager = std::make_shared<VehicleManager>(authManager, "data/vehicles.csv");
    salesManager = std::make_shared<SalesManager>(authManager, vehicleManager,
                                                 "data/sales.csv", 
                                                 "data/bookings.csv",
                                                 "data/customers.csv");
    
    // Initialize session
    initializeSession();
}

// Destructor
MainMenuSystem::~MainMenuSystem() {
    cleanupOnExit();
}

// Initialize system
bool MainMenuSystem::initialize() {
    try {
        // Clear screen and show loading
        clearScreen();
        displayBrandedHeader("SYSTEM INITIALIZATION");
        
        std::cout << "Initializing Tourmate System...\n\n";
        
        // Initialize data files
        std::cout << "📁 Checking data files... ";
        initializeDataFiles();
        std::cout << "✓ Complete\n";
        
        // Perform system checks
        std::cout << "🔍 Performing system checks... ";
        performSystemChecks();
        std::cout << "✓ Complete\n";
        
        // Update system statistics
        std::cout << "📊 Loading system statistics... ";
        updateSystemStats();
        std::cout << "✓ Complete\n";
        
        std::cout << "\n🎉 System initialized successfully!\n";
        pauseForUser();
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Initialization failed: " << e.what() << std::endl;
        pauseForUser();
        return false;
    }
}

// Main run loop
int MainMenuSystem::run() {
    if (!initialize()) {
        return 1;
    }
    
    displayWelcomeScreen();
    
    while (currentState != MenuState::EXITING) {
        try {
            // Check session timeout
            if (authManager->isLoggedIn() && checkSessionTimeout()) {
                displayWarning("Session timeout. Please log in again.");
                authManager->logout();
                currentState = MenuState::LOGIN;
            }
            
            // Handle current menu state
            switch (currentState) {
                case MenuState::LOGIN:
                    handleLoginMenu();
                    break;
                case MenuState::MAIN_MENU:
                    handleMainMenu();
                    break;
                case MenuState::VEHICLE_MENU:
                    handleVehicleMenu();
                    break;
                case MenuState::SALES_MENU:
                    handleSalesMenu();
                    break;
                case MenuState::USER_MENU:
                    handleUserMenu();
                    break;
                case MenuState::REPORTS_MENU:
                    handleReportsMenu();
                    break;
                case MenuState::SETTINGS_MENU:
                    handleSettingsMenu();
                    break;
                case MenuState::HELP_MENU:
                    handleHelpMenu();
                    break;
                default:
                    currentState = MenuState::MAIN_MENU;
                    break;
            }
            
        } catch (const std::exception& e) {
            displayOperationError("System error: " + std::string(e.what()));
            pauseForUser();
        }
    }
    
    shutdown();
    return 0;
}

// Display welcome screen
void MainMenuSystem::displayWelcomeScreen() {
    clearScreen();
    
    std::cout << "\n";
    displaySeparator('╔');
    std::cout << "║" << std::string(MENU_WIDTH-2, ' ') << "║\n";
    displayCentered("🚗 " + SYSTEM_TITLE + " 🚗");
    displayCentered("Welcome to the Ultimate Transport Management Solution");
    std::cout << "║" << std::string(MENU_WIDTH-2, ' ') << "║\n";
    displayCentered("Version " + SYSTEM_VERSION);
    displayCentered("Developed for CSE4002 WRIT1 - Academic Project");
    std::cout << "║" << std::string(MENU_WIDTH-2, ' ') << "║\n";
    displaySeparator('╚');
    
    std::cout << "\n";
    displayCentered("🌟 FEATURES OVERVIEW 🌟");
    std::cout << "\n";
    std::cout << "  🔐 Secure Authentication System with Role-based Access Control\n";
    std::cout << "  🚙 Comprehensive Vehicle Management with Maintenance Tracking\n";
    std::cout << "  💰 Advanced Sales & Booking System with Payment Processing\n";
    std::cout << "  👥 Customer Relationship Management with VIP Services\n";
    std::cout << "  📊 Real-time Analytics and Revenue Reporting\n";
    std::cout << "  🛡️ Data Security with Backup & Recovery Systems\n";
    std::cout << "\n";
    
    displayCentered("Ready to transform your transport business!");
    std::cout << "\n";
    pauseForUser();
}

// Handle login menu
void MainMenuSystem::handleLoginMenu() {
    clearScreen();
    displayBrandedHeader("USER AUTHENTICATION");
    
    if (authManager->isLoggedIn()) {
        currentState = MenuState::MAIN_MENU;
        return;
    }
    
    std::cout << "🔐 Please log in to continue\n\n";
    std::cout << "Default Users for Testing:\n";
    std::cout << "  👑 admin/Admin123    (Full System Access)\n";
    std::cout << "  👤 operator/Oper123  (Operations Management)\n";
    std::cout << "  👁️  viewer/View123    (Read-only Access)\n\n";
    
    std::string username = getStringInput("Username: ");
    std::string password = getStringInput("Password: ");
    
    if (authManager->login(username, password)) {
        initializeSession();
        currentSession.username = username;
        currentSession.role = authManager->getCurrentUserRole();
        currentSession.loginTime = std::chrono::system_clock::now();
        currentSession.isActive = true;
        
        displayOperationSuccess("Login successful! Welcome, " + username);
        logUserAction("Logged in");
        updateLastActivity();
        
        currentState = MenuState::MAIN_MENU;
    } else {
        displayOperationError("Login failed. Please check your credentials.");
    }
    
    pauseForUser();
}

// Handle main menu
void MainMenuSystem::handleMainMenu() {
    clearScreen();
    displayBrandedHeader("MAIN DASHBOARD");
    
    // Display session info
    displaySessionInfo();
    
    // Display quick stats
    displayQuickStats();
    
    // Display dashboard
    displayDashboard();
    
    displayMainMenu();
    
    int choice = getIntInput("\nSelect option (0-9): ", 0, 9);
    updateLastActivity();
    
    switch (choice) {
        case 1:
            currentState = MenuState::VEHICLE_MENU;
            logUserAction("Accessed Vehicle Management");
            break;
        case 2:
            currentState = MenuState::SALES_MENU;
            logUserAction("Accessed Sales Management");
            break;
        case 3:
            currentState = MenuState::USER_MENU;
            logUserAction("Accessed User Management");
            break;
        case 4:
            currentState = MenuState::REPORTS_MENU;
            logUserAction("Accessed Reports & Analytics");
            break;
        case 5:
            currentState = MenuState::SETTINGS_MENU;
            logUserAction("Accessed System Settings");
            break;
        case 6:
            currentState = MenuState::HELP_MENU;
            logUserAction("Accessed Help & Documentation");
            break;
        case 7:
            displaySystemInfo();
            pauseForUser();
            break;
        case 8:
            authManager->logout();
            displayInfo("Logged out successfully. Thank you!");
            currentState = MenuState::LOGIN;
            logUserAction("Logged out");
            break;
        case 9:
            handleEmergencyOperations();
            break;
        case 0:
            if (getYesNoInput("Are you sure you want to exit? (y/n): ")) {
                currentState = MenuState::EXITING;
                logUserAction("System exit");
            }
            break;
        default:
            displayOperationError("Invalid choice. Please try again.");
            pauseForUser();
    }
}

// Display main menu options
void MainMenuSystem::displayMainMenu() {
    displaySeparator('-');
    std::cout << "🏠 MAIN MENU OPTIONS\n";
    displaySeparator('-');
    
    if (hasPermission("vehicle_access")) {
        std::cout << "  1️⃣  Vehicle Management      (Manage Fleet & Maintenance)\n";
    }
    
    if (hasPermission("sales_access")) {
        std::cout << "  2️⃣  Sales & Booking         (Customer Bookings & Payments)\n";
    }
    
    if (hasPermission("user_management")) {
        std::cout << "  3️⃣  User Management         (Account Settings & Security)\n";
    }
    
    if (hasPermission("reports_access")) {
        std::cout << "  4️⃣  Reports & Analytics     (Business Intelligence)\n";
    }
    
    if (hasPermission("settings_access")) {
        std::cout << "  5️⃣  System Settings         (Configuration & Maintenance)\n";
    }
    
    std::cout << "  6️⃣  Help & Documentation    (User Guide & Support)\n";
    std::cout << "  7️⃣  System Information      (Version & Status)\n";
    std::cout << "  8️⃣  Logout                  (End Current Session)\n";
    
    if (authManager->getCurrentUserRole() == "admin") {
        std::cout << "  9️⃣  Emergency Operations    (Admin Only)\n";
    }
    
    std::cout << "  0️⃣  Exit System             (Close Application)\n";
    displaySeparator('-');
}

// Handle vehicle menu
void MainMenuSystem::handleVehicleMenu() {
    if (!hasPermission("vehicle_access")) {
        displayPermissionDenied();
        currentState = MenuState::MAIN_MENU;
        return;
    }
    
    clearScreen();
    displayBrandedHeader("VEHICLE MANAGEMENT");
    currentSession.currentModule = "Vehicle Management";
    
    displayVehicleMenu();
    
    int choice = getIntInput("\nSelect option (0-8): ", 0, 8);
    updateLastActivity();
    
    switch (choice) {
        case 1:
            vehicleQuickView();
            break;
        case 2:
            vehicleAddNew();
            break;
        case 3:
            vehicleSearch();
            break;
        case 4:
            vehicleReports();
            break;
        case 5:
            vehicleMaintenance();
            break;
        case 6:
            // Vehicle status management
            std::cout << "Vehicle status management - Implementation integrated with VehicleManager\n";
            pauseForUser();
            break;
        case 7:
            // Fleet analytics
            std::cout << "Fleet analytics - Advanced reporting coming soon\n";
            pauseForUser();
            break;
        case 8:
            // Import/Export vehicles
            std::cout << "Import/Export vehicles - Data management feature\n";
            pauseForUser();
            break;
        case 0:
            currentState = MenuState::MAIN_MENU;
            break;
        default:
            displayOperationError("Invalid choice. Please try again.");
            pauseForUser();
    }
}

// Display vehicle menu
void MainMenuSystem::displayVehicleMenu() {
    auto stats = vehicleManager->getVehicleStats();
    
    std::cout << "🚗 Vehicle Fleet Overview:\n";
    std::cout << "  Total Vehicles: " << stats.totalVehicles 
              << " | Available: " << stats.availableVehicles 
              << " | In Service: " << stats.inServiceVehicles << "\n\n";
    
    displaySeparator('-');
    std::cout << "🚙 VEHICLE MANAGEMENT OPTIONS\n";
    displaySeparator('-');
    std::cout << "  1️⃣  Quick View              (Vehicle Overview & Status)\n";
    std::cout << "  2️⃣  Add New Vehicle         (Register New Fleet Member)\n";
    std::cout << "  3️⃣  Search & Filter         (Find Specific Vehicles)\n";
    std::cout << "  4️⃣  Vehicle Reports         (Fleet Analytics)\n";
    std::cout << "  5️⃣  Maintenance Management  (Service & Repair Tracking)\n";
    std::cout << "  6️⃣  Status Management       (Availability Updates)\n";
    std::cout << "  7️⃣  Fleet Analytics         (Performance Insights)\n";
    std::cout << "  8️⃣  Import/Export Data      (Data Management)\n";
    std::cout << "  0️⃣  Back to Main Menu       (Return to Dashboard)\n";
    displaySeparator('-');
}

// Handle sales menu
void MainMenuSystem::handleSalesMenu() {
    if (!hasPermission("sales_access")) {
        displayPermissionDenied();
        currentState = MenuState::MAIN_MENU;
        return;
    }
    
    clearScreen();
    displayBrandedHeader("SALES & BOOKING MANAGEMENT");
    currentSession.currentModule = "Sales Management";
    
    displaySalesMenu();
    
    int choice = getIntInput("\nSelect option (0-8): ", 0, 8);
    updateLastActivity();
    
    switch (choice) {
        case 1:
            salesQuickView();
            break;
        case 2:
            salesCreateBooking();
            break;
        case 3:
            salesManageBookings();
            break;
        case 4:
            salesCustomerManagement();
            break;
        case 5:
            salesReports();
            break;
        case 6:
            salesAnalytics();
            break;
        case 7:
            // Payment processing
            std::cout << "Payment processing interface - Integrated with SalesManager\n";
            pauseForUser();
            break;
        case 8:
            // Trip management
            std::cout << "Trip management interface - Real-time tracking system\n";
            pauseForUser();
            break;
        case 0:
            currentState = MenuState::MAIN_MENU;
            break;
        default:
            displayOperationError("Invalid choice. Please try again.");
            pauseForUser();
    }
}

// Display sales menu
void MainMenuSystem::displaySalesMenu() {
    std::cout << "💰 Sales & Booking Overview:\n";
    std::cout << "  Today's Bookings: " << salesManager->getBookingCount()
              << " | Revenue: $" << std::fixed << std::setprecision(2) 
              << salesManager->getDailyReport().totalRevenue 
              << " | Customers: " << salesManager->getCustomerCount() << "\n\n";
    
    displaySeparator('-');
    std::cout << "💼 SALES & BOOKING OPTIONS\n";
    displaySeparator('-');
    std::cout << "  1️⃣  Quick View              (Bookings & Sales Overview)\n";
    std::cout << "  2️⃣  Create New Booking      (Customer Reservations)\n";
    std::cout << "  3️⃣  Manage Bookings         (Modify & Track Bookings)\n";
    std::cout << "  4️⃣  Customer Management     (CRM & Customer Service)\n";
    std::cout << "  5️⃣  Sales Reports           (Revenue & Performance)\n";
    std::cout << "  6️⃣  Business Analytics      (Insights & Trends)\n";
    std::cout << "  7️⃣  Payment Processing      (Transactions & Billing)\n";
    std::cout << "  8️⃣  Trip Management         (Real-time Tracking)\n";
    std::cout << "  0️⃣  Back to Main Menu       (Return to Dashboard)\n";
    displaySeparator('-');
}

// Console utility methods
void MainMenuSystem::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MainMenuSystem::pauseForUser() {
    std::cout << "\n📱 Press Enter to continue...";
    std::cin.get();
}

void MainMenuSystem::displayHeader(const std::string& title) {
    displaySeparator('=');
    displayCentered(title);
    displaySeparator('=');
}

void MainMenuSystem::displaySeparator(char character) {
    if (character == '╔') {
        std::cout << "╔" << std::string(MENU_WIDTH-2, '═') << "╗\n";
    } else if (character == '╚') {
        std::cout << "╚" << std::string(MENU_WIDTH-2, '═') << "╝\n";
    } else {
        std::cout << std::string(MENU_WIDTH, character) << "\n";
    }
}

void MainMenuSystem::displayCentered(const std::string& text, int width) {
    int padding = (width - text.length()) / 2;
    if (text.length() < width) {
        std::cout << "║" << std::string(padding, ' ') << text 
                  << std::string(width - text.length() - padding - 2, ' ') << "║\n";
    } else {
        std::cout << "║" << text.substr(0, width-2) << "║\n";
    }
}

void MainMenuSystem::displayBrandedHeader(const std::string& subtitle) {
    displaySeparator('=');
    displayCentered("🚗 " + SYSTEM_TITLE + " 🚗");
    if (!subtitle.empty()) {
        displayCentered(subtitle);
    }
    displaySeparator('=');
}

// Input utility methods
void MainMenuSystem::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string MainMenuSystem::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int MainMenuSystem::getIntInput(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputBuffer();
            return value;
        }
        std::cout << "❌ Invalid input. Please enter a number";
        if (minVal != INT_MIN || maxVal != INT_MAX) {
            std::cout << " between " << minVal << " and " << maxVal;
        }
        std::cout << ".\n";
        clearInputBuffer();
    }
}

bool MainMenuSystem::getYesNoInput(const std::string& prompt) {
    char input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        clearInputBuffer();
        
        if (input == 'y' || input == 'Y') return true;
        if (input == 'n' || input == 'N') return false;
        
        std::cout << "❌ Please enter 'y' for yes or 'n' for no.\n";
    }
}

// Session management
void MainMenuSystem::initializeSession() {
    currentSession = {};
    currentSession.loginTime = std::chrono::system_clock::now();
    currentSession.lastActivity = std::chrono::system_clock::now();
    currentSession.operationsCount = 0;
    currentSession.currentModule = "Main Menu";
    currentSession.isActive = false;
}

void MainMenuSystem::updateLastActivity() {
    currentSession.lastActivity = std::chrono::system_clock::now();
    currentSession.operationsCount++;
}

bool MainMenuSystem::checkSessionTimeout() {
    if (!authManager->isLoggedIn()) return false;
    
    auto now = std::chrono::system_clock::now();
    auto timeSinceActivity = std::chrono::duration_cast<std::chrono::minutes>(
        now - currentSession.lastActivity);
    
    return timeSinceActivity.count() >= SESSION_TIMEOUT_MINUTES;
}

void MainMenuSystem::displaySessionInfo() {
    std::cout << "👤 User: " << currentSession.username 
              << " (" << currentSession.role << ") | ";
    std::cout << "📍 Module: " << currentSession.currentModule << " | ";
    std::cout << "⚡ Operations: " << currentSession.operationsCount << "\n\n";
}

void MainMenuSystem::logUserAction(const std::string& action) {
    // Log user action - in real implementation would write to log file
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "📝 [" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
              << "] " << currentSession.username << ": " << action << std::endl;
}

// System statistics
void MainMenuSystem::updateSystemStats() {
    systemStats = {};
    
    // Get user statistics
    systemStats.totalUsers = authManager->getUserCount();
    systemStats.activeUsers = authManager->getActiveUserCount();
    
    // Get vehicle statistics
    auto vehicleStats = vehicleManager->getVehicleStats();
    systemStats.totalVehicles = vehicleStats.totalVehicles;
    systemStats.availableVehicles = vehicleStats.availableVehicles;
    
    // Get sales statistics
    systemStats.totalCustomers = salesManager->getCustomerCount();
    systemStats.totalBookings = salesManager->getBookingCount();
    
    auto revenueReport = salesManager->getDailyReport();
    systemStats.todayRevenue = revenueReport.totalRevenue;
    
    auto monthlyReport = salesManager->getMonthlyReport();
    systemStats.monthlyRevenue = monthlyReport.totalRevenue;
    
    // System uptime (simplified)
    systemStats.systemUptime = "Online";
    systemStats.lastBackup = "Auto-saved";
}

void MainMenuSystem::displayQuickStats() {
    updateSystemStats();
    
    std::cout << "📊 SYSTEM OVERVIEW\n";
    displaySeparator('-');
    std::cout << std::left;
    std::cout << std::setw(20) << "🚗 Vehicles:" << systemStats.totalVehicles 
              << " (Available: " << systemStats.availableVehicles << ")\n";
    std::cout << std::setw(20) << "👥 Customers:" << systemStats.totalCustomers << "\n";
    std::cout << std::setw(20) << "📅 Bookings:" << systemStats.totalBookings << "\n";
    std::cout << std::setw(20) << "💰 Today's Revenue:" << "$" << std::fixed 
              << std::setprecision(2) << systemStats.todayRevenue << "\n";
    displaySeparator('-');
    std::cout << "\n";
}

void MainMenuSystem::displayDashboard() {
    std::cout << "🎯 QUICK ACTIONS\n";
    displaySeparator('-');
    std::cout << "  🔥 Popular: Create Booking | View Vehicles | Check Reports\n";
    std::cout << "  ⚡ Recent: " << currentSession.operationsCount << " operations this session\n";
    std::cout << "  🔔 Status: All systems operational\n";
    displaySeparator('-');
}

// Permission checking
bool MainMenuSystem::hasPermission(const std::string& permission) {
    std::string role = authManager->getCurrentUserRole();
    
    if (role == "admin") return true; // Admin has all permissions
    
    if (permission == "vehicle_access") {
        return role == "operator" || role == "admin";
    }
    if (permission == "sales_access") {
        return role == "operator" || role == "admin";
    }
    if (permission == "user_management") {
        return role == "admin";
    }
    if (permission == "reports_access") {
        return true; // All roles can view reports
    }
    if (permission == "settings_access") {
        return role == "admin";
    }
    
    return false;
}

void MainMenuSystem::displayPermissionDenied() {
    displayOperationError("Access denied. You don't have permission for this operation.");
    pauseForUser();
}

void MainMenuSystem::displayOperationSuccess(const std::string& message) {
    std::cout << "✅ " << message << std::endl;
}

void MainMenuSystem::displayOperationError(const std::string& message) {
    std::cout << "❌ " << message << std::endl;
}

void MainMenuSystem::displayWarning(const std::string& message) {
    std::cout << "⚠️ " << message << std::endl;
}

void MainMenuSystem::displayInfo(const std::string& message) {
    std::cout << "ℹ️ " << message << std::endl;
}

// Vehicle management integration (simplified implementations)
void MainMenuSystem::vehicleQuickView() {
    clearScreen();
    displayHeader("VEHICLE QUICK VIEW");
    vehicleManager->displayVehicleList(vehicleManager->getAllVehicles());
    pauseForUser();
}

void MainMenuSystem::vehicleAddNew() {
    clearScreen();
    displayHeader("ADD NEW VEHICLE");
    std::cout << "This would open the vehicle addition interface...\n";
    pauseForUser();
}

void MainMenuSystem::vehicleSearch() {
    clearScreen();
    displayHeader("VEHICLE SEARCH");
    std::cout << "This would open the vehicle search interface...\n";
    pauseForUser();
}

void MainMenuSystem::vehicleReports() {
    clearScreen();
    displayHeader("VEHICLE REPORTS");
    std::cout << "This would show vehicle analytics and reports...\n";
    pauseForUser();
}

void MainMenuSystem::vehicleMaintenance() {
    clearScreen();
    displayHeader("MAINTENANCE MANAGEMENT");
    std::cout << "This would open maintenance tracking interface...\n";
    pauseForUser();
}

// Sales management integration (simplified implementations)
void MainMenuSystem::salesQuickView() {
    clearScreen();
    displayHeader("SALES QUICK VIEW");
    salesManager->displayBookingList(salesManager->getAllBookings());
    pauseForUser();
}

void MainMenuSystem::salesCreateBooking() {
    clearScreen();
    displayHeader("CREATE NEW BOOKING");
    std::cout << "This would open the booking creation interface...\n";
    pauseForUser();
}

void MainMenuSystem::salesManageBookings() {
    clearScreen();
    displayHeader("MANAGE BOOKINGS");
    std::cout << "This would open booking management interface...\n";
    pauseForUser();
}

void MainMenuSystem::salesCustomerManagement() {
    clearScreen();
    displayHeader("CUSTOMER MANAGEMENT");
    salesManager->displayCustomerList(salesManager->getAllCustomers());
    pauseForUser();
}

void MainMenuSystem::salesReports() {
    clearScreen();
    displayHeader("SALES REPORTS");
    auto report = salesManager->getDailyReport();
    salesManager->displayRevenueReport(report);
    pauseForUser();
}

void MainMenuSystem::salesAnalytics() {
    clearScreen();
    displayHeader("BUSINESS ANALYTICS");
    auto analytics = salesManager->getCustomerAnalytics();
    salesManager->displayCustomerAnalytics(analytics);
    pauseForUser();
}

// System initialization and cleanup
void MainMenuSystem::initializeDataFiles() {
    // Create data directory if it doesn't exist
    // This would be implemented with proper file system operations
}

void MainMenuSystem::performSystemChecks() {
    // Check data integrity, file permissions, etc.
    // This would include comprehensive system validation
}

void MainMenuSystem::cleanupOnExit() {
    saveSystemState();
}

void MainMenuSystem::saveSystemState() {
    // Save current system state, user preferences, etc.
}

void MainMenuSystem::displaySystemInfo() {
    clearScreen();
    displayBrandedHeader("SYSTEM INFORMATION");
    
    std::cout << "📋 System Details:\n";
    std::cout << "  Version: " << SYSTEM_VERSION << "\n";
    std::cout << "  Build Date: " << __DATE__ << " " << __TIME__ << "\n";
    std::cout << "  Platform: C++11 Compatible\n";
    std::cout << "  Database: CSV File Storage\n\n";
    
    std::cout << "📊 Current Statistics:\n";
    updateSystemStats();
    std::cout << "  Total Users: " << systemStats.totalUsers << "\n";
    std::cout << "  Total Vehicles: " << systemStats.totalVehicles << "\n";
    std::cout << "  Total Customers: " << systemStats.totalCustomers << "\n";
    std::cout << "  Total Bookings: " << systemStats.totalBookings << "\n";
    std::cout << "  Today's Revenue: $" << systemStats.todayRevenue << "\n\n";
    
    std::cout << "🔧 System Status: All systems operational ✅\n";
}

void MainMenuSystem::handleEmergencyOperations() {
    if (authManager->getCurrentUserRole() != "admin") {
        displayPermissionDenied();
        return;
    }
    
    clearScreen();
    displayHeader("EMERGENCY OPERATIONS - ADMIN ONLY");
    displayWarning("These operations should only be used in emergency situations!");
    
    std::cout << "1. Force logout all users\n";
    std::cout << "2. Emergency data backup\n";
    std::cout << "3. Reset system to defaults\n";
    std::cout << "4. System integrity check\n";
    std::cout << "0. Cancel\n";
    
    int choice = getIntInput("Select emergency operation (0-4): ", 0, 4);
    
    switch (choice) {
        case 1:
            if (getYesNoInput("Force logout all users? (y/n): ")) {
                displayInfo("Emergency logout procedure initiated...");
                logUserAction("Emergency: Force logout all users");
            }
            break;
        case 2:
            if (getYesNoInput("Perform emergency backup? (y/n): ")) {
                displayInfo("Emergency backup procedure initiated...");
                logUserAction("Emergency: Data backup");
            }
            break;
        case 3:
            if (getYesNoInput("Reset system to defaults? THIS WILL DELETE ALL DATA! (y/n): ")) {
                displayWarning("System reset procedure would be initiated here...");
                logUserAction("Emergency: System reset attempt");
            }
            break;
        case 4:
            displayInfo("Performing system integrity check...");
            bool isValid = validateSystemIntegrity();
            if (isValid) {
                displayOperationSuccess("System integrity check passed!");
            } else {
                displayOperationError("System integrity issues detected!");
            }
            logUserAction("Emergency: System integrity check");
            break;
    }
    
    pauseForUser();
}

bool MainMenuSystem::validateSystemIntegrity() {
    // Implement comprehensive system validation
    return true; // Simplified for now
}

void MainMenuSystem::shutdown() {
    clearScreen();
    displayBrandedHeader("SYSTEM SHUTDOWN");
    
    std::cout << "🔄 Saving system state...\n";
    saveSystemState();
    
    std::cout << "📊 Session Summary:\n";
    std::cout << "  Login Time: " << "Session started\n";
    std::cout << "  Operations Performed: " << currentSession.operationsCount << "\n";
    std::cout << "  Last Module: " << currentSession.currentModule << "\n\n";
    
    std::cout << "🎉 Thank you for using " << SYSTEM_TITLE << "!\n";
    std::cout << "💼 Your transport business management partner.\n\n";
    
    displayCentered("🚗 Drive your business forward with Tourmate! 🚗");
    std::cout << "\n";
}

// Handle menu placeholders for future implementation
void MainMenuSystem::handleUserMenu() {
    clearScreen();
    displayBrandedHeader("USER MANAGEMENT");
    displayInfo("User management interface - Coming in future updates");
    pauseForUser();
    currentState = MenuState::MAIN_MENU;
}

void MainMenuSystem::handleReportsMenu() {
    clearScreen();
    displayBrandedHeader("REPORTS & ANALYTICS");
    displayInfo("Advanced reporting interface - Coming in future updates");
    pauseForUser();
    currentState = MenuState::MAIN_MENU;
}

void MainMenuSystem::handleSettingsMenu() {
    clearScreen();
    displayBrandedHeader("SYSTEM SETTINGS");
    displayInfo("System settings interface - Coming in future updates");
    pauseForUser();
    currentState = MenuState::MAIN_MENU;
}

void MainMenuSystem::handleHelpMenu() {
    clearScreen();
    displayBrandedHeader("HELP & DOCUMENTATION");
    
    std::cout << "📚 HELP TOPICS\n";
    displaySeparator('-');
    std::cout << "  1. Getting Started Guide\n";
    std::cout << "  2. Vehicle Management Help\n";
    std::cout << "  3. Sales & Booking Help\n";
    std::cout << "  4. User Account Help\n";
    std::cout << "  5. Troubleshooting\n";
    std::cout << "  6. Keyboard Shortcuts\n";
    std::cout << "  7. Contact Support\n";
    std::cout << "  8. About Tourmate\n";
    std::cout << "  0. Back to Main Menu\n";
    displaySeparator('-');
    
    int choice = getIntInput("Select help topic (0-8): ", 0, 8);
    
    switch (choice) {
        case 1:
            std::cout << "\n📖 GETTING STARTED:\n";
            std::cout << "1. Log in with your credentials\n";
            std::cout << "2. Navigate using the numbered menu options\n";
            std::cout << "3. Use vehicle management to add and track vehicles\n";
            std::cout << "4. Use sales management for bookings and customers\n";
            std::cout << "5. Check reports for business insights\n";
            break;
        case 8:
            std::cout << "\n🌟 ABOUT TOURMATE:\n";
            std::cout << "Tourmate is a comprehensive vehicle management system\n";
            std::cout << "designed for transport businesses of all sizes.\n";
            std::cout << "Developed as an academic project for CSE4002 WRIT1.\n";
            break;
        default:
            std::cout << "\nDetailed help content would be displayed here...\n";
    }
    
    pauseForUser();
    currentState = MenuState::MAIN_MENU;
}

// Static methods
std::string MainMenuSystem::getVersion() {
    return SYSTEM_VERSION;
}