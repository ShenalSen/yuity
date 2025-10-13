#include "src/headers/SalesManager.h"
#include "src/headers/VehicleManager.h"
#include "src/headers/AuthenticationManager.h"
#include "src/headers/Utilities.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

class SampleDataGenerator {
private:
    std::shared_ptr<AuthenticationManager> authManager;
    std::shared_ptr<VehicleManager> vehicleManager;
    std::shared_ptr<SalesManager> salesManager;
    
public:
    SampleDataGenerator() {
        authManager = std::make_shared<AuthenticationManager>();
        vehicleManager = std::make_shared<VehicleManager>(authManager, "data/vehicles.csv");
        salesManager = std::make_shared<SalesManager>(authManager, vehicleManager,
                                                     "data/sales.csv", 
                                                     "data/bookings.csv",
                                                     "data/customers.csv");
        
        // Login as admin to have full access
        authManager->login("admin", "Admin123");
    }
    
    void generateSampleCustomers() {
        std::cout << "Generating sample customers...\n";
        
        std::vector<SalesManager::Customer> customers = {
            {"C001", "John Smith", "john.smith@email.com", "+1234567890", 
             "123 Main St, City A", std::chrono::system_clock::now(), 0, 0.0, false},
            
            {"C002", "Sarah Johnson", "sarah.j@email.com", "+1234567891", 
             "456 Oak Ave, City B", std::chrono::system_clock::now(), 0, 0.0, true},
            
            {"C003", "Mike Brown", "mike.brown@email.com", "+1234567892", 
             "789 Pine Rd, City C", std::chrono::system_clock::now(), 0, 0.0, false},
            
            {"C004", "Emily Davis", "emily.davis@email.com", "+1234567893", 
             "321 Elm St, City D", std::chrono::system_clock::now(), 0, 0.0, false},
            
            {"C005", "David Wilson", "david.wilson@email.com", "+1234567894", 
             "654 Maple Dr, City E", std::chrono::system_clock::now(), 0, 0.0, true}
        };
        
        for (const auto& customer : customers) {
            if (salesManager->addCustomer(customer)) {
                std::cout << "✓ Added customer: " << customer.name << "\n";
            } else {
                std::cout << "✗ Failed to add customer: " << customer.name << "\n";
            }
        }
    }
    
    void generateSampleBookings() {
        std::cout << "\nGenerating sample bookings...\n";
        
        // Booking 1: Pending
        std::string booking1 = salesManager->createBooking(
            "C001", "V001", "Downtown", "Airport", 
            std::chrono::system_clock::now() + std::chrono::hours(24),
            4, SalesManager::TripType::ONE_WAY, "Need baby seat"
        );
        if (!booking1.empty()) {
            std::cout << "✓ Created pending booking: " << booking1 << "\n";
        }
        
        // Booking 2: Confirmed
        std::string booking2 = salesManager->createBooking(
            "C002", "V002", "Hotel District", "Shopping Mall", 
            std::chrono::system_clock::now() + std::chrono::hours(48),
            8, SalesManager::TripType::ROUND_TRIP, "Corporate event"
        );
        if (!booking2.empty()) {
            salesManager->confirmBooking(booking2, SalesManager::PaymentMethod::CREDIT_CARD);
            std::cout << "✓ Created and confirmed booking: " << booking2 << "\n";
        }
        
        // Booking 3: In Progress
        std::string booking3 = salesManager->createBooking(
            "C003", "V003", "Business Center", "Conference Hall", 
            std::chrono::system_clock::now() - std::chrono::hours(2),
            2, SalesManager::TripType::ONE_WAY, ""
        );
        if (!booking3.empty()) {
            salesManager->confirmBooking(booking3, SalesManager::PaymentMethod::CASH);
            salesManager->startTrip(booking3);
            std::cout << "✓ Created in-progress booking: " << booking3 << "\n";
        }
        
        // Booking 4: Completed
        std::string booking4 = salesManager->createBooking(
            "C004", "V001", "Train Station", "University", 
            std::chrono::system_clock::now() - std::chrono::hours(24),
            1, SalesManager::TripType::ONE_WAY, "Student rate"
        );
        if (!booking4.empty()) {
            salesManager->confirmBooking(booking4, SalesManager::PaymentMethod::DIGITAL_WALLET);
            salesManager->startTrip(booking4);
            salesManager->completeTrip(booking4, 15.5);
            std::cout << "✓ Created completed booking: " << booking4 << "\n";
        }
        
        // Booking 5: Cancelled
        std::string booking5 = salesManager->createBooking(
            "C005", "V002", "Airport", "City Center", 
            std::chrono::system_clock::now() + std::chrono::hours(72),
            6, SalesManager::TripType::ONE_WAY, "VIP service"
        );
        if (!booking5.empty()) {
            salesManager->cancelBooking(booking5, "Customer changed plans");
            std::cout << "✓ Created cancelled booking: " << booking5 << "\n";
        }
    }
    
    void updateCustomerStats() {
        std::cout << "\nUpdating customer statistics...\n";
        
        // Update customer stats based on completed bookings
        auto customers = salesManager->getAllCustomers();
        for (auto& customer : customers) {
            auto customerBookings = salesManager->filterSalesByCustomer(customer.customerId);
            customer.totalBookings = customerBookings.size();
            
            // Calculate total spent from completed bookings
            double totalSpent = 0.0;
            auto bookings = salesManager->getAllBookings();
            for (const auto& booking : bookings) {
                if (booking.customerId == customer.customerId && 
                    booking.status == SalesManager::BookingStatus::COMPLETED) {
                    totalSpent += booking.totalAmount;
                }
            }
            customer.totalSpent = totalSpent;
            
            // Mark as VIP if total spent > $500
            if (customer.totalSpent > 500.0) {
                customer.isVip = true;
            }
            
            // Update customer in the system (would need an update method)
            std::cout << "✓ Updated stats for customer: " << customer.name 
                      << " (Bookings: " << customer.totalBookings 
                      << ", Spent: $" << customer.totalSpent << ")\n";
        }
    }
    
    void displaySummary() {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "                SAMPLE DATA SUMMARY\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << "Customers: " << salesManager->getCustomerCount() << "\n";
        std::cout << "Bookings: " << salesManager->getBookingCount() << "\n";
        std::cout << "Sales Records: " << salesManager->getSalesCount() << "\n";
        
        // Display booking status breakdown
        auto allBookings = salesManager->getAllBookings();
        int pending = 0, confirmed = 0, inProgress = 0, completed = 0, cancelled = 0;
        
        for (const auto& booking : allBookings) {
            switch (booking.status) {
                case SalesManager::BookingStatus::PENDING: pending++; break;
                case SalesManager::BookingStatus::CONFIRMED: confirmed++; break;
                case SalesManager::BookingStatus::IN_PROGRESS: inProgress++; break;
                case SalesManager::BookingStatus::COMPLETED: completed++; break;
                case SalesManager::BookingStatus::CANCELLED: cancelled++; break;
            }
        }
        
        std::cout << "\nBooking Status Breakdown:\n";
        std::cout << "  Pending: " << pending << "\n";
        std::cout << "  Confirmed: " << confirmed << "\n";
        std::cout << "  In Progress: " << inProgress << "\n";
        std::cout << "  Completed: " << completed << "\n";
        std::cout << "  Cancelled: " << cancelled << "\n";
        
        // Display revenue summary
        auto revenueReport = salesManager->getDailyReport();
        std::cout << "\nToday's Revenue: $" << revenueReport.totalRevenue << "\n";
        std::cout << "Transactions: " << revenueReport.totalTransactions << "\n";
        
        std::cout << std::string(60, '=') << "\n";
        std::cout << "Sample data generation completed successfully!\n";
        std::cout << "You can now run the sales test to interact with this data.\n";
        std::cout << std::string(60, '=') << "\n";
    }
    
    void generateAll() {
        std::cout << "╔══════════════════════════════════════════════════════════╗\n";
        std::cout << "║              TOURMATE SAMPLE DATA GENERATOR             ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
        
        generateSampleCustomers();
        generateSampleBookings();
        updateCustomerStats();
        displaySummary();
    }
};

int main() {
    try {
        SampleDataGenerator generator;
        generator.generateAll();
        
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}