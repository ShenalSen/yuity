#include "../headers/SalesManager.h"
#include "../headers/Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <set>
#include <cmath>

// Constructor
SalesManager::SalesManager(std::shared_ptr<AuthenticationManager> authMgr,
                          std::shared_ptr<VehicleManager> vehicleMgr,
                          const std::string& salesDataFile,
                          const std::string& bookingDataFile,
                          const std::string& customerDataFile)
    : authManager(authMgr), vehicleManager(vehicleMgr), 
      salesDataPath(salesDataFile), bookingDataPath(bookingDataFile),
      customerDataPath(customerDataFile), dataLoaded(false) {
    
    // Set up audit log path
    auditLogPath = "data/sales_audit.log";
    
    // Ensure data directory exists
    Utilities::createDirectoryIfNotExists("data");
    
    // Load existing data
    loadDataFromFiles();
    
    logOperation("SalesManager initialized", "", 
                "Sales Manager started with " + std::to_string(salesRecords.size()) + 
                " sales records, " + std::to_string(bookings.size()) + " bookings, " +
                std::to_string(customers.size()) + " customers");
}

// Destructor
SalesManager::~SalesManager() {
    saveDataToFiles();
    logOperation("SalesManager destroyed", "", "Sales Manager shutdown");
}

// Permission checking
bool SalesManager::hasPermission(const std::string& operation) const {
    if (!authManager || !authManager->isLoggedIn()) {
        setError(SalesError::AUTHENTICATION_REQUIRED, "User must be logged in");
        return false;
    }

    if (operation == "view") {
        return authManager->canViewSales();
    } else if (operation == "create" || operation == "edit") {
        return authManager->canManageSales();
    } else if (operation == "delete" || operation == "refund") {
        return authManager->canDeleteSales();
    } else if (operation == "reports") {
        return authManager->canViewReports();
    }
    
    return false;
}

// Audit logging
void SalesManager::logOperation(const std::string& operation, const std::string& recordId,
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
            
            if (!recordId.empty()) {
                logFile << " | Record: " << recordId;
            }
            
            if (!details.empty()) {
                logFile << " | " << details;
            }
            
            logFile << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Audit log error: " << e.what() << std::endl;
    }
}

// ID generation methods
std::string SalesManager::generateSalesId() const {
    std::string prefix = "SAL";
    int maxNum = 0;
    
    for (const auto& sales : salesRecords) {
        std::string id = sales->getSalesId();
        if (id.length() > 3 && id.substr(0, 3) == prefix) {
            try {
                int num = std::stoi(id.substr(3));
                maxNum = std::max(maxNum, num);
            } catch (const std::exception&) {
                // Skip invalid IDs
            }
        }
    }
    
    return prefix + std::to_string(maxNum + 1);
}

std::string SalesManager::generateBookingId() const {
    std::string prefix = "BK";
    int maxNum = 0;
    
    for (const auto& booking : bookings) {
        std::string id = booking.bookingId;
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

std::string SalesManager::generateCustomerId() const {
    std::string prefix = "CU";
    int maxNum = 0;
    
    for (const auto& customer : customers) {
        std::string id = customer.customerId;
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

// Data validation methods
bool SalesManager::isValidSalesData(const Sales& sales, std::string& errorMessage) const {
    if (sales.getSalesId().empty()) {
        errorMessage = "Sales ID cannot be empty";
        return false;
    }
    
    if (sales.getVehicleId().empty()) {
        errorMessage = "Vehicle ID cannot be empty";
        return false;
    }
    
    if (sales.getTotalAmount() < 0) {
        errorMessage = "Total amount cannot be negative";
        return false;
    }
    
    if (sales.getCustomerName().empty()) {
        errorMessage = "Customer name cannot be empty";
        return false;
    }
    
    return true;
}

bool SalesManager::isValidBookingData(const Booking& booking, std::string& errorMessage) const {
    if (booking.bookingId.empty()) {
        errorMessage = "Booking ID cannot be empty";
        return false;
    }
    
    if (booking.customerId.empty()) {
        errorMessage = "Customer ID cannot be empty";
        return false;
    }
    
    if (booking.vehicleId.empty()) {
        errorMessage = "Vehicle ID cannot be empty";
        return false;
    }
    
    if (booking.fromLocation.empty() || booking.toLocation.empty()) {
        errorMessage = "From and To locations cannot be empty";
        return false;
    }
    
    if (booking.passengers <= 0) {
        errorMessage = "Number of passengers must be greater than 0";
        return false;
    }
    
    if (booking.totalFare < 0) {
        errorMessage = "Total fare cannot be negative";
        return false;
    }
    
    return true;
}

bool SalesManager::isValidCustomerData(const Customer& customer, std::string& errorMessage) const {
    if (customer.customerId.empty()) {
        errorMessage = "Customer ID cannot be empty";
        return false;
    }
    
    if (customer.name.empty()) {
        errorMessage = "Customer name cannot be empty";
        return false;
    }
    
    if (customer.email.empty()) {
        errorMessage = "Email cannot be empty";
        return false;
    }
    
    if (customer.phone.empty()) {
        errorMessage = "Phone number cannot be empty";
        return false;
    }
    
    return true;
}

// CSV utility methods
std::vector<std::string> SalesManager::splitCSVLine(const std::string& line) const {
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

std::string SalesManager::escapeCSVField(const std::string& field) const {
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

std::string SalesManager::timePointToString(const std::chrono::system_clock::time_point& tp) const {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::chrono::system_clock::time_point SalesManager::stringToTimePoint(const std::string& str) const {
    std::tm tm = {};
    std::stringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Fare calculation utilities
double SalesManager::calculateBaseFare(const std::string& vehicleId, double distance) const {
    if (!vehicleManager) return 0.0;
    
    auto vehicle = vehicleManager->getVehicle(vehicleId);
    if (!vehicle) return 0.0;
    
    return vehicle->getFarePerKm() * distance;
}

double SalesManager::calculateDiscounts(const Customer& customer, double baseFare) const {
    double discount = 0.0;
    
    // VIP customer discount
    if (customer.isVip) {
        discount += baseFare * 0.10; // 10% discount for VIP
    }
    
    // Frequent customer discount
    if (customer.totalBookings > 10) {
        discount += baseFare * 0.05; // 5% discount for frequent customers
    }
    
    // Large spending customer discount
    if (customer.totalSpent > 1000.0) {
        discount += baseFare * 0.03; // 3% discount for high-value customers
    }
    
    return discount;
}

double SalesManager::calculateTaxes(double fare) const {
    return fare * 0.08; // 8% tax rate
}

// Vehicle availability checking
bool SalesManager::isVehicleAvailable(const std::string& vehicleId, 
                                     const std::chrono::system_clock::time_point& startTime,
                                     const std::chrono::system_clock::time_point& endTime) const {
    if (!vehicleManager) return false;
    
    // Check if vehicle exists and is available
    auto vehicle = vehicleManager->getVehicle(vehicleId);
    if (!vehicle || vehicle->getStatus() != VehicleStatus::AVAILABLE) {
        return false;
    }
    
    // Check for conflicting bookings
    for (const auto& booking : bookings) {
        if (booking.vehicleId == vehicleId && 
            (booking.status == BookingStatus::CONFIRMED || booking.status == BookingStatus::IN_PROGRESS)) {
            
            // Check for time overlap
            if (!(endTime <= booking.departureTime || startTime >= booking.arrivalTime)) {
                return false; // Time conflict
            }
        }
    }
    
    return true;
}

// File Operations
bool SalesManager::loadDataFromFiles() {
    bool success = true;
    
    // Load sales records
    try {
        std::ifstream salesFile(salesDataPath);
        if (salesFile.is_open()) {
            salesRecords.clear();
            std::string line;
            bool isFirstLine = true;
            
            while (std::getline(salesFile, line)) {
                if (isFirstLine) {
                    isFirstLine = false;
                    continue; // Skip header
                }
                
                if (line.empty()) continue;
                
                auto fields = splitCSVLine(line);
                if (fields.size() >= 7) {
                    try {
                        auto sales = std::make_shared<Sales>();
                        sales->setSalesId(fields[0]);
                        sales->setVehicleId(fields[1]);
                        sales->setCustomerName(fields[2]);
                        sales->setCustomerId(fields[3]);
                        sales->setTotalAmount(std::stod(fields[4]));
                        sales->setSaleDate(stringToTimePoint(fields[5]));
                        sales->setPaymentMethod(fields[6]);
                        
                        salesRecords.push_back(sales);
                    } catch (const std::exception& e) {
                        logOperation("Load error", "", "Invalid sales line: " + line + " - " + e.what());
                    }
                }
            }
            salesFile.close();
        } else {
            // Create empty file if it doesn't exist
            std::ofstream newFile(salesDataPath);
            if (newFile.is_open()) {
                newFile << "SalesId,VehicleId,CustomerName,CustomerId,TotalAmount,SaleDate,PaymentMethod\n";
                newFile.close();
            }
        }
    } catch (const std::exception& e) {
        logOperation("Load error", "", "Failed to load sales data: " + std::string(e.what()));
        success = false;
    }
    
    // Load customers
    try {
        std::ifstream customerFile(customerDataPath);
        if (customerFile.is_open()) {
            customers.clear();
            std::string line;
            bool isFirstLine = true;
            
            while (std::getline(customerFile, line)) {
                if (isFirstLine) {
                    isFirstLine = false;
                    continue; // Skip header
                }
                
                if (line.empty()) continue;
                
                auto fields = splitCSVLine(line);
                if (fields.size() >= 9) {
                    try {
                        Customer customer;
                        customer.customerId = fields[0];
                        customer.name = fields[1];
                        customer.email = fields[2];
                        customer.phone = fields[3];
                        customer.address = fields[4];
                        customer.registrationDate = stringToTimePoint(fields[5]);
                        customer.totalBookings = std::stoi(fields[6]);
                        customer.totalSpent = std::stod(fields[7]);
                        customer.isVip = (fields[8] == "true" || fields[8] == "1");
                        
                        customers.push_back(customer);
                    } catch (const std::exception& e) {
                        logOperation("Load error", "", "Invalid customer line: " + line + " - " + e.what());
                    }
                }
            }
            customerFile.close();
        } else {
            // Create empty file if it doesn't exist
            std::ofstream newFile(customerDataPath);
            if (newFile.is_open()) {
                newFile << "CustomerId,Name,Email,Phone,Address,RegistrationDate,TotalBookings,TotalSpent,IsVip\n";
                newFile.close();
            }
        }
    } catch (const std::exception& e) {
        logOperation("Load error", "", "Failed to load customer data: " + std::string(e.what()));
        success = false;
    }
    
    // Load bookings
    try {
        std::ifstream bookingFile(bookingDataPath);
        if (bookingFile.is_open()) {
            bookings.clear();
            std::string line;
            bool isFirstLine = true;
            
            while (std::getline(bookingFile, line)) {
                if (isFirstLine) {
                    isFirstLine = false;
                    continue; // Skip header
                }
                
                if (line.empty()) continue;
                
                auto fields = splitCSVLine(line);
                if (fields.size() >= 16) {
                    try {
                        Booking booking;
                        booking.bookingId = fields[0];
                        booking.customerId = fields[1];
                        booking.vehicleId = fields[2];
                        booking.fromLocation = fields[3];
                        booking.toLocation = fields[4];
                        booking.departureTime = stringToTimePoint(fields[5]);
                        booking.arrivalTime = stringToTimePoint(fields[6]);
                        booking.tripType = stringToTripType(fields[7]);
                        booking.passengers = std::stoi(fields[8]);
                        booking.basefare = std::stod(fields[9]);
                        booking.totalFare = std::stod(fields[10]);
                        booking.discount = std::stod(fields[11]);
                        booking.paymentMethod = stringToPaymentMethod(fields[12]);
                        booking.status = stringToBookingStatus(fields[13]);
                        booking.specialRequests = fields[14];
                        booking.bookingDate = stringToTimePoint(fields[15]);
                        if (fields.size() > 16) booking.bookedBy = fields[16];
                        
                        bookings.push_back(booking);
                    } catch (const std::exception& e) {
                        logOperation("Load error", "", "Invalid booking line: " + line + " - " + e.what());
                    }
                }
            }
            bookingFile.close();
        } else {
            // Create empty file if it doesn't exist
            std::ofstream newFile(bookingDataPath);
            if (newFile.is_open()) {
                newFile << "BookingId,CustomerId,VehicleId,FromLocation,ToLocation,DepartureTime,ArrivalTime,TripType,Passengers,BaseFare,TotalFare,Discount,PaymentMethod,Status,SpecialRequests,BookingDate,BookedBy\n";
                newFile.close();
            }
        }
    } catch (const std::exception& e) {
        logOperation("Load error", "", "Failed to load booking data: " + std::string(e.what()));
        success = false;
    }
    
    dataLoaded = success;
    
    if (success) {
        logOperation("Data loaded", "", 
                    std::to_string(salesRecords.size()) + " sales, " +
                    std::to_string(customers.size()) + " customers, " +
                    std::to_string(bookings.size()) + " bookings loaded");
    }
    
    return success;
}

bool SalesManager::saveDataToFiles() const {
    bool success = true;
    
    // Save sales records
    try {
        std::ofstream salesFile(salesDataPath);
        if (salesFile.is_open()) {
            salesFile << "SalesId,VehicleId,CustomerName,CustomerId,TotalAmount,SaleDate,PaymentMethod\n";
            
            for (const auto& sales : salesRecords) {
                salesFile << escapeCSVField(sales->getSalesId()) << ","
                         << escapeCSVField(sales->getVehicleId()) << ","
                         << escapeCSVField(sales->getCustomerName()) << ","
                         << escapeCSVField(sales->getCustomerId()) << ","
                         << sales->getTotalAmount() << ","
                         << escapeCSVField(timePointToString(sales->getSaleDate())) << ","
                         << escapeCSVField(sales->getPaymentMethod()) << "\n";
            }
            salesFile.close();
        } else {
            success = false;
        }
    } catch (const std::exception& e) {
        logOperation("Save error", "", "Failed to save sales data: " + std::string(e.what()));
        success = false;
    }
    
    // Save customers
    try {
        std::ofstream customerFile(customerDataPath);
        if (customerFile.is_open()) {
            customerFile << "CustomerId,Name,Email,Phone,Address,RegistrationDate,TotalBookings,TotalSpent,IsVip\n";
            
            for (const auto& customer : customers) {
                customerFile << escapeCSVField(customer.customerId) << ","
                            << escapeCSVField(customer.name) << ","
                            << escapeCSVField(customer.email) << ","
                            << escapeCSVField(customer.phone) << ","
                            << escapeCSVField(customer.address) << ","
                            << escapeCSVField(timePointToString(customer.registrationDate)) << ","
                            << customer.totalBookings << ","
                            << customer.totalSpent << ","
                            << (customer.isVip ? "true" : "false") << "\n";
            }
            customerFile.close();
        } else {
            success = false;
        }
    } catch (const std::exception& e) {
        logOperation("Save error", "", "Failed to save customer data: " + std::string(e.what()));
        success = false;
    }
    
    // Save bookings
    try {
        std::ofstream bookingFile(bookingDataPath);
        if (bookingFile.is_open()) {
            bookingFile << "BookingId,CustomerId,VehicleId,FromLocation,ToLocation,DepartureTime,ArrivalTime,TripType,Passengers,BaseFare,TotalFare,Discount,PaymentMethod,Status,SpecialRequests,BookingDate,BookedBy\n";
            
            for (const auto& booking : bookings) {
                bookingFile << escapeCSVField(booking.bookingId) << ","
                           << escapeCSVField(booking.customerId) << ","
                           << escapeCSVField(booking.vehicleId) << ","
                           << escapeCSVField(booking.fromLocation) << ","
                           << escapeCSVField(booking.toLocation) << ","
                           << escapeCSVField(timePointToString(booking.departureTime)) << ","
                           << escapeCSVField(timePointToString(booking.arrivalTime)) << ","
                           << escapeCSVField(tripTypeToString(booking.tripType)) << ","
                           << booking.passengers << ","
                           << booking.basefare << ","
                           << booking.totalFare << ","
                           << booking.discount << ","
                           << escapeCSVField(paymentMethodToString(booking.paymentMethod)) << ","
                           << escapeCSVField(bookingStatusToString(booking.status)) << ","
                           << escapeCSVField(booking.specialRequests) << ","
                           << escapeCSVField(timePointToString(booking.bookingDate)) << ","
                           << escapeCSVField(booking.bookedBy) << "\n";
            }
            bookingFile.close();
        } else {
            success = false;
        }
    } catch (const std::exception& e) {
        logOperation("Save error", "", "Failed to save booking data: " + std::string(e.what()));
        success = false;
    }
    
    if (success) {
        logOperation("Data saved", "", 
                    std::to_string(salesRecords.size()) + " sales, " +
                    std::to_string(customers.size()) + " customers, " +
                    std::to_string(bookings.size()) + " bookings saved");
    }
    
    return success;
}

// Sales CRUD Operations
bool SalesManager::addSalesRecord(const Sales& sales) {
    if (!hasPermission("create")) return false;
    
    std::string errorMessage;
    if (!isValidSalesData(sales, errorMessage)) {
        setError(SalesError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Check for duplicate ID
    if (salesRecordExists(sales.getSalesId())) {
        setError(SalesError::DUPLICATE_ID, "Sales ID already exists: " + sales.getSalesId());
        return false;
    }
    
    auto newSales = std::make_shared<Sales>(sales);
    salesRecords.push_back(newSales);
    
    saveDataToFiles();
    logOperation("Sales record added", sales.getSalesId(), 
                "Added sales for customer " + sales.getCustomerName() + 
                ", amount: $" + std::to_string(sales.getTotalAmount()));
    
    clearError();
    return true;
}

std::shared_ptr<Sales> SalesManager::getSalesRecord(const std::string& salesId) const {
    if (!hasPermission("view")) return nullptr;
    
    auto it = std::find_if(salesRecords.begin(), salesRecords.end(),
        [&salesId](const std::shared_ptr<Sales>& s) {
            return s->getSalesId() == salesId;
        });
    
    if (it != salesRecords.end()) {
        clearError();
        return *it;
    }
    
    setError(SalesError::NOT_FOUND, "Sales record not found: " + salesId);
    return nullptr;
}

std::vector<std::shared_ptr<Sales>> SalesManager::getAllSalesRecords() const {
    if (!hasPermission("view")) return {};
    
    clearError();
    return salesRecords;
}

bool SalesManager::updateSalesRecord(const std::string& salesId, const Sales& updatedSales) {
    if (!hasPermission("edit")) return false;
    
    auto it = std::find_if(salesRecords.begin(), salesRecords.end(),
        [&salesId](const std::shared_ptr<Sales>& s) {
            return s->getSalesId() == salesId;
        });
    
    if (it == salesRecords.end()) {
        setError(SalesError::NOT_FOUND, "Sales record not found: " + salesId);
        return false;
    }
    
    std::string errorMessage;
    if (!isValidSalesData(updatedSales, errorMessage)) {
        setError(SalesError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Update sales data
    **it = updatedSales;
    (*it)->setSalesId(salesId); // Ensure ID doesn't change
    
    saveDataToFiles();
    logOperation("Sales record updated", salesId, 
                "Updated sales for customer " + updatedSales.getCustomerName());
    
    clearError();
    return true;
}

bool SalesManager::deleteSalesRecord(const std::string& salesId) {
    if (!hasPermission("delete")) return false;
    
    auto it = std::find_if(salesRecords.begin(), salesRecords.end(),
        [&salesId](const std::shared_ptr<Sales>& s) {
            return s->getSalesId() == salesId;
        });
    
    if (it == salesRecords.end()) {
        setError(SalesError::NOT_FOUND, "Sales record not found: " + salesId);
        return false;
    }
    
    std::string details = "Deleted sales for customer " + (*it)->getCustomerName() + 
                         ", amount: $" + std::to_string((*it)->getTotalAmount());
    salesRecords.erase(it);
    
    saveDataToFiles();
    logOperation("Sales record deleted", salesId, details);
    
    clearError();
    return true;
}

// Customer Management
bool SalesManager::addCustomer(const Customer& customer) {
    if (!hasPermission("create")) return false;
    
    std::string errorMessage;
    if (!isValidCustomerData(customer, errorMessage)) {
        setError(SalesError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Check for duplicate ID, email, or phone
    if (customerExists(customer.customerId)) {
        setError(SalesError::DUPLICATE_ID, "Customer ID already exists: " + customer.customerId);
        return false;
    }
    
    if (findCustomerByEmail(customer.email)) {
        setError(SalesError::DUPLICATE_ID, "Email already registered: " + customer.email);
        return false;
    }
    
    if (findCustomerByPhone(customer.phone)) {
        setError(SalesError::DUPLICATE_ID, "Phone already registered: " + customer.phone);
        return false;
    }
    
    customers.push_back(customer);
    
    saveDataToFiles();
    logOperation("Customer added", customer.customerId, 
                "Added customer " + customer.name + " (" + customer.email + ")");
    
    clearError();
    return true;
}

SalesManager::Customer* SalesManager::getCustomer(const std::string& customerId) {
    if (!hasPermission("view")) return nullptr;
    
    auto it = std::find_if(customers.begin(), customers.end(),
        [&customerId](const Customer& c) {
            return c.customerId == customerId;
        });
    
    if (it != customers.end()) {
        clearError();
        return &(*it);
    }
    
    setError(SalesError::NOT_FOUND, "Customer not found: " + customerId);
    return nullptr;
}

// Continue with more Customer and Booking methods...

std::vector<SalesManager::Customer> SalesManager::getAllCustomers() const {
    if (!hasPermission("view")) return {};
    
    clearError();
    return customers;
}

bool SalesManager::updateCustomer(const std::string& customerId, const Customer& updatedCustomer) {
    if (!hasPermission("edit")) return false;
    
    auto it = std::find_if(customers.begin(), customers.end(),
        [&customerId](const Customer& c) {
            return c.customerId == customerId;
        });
    
    if (it == customers.end()) {
        setError(SalesError::NOT_FOUND, "Customer not found: " + customerId);
        return false;
    }
    
    std::string errorMessage;
    if (!isValidCustomerData(updatedCustomer, errorMessage)) {
        setError(SalesError::INVALID_DATA, errorMessage);
        return false;
    }
    
    // Update customer data
    *it = updatedCustomer;
    it->customerId = customerId; // Ensure ID doesn't change
    
    saveDataToFiles();
    logOperation("Customer updated", customerId, 
                "Updated customer " + updatedCustomer.name);
    
    clearError();
    return true;
}

bool SalesManager::deleteCustomer(const std::string& customerId) {
    if (!hasPermission("delete")) return false;
    
    auto it = std::find_if(customers.begin(), customers.end(),
        [&customerId](const Customer& c) {
            return c.customerId == customerId;
        });
    
    if (it == customers.end()) {
        setError(SalesError::NOT_FOUND, "Customer not found: " + customerId);
        return false;
    }
    
    std::string details = "Deleted customer " + it->name + " (" + it->email + ")";
    customers.erase(it);
    
    saveDataToFiles();
    logOperation("Customer deleted", customerId, details);
    
    clearError();
    return true;
}

std::vector<SalesManager::Customer> SalesManager::searchCustomers(const std::string& searchTerm) const {
    if (!hasPermission("view")) return {};
    
    std::vector<Customer> results;
    std::string lowerSearchTerm = searchTerm;
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);
    
    for (const auto& customer : customers) {
        std::string customerData = customer.customerId + " " + customer.name + " " + 
                                  customer.email + " " + customer.phone + " " + customer.address;
        
        std::transform(customerData.begin(), customerData.end(), customerData.begin(), ::tolower);
        
        if (customerData.find(lowerSearchTerm) != std::string::npos) {
            results.push_back(customer);
        }
    }
    
    return results;
}

// Booking Management
std::string SalesManager::createBooking(const std::string& customerId, const std::string& vehicleId,
                                       const std::string& fromLocation, const std::string& toLocation,
                                       const std::chrono::system_clock::time_point& departureTime,
                                       int passengers, TripType tripType,
                                       const std::string& specialRequests) {
    if (!hasPermission("create")) return "";
    
    // Check if customer exists
    auto customer = getCustomer(customerId);
    if (!customer) {
        setError(SalesError::NOT_FOUND, "Customer not found: " + customerId);
        return "";
    }
    
    // Check if vehicle exists and is available
    if (!vehicleManager || !vehicleManager->getVehicle(vehicleId)) {
        setError(SalesError::NOT_FOUND, "Vehicle not found: " + vehicleId);
        return "";
    }
    
    // Estimate trip duration (simplified - 1 hour default)
    auto arrivalTime = departureTime + std::chrono::hours(1);
    
    // Check vehicle availability
    if (!isVehicleAvailable(vehicleId, departureTime, arrivalTime)) {
        setError(SalesError::VEHICLE_UNAVAILABLE, "Vehicle not available for selected time");
        return "";
    }
    
    // Create booking
    Booking booking;
    booking.bookingId = generateBookingId();
    booking.customerId = customerId;
    booking.vehicleId = vehicleId;
    booking.fromLocation = fromLocation;
    booking.toLocation = toLocation;
    booking.departureTime = departureTime;
    booking.arrivalTime = arrivalTime;
    booking.tripType = tripType;
    booking.passengers = passengers;
    booking.specialRequests = specialRequests;
    booking.status = BookingStatus::PENDING;
    booking.bookingDate = std::chrono::system_clock::now();
    booking.bookedBy = getCurrentUsername();
    
    // Calculate fare (simplified - 50km default distance)
    double distance = 50.0;
    booking.basefare = calculateBaseFare(vehicleId, distance);
    booking.discount = calculateDiscounts(*customer, booking.basefare);
    booking.totalFare = booking.basefare - booking.discount + calculateTaxes(booking.basefare);
    
    booking.paymentMethod = PaymentMethod::UNKNOWN; // To be set when confirmed
    
    // Validate booking data
    std::string errorMessage;
    if (!isValidBookingData(booking, errorMessage)) {
        setError(SalesError::INVALID_DATA, errorMessage);
        return "";
    }
    
    bookings.push_back(booking);
    saveDataToFiles();
    
    logOperation("Booking created", booking.bookingId, 
                "Created booking for customer " + customerId + 
                " from " + fromLocation + " to " + toLocation);
    
    clearError();
    return booking.bookingId;
}

SalesManager::Booking* SalesManager::getBooking(const std::string& bookingId) {
    if (!hasPermission("view")) return nullptr;
    
    auto it = std::find_if(bookings.begin(), bookings.end(),
        [&bookingId](const Booking& b) {
            return b.bookingId == bookingId;
        });
    
    if (it != bookings.end()) {
        clearError();
        return &(*it);
    }
    
    setError(SalesError::NOT_FOUND, "Booking not found: " + bookingId);
    return nullptr;
}

std::vector<SalesManager::Booking> SalesManager::getAllBookings() const {
    if (!hasPermission("view")) return {};
    
    clearError();
    return bookings;
}

std::vector<SalesManager::Booking> SalesManager::getCustomerBookings(const std::string& customerId) const {
    if (!hasPermission("view")) return {};
    
    std::vector<Booking> results;
    std::copy_if(bookings.begin(), bookings.end(), std::back_inserter(results),
        [&customerId](const Booking& b) {
            return b.customerId == customerId;
        });
    
    return results;
}

std::vector<SalesManager::Booking> SalesManager::getVehicleBookings(const std::string& vehicleId) const {
    if (!hasPermission("view")) return {};
    
    std::vector<Booking> results;
    std::copy_if(bookings.begin(), bookings.end(), std::back_inserter(results),
        [&vehicleId](const Booking& b) {
            return b.vehicleId == vehicleId;
        });
    
    return results;
}

bool SalesManager::updateBookingStatus(const std::string& bookingId, BookingStatus newStatus) {
    if (!hasPermission("edit")) return false;
    
    auto booking = getBooking(bookingId);
    if (!booking) return false;
    
    BookingStatus oldStatus = booking->status;
    booking->status = newStatus;
    
    saveDataToFiles();
    logOperation("Booking status updated", bookingId, 
                bookingStatusToString(oldStatus) + " -> " + bookingStatusToString(newStatus));
    
    return true;
}

bool SalesManager::cancelBooking(const std::string& bookingId, const std::string& reason) {
    if (!hasPermission("edit")) return false;
    
    auto booking = getBooking(bookingId);
    if (!booking) return false;
    
    if (booking->status == BookingStatus::COMPLETED) {
        setError(SalesError::INVALID_DATA, "Cannot cancel completed booking");
        return false;
    }
    
    booking->status = BookingStatus::CANCELLED;
    
    saveDataToFiles();
    logOperation("Booking cancelled", bookingId, 
                "Cancelled booking" + (reason.empty() ? "" : " - Reason: " + reason));
    
    return true;
}

bool SalesManager::confirmBooking(const std::string& bookingId, PaymentMethod paymentMethod) {
    if (!hasPermission("edit")) return false;
    
    auto booking = getBooking(bookingId);
    if (!booking) return false;
    
    if (booking->status != BookingStatus::PENDING) {
        setError(SalesError::INVALID_DATA, "Only pending bookings can be confirmed");
        return false;
    }
    
    booking->status = BookingStatus::CONFIRMED;
    booking->paymentMethod = paymentMethod;
    
    // Update customer statistics
    auto customer = getCustomer(booking->customerId);
    if (customer) {
        customer->totalBookings++;
        customer->totalSpent += booking->totalFare;
        
        // Check for VIP status (customers with >5 bookings and >$500 spent)
        if (customer->totalBookings > 5 && customer->totalSpent > 500.0) {
            customer->isVip = true;
        }
    }
    
    saveDataToFiles();
    logOperation("Booking confirmed", bookingId, 
                "Confirmed booking with payment method: " + paymentMethodToString(paymentMethod));
    
    return true;
}

// Trip Management
bool SalesManager::startTrip(const std::string& bookingId) {
    if (!hasPermission("edit")) return false;
    
    auto booking = getBooking(bookingId);
    if (!booking) return false;
    
    if (booking->status != BookingStatus::CONFIRMED) {
        setError(SalesError::INVALID_DATA, "Only confirmed bookings can be started");
        return false;
    }
    
    booking->status = BookingStatus::IN_PROGRESS;
    
    // Update vehicle status to in-service
    if (vehicleManager) {
        vehicleManager->updateVehicleStatus(booking->vehicleId, VehicleStatus::IN_SERVICE);
    }
    
    saveDataToFiles();
    logOperation("Trip started", bookingId, "Trip started for booking");
    
    return true;
}

bool SalesManager::completeTrip(const std::string& bookingId, double actualDistance) {
    if (!hasPermission("edit")) return false;
    
    auto booking = getBooking(bookingId);
    if (!booking) return false;
    
    if (booking->status != BookingStatus::IN_PROGRESS) {
        setError(SalesError::INVALID_DATA, "Only in-progress trips can be completed");
        return false;
    }
    
    booking->status = BookingStatus::COMPLETED;
    
    // Recalculate fare if actual distance provided
    if (actualDistance > 0) {
        booking->basefare = calculateBaseFare(booking->vehicleId, actualDistance);
        auto customer = getCustomer(booking->customerId);
        if (customer) {
            booking->discount = calculateDiscounts(*customer, booking->basefare);
        }
        booking->totalFare = booking->basefare - booking->discount + calculateTaxes(booking->basefare);
    }
    
    // Create sales record
    Sales sales;
    sales.setSalesId(generateSalesId());
    sales.setVehicleId(booking->vehicleId);
    sales.setCustomerId(booking->customerId);
    auto customer = getCustomer(booking->customerId);
    sales.setCustomerName(customer ? customer->name : "Unknown");
    sales.setTotalAmount(booking->totalFare);
    sales.setSaleDate(std::chrono::system_clock::now());
    sales.setPaymentMethod(paymentMethodToString(booking->paymentMethod));
    
    addSalesRecord(sales);
    
    // Update vehicle status back to available
    if (vehicleManager) {
        vehicleManager->updateVehicleStatus(booking->vehicleId, VehicleStatus::AVAILABLE);
    }
    
    saveDataToFiles();
    logOperation("Trip completed", bookingId, 
                "Trip completed, sales record created: " + sales.getSalesId());
    
    return true;
}

double SalesManager::calculateTripDuration(const Booking& booking) const {
    auto duration = booking.arrivalTime - booking.departureTime;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    return static_cast<double>(hours.count());
}

std::string SalesManager::generateTripReceipt(const std::string& bookingId) const {
    auto booking = getBooking(const_cast<SalesManager*>(this)->getBooking(bookingId));
    if (!booking) return "";
    
    auto customer = const_cast<SalesManager*>(this)->getCustomer(booking->customerId);
    
    std::stringstream receipt;
    receipt << "\n" << std::string(50, '=') << "\n";
    receipt << "           TOURMATE TRIP RECEIPT\n";
    receipt << std::string(50, '=') << "\n";
    receipt << "Booking ID: " << booking->bookingId << "\n";
    receipt << "Customer: " << (customer ? customer->name : "Unknown") << "\n";
    receipt << "Vehicle ID: " << booking->vehicleId << "\n";
    receipt << "Route: " << booking->fromLocation << " -> " << booking->toLocation << "\n";
    receipt << "Departure: " << timePointToString(booking->departureTime) << "\n";
    receipt << "Arrival: " << timePointToString(booking->arrivalTime) << "\n";
    receipt << "Passengers: " << booking->passengers << "\n";
    receipt << "Trip Type: " << tripTypeToString(booking->tripType) << "\n";
    receipt << std::string(50, '-') << "\n";
    receipt << "Base Fare: $" << std::fixed << std::setprecision(2) << booking->basefare << "\n";
    receipt << "Discount: -$" << booking->discount << "\n";
    receipt << "Tax: $" << calculateTaxes(booking->basefare) << "\n";
    receipt << "Total Fare: $" << booking->totalFare << "\n";
    receipt << "Payment Method: " << paymentMethodToString(booking->paymentMethod) << "\n";
    receipt << "Status: " << bookingStatusToString(booking->status) << "\n";
    receipt << std::string(50, '=') << "\n";
    receipt << "Thank you for choosing Tourmate!\n";
    receipt << std::string(50, '=') << "\n";
    
    return receipt.str();
}

// Continue with more methods...

// Search and Filter Operations
std::vector<std::shared_ptr<Sales>> SalesManager::searchSales(const std::string& searchTerm) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Sales>> results;
    std::string lowerSearchTerm = searchTerm;
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);
    
    for (const auto& sales : salesRecords) {
        std::string salesData = sales->getSalesId() + " " + 
                               sales->getVehicleId() + " " +
                               sales->getCustomerName() + " " +
                               sales->getCustomerId() + " " +
                               sales->getPaymentMethod();
        
        std::transform(salesData.begin(), salesData.end(), salesData.begin(), ::tolower);
        
        if (salesData.find(lowerSearchTerm) != std::string::npos) {
            results.push_back(sales);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Sales>> SalesManager::filterSalesByVehicle(const std::string& vehicleId) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Sales>> results;
    std::copy_if(salesRecords.begin(), salesRecords.end(), std::back_inserter(results),
        [&vehicleId](const std::shared_ptr<Sales>& s) {
            return s->getVehicleId() == vehicleId;
        });
    
    return results;
}

std::vector<std::shared_ptr<Sales>> SalesManager::filterSalesByCustomer(const std::string& customerId) const {
    if (!hasPermission("view")) return {};
    
    std::vector<std::shared_ptr<Sales>> results;
    std::copy_if(salesRecords.begin(), salesRecords.end(), std::back_inserter(results),
        [&customerId](const std::shared_ptr<Sales>& s) {
            return s->getCustomerId() == customerId;
        });
    
    return results;
}

std::vector<SalesManager::Booking> SalesManager::filterBookingsByStatus(BookingStatus status) const {
    if (!hasPermission("view")) return {};
    
    std::vector<Booking> results;
    std::copy_if(bookings.begin(), bookings.end(), std::back_inserter(results),
        [status](const Booking& b) {
            return b.status == status;
        });
    
    return results;
}

// Revenue Analytics
SalesManager::RevenueReport SalesManager::generateRevenueReport(
    const std::chrono::system_clock::time_point& startDate,
    const std::chrono::system_clock::time_point& endDate) const {
    
    if (!hasPermission("reports")) return {};
    
    RevenueReport report;
    
    // Process sales records
    for (const auto& sales : salesRecords) {
        if (sales->getSaleDate() >= startDate && sales->getSaleDate() <= endDate) {
            report.totalRevenue += sales->getTotalAmount();
            report.totalTransactions++;
        }
    }
    
    // Process bookings
    for (const auto& booking : bookings) {
        if (booking.bookingDate >= startDate && booking.bookingDate <= endDate) {
            report.totalBookings++;
            
            if (booking.status == BookingStatus::COMPLETED) {
                report.completedTrips++;
                report.revenueByVehicle[booking.vehicleId] += booking.totalFare;
                report.revenueByPaymentMethod[booking.paymentMethod] += booking.totalFare;
                
                std::string route = booking.fromLocation + " -> " + booking.toLocation;
                report.bookingsByRoute[route]++;
            } else if (booking.status == BookingStatus::CANCELLED) {
                report.cancelledBookings++;
            } else if (booking.status == BookingStatus::REFUNDED) {
                report.totalRefunds += booking.totalFare;
            }
        }
    }
    
    report.netRevenue = report.totalRevenue - report.totalRefunds;
    
    if (report.totalTransactions > 0) {
        report.averageTransactionValue = report.totalRevenue / report.totalTransactions;
    }
    
    // Find top performers
    if (!report.revenueByVehicle.empty()) {
        auto maxVehicle = std::max_element(report.revenueByVehicle.begin(), 
                                          report.revenueByVehicle.end(),
                                          [](const auto& a, const auto& b) { 
                                              return a.second < b.second; 
                                          });
        report.topVehicle = maxVehicle->first;
    }
    
    if (!report.bookingsByRoute.empty()) {
        auto maxRoute = std::max_element(report.bookingsByRoute.begin(), 
                                        report.bookingsByRoute.end(),
                                        [](const auto& a, const auto& b) { 
                                            return a.second < b.second; 
                                        });
        report.topRoute = maxRoute->first;
    }
    
    return report;
}

SalesManager::RevenueReport SalesManager::getDailyReport() const {
    auto now = std::chrono::system_clock::now();
    auto startOfDay = std::chrono::time_point_cast<std::chrono::days>(now);
    auto endOfDay = startOfDay + std::chrono::hours(24);
    return generateRevenueReport(startOfDay, endOfDay);
}

SalesManager::RevenueReport SalesManager::getWeeklyReport() const {
    auto now = std::chrono::system_clock::now();
    auto startOfWeek = now - std::chrono::hours(24 * 7);
    return generateRevenueReport(startOfWeek, now);
}

SalesManager::RevenueReport SalesManager::getMonthlyReport() const {
    auto now = std::chrono::system_clock::now();
    auto startOfMonth = now - std::chrono::hours(24 * 30);
    return generateRevenueReport(startOfMonth, now);
}

// Customer Analytics
SalesManager::CustomerAnalytics SalesManager::getCustomerAnalytics() const {
    if (!hasPermission("reports")) return {};
    
    CustomerAnalytics analytics;
    analytics.totalCustomers = static_cast<int>(customers.size());
    
    double totalCustomerValue = 0.0;
    
    for (const auto& customer : customers) {
        if (customer.totalBookings > 0) {
            analytics.activeCustomers++;
        }
        
        if (customer.isVip) {
            analytics.vipCustomers++;
        }
        
        totalCustomerValue += customer.totalSpent;
        
        // Extract city from address (simplified)
        std::string location = customer.address;
        if (location.find(',') != std::string::npos) {
            location = location.substr(location.find_last_of(',') + 1);
        }
        analytics.customersByLocation[location]++;
    }
    
    if (analytics.totalCustomers > 0) {
        analytics.averageCustomerValue = totalCustomerValue / analytics.totalCustomers;
    }
    
    // Calculate retention rate (simplified)
    if (analytics.totalCustomers > 0) {
        analytics.customerRetentionRate = static_cast<double>(analytics.activeCustomers) / analytics.totalCustomers * 100.0;
    }
    
    // Get top customers
    analytics.topCustomers = getTopCustomers(5);
    
    return analytics;
}

std::vector<SalesManager::Customer> SalesManager::getTopCustomers(int count) const {
    if (!hasPermission("view")) return {};
    
    auto customersCopy = customers;
    std::sort(customersCopy.begin(), customersCopy.end(),
        [](const Customer& a, const Customer& b) {
            return a.totalSpent > b.totalSpent;
        });
    
    if (customersCopy.size() > static_cast<size_t>(count)) {
        customersCopy.resize(count);
    }
    
    return customersCopy;
}

double SalesManager::getCustomerLifetimeValue(const std::string& customerId) const {
    if (!hasPermission("view")) return 0.0;
    
    auto customer = const_cast<SalesManager*>(this)->getCustomer(customerId);
    return customer ? customer->totalSpent : 0.0;
}

// Display Methods
void SalesManager::displaySalesRecord(const std::shared_ptr<Sales>& sales) const {
    if (!sales) return;
    
    std::cout << "\n┌─────────────────────────────────────────────┐\n";
    std::cout << "│               SALES RECORD                  │\n";
    std::cout << "├─────────────────────────────────────────────┤\n";
    std::cout << "│ Sales ID: " << std::left << std::setw(31) << sales->getSalesId() << "│\n";
    std::cout << "│ Vehicle ID: " << std::left << std::setw(29) << sales->getVehicleId() << "│\n";
    std::cout << "│ Customer: " << std::left << std::setw(31) << sales->getCustomerName() << "│\n";
    std::cout << "│ Customer ID: " << std::left << std::setw(28) << sales->getCustomerId() << "│\n";
    std::cout << "│ Amount: $" << std::left << std::setw(30) << std::fixed << std::setprecision(2) << sales->getTotalAmount() << "│\n";
    std::cout << "│ Date: " << std::left << std::setw(35) << timePointToString(sales->getSaleDate()) << "│\n";
    std::cout << "│ Payment: " << std::left << std::setw(32) << sales->getPaymentMethod() << "│\n";
    std::cout << "└─────────────────────────────────────────────┘\n";
}

void SalesManager::displaySalesRecordList(const std::vector<std::shared_ptr<Sales>>& salesList) const {
    if (salesList.empty()) {
        std::cout << "\nNo sales records found.\n";
        return;
    }
    
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << std::left << std::setw(10) << "Sales ID" 
              << std::setw(12) << "Vehicle ID"
              << std::setw(20) << "Customer"
              << std::setw(12) << "Amount"
              << std::setw(20) << "Date"
              << "Payment Method\n";
    std::cout << std::string(100, '-') << "\n";
    
    for (const auto& sales : salesList) {
        std::cout << std::left << std::setw(10) << sales->getSalesId()
                  << std::setw(12) << sales->getVehicleId()
                  << std::setw(20) << sales->getCustomerName().substr(0, 19)
                  << std::setw(12) << std::fixed << std::setprecision(2) << sales->getTotalAmount()
                  << std::setw(20) << timePointToString(sales->getSaleDate()).substr(0, 19)
                  << sales->getPaymentMethod() << "\n";
    }
    
    std::cout << std::string(100, '=') << "\n";
    std::cout << "Total records: " << salesList.size() << "\n";
}

void SalesManager::displayBooking(const Booking& booking) const {
    std::cout << "\n┌─────────────────────────────────────────────┐\n";
    std::cout << "│               BOOKING DETAILS               │\n";
    std::cout << "├─────────────────────────────────────────────┤\n";
    std::cout << "│ Booking ID: " << std::left << std::setw(29) << booking.bookingId << "│\n";
    std::cout << "│ Customer ID: " << std::left << std::setw(28) << booking.customerId << "│\n";
    std::cout << "│ Vehicle ID: " << std::left << std::setw(29) << booking.vehicleId << "│\n";
    std::cout << "│ Route: " << std::left << std::setw(35) << (booking.fromLocation + " -> " + booking.toLocation) << "│\n";
    std::cout << "│ Departure: " << std::left << std::setw(30) << timePointToString(booking.departureTime) << "│\n";
    std::cout << "│ Passengers: " << std::left << std::setw(29) << booking.passengers << "│\n";
    std::cout << "│ Total Fare: $" << std::left << std::setw(27) << std::fixed << std::setprecision(2) << booking.totalFare << "│\n";
    std::cout << "│ Status: " << std::left << std::setw(33) << bookingStatusToString(booking.status) << "│\n";
    std::cout << "│ Payment: " << std::left << std::setw(32) << paymentMethodToString(booking.paymentMethod) << "│\n";
    std::cout << "└─────────────────────────────────────────────┘\n";
}

void SalesManager::displayRevenueReport(const RevenueReport& report) const {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    REVENUE REPORT                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Total Revenue: $" << std::left << std::setw(38) << std::fixed << std::setprecision(2) << report.totalRevenue << "║\n";
    std::cout << "║ Total Refunds: $" << std::left << std::setw(38) << report.totalRefunds << "║\n";
    std::cout << "║ Net Revenue: $" << std::left << std::setw(40) << report.netRevenue << "║\n";
    std::cout << "║ Total Transactions: " << std::left << std::setw(33) << report.totalTransactions << "║\n";
    std::cout << "║ Total Bookings: " << std::left << std::setw(37) << report.totalBookings << "║\n";
    std::cout << "║ Completed Trips: " << std::left << std::setw(36) << report.completedTrips << "║\n";
    std::cout << "║ Cancelled Bookings: " << std::left << std::setw(33) << report.cancelledBookings << "║\n";
    std::cout << "║ Avg Transaction: $" << std::left << std::setw(35) << report.averageTransactionValue << "║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Top Vehicle: " << std::left << std::setw(41) << report.topVehicle << "║\n";
    std::cout << "║ Top Route: " << std::left << std::setw(43) << report.topRoute << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
}

// Additional file operations and utility methods
bool SalesManager::createBackup() const {
    // Implementation would create backup files
    return true;
}

bool SalesManager::restoreFromBackup() {
    // Implementation would restore from backup files
    return true;
}

std::vector<std::string> SalesManager::getAuditLog() const {
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

bool SalesManager::clearAuditLog() {
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

std::string SalesManager::getNextAvailableSalesId() const {
    return generateSalesId();
}

std::string SalesManager::getNextAvailableBookingId() const {
    return generateBookingId();
}

std::string SalesManager::getNextAvailableCustomerId() const {
    return generateCustomerId();
}

// Error handling
void SalesManager::setError(SalesError error, const std::string& message) const {
    lastError = error;
    lastErrorMessage = message;
}

void SalesManager::clearError() const {
    lastError = SalesError::NONE;
    lastErrorMessage.clear();
}

// Helper Methods for Enums
std::string SalesManager::paymentMethodToString(PaymentMethod method) {
    switch (method) {
        case PaymentMethod::CASH: return "Cash";
        case PaymentMethod::CREDIT_CARD: return "Credit Card";
        case PaymentMethod::DEBIT_CARD: return "Debit Card";
        case PaymentMethod::DIGITAL_WALLET: return "Digital Wallet";
        case PaymentMethod::BANK_TRANSFER: return "Bank Transfer";
        default: return "Unknown";
    }
}

SalesManager::PaymentMethod SalesManager::stringToPaymentMethod(const std::string& str) {
    if (str == "Cash") return PaymentMethod::CASH;
    if (str == "Credit Card") return PaymentMethod::CREDIT_CARD;
    if (str == "Debit Card") return PaymentMethod::DEBIT_CARD;
    if (str == "Digital Wallet") return PaymentMethod::DIGITAL_WALLET;
    if (str == "Bank Transfer") return PaymentMethod::BANK_TRANSFER;
    return PaymentMethod::UNKNOWN;
}

std::string SalesManager::bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::PENDING: return "Pending";
        case BookingStatus::CONFIRMED: return "Confirmed";
        case BookingStatus::IN_PROGRESS: return "In Progress";
        case BookingStatus::COMPLETED: return "Completed";
        case BookingStatus::CANCELLED: return "Cancelled";
        case BookingStatus::REFUNDED: return "Refunded";
        default: return "Unknown";
    }
}

SalesManager::BookingStatus SalesManager::stringToBookingStatus(const std::string& str) {
    if (str == "Pending") return BookingStatus::PENDING;
    if (str == "Confirmed") return BookingStatus::CONFIRMED;
    if (str == "In Progress") return BookingStatus::IN_PROGRESS;
    if (str == "Completed") return BookingStatus::COMPLETED;
    if (str == "Cancelled") return BookingStatus::CANCELLED;
    if (str == "Refunded") return BookingStatus::REFUNDED;
    return BookingStatus::UNKNOWN;
}

std::string SalesManager::tripTypeToString(TripType type) {
    switch (type) {
        case TripType::ONE_WAY: return "One Way";
        case TripType::ROUND_TRIP: return "Round Trip";
        case TripType::MULTI_STOP: return "Multi Stop";
        case TripType::CHARTER: return "Charter";
        default: return "Unknown";
    }
}

SalesManager::TripType SalesManager::stringToTripType(const std::string& str) {
    if (str == "One Way") return TripType::ONE_WAY;
    if (str == "Round Trip") return TripType::ROUND_TRIP;
    if (str == "Multi Stop") return TripType::MULTI_STOP;
    if (str == "Charter") return TripType::CHARTER;
    return TripType::UNKNOWN;
}

// Basic utility methods (simplified for space)
int SalesManager::getSalesCount() const {
    return static_cast<int>(salesRecords.size());
}

int SalesManager::getBookingCount() const {
    return static_cast<int>(bookings.size());
}

int SalesManager::getCustomerCount() const {
    return static_cast<int>(customers.size());
}

bool SalesManager::salesRecordExists(const std::string& salesId) const {
    return std::any_of(salesRecords.begin(), salesRecords.end(),
        [&salesId](const std::shared_ptr<Sales>& s) {
            return s->getSalesId() == salesId;
        });
}

bool SalesManager::bookingExists(const std::string& bookingId) const {
    return std::any_of(bookings.begin(), bookings.end(),
        [&bookingId](const Booking& b) {
            return b.bookingId == bookingId;
        });
}

bool SalesManager::customerExists(const std::string& customerId) const {
    return std::any_of(customers.begin(), customers.end(),
        [&customerId](const Customer& c) {
            return c.customerId == customerId;
        });
}

SalesManager::Customer* SalesManager::findCustomerByEmail(const std::string& email) {
    auto it = std::find_if(customers.begin(), customers.end(),
        [&email](const Customer& c) {
            return c.email == email;
        });
    return (it != customers.end()) ? &(*it) : nullptr;
}

SalesManager::Customer* SalesManager::findCustomerByPhone(const std::string& phone) {
    auto it = std::find_if(customers.begin(), customers.end(),
        [&phone](const Customer& c) {
            return c.phone == phone;
        });
    return (it != customers.end()) ? &(*it) : nullptr;
}

// Authentication Integration
bool SalesManager::canCreateSales() const {
    return hasPermission("create");
}

bool SalesManager::canEditSales() const {
    return hasPermission("edit");
}

bool SalesManager::canDeleteSales() const {
    return hasPermission("delete");
}

bool SalesManager::canViewSales() const {
    return hasPermission("view");
}

bool SalesManager::canProcessRefunds() const {
    return hasPermission("refund");
}

bool SalesManager::canViewReports() const {
    return hasPermission("reports");
}

std::string SalesManager::getCurrentUserRole() const {
    if (!authManager || !authManager->isLoggedIn()) return "Not logged in";
    
    auto user = authManager->getCurrentUser();
    return user ? Utilities::roleToString(user->getRole()) : "Unknown";
}

std::string SalesManager::getCurrentUsername() const {
    if (!authManager || !authManager->isLoggedIn()) return "Not logged in";
    
    auto user = authManager->getCurrentUser();
    return user ? user->getUsername() : "Unknown";
}