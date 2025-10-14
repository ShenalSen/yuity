#ifndef SALES_H
#define SALES_H

#include <string>
#include <vector>

using namespace std;

class Sales {
private:
    string saleId;
    string vehicleId;
    string customerName;
    string customerContact;
    string startDate;
    string endDate;
    double amount;
    string paymentStatus; // Paid, Pending, etc.

public:
    // Constructor
    Sales(string sId, string vId, string custName, string custContact,
          string start, string end, double amt, string status);
    
    // Default constructor
    Sales();
    
    // Getters
    string getSaleId() const;
    string getVehicleId() const;
    string getCustomerName() const;
    string getCustomerContact() const;
    string getStartDate() const;
    string getEndDate() const;
    double getAmount() const;
    string getPaymentStatus() const;
    
    // Setters
    void setSaleId(string sId);
    void setVehicleId(string vId);
    void setCustomerName(string custName);
    void setCustomerContact(string custContact);
    void setStartDate(string start);
    void setEndDate(string end);
    void setAmount(double amt);
    void setPaymentStatus(string status);
    
    // Display sales details
    void displayDetails() const;
    
    // String representation for file storage
    string toString() const;
    
    // Create sales from string (read from file)
    static Sales fromString(string str);
};

// Function prototypes for sales management
void viewAllSales();
void addSale();
void searchSales();
void generateSalesReport();
vector<Sales> loadSalesFromFile();
void saveSalesToFile(const vector<Sales>& sales);

#endif // SALES_H