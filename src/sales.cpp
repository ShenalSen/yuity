#include "sales.h"
#include "vehicle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <ctime>

using namespace std;

// Constructor
Sales::Sales(string sId, string vId, string custName, string custContact,
             string start, string end, double amt, string status) {
    saleId = sId;
    vehicleId = vId;
    customerName = custName;
    customerContact = custContact;
    startDate = start;
    endDate = end;
    amount = amt;
    paymentStatus = status;
}

// Default constructor
Sales::Sales() {
    saleId = "";
    vehicleId = "";
    customerName = "";
    customerContact = "";
    startDate = "";
    endDate = "";
    amount = 0.0;
    paymentStatus = "Pending";
}

// Getters
string Sales::getSaleId() const {
    return saleId;
}

string Sales::getVehicleId() const {
    return vehicleId;
}

string Sales::getCustomerName() const {
    return customerName;
}

string Sales::getCustomerContact() const {
    return customerContact;
}

string Sales::getStartDate() const {
    return startDate;
}

string Sales::getEndDate() const {
    return endDate;
}

double Sales::getAmount() const {
    return amount;
}

string Sales::getPaymentStatus() const {
    return paymentStatus;
}

// Setters
void Sales::setSaleId(string sId) {
    saleId = sId;
}

void Sales::setVehicleId(string vId) {
    vehicleId = vId;
}

void Sales::setCustomerName(string custName) {
    customerName = custName;
}

void Sales::setCustomerContact(string custContact) {
    customerContact = custContact;
}

void Sales::setStartDate(string start) {
    startDate = start;
}

void Sales::setEndDate(string end) {
    endDate = end;
}

void Sales::setAmount(double amt) {
    amount = amt;
}

void Sales::setPaymentStatus(string status) {
    paymentStatus = status;
}

// Display sales details
void Sales::displayDetails() const {
    cout << "Sale ID: " << saleId << endl;
    cout << "Vehicle ID: " << vehicleId << endl;
    cout << "Customer Name: " << customerName << endl;
    cout << "Customer Contact: " << customerContact << endl;
    cout << "Start Date: " << startDate << endl;
    cout << "End Date: " << endDate << endl;
    cout << "Amount: $" << fixed << setprecision(2) << amount << endl;
    cout << "Payment Status: " << paymentStatus << endl;
}

// String representation for file storage
string Sales::toString() const {
    ostringstream oss;
    oss << saleId << "|" << vehicleId << "|" << customerName << "|" 
        << customerContact << "|" << startDate << "|" << endDate << "|" 
        << amount << "|" << paymentStatus;
    return oss.str();
}

// Create sales from string (read from file)
Sales Sales::fromString(string str) {
    Sales sale;
    stringstream ss(str);
    string token;
    
    // Parse sale ID
    if (getline(ss, token, '|')) {
        sale.setSaleId(token);
    }
    
    // Parse vehicle ID
    if (getline(ss, token, '|')) {
        sale.setVehicleId(token);
    }
    
    // Parse customer name
    if (getline(ss, token, '|')) {
        sale.setCustomerName(token);
    }
    
    // Parse customer contact
    if (getline(ss, token, '|')) {
        sale.setCustomerContact(token);
    }
    
    // Parse start date
    if (getline(ss, token, '|')) {
        sale.setStartDate(token);
    }
    
    // Parse end date
    if (getline(ss, token, '|')) {
        sale.setEndDate(token);
    }
    
    // Parse amount
    if (getline(ss, token, '|')) {
        sale.setAmount(stod(token));
    }
    
    // Parse payment status
    if (getline(ss, token, '|')) {
        sale.setPaymentStatus(token);
    }
    
    return sale;
}

// Load sales from file
vector<Sales> loadSalesFromFile() {
    vector<Sales> sales;
    ifstream file("sales.txt");
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                Sales sale = Sales::fromString(line);
                sales.push_back(sale);
            }
        }
        file.close();
    } else {
        cout << "Warning: Could not open sales.txt. A new file will be created when sales are added." << endl;
    }
    
    return sales;
}

// Save sales to file
void saveSalesToFile(const vector<Sales>& sales) {
    ofstream file("sales.txt");
    
    if (file.is_open()) {
        for (const auto& sale : sales) {
            file << sale.toString() << endl;
        }
        file.close();
    } else {
        cout << "Error: Could not open sales.txt for writing." << endl;
    }
}

// View all sales
void viewAllSales() {
    vector<Sales> sales = loadSalesFromFile();
    
    if (sales.empty()) {
        cout << "No sales found in the system." << endl;
        return;
    }
    
    cout << "\n===== SALES LIST =====\n";
    cout << "Total sales: " << sales.size() << endl << endl;
    
    for (size_t i = 0; i < sales.size(); i++) {
        cout << "Sale #" << (i + 1) << ":" << endl;
        sales[i].displayDetails();
        cout << "------------------------" << endl;
    }
}

// Add a new sale
void addSale() {
    vector<Sales> sales = loadSalesFromFile();
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    Sales newSale;
    string input;
    double amountInput;
    
    if (vehicles.empty()) {
        cout << "No vehicles found in the system. Cannot add a sale." << endl;
        return;
    }
    
    cout << "\n===== ADD NEW SALE =====\n";
    
    // Generate a sale ID
    string saleId = "S" + to_string(sales.size() + 1);
    newSale.setSaleId(saleId);
    
    // Show available vehicles
    cout << "Available vehicles:" << endl;
    bool availableVehiclesExist = false;
    
    for (const auto& vehicle : vehicles) {
        if (vehicle.getStatus() == "Available") {
            cout << "Vehicle ID: " << vehicle.getVehicleId() 
                << " - " << vehicle.getMakeModel() 
                << " (" << vehicle.getRegistrationNumber() << ")" << endl;
            availableVehiclesExist = true;
        }
    }
    
    if (!availableVehiclesExist) {
        cout << "No available vehicles found. Cannot add a sale." << endl;
        return;
    }
    
    // Get sale details from user
    cout << "\nEnter Vehicle ID: ";
    getline(cin, input);
    newSale.setVehicleId(input);
    
    // Check if vehicle exists and is available
    auto it = find_if(vehicles.begin(), vehicles.end(),
                     [&input](const Vehicle& v) { return v.getVehicleId() == input; });
    
    if (it == vehicles.end()) {
        cout << "Vehicle not found with ID: " << input << endl;
        return;
    }
    
    if (it->getStatus() != "Available") {
        cout << "Vehicle is not available for sale." << endl;
        return;
    }
    
    cout << "Customer Name: ";
    getline(cin, input);
    newSale.setCustomerName(input);
    
    cout << "Customer Contact: ";
    getline(cin, input);
    newSale.setCustomerContact(input);
    
    cout << "Start Date (YYYY-MM-DD): ";
    getline(cin, input);
    newSale.setStartDate(input);
    
    cout << "End Date (YYYY-MM-DD): ";
    getline(cin, input);
    newSale.setEndDate(input);
    
    cout << "Amount ($): ";
    cin >> amountInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newSale.setAmount(amountInput);
    
    cout << "Payment Status (Paid, Pending): ";
    getline(cin, input);
    newSale.setPaymentStatus(input);
    
    // Add the new sale to the vector
    sales.push_back(newSale);
    
    // Update vehicle status to 'Rented'
    it->setStatus("Rented");
    saveVehiclesToFile(vehicles);
    
    // Save the updated sales to file
    saveSalesToFile(sales);
    
    cout << "\nSale added successfully with ID: " << saleId << endl;
}

// Search for sales
void searchSales() {
    vector<Sales> sales = loadSalesFromFile();
    
    if (sales.empty()) {
        cout << "No sales found in the system." << endl;
        return;
    }
    
    int searchOption;
    string searchTerm;
    bool found = false;
    
    cout << "\n===== SEARCH SALES =====\n";
    cout << "Search by:\n";
    cout << "1. Sale ID\n";
    cout << "2. Vehicle ID\n";
    cout << "3. Customer Name\n";
    cout << "4. Payment Status\n";
    cout << "Enter your choice: ";
    
    cin >> searchOption;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter search term: ";
    getline(cin, searchTerm);
    
    cout << "\nSearch Results:\n";
    
    for (const auto& sale : sales) {
        bool match = false;
        
        switch (searchOption) {
            case 1: // Sale ID
                match = sale.getSaleId() == searchTerm;
                break;
            case 2: // Vehicle ID
                match = sale.getVehicleId() == searchTerm;
                break;
            case 3: // Customer Name
                match = sale.getCustomerName().find(searchTerm) != string::npos;
                break;
            case 4: // Payment Status
                match = sale.getPaymentStatus() == searchTerm;
                break;
            default:
                cout << "Invalid search option." << endl;
                return;
        }
        
        if (match) {
            found = true;
            cout << "------------------------" << endl;
            sale.displayDetails();
        }
    }
    
    if (!found) {
        cout << "No matching sales found." << endl;
    }
}

// Generate a sales report
void generateSalesReport() {
    vector<Sales> sales = loadSalesFromFile();
    
    if (sales.empty()) {
        cout << "No sales data available for report generation." << endl;
        return;
    }
    
    cout << "\n===== SALES REPORT =====\n";
    
    // Calculate total sales amount
    double totalAmount = 0.0;
    int paidCount = 0;
    int pendingCount = 0;
    
    for (const auto& sale : sales) {
        totalAmount += sale.getAmount();
        
        if (sale.getPaymentStatus() == "Paid") {
            paidCount++;
        } else if (sale.getPaymentStatus() == "Pending") {
            pendingCount++;
        }
    }
    
    // Display report
    cout << "Total Number of Sales: " << sales.size() << endl;
    cout << "Total Sales Amount: $" << fixed << setprecision(2) << totalAmount << endl;
    cout << "Paid Sales: " << paidCount << endl;
    cout << "Pending Payments: " << pendingCount << endl;
    
    // Calculate amount received and pending
    double paidAmount = 0.0;
    double pendingAmount = 0.0;
    
    for (const auto& sale : sales) {
        if (sale.getPaymentStatus() == "Paid") {
            paidAmount += sale.getAmount();
        } else if (sale.getPaymentStatus() == "Pending") {
            pendingAmount += sale.getAmount();
        }
    }
    
    cout << "Total Amount Received: $" << fixed << setprecision(2) << paidAmount << endl;
    cout << "Total Amount Pending: $" << fixed << setprecision(2) << pendingAmount << endl;
    
    // Export report to a file
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    string filename = "sales_report_" + 
                      to_string(1900 + ltm->tm_year) + "-" +
                      to_string(1 + ltm->tm_mon) + "-" +
                      to_string(ltm->tm_mday) + ".txt";
    
    ofstream reportFile(filename);
    
    if (reportFile.is_open()) {
        reportFile << "TOUR MATE - SALES REPORT\n";
        reportFile << "Date: " << (1900 + ltm->tm_year) << "-" << (1 + ltm->tm_mon) << "-" << ltm->tm_mday << "\n\n";
        
        reportFile << "Total Number of Sales: " << sales.size() << "\n";
        reportFile << "Total Sales Amount: $" << fixed << setprecision(2) << totalAmount << "\n";
        reportFile << "Paid Sales: " << paidCount << "\n";
        reportFile << "Pending Payments: " << pendingCount << "\n";
        reportFile << "Total Amount Received: $" << fixed << setprecision(2) << paidAmount << "\n";
        reportFile << "Total Amount Pending: $" << fixed << setprecision(2) << pendingAmount << "\n\n";
        
        reportFile << "DETAILED SALES:\n";
        for (const auto& sale : sales) {
            reportFile << "------------------------\n";
            reportFile << "Sale ID: " << sale.getSaleId() << "\n";
            reportFile << "Vehicle ID: " << sale.getVehicleId() << "\n";
            reportFile << "Customer: " << sale.getCustomerName() << "\n";
            reportFile << "Amount: $" << fixed << setprecision(2) << sale.getAmount() << "\n";
            reportFile << "Status: " << sale.getPaymentStatus() << "\n";
        }
        
        reportFile.close();
        cout << "\nReport exported to " << filename << endl;
    } else {
        cout << "Error: Could not create report file." << endl;
    }
}