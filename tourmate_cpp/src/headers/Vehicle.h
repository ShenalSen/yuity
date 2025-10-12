#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
#include <vector>

/**
 * @brief Vehicle class representing a vehicle in the Tourmate fleet
 * 
 * This class encapsulates all vehicle-related data and provides
 * methods for vehicle manipulation and validation.
 */
class Vehicle {
private:
    int vehicleId_;
    std::string licensePlate_;
    std::string model_;
    int year_;
    std::string type_;
    int seatingCapacity_;
    std::string status_;
    std::string registrationDate_;
    std::string lastServiceDate_;

public:
    // Constructors
    Vehicle();
    Vehicle(int id, const std::string& licensePlate, const std::string& model,
            int year, const std::string& type, int seatingCapacity,
            const std::string& status, const std::string& registrationDate,
            const std::string& lastServiceDate);

    // Destructor
    ~Vehicle();

    // Getters
    int getVehicleId() const;
    std::string getLicensePlate() const;
    std::string getModel() const;
    int getYear() const;
    std::string getType() const;
    int getSeatingCapacity() const;
    std::string getStatus() const;
    std::string getRegistrationDate() const;
    std::string getLastServiceDate() const;

    // Setters
    void setVehicleId(int id);
    void setLicensePlate(const std::string& licensePlate);
    void setModel(const std::string& model);
    void setYear(int year);
    void setType(const std::string& type);
    void setSeatingCapacity(int capacity);
    void setStatus(const std::string& status);
    void setRegistrationDate(const std::string& date);
    void setLastServiceDate(const std::string& date);

    // Validation methods
    bool isValidLicensePlate(const std::string& licensePlate) const;
    bool isValidYear(int year) const;
    bool isValidStatus(const std::string& status) const;
    bool isValidCapacity(int capacity) const;
    bool validateVehicleData() const;

    // Utility methods
    void displayVehicle() const;
    std::string toString() const;
    std::string toCSV() const;
    bool fromCSV(const std::string& csvLine);
    
    // Status management
    bool isAvailable() const;
    bool isInService() const;
    bool isInMaintenance() const;
    void setAvailable();
    void setInService();
    void setInMaintenance();

    // Comparison operators
    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;
    bool operator<(const Vehicle& other) const;

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle);

    // Static constants
    static const std::vector<std::string> VALID_STATUSES;
    static const std::vector<std::string> VALID_TYPES;
    static const int MIN_YEAR;
    static const int MAX_YEAR;
    static const int MIN_CAPACITY;
    static const int MAX_CAPACITY;
};

#endif // VEHICLE_H