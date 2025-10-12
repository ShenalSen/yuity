#include "Vehicle.h"
#include "Utilities.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

// Static constants definition
const std::vector<std::string> Vehicle::VALID_STATUSES = {"Available", "In-Service", "Maintenance"};
const std::vector<std::string> Vehicle::VALID_TYPES = {"Sedan", "SUV", "Hatchback", "Coupe", "Convertible", "Wagon"};
const int Vehicle::MIN_YEAR = 2000;
const int Vehicle::MAX_YEAR = 2030;
const int Vehicle::MIN_CAPACITY = 2;
const int Vehicle::MAX_CAPACITY = 15;

// Default constructor
Vehicle::Vehicle() 
    : vehicleId_(0), licensePlate_(""), model_(""), year_(0), type_(""),
      seatingCapacity_(0), status_("Available"), registrationDate_(""), lastServiceDate_("") {
}

// Parameterized constructor
Vehicle::Vehicle(int id, const std::string& licensePlate, const std::string& model,
                int year, const std::string& type, int seatingCapacity,
                const std::string& status, const std::string& registrationDate,
                const std::string& lastServiceDate)
    : vehicleId_(id), licensePlate_(licensePlate), model_(model), year_(year),
      type_(type), seatingCapacity_(seatingCapacity), status_(status),
      registrationDate_(registrationDate), lastServiceDate_(lastServiceDate) {
}

// Destructor
Vehicle::~Vehicle() {
    // No dynamic memory to clean up
}

// Getters implementation
int Vehicle::getVehicleId() const { return vehicleId_; }
std::string Vehicle::getLicensePlate() const { return licensePlate_; }
std::string Vehicle::getModel() const { return model_; }
int Vehicle::getYear() const { return year_; }
std::string Vehicle::getType() const { return type_; }
int Vehicle::getSeatingCapacity() const { return seatingCapacity_; }
std::string Vehicle::getStatus() const { return status_; }
std::string Vehicle::getRegistrationDate() const { return registrationDate_; }
std::string Vehicle::getLastServiceDate() const { return lastServiceDate_; }

// Setters implementation
void Vehicle::setVehicleId(int id) { vehicleId_ = id; }
void Vehicle::setLicensePlate(const std::string& licensePlate) { licensePlate_ = licensePlate; }
void Vehicle::setModel(const std::string& model) { model_ = model; }
void Vehicle::setYear(int year) { year_ = year; }
void Vehicle::setType(const std::string& type) { type_ = type; }
void Vehicle::setSeatingCapacity(int capacity) { seatingCapacity_ = capacity; }
void Vehicle::setStatus(const std::string& status) { status_ = status; }
void Vehicle::setRegistrationDate(const std::string& date) { registrationDate_ = date; }
void Vehicle::setLastServiceDate(const std::string& date) { lastServiceDate_ = date; }

// Validation methods
bool Vehicle::isValidLicensePlate(const std::string& licensePlate) const {
    if (licensePlate.empty() || licensePlate.length() < 5 || licensePlate.length() > 10) {
        return false;
    }
    
    // Check for valid characters (alphanumeric and hyphens)
    for (char c : licensePlate) {
        if (!std::isalnum(c) && c != '-') {
            return false;
        }
    }
    return true;
}

bool Vehicle::isValidYear(int year) const {
    return year >= MIN_YEAR && year <= MAX_YEAR;
}

bool Vehicle::isValidStatus(const std::string& status) const {
    return std::find(VALID_STATUSES.begin(), VALID_STATUSES.end(), status) != VALID_STATUSES.end();
}

bool Vehicle::isValidCapacity(int capacity) const {
    return capacity >= MIN_CAPACITY && capacity <= MAX_CAPACITY;
}

bool Vehicle::validateVehicleData() const {
    return !TourmateUtils::isEmpty(licensePlate_) &&
           !TourmateUtils::isEmpty(model_) &&
           !TourmateUtils::isEmpty(type_) &&
           isValidLicensePlate(licensePlate_) &&
           isValidYear(year_) &&
           isValidStatus(status_) &&
           isValidCapacity(seatingCapacity_);
}

// Utility methods
void Vehicle::displayVehicle() const {
    std::cout << "==================================================\n";
    std::cout << "                 VEHICLE DETAILS\n";
    std::cout << "==================================================\n";
    std::cout << "Vehicle ID       : " << vehicleId_ << "\n";
    std::cout << "License Plate    : " << licensePlate_ << "\n";
    std::cout << "Model            : " << model_ << "\n";
    std::cout << "Year             : " << year_ << "\n";
    std::cout << "Type             : " << type_ << "\n";
    std::cout << "Seating Capacity : " << seatingCapacity_ << "\n";
    std::cout << "Status           : " << status_ << "\n";
    std::cout << "Registration Date: " << registrationDate_ << "\n";
    std::cout << "Last Service Date: " << lastServiceDate_ << "\n";
    std::cout << "==================================================\n";
}

std::string Vehicle::toString() const {
    std::ostringstream oss;
    oss << "Vehicle[ID=" << vehicleId_ 
        << ", Plate=" << licensePlate_
        << ", Model=" << model_
        << ", Year=" << year_
        << ", Type=" << type_
        << ", Capacity=" << seatingCapacity_
        << ", Status=" << status_
        << ", RegDate=" << registrationDate_
        << ", ServiceDate=" << lastServiceDate_ << "]";
    return oss.str();
}

std::string Vehicle::toCSV() const {
    std::vector<std::string> fields = {
        TourmateUtils::intToString(vehicleId_),
        licensePlate_,
        model_,
        TourmateUtils::intToString(year_),
        type_,
        TourmateUtils::intToString(seatingCapacity_),
        status_,
        registrationDate_,
        lastServiceDate_
    };
    return TourmateUtils::createCSVLine(fields);
}

bool Vehicle::fromCSV(const std::string& csvLine) {
    std::vector<std::string> fields = TourmateUtils::parseCSVLine(csvLine);
    
    if (fields.size() != 9) {
        return false;
    }
    
    try {
        vehicleId_ = TourmateUtils::stringToInt(fields[0]);
        licensePlate_ = TourmateUtils::trim(fields[1]);
        model_ = TourmateUtils::trim(fields[2]);
        year_ = TourmateUtils::stringToInt(fields[3]);
        type_ = TourmateUtils::trim(fields[4]);
        seatingCapacity_ = TourmateUtils::stringToInt(fields[5]);
        status_ = TourmateUtils::trim(fields[6]);
        registrationDate_ = TourmateUtils::trim(fields[7]);
        lastServiceDate_ = TourmateUtils::trim(fields[8]);
        
        return validateVehicleData();
    } catch (...) {
        return false;
    }
}

// Status management
bool Vehicle::isAvailable() const { return status_ == "Available"; }
bool Vehicle::isInService() const { return status_ == "In-Service"; }
bool Vehicle::isInMaintenance() const { return status_ == "Maintenance"; }

void Vehicle::setAvailable() { status_ = "Available"; }
void Vehicle::setInService() { status_ = "In-Service"; }
void Vehicle::setInMaintenance() { status_ = "Maintenance"; }

// Comparison operators
bool Vehicle::operator==(const Vehicle& other) const {
    return vehicleId_ == other.vehicleId_;
}

bool Vehicle::operator!=(const Vehicle& other) const {
    return !(*this == other);
}

bool Vehicle::operator<(const Vehicle& other) const {
    return vehicleId_ < other.vehicleId_;
}

// Stream operators
std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
    os << vehicle.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Vehicle& vehicle) {
    std::string line;
    std::getline(is, line);
    vehicle.fromCSV(line);
    return is;
}