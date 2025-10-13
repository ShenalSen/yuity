#include "src/headers/VehicleManager.h"
#include "src/headers/AuthenticationManager.h"
#include "src/headers/Utilities.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <string>

class VehicleTestSuite {
private:
    std::shared_ptr<AuthenticationManager> authManager;
    std::shared_ptr<VehicleManager> vehicleManager;
    
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
    
    VehicleType selectVehicleType() {
        std::cout << "\nSelect Vehicle Type:\n";
        std::cout << "1. Bus\n";
        std::cout << "2. Car\n";
        std::cout << "3. Truck\n";
        std::cout << "4. Motorcycle\n";
        std::cout << "0. Cancel\n";
        
        int choice = getIntInput("Enter choice (0-4): ", 0, 4);
        
        switch (choice) {
            case 1: return VehicleType::BUS;
            case 2: return VehicleType::CAR;
            case 3: return VehicleType::TRUCK;
            case 4: return VehicleType::MOTORCYCLE;
            default: return VehicleType::UNKNOWN;
        }
    }
    
    VehicleStatus selectVehicleStatus() {
        std::cout << "\nSelect Vehicle Status:\n";
        std::cout << "1. Available\n";
        std::cout << "2. In Service\n";
        std::cout << "3. Maintenance\n";
        std::cout << "4. Retired\n";
        std::cout << "0. Cancel\n";
        
        int choice = getIntInput("Enter choice (0-4): ", 0, 4);
        
        switch (choice) {
            case 1: return VehicleStatus::AVAILABLE;
            case 2: return VehicleStatus::IN_SERVICE;
            case 3: return VehicleStatus::MAINTENANCE;
            case 4: return VehicleStatus::RETIRED;
            default: return VehicleStatus::UNKNOWN;
        }
    }
    
    Vehicle createVehicleFromInput() {
        Vehicle vehicle;
        
        std::cout << "\n=== ENTER VEHICLE DETAILS ===\n";
        
        // Vehicle ID
        std::string id = getInput("Vehicle ID (leave empty for auto-generation): ");
        if (id.empty()) {
            id = vehicleManager->getNextAvailableId();
            std::cout << "Generated ID: " << id << "\n";
        }
        vehicle.setVehicleId(id);
        
        // Vehicle Type
        VehicleType type = selectVehicleType();
        if (type == VehicleType::UNKNOWN) {
            std::cout << "Operation cancelled.\n";
            return Vehicle(); // Return empty vehicle
        }
        vehicle.setType(type);
        
        // Registration Number
        std::string regNum = getInput("Registration Number: ");
        vehicle.setRegistrationNumber(regNum);
        
        // Manufacturer
        std::string manufacturer = getInput("Manufacturer: ");
        vehicle.setManufacturer(manufacturer);
        
        // Model
        std::string model = getInput("Model: ");
        vehicle.setModel(model);
        
        // Year
        int year = getIntInput("Year: ", 1900, 2030);
        vehicle.setYear(year);
        
        // Capacity
        int capacity = getIntInput("Capacity (passengers): ", 1, 1000);
        vehicle.setCapacity(capacity);
        
        // Fare per km
        double fare = getDoubleInput("Fare per km ($): ", 0.0, 1000.0);
        vehicle.setFarePerKm(fare);
        
        // Route
        std::string route = getInput("Route: ");
        vehicle.setRoute(route);
        
        // Status
        VehicleStatus status = selectVehicleStatus();
        if (status == VehicleStatus::UNKNOWN) {
            status = VehicleStatus::AVAILABLE; // Default status
        }
        vehicle.setStatus(status);
        
        return vehicle;
    }
    
    void displayPermissions() {
        std::cout << "\n=== CURRENT PERMISSIONS ===\n";
        std::cout << "User: " << vehicleManager->getCurrentUsername() 
                  << " (" << vehicleManager->getCurrentUserRole() << ")\n";
        std::cout << "Can View Vehicles: " << (vehicleManager->canViewVehicles() ? "Yes" : "No") << "\n";
        std::cout << "Can Add Vehicles: " << (vehicleManager->canAddVehicles() ? "Yes" : "No") << "\n";
        std::cout << "Can Edit Vehicles: " << (vehicleManager->canEditVehicles() ? "Yes" : "No") << "\n";
        std::cout << "Can Delete Vehicles: " << (vehicleManager->canDeleteVehicles() ? "Yes" : "No") << "\n";
        std::cout << "============================\n";
    }
    
public:
    VehicleTestSuite() {
        authManager = std::make_shared<AuthenticationManager>();
        vehicleManager = std::make_shared<VehicleManager>(authManager, "data/vehicles.csv");
        
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
        std::cout << "  operator/Oper123 (Manage vehicles)\n";
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
    
    void testAddVehicle() {
        std::cout << "\n=== ADD VEHICLE TEST ===\n";
        
        if (!vehicleManager->canAddVehicles()) {
            std::cout << "Access denied. You don't have permission to add vehicles.\n";
            return;
        }
        
        Vehicle vehicle = createVehicleFromInput();
        
        if (vehicle.getVehicleId().empty()) {
            return; // Operation cancelled
        }
        
        if (vehicleManager->addVehicle(vehicle)) {
            std::cout << "\n✅ Vehicle added successfully!\n";
            vehicleManager->displayVehicle(std::make_shared<Vehicle>(vehicle));
        } else {
            std::cout << "\n❌ Failed to add vehicle: " << vehicleManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testViewVehicle() {
        std::cout << "\n=== VIEW VEHICLE TEST ===\n";
        
        if (!vehicleManager->canViewVehicles()) {
            std::cout << "Access denied. You don't have permission to view vehicles.\n";
            return;
        }
        
        std::string id = getInput("Enter Vehicle ID: ");
        
        auto vehicle = vehicleManager->getVehicle(id);
        if (vehicle) {
            vehicleManager->displayVehicle(vehicle);
        } else {
            std::cout << "❌ Vehicle not found: " << vehicleManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testUpdateVehicle() {
        std::cout << "\n=== UPDATE VEHICLE TEST ===\n";
        
        if (!vehicleManager->canEditVehicles()) {
            std::cout << "Access denied. You don't have permission to edit vehicles.\n";
            return;
        }
        
        std::string id = getInput("Enter Vehicle ID to update: ");
        
        auto existingVehicle = vehicleManager->getVehicle(id);
        if (!existingVehicle) {
            std::cout << "❌ Vehicle not found: " << vehicleManager->getLastErrorMessage() << "\n";
            return;
        }
        
        std::cout << "\nCurrent vehicle details:\n";
        vehicleManager->displayVehicle(existingVehicle);
        
        std::cout << "\nEnter new details (leave empty to keep current value):\n";
        
        Vehicle updatedVehicle = *existingVehicle;
        
        // Update fields (simplified for brevity)
        std::string manufacturer = getInput("Manufacturer [" + existingVehicle->getManufacturer() + "]: ");
        if (!manufacturer.empty()) updatedVehicle.setManufacturer(manufacturer);
        
        std::string model = getInput("Model [" + existingVehicle->getModel() + "]: ");
        if (!model.empty()) updatedVehicle.setModel(model);
        
        std::string route = getInput("Route [" + existingVehicle->getRoute() + "]: ");
        if (!route.empty()) updatedVehicle.setRoute(route);
        
        std::cout << "Update capacity? (y/n): ";
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            int capacity = getIntInput("Capacity [" + std::to_string(existingVehicle->getCapacity()) + "]: ", 1, 1000);
            updatedVehicle.setCapacity(capacity);
        }
        
        std::cout << "Update fare? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            double fare = getDoubleInput("Fare per km [" + std::to_string(existingVehicle->getFarePerKm()) + "]: ", 0.0, 1000.0);
            updatedVehicle.setFarePerKm(fare);
        }
        
        if (vehicleManager->updateVehicle(id, updatedVehicle)) {
            std::cout << "\n✅ Vehicle updated successfully!\n";
            vehicleManager->displayVehicle(std::make_shared<Vehicle>(updatedVehicle));
        } else {
            std::cout << "\n❌ Failed to update vehicle: " << vehicleManager->getLastErrorMessage() << "\n";
        }
    }
    
    void testDeleteVehicle() {
        std::cout << "\n=== DELETE VEHICLE TEST ===\n";
        
        if (!vehicleManager->canDeleteVehicles()) {
            std::cout << "Access denied. You don't have permission to delete vehicles.\n";
            return;
        }
        
        std::string id = getInput("Enter Vehicle ID to delete: ");
        
        auto vehicle = vehicleManager->getVehicle(id);
        if (!vehicle) {
            std::cout << "❌ Vehicle not found: " << vehicleManager->getLastErrorMessage() << "\n";
            return;
        }
        
        std::cout << "\nVehicle to delete:\n";
        vehicleManager->displayVehicle(vehicle);
        
        std::cout << "\nAre you sure you want to delete this vehicle? (y/N): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();
        
        if (confirm == 'y' || confirm == 'Y') {
            if (vehicleManager->deleteVehicle(id)) {
                std::cout << "\n✅ Vehicle deleted successfully!\n";
            } else {
                std::cout << "\n❌ Failed to delete vehicle: " << vehicleManager->getLastErrorMessage() << "\n";
            }
        } else {
            std::cout << "Delete operation cancelled.\n";
        }
    }
    
    void testListAllVehicles() {
        std::cout << "\n=== LIST ALL VEHICLES TEST ===\n";
        
        if (!vehicleManager->canViewVehicles()) {
            std::cout << "Access denied. You don't have permission to view vehicles.\n";
            return;
        }
        
        auto vehicles = vehicleManager->getAllVehicles();
        std::cout << "Total vehicles: " << vehicles.size() << "\n";
        
        if (!vehicles.empty()) {
            vehicleManager->displayVehicleList(vehicles);
        }
    }
    
    void testSearchVehicles() {
        std::cout << "\n=== SEARCH VEHICLES TEST ===\n";
        
        if (!vehicleManager->canViewVehicles()) {
            std::cout << "Access denied. You don't have permission to view vehicles.\n";
            return;
        }
        
        vehicleManager->displaySearchHelp();
        
        std::cout << "\nSearch Options:\n";
        std::cout << "1. Text Search\n";
        std::cout << "2. Filter by Type\n";
        std::cout << "3. Filter by Status\n";
        std::cout << "4. Filter by Capacity Range\n";
        std::cout << "5. Filter by Fare Range\n";
        std::cout << "6. Advanced Search\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-6): ", 0, 6);
        
        std::vector<std::shared_ptr<Vehicle>> results;
        
        switch (choice) {
            case 1: {
                std::string searchTerm = getInput("Enter search term: ");
                results = vehicleManager->searchVehicles(searchTerm);
                break;
            }
            case 2: {
                VehicleType type = selectVehicleType();
                if (type != VehicleType::UNKNOWN) {
                    results = vehicleManager->filterByType(type);
                }
                break;
            }
            case 3: {
                VehicleStatus status = selectVehicleStatus();
                if (status != VehicleStatus::UNKNOWN) {
                    results = vehicleManager->filterByStatus(status);
                }
                break;
            }
            case 4: {
                int minCapacity = getIntInput("Minimum capacity: ", 1, 1000);
                int maxCapacity = getIntInput("Maximum capacity (-1 for no limit): ", -1, 1000);
                results = vehicleManager->filterByCapacity(minCapacity, maxCapacity);
                break;
            }
            case 5: {
                double minFare = getDoubleInput("Minimum fare: ", 0.0, 1000.0);
                double maxFare = getDoubleInput("Maximum fare (-1 for no limit): ", -1.0, 1000.0);
                results = vehicleManager->filterByFareRange(minFare, maxFare);
                break;
            }
            case 6: {
                testAdvancedSearch();
                return;
            }
            default:
                return;
        }
        
        std::cout << "\nSearch Results (" << results.size() << " found):\n";
        vehicleManager->displayVehicleList(results);
    }
    
    void testAdvancedSearch() {
        std::cout << "\n=== ADVANCED SEARCH TEST ===\n";
        
        VehicleManager::SearchCriteria criteria;
        
        std::string searchTerm = getInput("Search term (optional): ");
        if (!searchTerm.empty()) criteria.searchTerm = searchTerm;
        
        std::cout << "Filter by type? (y/n): ";
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            criteria.type = selectVehicleType();
        }
        
        std::cout << "Filter by status? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            criteria.status = selectVehicleStatus();
        }
        
        std::cout << "Filter by capacity range? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            criteria.minCapacity = getIntInput("Minimum capacity: ", 1, 1000);
            criteria.maxCapacity = getIntInput("Maximum capacity (-1 for no limit): ", -1, 1000);
        }
        
        std::cout << "Sort results? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Sort by: 1=ID, 2=Capacity, 3=Fare, 4=Year, 5=Manufacturer: ";
            int sortChoice = getIntInput("", 1, 5);
            switch (sortChoice) {
                case 1: criteria.sortBy = "vehicleId"; break;
                case 2: criteria.sortBy = "capacity"; break;
                case 3: criteria.sortBy = "fare"; break;
                case 4: criteria.sortBy = "year"; break;
                case 5: criteria.sortBy = "manufacturer"; break;
            }
            
            std::cout << "Ascending order? (y/n): ";
            std::cin >> choice;
            clearInputBuffer();
            criteria.ascending = (choice == 'y' || choice == 'Y');
        }
        
        auto results = vehicleManager->advancedSearch(criteria);
        std::cout << "\nAdvanced Search Results (" << results.size() << " found):\n";
        vehicleManager->displayVehicleList(results);
    }
    
    void testVehicleStatus() {
        std::cout << "\n=== VEHICLE STATUS TEST ===\n";
        
        if (!vehicleManager->canEditVehicles()) {
            std::cout << "Access denied. You don't have permission to edit vehicles.\n";
            return;
        }
        
        std::cout << "1. Update Vehicle Status\n";
        std::cout << "2. Set Vehicle for Maintenance\n";
        std::cout << "3. Release from Maintenance\n";
        std::cout << "4. View Available Vehicles\n";
        std::cout << "5. View In-Service Vehicles\n";
        std::cout << "6. View Maintenance Vehicles\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-6): ", 0, 6);
        
        switch (choice) {
            case 1: {
                std::string id = getInput("Vehicle ID: ");
                VehicleStatus status = selectVehicleStatus();
                if (status != VehicleStatus::UNKNOWN) {
                    if (vehicleManager->updateVehicleStatus(id, status)) {
                        std::cout << "✅ Status updated successfully!\n";
                    } else {
                        std::cout << "❌ Failed to update status: " << vehicleManager->getLastErrorMessage() << "\n";
                    }
                }
                break;
            }
            case 2: {
                std::string id = getInput("Vehicle ID: ");
                std::string reason = getInput("Maintenance reason: ");
                if (vehicleManager->setVehicleForMaintenance(id, reason)) {
                    std::cout << "✅ Vehicle set for maintenance!\n";
                } else {
                    std::cout << "❌ Failed: " << vehicleManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 3: {
                std::string id = getInput("Vehicle ID: ");
                if (vehicleManager->releaseFromMaintenance(id)) {
                    std::cout << "✅ Vehicle released from maintenance!\n";
                } else {
                    std::cout << "❌ Failed: " << vehicleManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 4: {
                auto vehicles = vehicleManager->getAvailableVehicles();
                std::cout << "\nAvailable Vehicles (" << vehicles.size() << "):\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
            case 5: {
                auto vehicles = vehicleManager->getInServiceVehicles();
                std::cout << "\nIn-Service Vehicles (" << vehicles.size() << "):\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
            case 6: {
                auto vehicles = vehicleManager->getMaintenanceVehicles();
                std::cout << "\nMaintenance Vehicles (" << vehicles.size() << "):\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
        }
    }
    
    void testStatistics() {
        std::cout << "\n=== STATISTICS TEST ===\n";
        
        if (!vehicleManager->canViewVehicles()) {
            std::cout << "Access denied. You don't have permission to view statistics.\n";
            return;
        }
        
        vehicleManager->displayStatistics();
        
        std::cout << "\nDetailed Statistics:\n";
        std::cout << "1. Top Vehicles by Capacity\n";
        std::cout << "2. Newest Vehicles\n";
        std::cout << "3. Oldest Vehicles\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-3): ", 0, 3);
        
        switch (choice) {
            case 1: {
                int count = getIntInput("Number of vehicles to show: ", 1, 50);
                auto vehicles = vehicleManager->getTopVehiclesByCapacity(count);
                std::cout << "\nTop " << count << " Vehicles by Capacity:\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
            case 2: {
                int count = getIntInput("Number of vehicles to show: ", 1, 50);
                auto vehicles = vehicleManager->getNewestVehicles(count);
                std::cout << "\nNewest " << count << " Vehicles:\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
            case 3: {
                int count = getIntInput("Number of vehicles to show: ", 1, 50);
                auto vehicles = vehicleManager->getOldestVehicles(count);
                std::cout << "\nOldest " << count << " Vehicles:\n";
                vehicleManager->displayVehicleList(vehicles);
                break;
            }
        }
    }
    
    void testFileOperations() {
        std::cout << "\n=== FILE OPERATIONS TEST ===\n";
        
        std::cout << "1. Create Backup\n";
        std::cout << "2. Restore from Backup\n";
        std::cout << "3. Export to CSV\n";
        std::cout << "4. Import from CSV\n";
        std::cout << "5. View Audit Log\n";
        std::cout << "6. Clear Audit Log\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-6): ", 0, 6);
        
        switch (choice) {
            case 1: {
                if (vehicleManager->createBackup()) {
                    std::cout << "✅ Backup created successfully!\n";
                } else {
                    std::cout << "❌ Failed to create backup: " << vehicleManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 2: {
                if (!vehicleManager->canEditVehicles()) {
                    std::cout << "Access denied. You don't have permission to restore data.\n";
                    break;
                }
                std::cout << "Warning: This will overwrite current data. Continue? (y/N): ";
                char confirm;
                std::cin >> confirm;
                clearInputBuffer();
                if (confirm == 'y' || confirm == 'Y') {
                    if (vehicleManager->restoreFromBackup()) {
                        std::cout << "✅ Data restored from backup!\n";
                    } else {
                        std::cout << "❌ Failed to restore: " << vehicleManager->getLastErrorMessage() << "\n";
                    }
                }
                break;
            }
            case 3: {
                std::string filename = getInput("Export filename: ");
                auto vehicles = vehicleManager->getAllVehicles();
                if (vehicleManager->exportToCSV(filename, vehicles)) {
                    std::cout << "✅ Data exported to " << filename << "\n";
                } else {
                    std::cout << "❌ Export failed: " << vehicleManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 4: {
                if (!vehicleManager->canAddVehicles()) {
                    std::cout << "Access denied. You don't have permission to import data.\n";
                    break;
                }
                std::string filename = getInput("Import filename: ");
                if (vehicleManager->importFromCSV(filename)) {
                    std::cout << "✅ Data imported from " << filename << "\n";
                } else {
                    std::cout << "❌ Import failed: " << vehicleManager->getLastErrorMessage() << "\n";
                }
                break;
            }
            case 5: {
                vehicleManager->printAuditSummary();
                break;
            }
            case 6: {
                if (!vehicleManager->canDeleteVehicles()) {
                    std::cout << "Access denied. You don't have permission to clear audit log.\n";
                    break;
                }
                if (vehicleManager->clearAuditLog()) {
                    std::cout << "✅ Audit log cleared!\n";
                } else {
                    std::cout << "❌ Failed to clear audit log.\n";
                }
                break;
            }
        }
    }
    
    void testDataValidation() {
        std::cout << "\n=== DATA VALIDATION TEST ===\n";
        
        if (!vehicleManager->canViewVehicles()) {
            std::cout << "Access denied. You don't have permission to validate data.\n";
            return;
        }
        
        std::cout << "1. Validate All Vehicle Data\n";
        std::cout << "2. Find Duplicate Vehicles\n";
        std::cout << "3. Find Invalid Vehicles\n";
        std::cout << "4. Repair Data Inconsistencies\n";
        std::cout << "0. Back\n";
        
        int choice = getIntInput("Enter choice (0-4): ", 0, 4);
        
        switch (choice) {
            case 1: {
                bool isValid = vehicleManager->validateVehicleData();
                std::cout << "Data validation: " << (isValid ? "✅ PASSED" : "❌ FAILED") << "\n";
                break;
            }
            case 2: {
                auto duplicates = vehicleManager->findDuplicateVehicles();
                std::cout << "\nDuplicate vehicles found: " << duplicates.size() << "\n";
                for (const auto& duplicate : duplicates) {
                    std::cout << "  " << duplicate << "\n";
                }
                break;
            }
            case 3: {
                auto invalid = vehicleManager->findInvalidVehicles();
                std::cout << "\nInvalid vehicles found: " << invalid.size() << "\n";
                for (const auto& inv : invalid) {
                    std::cout << "  " << inv << "\n";
                }
                break;
            }
            case 4: {
                if (!vehicleManager->canEditVehicles()) {
                    std::cout << "Access denied. You don't have permission to repair data.\n";
                    break;
                }
                int repaired = vehicleManager->repairDataInconsistencies();
                std::cout << "Data inconsistencies repaired: " << repaired << "\n";
                break;
            }
        }
    }
    
    void runMainMenu() {
        while (true) {
            std::cout << "\n" << std::string(60, '=') << "\n";
            std::cout << "           TOURMATE VEHICLE MANAGEMENT SYSTEM\n";
            std::cout << std::string(60, '=') << "\n";
            std::cout << "Current User: " << vehicleManager->getCurrentUsername() 
                      << " (" << vehicleManager->getCurrentUserRole() << ")\n";
            std::cout << "Vehicle Count: " << vehicleManager->getVehicleCount() << "\n";
            std::cout << std::string(60, '-') << "\n";
            
            std::cout << " 1. Add Vehicle                11. Vehicle Status Management\n";
            std::cout << " 2. View Vehicle               12. Statistics & Reports\n";
            std::cout << " 3. Update Vehicle             13. File Operations\n";
            std::cout << " 4. Delete Vehicle             14. Data Validation\n";
            std::cout << " 5. List All Vehicles          15. Display Permissions\n";
            std::cout << " 6. Search Vehicles            16. Login/Switch User\n";
            std::cout << " 7. Advanced Search            17. Logout\n";
            std::cout << " 8. Filter by Type             18. Help\n";
            std::cout << " 9. Filter by Status           19. About\n";
            std::cout << "10. Filter by Capacity         0.  Exit\n";
            std::cout << std::string(60, '=') << "\n";
            
            int choice = getIntInput("Enter your choice (0-19): ", 0, 19);
            
            switch (choice) {
                case 1: testAddVehicle(); break;
                case 2: testViewVehicle(); break;
                case 3: testUpdateVehicle(); break;
                case 4: testDeleteVehicle(); break;
                case 5: testListAllVehicles(); break;
                case 6: testSearchVehicles(); break;
                case 7: testAdvancedSearch(); break;
                case 8: {
                    VehicleType type = selectVehicleType();
                    if (type != VehicleType::UNKNOWN && vehicleManager->canViewVehicles()) {
                        auto results = vehicleManager->filterByType(type);
                        std::cout << "\nVehicles of type " << Utilities::vehicleTypeToString(type) << ":\n";
                        vehicleManager->displayVehicleList(results);
                    }
                    break;
                }
                case 9: {
                    VehicleStatus status = selectVehicleStatus();
                    if (status != VehicleStatus::UNKNOWN && vehicleManager->canViewVehicles()) {
                        auto results = vehicleManager->filterByStatus(status);
                        std::cout << "\nVehicles with status " << Utilities::vehicleStatusToString(status) << ":\n";
                        vehicleManager->displayVehicleList(results);
                    }
                    break;
                }
                case 10: {
                    if (vehicleManager->canViewVehicles()) {
                        int minCap = getIntInput("Minimum capacity: ", 1, 1000);
                        int maxCap = getIntInput("Maximum capacity (-1 for no limit): ", -1, 1000);
                        auto results = vehicleManager->filterByCapacity(minCap, maxCap);
                        std::cout << "\nVehicles with capacity " << minCap << "-" << 
                                     (maxCap == -1 ? "unlimited" : std::to_string(maxCap)) << ":\n";
                        vehicleManager->displayVehicleList(results);
                    }
                    break;
                }
                case 11: testVehicleStatus(); break;
                case 12: testStatistics(); break;
                case 13: testFileOperations(); break;
                case 14: testDataValidation(); break;
                case 15: displayPermissions(); break;
                case 16: loginMenu(); break;
                case 17: 
                    authManager->logout();
                    std::cout << "Logged out successfully.\n";
                    loginMenu();
                    break;
                case 18:
                    vehicleManager->displaySearchHelp();
                    std::cout << "\nVehicle Management System Help:\n";
                    std::cout << "- Use the menu options to manage vehicles\n";
                    std::cout << "- Your access level determines available operations\n";
                    std::cout << "- All operations are logged for audit purposes\n";
                    std::cout << "- Data is automatically saved to CSV files\n";
                    break;
                case 19:
                    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
                    std::cout << "║              TOURMATE VEHICLE MANAGEMENT SYSTEM         ║\n";
                    std::cout << "║                     Version 1.0                         ║\n";
                    std::cout << "║                                                          ║\n";
                    std::cout << "║  Features:                                               ║\n";
                    std::cout << "║  • Complete Vehicle CRUD Operations                     ║\n";
                    std::cout << "║  • Advanced Search and Filtering                        ║\n";
                    std::cout << "║  • Role-based Access Control                            ║\n";
                    std::cout << "║  • CSV File Import/Export                               ║\n";
                    std::cout << "║  • Comprehensive Audit Logging                          ║\n";
                    std::cout << "║  • Data Validation and Integrity Checks                 ║\n";
                    std::cout << "║  • Statistical Reports and Analytics                    ║\n";
                    std::cout << "║                                                          ║\n";
                    std::cout << "║  Developed for CSE4002 WRIT1 - Academic Project         ║\n";
                    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
                    break;
                case 0:
                    std::cout << "Thank you for using Tourmate Vehicle Management System!\n";
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
        std::cout << "║       WELCOME TO TOURMATE VEHICLE MANAGEMENT SYSTEM     ║\n";
        std::cout << "║                      Test Suite                          ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n";
        
        VehicleTestSuite testSuite;
        testSuite.runMainMenu();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}