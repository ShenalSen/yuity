#include "../headers/VehicleManager.h"
#include "../headers/Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <set>

// Constructor
VehicleManager::VehicleManager(std::shared_ptr<AuthenticationManager> authMgr,
                               const std::string& dataFile)
    : authManager(authMgr), dataFilePath(dataFile), fileLoaded(false) {
    
    // Set up file paths
    backupFilePath = dataFile + ".backup";
    auditLogPath = "data/vehicle_audit.log";
    
    // Ensure data directory exists
    Utilities::createDirectoryIfNotExists("data");
    
    // Load existing vehicles
    loadVehiclesFromFile();
    
    logOperation("VehicleManager initialized", "", 
                "Vehicle Manager started with " + std::to_string(vehicles.size()) + " vehicles");
}

// Destructor
VehicleManager::~VehicleManager() {
    saveVehiclesToFile();
    logOperation("VehicleManager destroyed", "", "Vehicle Manager shutdown");
}

// Permission checking
bool VehicleManager::hasPermission(const std::string& operation) const {
    if (!authManager || !authManager->isLoggedIn()) {
        setError(VehicleError::AUTHENTICATION_REQUIRED, "User must be logged in");
        return false;
    }

    if (operation == "view") {
        return authManager->canViewVehicles();
    } else if (operation == "add" || operation == "edit") {
        return authManager->canManageVehicles();
    } else if (operation == "delete") {
        return authManager->canDeleteVehicles();
    }
    
    return false;
}

// Audit logging
void VehicleManager::logOperation(const std::string& operation, const std::string& vehicleId,
                                  const std::string& details) const {
    try {
        std::ofstream logFile(auditLogPath, std::ios::app);
        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            
            std::string username = "SYSTEM";
            if (authManager && authManager->isLoggedIn()) {
                auto user = authManager->getCurrentUser();
                if (user) {
                    username = user->getUsername();
                }
            }
            
            logFile << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
                    << " | " << username << " | " << operation;
            
            if (!vehicleId.empty()) {
                logFile << " | Vehicle: " << vehicleId;
            }
            
            if (!details.empty()) {
                logFile << " | " << details;
            }
            
            logFile << std::endl;
        }
    } catch (const std::exception& e) {
        // Log errors silently to avoid disrupting main operations
        std::cerr << "Audit log error: " << e.what() << std::endl;
    }
}

// Generate unique vehicle ID
std::string VehicleManager::generateVehicleId() const {
    std::string prefix = "VH";
    int maxNum = 0;
    
    for (const auto& vehicle : vehicles) {
        std::string id = vehicle->getVehicleId();
        if (id.length() > 2 && id.substr(0, 2) == prefix) {
            try {
                int num = std::stoi(id.substr(2));
                maxNum = std::max(maxNum, num);
            } catch (const std::exception&) {
                // Skip invalid IDs
            }
        }
    }
    
    return prefix + std::to_string(maxNum + 1);
}

// Data validation
bool VehicleManager::isValidVehicleData(const Vehicle& vehicle, std::string& errorMessage) const {
    if (vehicle.getVehicleId().empty()) {
        errorMessage = "Vehicle ID cannot be empty";
        return false;
    }
    
    if (vehicle.getRegistrationNumber().empty()) {
        errorMessage = "Registration number cannot be empty";
        return false;
    }
    
    if (vehicle.getCapacity() <= 0) {
        errorMessage = "Capacity must be greater than 0";
        return false;
    }
    
    if (vehicle.getFarePerKm() < 0) {
        errorMessage = "Fare per km cannot be negative";
        return false;
    }
    
    if (vehicle.getType() == VehicleType::UNKNOWN) {
        errorMessage = "Vehicle type must be specified";
        return false;
    }
    
    return true;
}

// CSV utility methods
std::vector<std::string> VehicleManager::splitCSVLine(const std::string& line) const {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;
    
    while (std::getline(ss, cell, ',')) {
        // Remove quotes if present
        if (cell.length() >= 2 && cell.front() == '"' && cell.back() == '"') {
            cell = cell.substr(1, cell.length() - 2);
        }
        result.push_back(cell);
    }
    
    return result;
}

std::string VehicleManager::escapeCSVField(const std::string& field) const {
    if (field.find(',') != std::string::npos || field.find('"') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : field) {
            if (c == '"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}

// File Operations
bool VehicleManager::loadVehiclesFromFile() {
    try {
        std::ifstream file(dataFilePath);
        if (!file.is_open()) {
            // Create empty file if it doesn't exist
            std::ofstream newFile(dataFilePath);
            if (newFile.is_open()) {
                newFile << "VehicleId,Type,RegistrationNumber,Manufacturer,Model,Year,Capacity,FarePerKm,Route,Status\n";
                newFile.close();
                fileLoaded = true;
                logOperation("File created", "", dataFilePath);
                return true;
            }
            setError(VehicleError::FILE_ERROR, "Could not create data file: " + dataFilePath);
            return false;
        }
        
        vehicles.clear();
        std::string line;
        bool isFirstLine = true;
        
        while (std::getline(file, line)) {
            if (isFirstLine) {
                isFirstLine = false;
                continue; // Skip header
            }
            
            if (line.empty()) continue;
            
            auto fields = splitCSVLine(line);
            if (fields.size() >= 10) {
                try {
                    auto vehicle = std::make_shared<Vehicle>();
                    vehicle->setVehicleId(fields[0]);
                    vehicle->setType(Utilities::stringToVehicleType(fields[1]));
                    vehicle->setRegistrationNumber(fields[2]);
                    vehicle->setManufacturer(fields[3]);
                    vehicle->setModel(fields[4]);
                    vehicle->setYear(std::stoi(fields[5]));
                    vehicle->setCapacity(std::stoi(fields[6]));
                    vehicle->setFarePerKm(std::stod(fields[7]));
                    vehicle->setRoute(fields[8]);
                    vehicle->setStatus(Utilities::stringToVehicleStatus(fields[9]));
                    
                    vehicles.push_back(vehicle);
                } catch (const std::exception& e) {
                    logOperation("Load error", "", "Invalid line: " + line + " - " + e.what());
                }
            }
        }
        
        file.close();
        fileLoaded = true;
        
        logOperation("Vehicles loaded", "", std::to_string(vehicles.size()) + " vehicles loaded from " + dataFilePath);
        return true;
        
    } catch (const std::exception& e) {
        setError(VehicleError::FILE_ERROR, "Error loading vehicles: " + std::string(e.what()));
        return false;
    }
}

bool VehicleManager::saveVehiclesToFile() const {
    try {
        std::ofstream file(dataFilePath);
        if (!file.is_open()) {
            setError(VehicleError::FILE_ERROR, "Could not open file for writing: " + dataFilePath);
            return false;
        }
        
        // Write header
        file << "VehicleId,Type,RegistrationNumber,Manufacturer,Model,Year,Capacity,FarePerKm,Route,Status\n";
        
        // Write vehicle data
        for (const auto& vehicle : vehicles) {
            file << escapeCSVField(vehicle->getVehicleId()) << ","
                 << escapeCSVField(Utilities::vehicleTypeToString(vehicle->getType())) << ","
                 << escapeCSVField(vehicle->getRegistrationNumber()) << ","
                 << escapeCSVField(vehicle->getManufacturer()) << ","
                 << escapeCSVField(vehicle->getModel()) << ","
                 << vehicle->getYear() << ","
                 << vehicle->getCapacity() << ","
                 << vehicle->getFarePerKm() << ","
                 << escapeCSVField(vehicle->getRoute()) << ","
                 << escapeCSVField(Utilities::vehicleStatusToString(vehicle->getStatus())) << "\n";
        }
        
        file.close();
        logOperation("Vehicles saved", "", std::to_string(vehicles.size()) + " vehicles saved to " + dataFilePath);
        return true;
        
    } catch (const std::exception& e) {
        setError(VehicleError::FILE_ERROR, "Error saving vehicles: " + std::string(e.what()));
        return false;
    }
}

bool VehicleManager::createBackup() const {
    try {
        std::ifstream src(dataFilePath, std::ios::binary);
        std::ofstream dst(backupFilePath, std::ios::binary);
        
        if (!src.is_open() || !dst.is_open()) {
            setError(VehicleError::FILE_ERROR, "Could not create backup");
            return false;
        }
        
        dst << src.rdbuf();
        
        logOperation("Backup created", "", "Backup saved to " + backupFilePath);
        return true;
        
    } catch (const std::exception& e) {
        setError(VehicleError::FILE_ERROR, "Error creating backup: " + std::string(e.what()));
        return false;
    }
}

bool VehicleManager::restoreFromBackup() {
    if (!hasPermission("edit")) return false;
    
    try {
        std::ifstream src(backupFilePath, std::ios::binary);
        std::ofstream dst(dataFilePath, std::ios::binary);
        
        if (!src.is_open() || !dst.is_open()) {
            setError(VehicleError::FILE_ERROR, "Could not restore from backup");
            return false;
        }
        
        dst << src.rdbuf();
        
        // Reload vehicles
        loadVehiclesFromFile();
        
        logOperation("Backup restored", "", "Data restored from " + backupFilePath);
        return true;
        
    } catch (const std::exception& e) {
        setError(VehicleError::FILE_ERROR, "Error restoring backup: " + std::string(e.what()));
        return false;
    }
}

// CRUD Operations
bool VehicleManager::addVehicle(const Vehicle& vehicle) {
    if (!hasPermission("add")) return false;
    
    std::string errorMessage;
    if (!isValidVehicleData(vehicle, errorMessage)) {
        setError(VehicleError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Check for duplicate ID
    if (vehicleExists(vehicle.getVehicleId())) {
        setError(VehicleError::DUPLICATE_ID, "Vehicle ID already exists: " + vehicle.getVehicleId());
        return false;
    }
    
    auto newVehicle = std::make_shared<Vehicle>(vehicle);
    vehicles.push_back(newVehicle);
    
    saveVehiclesToFile();
    logOperation("Vehicle added", vehicle.getVehicleId(), 
                "Added " + vehicle.getManufacturer() + " " + vehicle.getModel());
    
    clearError();
    return true;
}

std::shared_ptr<Vehicle> VehicleManager::getVehicle(const std::string& vehicleId) const {
    if (!hasPermission("view")) return nullptr;
    
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
        [&vehicleId](const std::shared_ptr<Vehicle>& v) {
            return v->getVehicleId() == vehicleId;
        });
    
    if (it != vehicles.end()) {
        clearError();
        return *it;
    }
    
    setError(VehicleError::NOT_FOUND, "Vehicle not found: " + vehicleId);
    return nullptr;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getAllVehicles() const {
    if (!hasPermission("view")) return {};
    
    clearError();
    return vehicles;
}

bool VehicleManager::updateVehicle(const std::string& vehicleId, const Vehicle& updatedVehicle) {
    if (!hasPermission("edit")) return false;
    
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
        [&vehicleId](const std::shared_ptr<Vehicle>& v) {
            return v->getVehicleId() == vehicleId;
        });
    
    if (it == vehicles.end()) {
        setError(VehicleError::NOT_FOUND, "Vehicle not found: " + vehicleId);
        return false;
    }
    
    std::string errorMessage;
    if (!isValidVehicleData(updatedVehicle, errorMessage)) {
        setError(VehicleError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Update vehicle data
    **it = updatedVehicle;
    (*it)->setVehicleId(vehicleId); // Ensure ID doesn't change
    
    saveVehiclesToFile();
    logOperation("Vehicle updated", vehicleId, 
                "Updated " + updatedVehicle.getManufacturer() + " " + updatedVehicle.getModel());
    
    clearError();
    return true;
}

bool VehicleManager::deleteVehicle(const std::string& vehicleId) {
    if (!hasPermission("delete")) return false;
    
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
        [&vehicleId](const std::shared_ptr<Vehicle>& v) {
            return v->getVehicleId() == vehicleId;
        });
    
    if (it == vehicles.end()) {
        setError(VehicleError::NOT_FOUND, "Vehicle not found: " + vehicleId);
        return false;
    }
    
    std::string details = (*it)->getManufacturer() + " " + (*it)->getModel();
    vehicles.erase(it);
    
    saveVehiclesToFile();
    logOperation("Vehicle deleted", vehicleId, "Deleted " + details);
    
    clearError();
    return true;
}

// Search Operations
std::vector<std::shared_ptr<Vehicle>> VehicleManager::searchVehicles(const std::string& searchTerm) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::string lowerSearchTerm = searchTerm;
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);
    
    for (const auto& vehicle : vehicles) {
        std::string vehicleData = vehicle->getVehicleId() + " " + 
                                  vehicle->getRegistrationNumber() + " " +
                                  vehicle->getManufacturer() + " " +
                                  vehicle->getModel() + " " +
                                  vehicle->getRoute();
        
        std::transform(vehicleData.begin(), vehicleData.end(), vehicleData.begin(), ::tolower);
        
        if (vehicleData.find(lowerSearchTerm) != std::string::npos) {
            results.push_back(vehicle);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByType(VehicleType type) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [type](const std::shared_ptr<Vehicle>& v) {
            return v->getType() == type;
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByStatus(VehicleStatus status) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [status](const std::shared_ptr<Vehicle>& v) {
            return v->getStatus() == status;
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByCapacity(int minCapacity, int maxCapacity) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [minCapacity, maxCapacity](const std::shared_ptr<Vehicle>& v) {
            int capacity = v->getCapacity();
            return capacity >= minCapacity && (maxCapacity == -1 || capacity <= maxCapacity);
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByFareRange(double minFare, double maxFare) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [minFare, maxFare](const std::shared_ptr<Vehicle>& v) {
            double fare = v->getFarePerKm();
            return fare >= minFare && (maxFare == -1.0 || fare <= maxFare);
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByRoute(const std::string& route) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [&route](const std::shared_ptr<Vehicle>& v) {
            return v->getRoute().find(route) != std::string::npos;
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByManufacturer(const std::string& manufacturer) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [&manufacturer](const std::shared_ptr<Vehicle>& v) {
            return v->getManufacturer().find(manufacturer) != std::string::npos;
        });
    
    return results;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::filterByYearRange(int minYear, int maxYear) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results;
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(results),
        [minYear, maxYear](const std::shared_ptr<Vehicle>& v) {
            int year = v->getYear();
            return year >= minYear && (maxYear == -1 || year <= maxYear);
        });
    
    return results;
}

// Advanced Search
std::vector<std::shared_ptr<Vehicle>> VehicleManager::advancedSearch(const SearchCriteria& criteria) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Vehicle>> results = vehicles;
    
    // Apply search term filter
    if (!criteria.searchTerm.empty()) {
        auto searchResults = searchVehicles(criteria.searchTerm);
        std::set<std::string> searchIds;
        for (const auto& v : searchResults) {
            searchIds.insert(v->getVehicleId());
        }
        
        results.erase(std::remove_if(results.begin(), results.end(),
            [&searchIds](const std::shared_ptr<Vehicle>& v) {
                return searchIds.find(v->getVehicleId()) == searchIds.end();
            }), results.end());
    }
    
    // Apply type filter
    if (criteria.type != VehicleType::UNKNOWN) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                return v->getType() != criteria.type;
            }), results.end());
    }
    
    // Apply status filter
    if (criteria.status != VehicleStatus::UNKNOWN) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                return v->getStatus() != criteria.status;
            }), results.end());
    }
    
    // Apply capacity filter
    if (criteria.minCapacity > 0) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                int capacity = v->getCapacity();
                return capacity < criteria.minCapacity || 
                       (criteria.maxCapacity > 0 && capacity > criteria.maxCapacity);
            }), results.end());
    }
    
    // Apply fare filter
    if (criteria.minFare > 0) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                double fare = v->getFarePerKm();
                return fare < criteria.minFare || 
                       (criteria.maxFare > 0 && fare > criteria.maxFare);
            }), results.end());
    }
    
    // Apply route filter
    if (!criteria.route.empty()) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                return v->getRoute().find(criteria.route) == std::string::npos;
            }), results.end());
    }
    
    // Apply manufacturer filter
    if (!criteria.manufacturer.empty()) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                return v->getManufacturer().find(criteria.manufacturer) == std::string::npos;
            }), results.end());
    }
    
    // Apply year filter
    if (criteria.minYear > 0) {
        results.erase(std::remove_if(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& v) {
                int year = v->getYear();
                return year < criteria.minYear || 
                       (criteria.maxYear > 0 && year > criteria.maxYear);
            }), results.end());
    }
    
    // Sort results
    if (!criteria.sortBy.empty()) {
        std::sort(results.begin(), results.end(),
            [&criteria](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
                if (criteria.sortBy == "vehicleId") {
                    return criteria.ascending ? a->getVehicleId() < b->getVehicleId() : 
                                               a->getVehicleId() > b->getVehicleId();
                } else if (criteria.sortBy == "capacity") {
                    return criteria.ascending ? a->getCapacity() < b->getCapacity() : 
                                               a->getCapacity() > b->getCapacity();
                } else if (criteria.sortBy == "fare") {
                    return criteria.ascending ? a->getFarePerKm() < b->getFarePerKm() : 
                                               a->getFarePerKm() > b->getFarePerKm();
                } else if (criteria.sortBy == "year") {
                    return criteria.ascending ? a->getYear() < b->getYear() : 
                                               a->getYear() > b->getYear();
                } else if (criteria.sortBy == "manufacturer") {
                    return criteria.ascending ? a->getManufacturer() < b->getManufacturer() : 
                                               a->getManufacturer() > b->getManufacturer();
                } else if (criteria.sortBy == "model") {
                    return criteria.ascending ? a->getModel() < b->getModel() : 
                                               a->getModel() > b->getModel();
                }
                return false;
            });
    }
    
    return results;
}

// Vehicle Status Management
bool VehicleManager::updateVehicleStatus(const std::string& vehicleId, VehicleStatus newStatus) {
    if (!hasPermission("edit")) return false;
    
    auto vehicle = getVehicle(vehicleId);
    if (!vehicle) return false;
    
    VehicleStatus oldStatus = vehicle->getStatus();
    vehicle->setStatus(newStatus);
    
    saveVehiclesToFile();
    logOperation("Status updated", vehicleId, 
                Utilities::vehicleStatusToString(oldStatus) + " -> " + 
                Utilities::vehicleStatusToString(newStatus));
    
    return true;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getAvailableVehicles() const {
    return filterByStatus(VehicleStatus::AVAILABLE);
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getInServiceVehicles() const {
    return filterByStatus(VehicleStatus::IN_SERVICE);
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getMaintenanceVehicles() const {
    return filterByStatus(VehicleStatus::MAINTENANCE);
}

bool VehicleManager::setVehicleForMaintenance(const std::string& vehicleId, const std::string& reason) {
    bool result = updateVehicleStatus(vehicleId, VehicleStatus::MAINTENANCE);
    if (result && !reason.empty()) {
        logOperation("Maintenance scheduled", vehicleId, "Reason: " + reason);
    }
    return result;
}

bool VehicleManager::releaseFromMaintenance(const std::string& vehicleId) {
    return updateVehicleStatus(vehicleId, VehicleStatus::AVAILABLE);
}

// Statistics and Reporting
VehicleManager::VehicleStatistics VehicleManager::getStatistics() const {
    if (!hasPermission("view")) return {};
    
    VehicleStatistics stats;
    stats.totalVehicles = static_cast<int>(vehicles.size());
    
    if (vehicles.empty()) return stats;
    
    double totalCapacity = 0;
    double totalFare = 0;
    long totalYear = 0;
    
    std::map<VehicleType, int> typeCount;
    std::map<std::string, int> manufacturerCount;
    
    for (const auto& vehicle : vehicles) {
        // Status distribution
        switch (vehicle->getStatus()) {
            case VehicleStatus::AVAILABLE:
                stats.availableCount++;
                break;
            case VehicleStatus::IN_SERVICE:
                stats.inServiceCount++;
                break;
            case VehicleStatus::MAINTENANCE:
                stats.maintenanceCount++;
                break;
            case VehicleStatus::RETIRED:
                stats.retiredCount++;
                break;
            default:
                break;
        }
        
        // Type distribution
        typeCount[vehicle->getType()]++;
        
        // Manufacturer distribution
        manufacturerCount[vehicle->getManufacturer()]++;
        
        // Averages
        totalCapacity += vehicle->getCapacity();
        totalFare += vehicle->getFarePerKm();
        totalYear += vehicle->getYear();
    }
    
    stats.typeDistribution = typeCount;
    stats.manufacturerDistribution = manufacturerCount;
    stats.averageCapacity = totalCapacity / vehicles.size();
    stats.averageFare = totalFare / vehicles.size();
    stats.averageYear = static_cast<int>(totalYear / vehicles.size());
    stats.totalCapacity = totalCapacity;
    
    // Find most common type and manufacturer
    auto maxType = std::max_element(typeCount.begin(), typeCount.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    if (maxType != typeCount.end()) {
        stats.mostCommonType = Utilities::vehicleTypeToString(maxType->first);
    }
    
    auto maxManufacturer = std::max_element(manufacturerCount.begin(), manufacturerCount.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    if (maxManufacturer != manufacturerCount.end()) {
        stats.mostCommonManufacturer = maxManufacturer->first;
    }
    
    return stats;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getTopVehiclesByCapacity(int count) const {
    if (!hasPermission("view")) return {};
    
    auto result = vehicles;
    std::sort(result.begin(), result.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->getCapacity() > b->getCapacity();
        });
    
    if (result.size() > static_cast<size_t>(count)) {
        result.resize(count);
    }
    
    return result;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getNewestVehicles(int count) const {
    if (!hasPermission("view")) return {};
    
    auto result = vehicles;
    std::sort(result.begin(), result.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->getYear() > b->getYear();
        });
    
    if (result.size() > static_cast<size_t>(count)) {
        result.resize(count);
    }
    
    return result;
}

std::vector<std::shared_ptr<Vehicle>> VehicleManager::getOldestVehicles(int count) const {
    if (!hasPermission("view")) return {};
    
    auto result = vehicles;
    std::sort(result.begin(), result.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->getYear() < b->getYear();
        });
    
    if (result.size() > static_cast<size_t>(count)) {
        result.resize(count);
    }
    
    return result;
}

// Data validation and integrity
bool VehicleManager::validateVehicleData() const {
    for (const auto& vehicle : vehicles) {
        std::string error;
        if (!isValidVehicleData(*vehicle, error)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> VehicleManager::findDuplicateVehicles() const {
    std::vector<std::string> duplicates;
    std::set<std::string> seenIds;
    std::set<std::string> seenRegistrations;
    
    for (const auto& vehicle : vehicles) {
        if (seenIds.count(vehicle->getVehicleId())) {
            duplicates.push_back("Duplicate ID: " + vehicle->getVehicleId());
        } else {
            seenIds.insert(vehicle->getVehicleId());
        }
        
        if (seenRegistrations.count(vehicle->getRegistrationNumber())) {
            duplicates.push_back("Duplicate Registration: " + vehicle->getRegistrationNumber());
        } else {
            seenRegistrations.insert(vehicle->getRegistrationNumber());
        }
    }
    
    return duplicates;
}

std::vector<std::string> VehicleManager::findInvalidVehicles() const {
    std::vector<std::string> invalid;
    
    for (const auto& vehicle : vehicles) {
        std::string error;
        if (!isValidVehicleData(*vehicle, error)) {
            invalid.push_back(vehicle->getVehicleId() + ": " + error);
        }
    }
    
    return invalid;
}

int VehicleManager::repairDataInconsistencies() {
    if (!hasPermission("edit")) return 0;
    
    int repairs = 0;
    
    // Remove vehicles with invalid data
    auto it = vehicles.begin();
    while (it != vehicles.end()) {
        std::string error;
        if (!isValidVehicleData(**it, error)) {
            logOperation("Data repair", (*it)->getVehicleId(), "Removed invalid vehicle: " + error);
            it = vehicles.erase(it);
            repairs++;
        } else {
            ++it;
        }
    }
    
    if (repairs > 0) {
        saveVehiclesToFile();
        logOperation("Data repair complete", "", std::to_string(repairs) + " inconsistencies repaired");
    }
    
    return repairs;
}

// Utility Methods
int VehicleManager::getVehicleCount() const {
    return static_cast<int>(vehicles.size());
}

bool VehicleManager::vehicleExists(const std::string& vehicleId) const {
    return std::any_of(vehicles.begin(), vehicles.end(),
        [&vehicleId](const std::shared_ptr<Vehicle>& v) {
            return v->getVehicleId() == vehicleId;
        });
}

std::string VehicleManager::getNextAvailableId() const {
    return generateVehicleId();
}

void VehicleManager::clearAllVehicles() {
    if (!hasPermission("delete")) return;
    
    int count = static_cast<int>(vehicles.size());
    vehicles.clear();
    saveVehiclesToFile();
    
    logOperation("All vehicles cleared", "", std::to_string(count) + " vehicles removed");
}

std::vector<std::string> VehicleManager::getAllVehicleIds() const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::string> ids;
    for (const auto& vehicle : vehicles) {
        ids.push_back(vehicle->getVehicleId());
    }
    return ids;
}

std::vector<std::string> VehicleManager::getAllRoutes() const {
    if (!hasPermission("view")) return {};
    
    std::set<std::string> uniqueRoutes;
    for (const auto& vehicle : vehicles) {
        if (!vehicle->getRoute().empty()) {
            uniqueRoutes.insert(vehicle->getRoute());
        }
    }
    
    return std::vector<std::string>(uniqueRoutes.begin(), uniqueRoutes.end());
}

std::vector<std::string> VehicleManager::getAllManufacturers() const {
    if (!hasPermission("view")) return {};
    
    std::set<std::string> uniqueManufacturers;
    for (const auto& vehicle : vehicles) {
        if (!vehicle->getManufacturer().empty()) {
            uniqueManufacturers.insert(vehicle->getManufacturer());
        }
    }
    
    return std::vector<std::string>(uniqueManufacturers.begin(), uniqueManufacturers.end());
}

// Authentication Integration
bool VehicleManager::canAddVehicles() const {
    return hasPermission("add");
}

bool VehicleManager::canEditVehicles() const {
    return hasPermission("edit");
}

bool VehicleManager::canDeleteVehicles() const {
    return hasPermission("delete");
}

bool VehicleManager::canViewVehicles() const {
    return hasPermission("view");
}

std::string VehicleManager::getCurrentUserRole() const {
    if (!authManager || !authManager->isLoggedIn()) return "Not logged in";
    
    auto user = authManager->getCurrentUser();
    return user ? Utilities::roleToString(user->getRole()) : "Unknown";
}

std::string VehicleManager::getCurrentUsername() const {
    if (!authManager || !authManager->isLoggedIn()) return "Not logged in";
    
    auto user = authManager->getCurrentUser();
    return user ? user->getUsername() : "Unknown";
}

// Audit and Logging
std::vector<std::string> VehicleManager::getAuditLog() const {
    std::vector<std::string> logEntries;
    
    try {
        std::ifstream logFile(auditLogPath);
        if (logFile.is_open()) {
            std::string line;
            while (std::getline(logFile, line)) {
                logEntries.push_back(line);
            }
        }
    } catch (const std::exception&) {
        // Return empty log on error
    }
    
    return logEntries;
}

bool VehicleManager::clearAuditLog() {
    if (!hasPermission("delete")) return false;
    
    try {
        std::ofstream logFile(auditLogPath, std::ios::trunc);
        if (logFile.is_open()) {
            logOperation("Audit log cleared", "", "Log file cleared by user");
            return true;
        }
    } catch (const std::exception&) {
        // Ignore errors
    }
    
    return false;
}

void VehicleManager::printAuditSummary() const {
    auto logEntries = getAuditLog();
    
    std::cout << "\n=== VEHICLE AUDIT SUMMARY ===\n";
    std::cout << "Total log entries: " << logEntries.size() << "\n";
    
    if (!logEntries.empty()) {
        std::cout << "Recent entries (last 10):\n";
        int start = std::max(0, static_cast<int>(logEntries.size()) - 10);
        for (int i = start; i < static_cast<int>(logEntries.size()); ++i) {
            std::cout << "  " << logEntries[i] << "\n";
        }
    }
    
    std::cout << "Current user: " << getCurrentUsername() << " (" << getCurrentUserRole() << ")\n";
    std::cout << "================================\n\n";
}

// Display Methods
void VehicleManager::displayVehicle(const std::shared_ptr<Vehicle>& vehicle) const {
    if (!vehicle) return;
    
    std::cout << "\n┌─────────────────────────────────────────────┐\n";
    std::cout << "│               VEHICLE DETAILS               │\n";
    std::cout << "├─────────────────────────────────────────────┤\n";
    std::cout << "│ ID: " << std::left << std::setw(37) << vehicle->getVehicleId() << "│\n";
    std::cout << "│ Type: " << std::left << std::setw(35) << Utilities::vehicleTypeToString(vehicle->getType()) << "│\n";
    std::cout << "│ Registration: " << std::left << std::setw(27) << vehicle->getRegistrationNumber() << "│\n";
    std::cout << "│ Manufacturer: " << std::left << std::setw(27) << vehicle->getManufacturer() << "│\n";
    std::cout << "│ Model: " << std::left << std::setw(33) << vehicle->getModel() << "│\n";
    std::cout << "│ Year: " << std::left << std::setw(34) << vehicle->getYear() << "│\n";
    std::cout << "│ Capacity: " << std::left << std::setw(31) << vehicle->getCapacity() << "│\n";
    std::cout << "│ Fare/Km: $" << std::left << std::setw(29) << std::fixed << std::setprecision(2) << vehicle->getFarePerKm() << "│\n";
    std::cout << "│ Route: " << std::left << std::setw(33) << vehicle->getRoute() << "│\n";
    std::cout << "│ Status: " << std::left << std::setw(32) << Utilities::vehicleStatusToString(vehicle->getStatus()) << "│\n";
    std::cout << "└─────────────────────────────────────────────┘\n";
}

void VehicleManager::displayVehicleList(const std::vector<std::shared_ptr<Vehicle>>& vehicleList) const {
    if (vehicleList.empty()) {
        std::cout << "\nNo vehicles found.\n";
        return;
    }
    
    std::cout << "\n" << std::string(120, '=') << "\n";
    std::cout << std::left << std::setw(8) << "ID" 
              << std::setw(12) << "Type"
              << std::setw(15) << "Registration"
              << std::setw(15) << "Manufacturer"
              << std::setw(15) << "Model"
              << std::setw(8) << "Year"
              << std::setw(10) << "Capacity"
              << std::setw(10) << "Fare/Km"
              << std::setw(15) << "Route"
              << "Status\n";
    std::cout << std::string(120, '-') << "\n";
    
    for (const auto& vehicle : vehicleList) {
        std::cout << std::left << std::setw(8) << vehicle->getVehicleId()
                  << std::setw(12) << Utilities::vehicleTypeToString(vehicle->getType()).substr(0, 11)
                  << std::setw(15) << vehicle->getRegistrationNumber().substr(0, 14)
                  << std::setw(15) << vehicle->getManufacturer().substr(0, 14)
                  << std::setw(15) << vehicle->getModel().substr(0, 14)
                  << std::setw(8) << vehicle->getYear()
                  << std::setw(10) << vehicle->getCapacity()
                  << std::setw(10) << std::fixed << std::setprecision(2) << vehicle->getFarePerKm()
                  << std::setw(15) << vehicle->getRoute().substr(0, 14)
                  << Utilities::vehicleStatusToString(vehicle->getStatus()) << "\n";
    }
    
    std::cout << std::string(120, '=') << "\n";
    std::cout << "Total vehicles: " << vehicleList.size() << "\n";
}

void VehicleManager::displayStatistics() const {
    auto stats = getStatistics();
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    VEHICLE STATISTICS                   ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Total Vehicles: " << std::left << std::setw(40) << stats.totalVehicles << "║\n";
    std::cout << "║ Available: " << std::left << std::setw(45) << stats.availableCount << "║\n";
    std::cout << "║ In Service: " << std::left << std::setw(44) << stats.inServiceCount << "║\n";
    std::cout << "║ Maintenance: " << std::left << std::setw(43) << stats.maintenanceCount << "║\n";
    std::cout << "║ Retired: " << std::left << std::setw(47) << stats.retiredCount << "║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Average Capacity: " << std::left << std::setw(38) << std::fixed << std::setprecision(1) << stats.averageCapacity << "║\n";
    std::cout << "║ Average Fare/Km: $" << std::left << std::setw(36) << std::fixed << std::setprecision(2) << stats.averageFare << "║\n";
    std::cout << "║ Average Year: " << std::left << std::setw(42) << stats.averageYear << "║\n";
    std::cout << "║ Total Fleet Capacity: " << std::left << std::setw(33) << std::fixed << std::setprecision(0) << stats.totalCapacity << "║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Most Common Type: " << std::left << std::setw(38) << stats.mostCommonType << "║\n";
    std::cout << "║ Most Common Manufacturer: " << std::left << std::setw(29) << stats.mostCommonManufacturer << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
}

void VehicleManager::displaySearchHelp() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     SEARCH HELP                         ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Search Criteria:                                        ║\n";
    std::cout << "║ • Text Search: ID, registration, manufacturer, model    ║\n";
    std::cout << "║ • Vehicle Type: Bus, Car, Truck, Motorcycle            ║\n";
    std::cout << "║ • Status: Available, In-Service, Maintenance, Retired   ║\n";
    std::cout << "║ • Capacity Range: Min and max passenger capacity        ║\n";
    std::cout << "║ • Fare Range: Min and max fare per kilometer           ║\n";
    std::cout << "║ • Route: Partial route name matching                   ║\n";
    std::cout << "║ • Manufacturer: Partial manufacturer name matching      ║\n";
    std::cout << "║ • Year Range: Min and max manufacturing year           ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Sorting Options:                                        ║\n";
    std::cout << "║ • vehicleId, capacity, fare, year, manufacturer, model  ║\n";
    std::cout << "║ • Ascending (default) or descending order              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
}

// Error handling
void VehicleManager::setError(VehicleError error, const std::string& message) const {
    lastError = error;
    lastErrorMessage = message;
}

void VehicleManager::clearError() const {
    lastError = VehicleError::NONE;
    lastErrorMessage.clear();
}