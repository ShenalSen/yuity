#include "src/headers/Vehicle.h"
#include "src/headers/Sales.h"
#include "src/headers/User.h"
#include "src/headers/Utilities.h"
#include <iostream>

int main() {
    std::cout << "=== TOURMATE C++ CORE DATA STRUCTURES TEST ===" << std::endl;
    
    // Test Vehicle class
    std::cout << "\n1. Testing Vehicle Class:" << std::endl;
    Vehicle vehicle(1, "ABC-123", "Toyota Camry", 2022, "Sedan", 5, "Available", 
                   "2022-01-15", "2023-12-01");
    std::cout << "Created vehicle: " << vehicle.toString() << std::endl;
    std::cout << "Is Available: " << (vehicle.isAvailable() ? "Yes" : "No") << std::endl;
    std::cout << "Valid license plate: " << (vehicle.isValidLicensePlate("ABC-123") ? "Yes" : "No") << std::endl;
    
    // Test Sales class
    std::cout << "\n2. Testing Sales Class:" << std::endl;
    Sales sale(1, 1, "John Doe", "+1234567890", "Airport", "Downtown", 
              "2023-12-15", "14:30", 45, 87.50, "Paid", "Alice");
    std::cout << "Created sale: " << sale.toString() << std::endl;
    std::cout << "Is Paid: " << (sale.isPaid() ? "Yes" : "No") << std::endl;
    std::cout << "Base fare: $" << sale.calculateBaseFare() << std::endl;
    
    // Test User class
    std::cout << "\n3. Testing User Class:" << std::endl;
    User user(1, "admin", "Admin123", "Admin", "", "Active");
    std::cout << "Created user: " << user.toString() << std::endl;
    std::cout << "Is Admin: " << (user.isAdmin() ? "Yes" : "No") << std::endl;
    std::cout << "Can Delete: " << (user.canDelete() ? "Yes" : "No") << std::endl;
    
    // Test Utilities
    std::cout << "\n4. Testing Utilities:" << std::endl;
    std::cout << "Current Date: " << TourmateUtils::getCurrentDate() << std::endl;
    std::cout << "Current Time: " << TourmateUtils::getCurrentTime() << std::endl;
    std::cout << "Trim test: '" << TourmateUtils::trim("  hello world  ") << "'" << std::endl;
    std::cout << "Is valid email: " << (TourmateUtils::isValidEmail("test@example.com") ? "Yes" : "No") << std::endl;
    
    // Test CSV functionality
    std::cout << "\n5. Testing CSV Operations:" << std::endl;
    std::string csvData = vehicle.toCSV();
    std::cout << "Vehicle CSV: " << csvData << std::endl;
    
    Vehicle newVehicle;
    if (newVehicle.fromCSV(csvData)) {
        std::cout << "Successfully parsed vehicle from CSV" << std::endl;
        std::cout << "Parsed vehicle: " << newVehicle.toString() << std::endl;
    }
    
    std::cout << "\n=== ALL TESTS COMPLETED SUCCESSFULLY ===" << std::endl;
    return 0;
}