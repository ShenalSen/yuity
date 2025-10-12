#include "Sales.h"
#include "Utilities.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

// Static constants definition
const std::vector<std::string> Sales::VALID_PAYMENT_STATUSES = {"Pending", "Paid", "Cancelled", "Refunded"};
const double Sales::BASE_FARE = 50.0;
const double Sales::PER_KM_RATE = 2.5;
const double Sales::PER_MINUTE_RATE = 1.0;

// Default constructor
Sales::Sales() 
    : salesId_(0), vehicleId_(0), customerName_(""), customerContact_(""),
      startLocation_(""), endLocation_(""), tripDate_(""), tripTime_(""),
      duration_(0), fareAmount_(0.0), paymentStatus_("Pending"), salesRep_("") {
}

// Parameterized constructor
Sales::Sales(int salesId, int vehicleId, const std::string& customerName,
            const std::string& customerContact, const std::string& startLocation,
            const std::string& endLocation, const std::string& tripDate,
            const std::string& tripTime, int duration, double fareAmount,
            const std::string& paymentStatus, const std::string& salesRep)
    : salesId_(salesId), vehicleId_(vehicleId), customerName_(customerName),
      customerContact_(customerContact), startLocation_(startLocation),
      endLocation_(endLocation), tripDate_(tripDate), tripTime_(tripTime),
      duration_(duration), fareAmount_(fareAmount), paymentStatus_(paymentStatus),
      salesRep_(salesRep) {
}

// Destructor
Sales::~Sales() {
    // No dynamic memory to clean up
}

// Getters implementation
int Sales::getSalesId() const { return salesId_; }
int Sales::getVehicleId() const { return vehicleId_; }
std::string Sales::getCustomerName() const { return customerName_; }
std::string Sales::getCustomerContact() const { return customerContact_; }
std::string Sales::getStartLocation() const { return startLocation_; }
std::string Sales::getEndLocation() const { return endLocation_; }
std::string Sales::getTripDate() const { return tripDate_; }
std::string Sales::getTripTime() const { return tripTime_; }
int Sales::getDuration() const { return duration_; }
double Sales::getFareAmount() const { return fareAmount_; }
std::string Sales::getPaymentStatus() const { return paymentStatus_; }
std::string Sales::getSalesRep() const { return salesRep_; }

// Setters implementation
void Sales::setSalesId(int id) { salesId_ = id; }
void Sales::setVehicleId(int vehicleId) { vehicleId_ = vehicleId; }
void Sales::setCustomerName(const std::string& name) { customerName_ = name; }
void Sales::setCustomerContact(const std::string& contact) { customerContact_ = contact; }
void Sales::setStartLocation(const std::string& location) { startLocation_ = location; }
void Sales::setEndLocation(const std::string& location) { endLocation_ = location; }
void Sales::setTripDate(const std::string& date) { tripDate_ = date; }
void Sales::setTripTime(const std::string& time) { tripTime_ = time; }
void Sales::setDuration(int duration) { duration_ = duration; }
void Sales::setFareAmount(double amount) { fareAmount_ = amount; }
void Sales::setPaymentStatus(const std::string& status) { paymentStatus_ = status; }
void Sales::setSalesRep(const std::string& rep) { salesRep_ = rep; }

// Validation methods
bool Sales::isValidContact(const std::string& contact) const {
    if (contact.empty()) return false;
    
    // Check for valid phone number format (digits, +, -, spaces, parentheses)
    for (char c : contact) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != ' ' && c != '(' && c != ')') {
            return false;
        }
    }
    return contact.length() >= 10 && contact.length() <= 15;
}

bool Sales::isValidDate(const std::string& date) const {
    return TourmateUtils::isValidDate(date);
}

bool Sales::isValidTime(const std::string& time) const {
    return TourmateUtils::isValidTime(time);
}

bool Sales::isValidPaymentStatus(const std::string& status) const {
    return std::find(VALID_PAYMENT_STATUSES.begin(), VALID_PAYMENT_STATUSES.end(), status) 
           != VALID_PAYMENT_STATUSES.end();
}

bool Sales::isValidDuration(int duration) const {
    return duration > 0 && duration <= 1440; // Max 24 hours in minutes
}

bool Sales::isValidFareAmount(double amount) const {
    return amount >= 0.0;
}

bool Sales::validateSalesData() const {
    return salesId_ > 0 &&
           vehicleId_ > 0 &&
           !TourmateUtils::isEmpty(customerName_) &&
           !TourmateUtils::isEmpty(customerContact_) &&
           !TourmateUtils::isEmpty(startLocation_) &&
           !TourmateUtils::isEmpty(endLocation_) &&
           !TourmateUtils::isEmpty(tripDate_) &&
           !TourmateUtils::isEmpty(tripTime_) &&
           isValidContact(customerContact_) &&
           isValidDate(tripDate_) &&
           isValidTime(tripTime_) &&
           isValidPaymentStatus(paymentStatus_) &&
           isValidDuration(duration_) &&
           isValidFareAmount(fareAmount_);
}

// Fare calculation methods
double Sales::calculateBaseFare() const {
    return BASE_FARE;
}

double Sales::calculateTimeFare() const {
    return duration_ * PER_MINUTE_RATE;
}

double Sales::calculateDistanceFare(double distance) const {
    return distance * PER_KM_RATE;
}

double Sales::calculateTotalFare(double distance) const {
    return calculateBaseFare() + calculateTimeFare() + calculateDistanceFare(distance);
}

void Sales::updateFareAmount(double distance) {
    fareAmount_ = calculateTotalFare(distance);
}

// Payment status management
bool Sales::isPending() const { return paymentStatus_ == "Pending"; }
bool Sales::isPaid() const { return paymentStatus_ == "Paid"; }
bool Sales::isCancelled() const { return paymentStatus_ == "Cancelled"; }
bool Sales::isRefunded() const { return paymentStatus_ == "Refunded"; }

void Sales::markAsPaid() { paymentStatus_ = "Paid"; }
void Sales::markAsCancelled() { paymentStatus_ = "Cancelled"; }
void Sales::markAsRefunded() { paymentStatus_ = "Refunded"; }
void Sales::markAsPending() { paymentStatus_ = "Pending"; }

// Utility methods
void Sales::displaySales() const {
    std::cout << "==================================================\n";
    std::cout << "                 SALES RECORD\n";
    std::cout << "==================================================\n";
    std::cout << "Sales ID         : " << salesId_ << "\n";
    std::cout << "Vehicle ID       : " << vehicleId_ << "\n";
    std::cout << "Customer Name    : " << customerName_ << "\n";
    std::cout << "Customer Contact : " << customerContact_ << "\n";
    std::cout << "Start Location   : " << startLocation_ << "\n";
    std::cout << "End Location     : " << endLocation_ << "\n";
    std::cout << "Trip Date        : " << tripDate_ << "\n";
    std::cout << "Trip Time        : " << tripTime_ << "\n";
    std::cout << "Duration (mins)  : " << duration_ << "\n";
    std::cout << "Fare Amount      : $" << std::fixed << std::setprecision(2) << fareAmount_ << "\n";
    std::cout << "Payment Status   : " << paymentStatus_ << "\n";
    std::cout << "Sales Rep        : " << salesRep_ << "\n";
    std::cout << "==================================================\n";
}

std::string Sales::toString() const {
    std::ostringstream oss;
    oss << "Sales[ID=" << salesId_ 
        << ", VehicleID=" << vehicleId_
        << ", Customer=" << customerName_
        << ", Contact=" << customerContact_
        << ", From=" << startLocation_
        << ", To=" << endLocation_
        << ", Date=" << tripDate_
        << ", Time=" << tripTime_
        << ", Duration=" << duration_
        << ", Fare=$" << std::fixed << std::setprecision(2) << fareAmount_
        << ", Status=" << paymentStatus_
        << ", Rep=" << salesRep_ << "]";
    return oss.str();
}

std::string Sales::toCSV() const {
    std::vector<std::string> fields = {
        TourmateUtils::intToString(salesId_),
        TourmateUtils::intToString(vehicleId_),
        customerName_,
        customerContact_,
        startLocation_,
        endLocation_,
        tripDate_,
        tripTime_,
        TourmateUtils::intToString(duration_),
        TourmateUtils::doubleToString(fareAmount_),
        paymentStatus_,
        salesRep_
    };
    return TourmateUtils::createCSVLine(fields);
}

bool Sales::fromCSV(const std::string& csvLine) {
    std::vector<std::string> fields = TourmateUtils::parseCSVLine(csvLine);
    
    if (fields.size() != 12) {
        return false;
    }
    
    try {
        salesId_ = TourmateUtils::stringToInt(fields[0]);
        vehicleId_ = TourmateUtils::stringToInt(fields[1]);
        customerName_ = TourmateUtils::trim(fields[2]);
        customerContact_ = TourmateUtils::trim(fields[3]);
        startLocation_ = TourmateUtils::trim(fields[4]);
        endLocation_ = TourmateUtils::trim(fields[5]);
        tripDate_ = TourmateUtils::trim(fields[6]);
        tripTime_ = TourmateUtils::trim(fields[7]);
        duration_ = TourmateUtils::stringToInt(fields[8]);
        fareAmount_ = TourmateUtils::stringToDouble(fields[9]);
        paymentStatus_ = TourmateUtils::trim(fields[10]);
        salesRep_ = TourmateUtils::trim(fields[11]);
        
        return validateSalesData();
    } catch (...) {
        return false;
    }
}

// Comparison operators
bool Sales::operator==(const Sales& other) const {
    return salesId_ == other.salesId_;
}

bool Sales::operator!=(const Sales& other) const {
    return !(*this == other);
}

bool Sales::operator<(const Sales& other) const {
    return salesId_ < other.salesId_;
}

// Stream operators
std::ostream& operator<<(std::ostream& os, const Sales& sales) {
    os << sales.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Sales& sales) {
    std::string line;
    std::getline(is, line);
    sales.fromCSV(line);
    return is;
}