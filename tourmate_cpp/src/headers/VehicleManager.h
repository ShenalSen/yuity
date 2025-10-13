#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "Vehicle.h"
#include "User.h"
#include "AuthenticationManager.h"

/**
 * @class VehicleManager
 * @brief Comprehensive vehicle management system with CRUD operations,
 *        search functionality, file I/O, and authentication integration
 * 
 * Features:
 * - Complete CRUD operations (Create, Read, Update, Delete)
 * - Advanced search and filtering capabilities
 * - CSV file-based persistence with backup/restore
 * - Role-based permission system integration
 * - Vehicle status management (Available, In-Service, Maintenance, Retired)
 * - Audit logging with user tracking
 * - Data validation and error handling
 * - Statistical reporting and analytics
 */
class VehicleManager {
private:
    std::vector<std::shared_ptr<Vehicle>> vehicles;           // Vehicle collection
    std::shared_ptr<AuthenticationManager> authManager;       // Authentication system
    std::string dataFilePath;                                 // CSV file path
    std::string backupFilePath;                              // Backup file path
    std::string auditLogPath;                                // Audit log path
    bool fileLoaded;                                         // File load status
    
    // Internal utility methods
    bool hasPermission(const std::string& operation) const;
    void logOperation(const std::string& operation, const std::string& vehicleId = "", 
                     const std::string& details = "") const;
    std::string generateVehicleId() const;
    bool isValidVehicleData(const Vehicle& vehicle, std::string& errorMessage) const;
    std::vector<std::string> splitCSVLine(const std::string& line) const;
    std::string escapeCSVField(const std::string& field) const;
    void sortVehicles(const std::string& sortBy, bool ascending = true);

public:
    // Constructor and Destructor
    explicit VehicleManager(std::shared_ptr<AuthenticationManager> authMgr,
                           const std::string& dataFile = "data/vehicles.csv");
    ~VehicleManager();

    // File Operations
    bool loadVehiclesFromFile();
    bool saveVehiclesToFile() const;
    bool createBackup() const;
    bool restoreFromBackup();
    bool exportToCSV(const std::string& filename, 
                     const std::vector<std::shared_ptr<Vehicle>>& vehicleList) const;
    bool importFromCSV(const std::string& filename);

    // CRUD Operations
    bool addVehicle(const Vehicle& vehicle);
    std::shared_ptr<Vehicle> getVehicle(const std::string& vehicleId) const;
    std::vector<std::shared_ptr<Vehicle>> getAllVehicles() const;
    bool updateVehicle(const std::string& vehicleId, const Vehicle& updatedVehicle);
    bool deleteVehicle(const std::string& vehicleId);

    // Search and Filter Operations
    std::vector<std::shared_ptr<Vehicle>> searchVehicles(const std::string& searchTerm) const;
    std::vector<std::shared_ptr<Vehicle>> filterByType(VehicleType type) const;
    std::vector<std::shared_ptr<Vehicle>> filterByStatus(VehicleStatus status) const;
    std::vector<std::shared_ptr<Vehicle>> filterByCapacity(int minCapacity, int maxCapacity = -1) const;
    std::vector<std::shared_ptr<Vehicle>> filterByFareRange(double minFare, double maxFare = -1.0) const;
    std::vector<std::shared_ptr<Vehicle>> filterByRoute(const std::string& route) const;
    std::vector<std::shared_ptr<Vehicle>> filterByManufacturer(const std::string& manufacturer) const;
    std::vector<std::shared_ptr<Vehicle>> filterByYearRange(int minYear, int maxYear = -1) const;

    // Advanced Search with Multiple Criteria
    struct SearchCriteria {
        std::string searchTerm;
        VehicleType type = VehicleType::UNKNOWN;
        VehicleStatus status = VehicleStatus::UNKNOWN;
        int minCapacity = -1;
        int maxCapacity = -1;
        double minFare = -1.0;
        double maxFare = -1.0;
        std::string route;
        std::string manufacturer;
        int minYear = -1;
        int maxYear = -1;
        std::string sortBy = "vehicleId";
        bool ascending = true;
    };
    
    std::vector<std::shared_ptr<Vehicle>> advancedSearch(const SearchCriteria& criteria) const;

    // Vehicle Status Management
    bool updateVehicleStatus(const std::string& vehicleId, VehicleStatus newStatus);
    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles() const;
    std::vector<std::shared_ptr<Vehicle>> getInServiceVehicles() const;
    std::vector<std::shared_ptr<Vehicle>> getMaintenanceVehicles() const;
    bool setVehicleForMaintenance(const std::string& vehicleId, const std::string& reason = "");
    bool releaseFromMaintenance(const std::string& vehicleId);

    // Statistics and Reporting
    struct VehicleStatistics {
        int totalVehicles = 0;
        int availableCount = 0;
        int inServiceCount = 0;
        int maintenanceCount = 0;
        int retiredCount = 0;
        std::map<VehicleType, int> typeDistribution;
        std::map<std::string, int> manufacturerDistribution;
        double averageCapacity = 0.0;
        double averageFare = 0.0;
        int averageYear = 0;
        double totalCapacity = 0.0;
        std::string mostCommonType;
        std::string mostCommonManufacturer;
    };
    
    VehicleStatistics getStatistics() const;
    std::vector<std::shared_ptr<Vehicle>> getTopVehiclesByCapacity(int count = 10) const;
    std::vector<std::shared_ptr<Vehicle>> getNewestVehicles(int count = 10) const;
    std::vector<std::shared_ptr<Vehicle>> getOldestVehicles(int count = 10) const;

    // Data Validation and Integrity
    bool validateVehicleData() const;
    std::vector<std::string> findDuplicateVehicles() const;
    std::vector<std::string> findInvalidVehicles() const;
    int repairDataInconsistencies();

    // Utility Methods
    int getVehicleCount() const;
    bool vehicleExists(const std::string& vehicleId) const;
    std::string getNextAvailableId() const;
    void clearAllVehicles();
    std::vector<std::string> getAllVehicleIds() const;
    std::vector<std::string> getAllRoutes() const;
    std::vector<std::string> getAllManufacturers() const;

    // Authentication Integration
    bool canAddVehicles() const;
    bool canEditVehicles() const;
    bool canDeleteVehicles() const;
    bool canViewVehicles() const;
    std::string getCurrentUserRole() const;
    std::string getCurrentUsername() const;

    // Audit and Logging
    std::vector<std::string> getAuditLog() const;
    bool clearAuditLog();
    void printAuditSummary() const;

    // Display Methods
    void displayVehicle(const std::shared_ptr<Vehicle>& vehicle) const;
    void displayVehicleList(const std::vector<std::shared_ptr<Vehicle>>& vehicleList) const;
    void displayStatistics() const;
    void displaySearchHelp() const;

    // Error Handling
    enum class VehicleError {
        NONE,
        INVALID_DATA,
        DUPLICATE_ID,
        NOT_FOUND,
        PERMISSION_DENIED,
        FILE_ERROR,
        AUTHENTICATION_REQUIRED
    };
    
    VehicleError getLastError() const { return lastError; }
    std::string getLastErrorMessage() const { return lastErrorMessage; }

private:
    mutable VehicleError lastError = VehicleError::NONE;
    mutable std::string lastErrorMessage;
    
    void setError(VehicleError error, const std::string& message) const;
    void clearError() const;
};

#endif // VEHICLEMANAGER_H