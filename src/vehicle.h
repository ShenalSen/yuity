#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

using namespace std;

class Vehicle {
private:
    string vehicleId;
    string makeModel;
    int year;
    string type;
    string registrationNumber;
    string status; // Available, In maintenance, etc.
    double ratePerDay;

public:
    // Constructor
    Vehicle(string id, string make, int yr, string tp, string reg, string st, double rate);
    
    // Default constructor
    Vehicle();
    
    // Getters
    string getVehicleId() const;
    string getMakeModel() const;
    int getYear() const;
    string getType() const;
    string getRegistrationNumber() const;
    string getStatus() const;
    double getRatePerDay() const;
    
    // Setters
    void setVehicleId(string id);
    void setMakeModel(string make);
    void setYear(int yr);
    void setType(string tp);
    void setRegistrationNumber(string reg);
    void setStatus(string st);
    void setRatePerDay(double rate);
    
    // Display vehicle details
    void displayDetails() const;
    
    // String representation for file storage
    string toString() const;
    
    // Create vehicle from string (read from file)
    static Vehicle fromString(string str);
};

// Function prototypes for vehicle management
void viewAllVehicles();
void addVehicle();
void updateVehicle();
void deleteVehicle();
void searchVehicle();
vector<Vehicle> loadVehiclesFromFile();
void saveVehiclesToFile(const vector<Vehicle>& vehicles);

#endif // VEHICLE_H