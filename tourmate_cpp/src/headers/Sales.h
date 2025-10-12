#ifndef SALES_H
#define SALES_H

#include <string>
#include <iostream>
#include <vector>

/**
 * @brief Sales class representing a sales transaction in the Tourmate system
 * 
 * This class encapsulates all sales-related data and provides
 * methods for sales manipulation and validation.
 */
class Sales {
private:
    int salesId_;
    int vehicleId_;
    std::string customerName_;
    std::string customerContact_;
    std::string startLocation_;
    std::string endLocation_;
    std::string tripDate_;
    std::string tripTime_;
    int duration_;
    double fareAmount_;
    std::string paymentStatus_;
    std::string salesRep_;

public:
    // Constructors
    Sales();
    Sales(int salesId, int vehicleId, const std::string& customerName,
          const std::string& customerContact, const std::string& startLocation,
          const std::string& endLocation, const std::string& tripDate,
          const std::string& tripTime, int duration, double fareAmount,
          const std::string& paymentStatus, const std::string& salesRep);

    // Destructor
    ~Sales();

    // Getters
    int getSalesId() const;
    int getVehicleId() const;
    std::string getCustomerName() const;
    std::string getCustomerContact() const;
    std::string getStartLocation() const;
    std::string getEndLocation() const;
    std::string getTripDate() const;
    std::string getTripTime() const;
    int getDuration() const;
    double getFareAmount() const;
    std::string getPaymentStatus() const;
    std::string getSalesRep() const;

    // Setters
    void setSalesId(int id);
    void setVehicleId(int vehicleId);
    void setCustomerName(const std::string& name);
    void setCustomerContact(const std::string& contact);
    void setStartLocation(const std::string& location);
    void setEndLocation(const std::string& location);
    void setTripDate(const std::string& date);
    void setTripTime(const std::string& time);
    void setDuration(int duration);
    void setFareAmount(double amount);
    void setPaymentStatus(const std::string& status);
    void setSalesRep(const std::string& rep);

    // Validation methods
    bool isValidCustomerName(const std::string& name) const;
    bool isValidContact(const std::string& contact) const;
    bool isValidLocation(const std::string& location) const;
    bool isValidDate(const std::string& date) const;
    bool isValidTime(const std::string& time) const;
    bool isValidDuration(int duration) const;
    bool isValidFareAmount(double amount) const;
    bool isValidPaymentStatus(const std::string& status) const;
    bool validateSalesData() const;

    // Calculation methods
    double calculateBaseFare() const;
    double calculateTimeFare() const;
    double calculateDistanceFare(double distance) const;
    double calculateTotalFare(double distance) const;
    void updateFareAmount(double distance);

    // Utility methods
    void displaySales() const;
    std::string toString() const;
    std::string toCSV() const;
    bool fromCSV(const std::string& csvLine);

    // Payment status management
    bool isPending() const;
    bool isPaid() const;
    bool isCancelled() const;
    bool isRefunded() const;
    void markAsPending();
    void markAsPaid();
    void markAsCancelled();
    void markAsRefunded();

    // Comparison operators
    bool operator==(const Sales& other) const;
    bool operator!=(const Sales& other) const;
    bool operator<(const Sales& other) const;

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Sales& sales);
    friend std::istream& operator>>(std::istream& is, Sales& sales);

    // Static constants
    static const std::vector<std::string> VALID_PAYMENT_STATUSES;
    static const double BASE_FARE;
    static const double PER_KM_RATE;
    static const double PER_MINUTE_RATE;
};

#endif // SALES_H