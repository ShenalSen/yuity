#include "src/headers/SalesManager.h"
#include "src/headers/VehicleManager.h"
#include "src/headers/AuthenticationManager.h"
#include "src/headers/Utilities.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <string>
#include <chrono>

class SalesTestSuite {
private:
    std::shared_ptr<AuthenticationManager> authManager;
    std::shared_ptr<VehicleManager> vehicleManager;
    std::shared_ptr<SalesManager> salesManager;
    
    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    std::string getInput(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    }
    
    int getIntInput(const std::string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            }
            std::cout << "Invalid input. Please enter a number";
            if (minVal != INT_MIN || maxVal != INT_MAX) {
                std::cout << " between " << minVal << " and " << maxVal;
            }
            std::cout << ".\n";
            clearInputBuffer();
        }
    }
    
    double getDoubleInput(const std::string& prompt, double minVal = -DBL_MAX, double maxVal = DBL_MAX) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            }
            std::cout << "Invalid input. Please enter a number";
            if (minVal != -DBL_MAX || maxVal != DBL_MAX) {
                std::cout << " between " << minVal << " and " << maxVal;
            }
            std::cout << ".\n";
            clearInputBuffer();
        }
    }
    
    SalesManager::PaymentMethod selectPaymentMethod() {
        std::cout << "\nSelect Payment Method:\n";
        std::cout << "1. Cash\n";
        std::cout << "2. Credit Card\n";
        std::cout << "3. Debit Card\n";
        std::cout << "4. Digital Wallet\n";
        std::cout << "5. Bank Transfer\n";
        std::cout << "0. Cancel\n";
        
        int choice = getIntInput("Enter choice (0-5): ", 0, 5);
        
        switch (choice) {
            case 1: return SalesManager::PaymentMethod::CASH;
            case 2: return SalesManager::PaymentMethod::CREDIT_CARD;
            case 3: return SalesManager::PaymentMethod::DEBIT_CARD;
            case 4: return SalesManager::PaymentMethod::DIGITAL_WALLET;
            case 5: return SalesManager::PaymentMethod::BANK_TRANSFER;
            default: return SalesManager::PaymentMethod::UNKNOWN;
        }
    }
    
    SalesManager::TripType selectTripType() {
        std::cout << "\nSelect Trip Type:\n";
        std::cout << "1. One Way\n";
        std::cout << "2. Round Trip\n";
        std::cout << "3. Multi Stop\n";
        std::cout << "4. Charter\n";
        std::cout << "0. Cancel\n";
        
        int choice = getIntInput("Enter choice (0-4): ", 0, 4);
        
        switch (choice) {
            case 1: return SalesManager::TripType::ONE_WAY;
            case 2: return SalesManager::TripType::ROUND_TRIP;
            case 3: return SalesManager::TripType::MULTI_STOP;
            case 4: return SalesManager::TripType::CHARTER;
            default: return SalesManager::TripType::UNKNOWN;
        }
    }
    
    std::chrono::system_clock::time_point getDateTimeInput(const std::string& prompt) {
        std::cout << prompt << " (YYYY-MM-DD HH:MM): ";
        std::string input;
        std::getline(std::cin, input);
        
        // Simple parsing - in real implementation would be more robust
        std::tm tm = {};
        std::stringstream ss(input);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
        
        if (ss.fail()) {
            std::cout << "Invalid date format. Using current time.\n";
            return std::chrono::system_clock::now();
        }
        
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
    
    SalesManager::Customer createCustomerFromInput() {
        SalesManager::Customer customer;
        
        std::cout << "\n=== ENTER CUSTOMER DETAILS ===\n";
        
        // Customer ID
        std::string id = getInput("Customer ID (leave empty for auto-generation): ");
        if (id.empty()) {
            id = salesManager->getNextAvailableCustomerId();
            std::cout << "Generated ID: " << id << "\n";
        }
        customer.customerId = id;
        
        // Customer details
        customer.name = getInput("Full Name: ");
        customer.email = getInput("Email: ");
        customer.phone = getInput("Phone: ");
        customer.address = getInput("Address: ");
        customer.registrationDate = std::chrono::system_clock::now();
        customer.totalBookings = 0;
        customer.totalSpent = 0.0;
        customer.isVip = false;
        
        return customer;
    }
    
    void displayPermissions() {
        std::cout << "\n=== CURRENT PERMISSIONS ===\n";
        std::cout << "User: " << salesManager->getCurrentUsername() 
                  << " (" << salesManager->getCurrentUserRole() << ")\n";
        std::cout << "Can View Sales: " << (salesManager->canViewSales() ? "Yes" : "No") << "\n";
        std::cout << "Can Create Sales: " << (salesManager->canCreateSales() ? "Yes" : "No") << "\n";
        std::cout << "Can Edit Sales: " << (salesManager->canEditSales() ? "Yes" : "No") << "\n";
        std::cout << "Can Delete Sales: " << (salesManager->canDeleteSales() ? "Yes" : "No") << "\n";
        std::cout << "Can Process Refunds: " << (salesManager->canProcessRefunds() ? "Yes" : "No") << "\n";
        std::cout << "Can View Reports: " << (salesManager->canViewReports() ? "Yes" : "No") << "\n";
        std::cout << "============================\n";
    }
    
public:
    SalesTestSuite() {
        authManager = std::make_shared<AuthenticationManager>();
        vehicleManager = std::make_shared<VehicleManager>(authManager, "data/vehicles.csv");
        salesManager = std::make_shared<SalesManager>(authManager, vehicleManager,
                                                     "data/sales.csv", 
                                                     "data/bookings.csv",
                                                     "data/customers.csv");
        
        // Ensure user is logged in
        if (!authManager->isLoggedIn()) {
            std::cout << "Please log in to continue.\n";
            loginMenu();
        }
    }
    
    void loginMenu() {
        std::cout << "\n=== LOGIN REQUIRED ===\n";
        std::cout << "Default users:\n";
        std::cout << "  admin/Admin123 (Full access)\n";
        std::cout << "  operator/Oper123 (Manage sales)\n";
        std::cout << "  viewer/View123 (View only)\n";
        
        std::string username = getInput("Username: ");
        std::string password = getInput("Password: ");
        
        if (authManager->login(username, password)) {
            std::cout << "Login successful!\n";
            displayPermissions();
        } else {
            std::cout << "Login failed. Using guest access (view only).\n";
        }
    }
    
    void testAddCustomer() {
        std::cout << "\n=== ADD CUSTOMER TEST ===\n";
        
        if (!salesManager->canCreateSales()) {
            std::cout << "Access denied. You don't have permission to add customers.\n";
            return;
        }
        
        auto customer = createCustomerFromInput();
        
        if (customer.customerId.empty()) {
            return; // Operation cancelled
        }
        
        if (salesManager->addCustomer(customer)) {
            std::cout << "\n✅ Customer added successfully!\n";
            salesManager->displayCustomer(customer);
        } else {
            std::cout << "\n❌ Failed to add customer: " << salesManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testViewCustomer() {
        std::cout << "\n=== VIEW CUSTOMER TEST ===\n";
        
        if (!salesManager->canViewSales()) {
            std::cout << "Access denied. You don't have permission to view customers.\n";
            return;
        }
        
        std::string id = getInput("Enter Customer ID: ");
        
        auto customer = salesManager->getCustomer(id);
        if (customer) {
            salesManager->displayCustomer(*customer);
        } else {
            std::cout << "❌ Customer not found: " << salesManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testCreateBooking() {
        std::cout << "\n=== CREATE BOOKING TEST ===\n";
        
        if (!salesManager->canCreateSales()) {
            std::cout << "Access denied. You don't have permission to create bookings.\n";
            return;
        }
        
        std::cout << "\n=== BOOKING DETAILS ===\n";
        
        // Customer selection
        std::string customerId = getInput("Customer ID: ");
        auto customer = salesManager->getCustomer(customerId);
        if (!customer) {
            std::cout << "❌ Customer not found. Please add customer first.\n";
            return;
        }
        
        // Vehicle selection
        std::string vehicleId = getInput("Vehicle ID: ");
        auto vehicle = vehicleManager->getVehicle(vehicleId);
        if (!vehicle) {
            std::cout << "❌ Vehicle not found.\n";
            return;
        }
        
        // Trip details
        std::string fromLocation = getInput("From Location: ");
        std::string toLocation = getInput("To Location: ");
        
        auto departureTime = getDateTimeInput("Departure Time");
        
        int passengers = getIntInput("Number of Passengers: ", 1, 100);
        
        auto tripType = selectTripType();
        if (tripType == SalesManager::TripType::UNKNOWN) {
            tripType = SalesManager::TripType::ONE_WAY;
        }
        
        std::string specialRequests = getInput("Special Requests (optional): ");
        
        // Create booking
        std::string bookingId = salesManager->createBooking(customerId, vehicleId, 
                                                           fromLocation, toLocation,
                                                           departureTime, passengers, 
                                                           tripType, specialRequests);
        
        if (!bookingId.empty()) {
            std::cout << "\n✅ Booking created successfully!\n";
            std::cout << "Booking ID: " << bookingId << "\n";
            
            auto booking = salesManager->getBooking(bookingId);
            if (booking) {
                salesManager->displayBooking(*booking);
            }
        } else {
            std::cout << "\n❌ Failed to create booking: " << salesManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testConfirmBooking() {
        std::cout << "\n=== CONFIRM BOOKING TEST ===\n";
        
        if (!salesManager->canEditSales()) {
            std::cout << "Access denied. You don't have permission to confirm bookings.\n";
            return;
        }
        
        std::string bookingId = getInput("Enter Booking ID: ");
        
        auto booking = salesManager->getBooking(bookingId);
        if (!booking) {
            std::cout << "❌ Booking not found: " << salesManager->getLastErrorMessage() << "\n";
            return;
        }
        
        std::cout << "\nBooking details:\n";
        salesManager->displayBooking(*booking);
        
        if (booking->status != SalesManager::BookingStatus::PENDING) {
            std::cout << "❌ Only pending bookings can be confirmed.\n";
            return;
        }
        
        auto paymentMethod = selectPaymentMethod();
        if (paymentMethod == SalesManager::PaymentMethod::UNKNOWN) {
            std::cout << "Operation cancelled.\n";
            return;
        }
        
        if (salesManager->confirmBooking(bookingId, paymentMethod)) {
            std::cout << "\n✅ Booking confirmed successfully!\n";
            
            // Display updated booking
            auto updatedBooking = salesManager->getBooking(bookingId);
            if (updatedBooking) {
                salesManager->displayBooking(*updatedBooking);
            }
        } else {
            std::cout << "\n❌ Failed to confirm booking: " << salesManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testTripManagement() {
        std::cout << "\n=== TRIP MANAGEMENT TEST ===\n";
        
        if (!salesManager->canEditSales()) {
            std::cout << "Access denied. You don't have permission to manage trips.\n";
            return;
        }
        
        std::cout << "1. Start Trip\n";
        std::cout << "2. Complete Trip\n";
        std::cout << "3. Cancel Booking\n";
        std::cout << "4. Generate Receipt\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-4): ", 0, 4);
        
        std::string bookingId = getInput("Enter Booking ID: ");
        
        switch (choice) {
            case 1: {
                if (salesManager->startTrip(bookingId)) {
                    std::cout << "✅ Trip started successfully!\n";
                } else {
                    std::cout << "❌ Failed to start trip: " << salesManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 2: {
                std::cout << "Enter actual distance? (y/n): ";
                char useActual;
                std::cin >> useActual;
                clearInputBuffer();
                
                double actualDistance = -1.0;
                if (useActual == 'y' || useActual == 'Y') {
                    actualDistance = getDoubleInput("Actual distance (km): ", 0.0, 10000.0);
                }
                
                if (salesManager->completeTrip(bookingId, actualDistance)) {
                    std::cout << "✅ Trip completed successfully!\n";
                } else {
                    std::cout << "❌ Failed to complete trip: " << salesManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 3: {
                std::string reason = getInput("Cancellation reason (optional): ");
                if (salesManager->cancelBooking(bookingId, reason)) {
                    std::cout << "✅ Booking cancelled successfully!\n";
                } else {
                    std::cout << "❌ Failed to cancel booking: " << salesManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 4: {
                std::string receipt = salesManager->generateTripReceipt(bookingId);
                if (!receipt.empty()) {
                    std::cout << receipt;
                } else {
                    std::cout << "❌ Failed to generate receipt.\n";
                }
                break;
            }
        }
    }
    
    void testListAllCustomers() {
        std::cout << "\n=== LIST ALL CUSTOMERS TEST ===\n";
        
        if (!salesManager->canViewSales()) {
            std::cout << "Access denied. You don't have permission to view customers.\n";
            return;
        }
        
        auto customers = salesManager->getAllCustomers();
        std::cout << "Total customers: " << customers.size() << "\n";
        
        if (!customers.empty()) {
            salesManager->displayCustomerList(customers);
        }
    }
    
    void testListAllBookings() {
        std::cout << "\n=== LIST ALL BOOKINGS TEST ===\n";
        
        if (!salesManager->canViewSales()) {
            std::cout << "Access denied. You don't have permission to view bookings.\n";
            return;
        }
        
        auto bookings = salesManager->getAllBookings();
        std::cout << "Total bookings: " << bookings.size() << "\n";
        
        if (!bookings.empty()) {
            salesManager->displayBookingList(bookings);
        }
    }
    
    void testListAllSales() {
        std::cout << "\n=== LIST ALL SALES TEST ===\n";
        
        if (!salesManager->canViewSales()) {
            std::cout << "Access denied. You don't have permission to view sales.\n";
            return;
        }
        
        auto sales = salesManager->getAllSalesRecords();
        std::cout << "Total sales records: " << sales.size() << "\n";
        
        if (!sales.empty()) {
            salesManager->displaySalesRecordList(sales);
        }
    }
    
    void testReports() {
        std::cout << "\n=== REPORTS TEST ===\n";
        
        if (!salesManager->canViewReports()) {
            std::cout << "Access denied. You don't have permission to view reports.\n";
            return;
        }
        
        std::cout << "1. Daily Report\n";
        std::cout << "2. Weekly Report\n";
        std::cout << "3. Monthly Report\n";
        std::cout << "4. Customer Analytics\n";
        std::cout << "5. Top Customers\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-5): ", 0, 5);
        
        switch (choice) {
            case 1: {
                auto report = salesManager->getDailyReport();
                std::cout << "\n=== DAILY REVENUE REPORT ===\n";
                salesManager->displayRevenueReport(report);
                break;
            }
            case 2: {
                auto report = salesManager->getWeeklyReport();
                std::cout << "\n=== WEEKLY REVENUE REPORT ===\n";
                salesManager->displayRevenueReport(report);
                break;
            }
            case 3: {
                auto report = salesManager->getMonthlyReport();
                std::cout << "\n=== MONTHLY REVENUE REPORT ===\n";
                salesManager->displayRevenueReport(report);
                break;
            }
            case 4: {
                auto analytics = salesManager->getCustomerAnalytics();
                std::cout << "\n=== CUSTOMER ANALYTICS ===\n";
                salesManager->displayCustomerAnalytics(analytics);
                break;
            }
            case 5: {
                int count = getIntInput("Number of top customers to show: ", 1, 50);
                auto topCustomers = salesManager->getTopCustomers(count);
                std::cout << "\nTop " << count << " Customers by Spending:\n";
                salesManager->displayCustomerList(topCustomers);
                break;
            }
        }
    }
    
    void testSearchFunctions() {
        std::cout << "\n=== SEARCH FUNCTIONS TEST ===\n";
        
        if (!salesManager->canViewSales()) {
            std::cout << "Access denied. You don't have permission to search.\n";
            return;
        }
        
        std::cout << "1. Search Sales Records\n";
        std::cout << "2. Search Customers\n";
        std::cout << "3. Filter Bookings by Status\n";
        std::cout << "4. Filter Sales by Vehicle\n";
        std::cout << "5. Filter Sales by Customer\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-5): ", 0, 5);
        
        switch (choice) {
            case 1: {
                std::string searchTerm = getInput("Enter search term: ");
                auto results = salesManager->searchSales(searchTerm);
                std::cout << "\nSales Search Results (" << results.size() << " found):\n";
                salesManager->displaySalesRecordList(results);
                break;
            }
            case 2: {
                std::string searchTerm = getInput("Enter search term: ");
                auto results = salesManager->searchCustomers(searchTerm);
                std::cout << "\nCustomer Search Results (" << results.size() << " found):\n";
                salesManager->displayCustomerList(results);
                break;
            }
            case 3: {
                std::cout << "Select Status:\n";
                std::cout << "1. Pending\n2. Confirmed\n3. In Progress\n4. Completed\n5. Cancelled\n";
                int statusChoice = getIntInput("Enter choice (1-5): ", 1, 5);
                
                SalesManager::BookingStatus status;
                switch (statusChoice) {
                    case 1: status = SalesManager::BookingStatus::PENDING; break;
                    case 2: status = SalesManager::BookingStatus::CONFIRMED; break;
                    case 3: status = SalesManager::BookingStatus::IN_PROGRESS; break;
                    case 4: status = SalesManager::BookingStatus::COMPLETED; break;
                    case 5: status = SalesManager::BookingStatus::CANCELLED; break;
                    default: return;
                }
                
                auto results = salesManager->filterBookingsByStatus(status);
                std::cout << "\nBookings with status " << salesManager->bookingStatusToString(status) 
                          << " (" << results.size() << " found):\n";
                salesManager->displayBookingList(results);
                break;
            }
            case 4: {
                std::string vehicleId = getInput("Enter Vehicle ID: ");
                auto results = salesManager->filterSalesByVehicle(vehicleId);
                std::cout << "\nSales for Vehicle " << vehicleId << " (" << results.size() << " found):\n";
                salesManager->displaySalesRecordList(results);
                break;
            }
            case 5: {
                std::string customerId = getInput("Enter Customer ID: ");
                auto results = salesManager->filterSalesByCustomer(customerId);
                std::cout << "\nSales for Customer " << customerId << " (" << results.size() << " found):\n";
                salesManager->displaySalesRecordList(results);
                break;
            }
        }
    }
    
    void runMainMenu() {
        while (true) {
            std::cout << "\n" << std::string(60, '=') << "\n";
            std::cout << "           TOURMATE SALES MANAGEMENT SYSTEM\n";
            std::cout << std::string(60, '=') << "\n";
            std::cout << "Current User: " << salesManager->getCurrentUsername() 
                      << " (" << salesManager->getCurrentUserRole() << ")\n";
            std::cout << "Sales Records: " << salesManager->getSalesCount() 
                      << " | Bookings: " << salesManager->getBookingCount()
                      << " | Customers: " << salesManager->getCustomerCount() << "\n";
            std::cout << std::string(60, '-') << "\n";
            
            std::cout << " 1. Add Customer               11. List All Sales Records\n";
            std::cout << " 2. View Customer              12. Search Functions\n";
            std::cout << " 3. Create Booking             13. Reports & Analytics\n";
            std::cout << " 4. Confirm Booking            14. Trip Management\n";
            std::cout << " 5. List All Customers         15. Display Permissions\n";
            std::cout << " 6. List All Bookings          16. Login/Switch User\n";
            std::cout << " 7. View Booking               17. Logout\n";
            std::cout << " 8. Update Customer            18. Help\n";
            std::cout << " 9. Delete Customer            19. About\n";
            std::cout << "10. Add Sales Record           0.  Exit\n";
            std::cout << std::string(60, '=') << "\n";
            
            int choice = getIntInput("Enter your choice (0-19): ", 0, 19);
            
            switch (choice) {
                case 1: testAddCustomer(); break;
                case 2: testViewCustomer(); break;
                case 3: testCreateBooking(); break;
                case 4: testConfirmBooking(); break;
                case 5: testListAllCustomers(); break;
                case 6: testListAllBookings(); break;
                case 7: {
                    std::string bookingId = getInput("Enter Booking ID: ");
                    auto booking = salesManager->getBooking(bookingId);
                    if (booking) {
                        salesManager->displayBooking(*booking);
                    } else {
                        std::cout << "❌ Booking not found.\n";
                    }
                    break;
                }
                case 8: {
                    std::cout << "Update Customer feature - Implementation pending\n";
                    break;
                }
                case 9: {
                    std::string customerId = getInput("Enter Customer ID to delete: ");
                    if (salesManager->deleteCustomer(customerId)) {
                        std::cout << "✅ Customer deleted successfully!\n";
                    } else {
                        std::cout << "❌ Failed to delete customer.\n";
                    }
                    break;
                }
                case 10: {
                    std::cout << "Add Sales Record feature - Implementation pending\n";
                    break;
                }
                case 11: testListAllSales(); break;
                case 12: testSearchFunctions(); break;
                case 13: testReports(); break;
                case 14: testTripManagement(); break;
                case 15: displayPermissions(); break;
                case 16: loginMenu(); break;
                case 17: 
                    authManager->logout();
                    std::cout << "Logged out successfully.\n";
                    loginMenu();
                    break;
                case 18:
                    std::cout << "\nSales Management System Help:\n";
                    std::cout << "- Use the menu options to manage sales and bookings\n";
                    std::cout << "- Your access level determines available operations\n";
                    std::cout << "- All operations are logged for audit purposes\n";
                    std::cout << "- Data is automatically saved to CSV files\n";
                    std::cout << "- Reports provide insights into revenue and customer behavior\n";
                    break;
                case 19:
                    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
                    std::cout << "║              TOURMATE SALES MANAGEMENT SYSTEM           ║\n";
                    std::cout << "║                     Version 1.0                         ║\n";
                    std::cout << "║                                                          ║\n";
                    std::cout << "║  Features:                                               ║\n";
                    std::cout << "║  • Complete Sales & Booking Management                  ║\n";
                    std::cout << "║  • Customer Relationship Management                     ║\n";
                    std::cout << "║  • Payment Processing & Revenue Analytics               ║\n";
                    std::cout << "║  • Trip Management & Receipt Generation                 ║\n";
                    std::cout << "║  • Role-based Access Control                            ║\n";
                    std::cout << "║  • Comprehensive Reporting & Analytics                  ║\n";
                    std::cout << "║  • Integration with Vehicle Management                  ║\n";
                    std::cout << "║                                                          ║\n";
                    std::cout << "║  Developed for CSE4002 WRIT1 - Academic Project         ║\n";
                    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
                    break;
                case 0:
                    std::cout << "Thank you for using Tourmate Sales Management System!\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
            
            if (choice != 0) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        }
    }
};

int main() {
    try {
        std::cout << "╔══════════════════════════════════════════════════════════╗\n";
        std::cout << "║       WELCOME TO TOURMATE SALES MANAGEMENT SYSTEM       ║\n";
        std::cout << "║                      Test Suite                          ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n";
        
        SalesTestSuite testSuite;
        testSuite.runMainMenu();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}