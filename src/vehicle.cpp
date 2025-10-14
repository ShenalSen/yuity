#include "vehicle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

// Constructor
Vehicle::Vehicle(string id, string make, int yr, string tp, string reg, string st, double rate) {
    vehicleId = id;
    makeModel = make;
    year = yr;
    type = tp;
    registrationNumber = reg;
    status = st;
    ratePerDay = rate;
}

// Default constructor
Vehicle::Vehicle() {
    vehicleId = "";
    makeModel = "";
    year = 0;
    type = "";
    registrationNumber = "";
    status = "Available";
    ratePerDay = 0.0;
}

// Getters
string Vehicle::getVehicleId() const {
    return vehicleId;
}

string Vehicle::getMakeModel() const {
    return makeModel;
}

int Vehicle::getYear() const {
    return year;
}

string Vehicle::getType() const {
    return type;
}

string Vehicle::getRegistrationNumber() const {
    return registrationNumber;
}

string Vehicle::getStatus() const {
    return status;
}

double Vehicle::getRatePerDay() const {
    return ratePerDay;
}

// Setters
void Vehicle::setVehicleId(string id) {
    vehicleId = id;
}

void Vehicle::setMakeModel(string make) {
    makeModel = make;
}

void Vehicle::setYear(int yr) {
    year = yr;
}

void Vehicle::setType(string tp) {
    type = tp;
}

void Vehicle::setRegistrationNumber(string reg) {
    registrationNumber = reg;
}

void Vehicle::setStatus(string st) {
    status = st;
}

void Vehicle::setRatePerDay(double rate) {
    ratePerDay = rate;
}

// Display vehicle details
void Vehicle::displayDetails() const {
    cout << "Vehicle ID: " << vehicleId << endl;
    cout << "Make/Model: " << makeModel << endl;
    cout << "Year: " << year << endl;
    cout << "Type: " << type << endl;
    cout << "Registration Number: " << registrationNumber << endl;
    cout << "Status: " << status << endl;
    cout << "Rate per day: $" << ratePerDay << endl;
}

// String representation for file storage
string Vehicle::toString() const {
    ostringstream oss;
    oss << vehicleId << "|" << makeModel << "|" << year << "|" 
        << type << "|" << registrationNumber << "|" << status << "|" << ratePerDay;
    return oss.str();
}

// Create vehicle from string (read from file)
Vehicle Vehicle::fromString(string str) {
    Vehicle vehicle;
    stringstream ss(str);
    string token;
    
    // Parse vehicle ID
    if (getline(ss, token, '|')) {
        vehicle.setVehicleId(token);
    }
    
    // Parse make/model
    if (getline(ss, token, '|')) {
        vehicle.setMakeModel(token);
    }
    
    // Parse year
    if (getline(ss, token, '|')) {
        vehicle.setYear(stoi(token));
    }
    
    // Parse type
    if (getline(ss, token, '|')) {
        vehicle.setType(token);
    }
    
    // Parse registration number
    if (getline(ss, token, '|')) {
        vehicle.setRegistrationNumber(token);
    }
    
    // Parse status
    if (getline(ss, token, '|')) {
        vehicle.setStatus(token);
    }
    
    // Parse rate per day
    if (getline(ss, token, '|')) {
        vehicle.setRatePerDay(stod(token));
    }
    
    return vehicle;
}

// Load vehicles from file
vector<Vehicle> loadVehiclesFromFile() {
    vector<Vehicle> vehicles;
    ifstream file("vehicles.txt");
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                Vehicle vehicle = Vehicle::fromString(line);
                vehicles.push_back(vehicle);
            }
        }
        file.close();
    } else {
        cout << "Warning: Could not open vehicles.txt. A new file will be created when vehicles are added." << endl;
    }
    
    return vehicles;
}

// Save vehicles to file
void saveVehiclesToFile(const vector<Vehicle>& vehicles) {
    ofstream file("vehicles.txt");
    
    if (file.is_open()) {
        for (const auto& vehicle : vehicles) {
            file << vehicle.toString() << endl;
        }
        file.close();
    } else {
        cout << "Error: Could not open vehicles.txt for writing." << endl;
    }
}

// View all vehicles
void viewAllVehicles() {
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    
    if (vehicles.empty()) {
        cout << "No vehicles found in the system." << endl;
        return;
    }
    
    cout << "\n===== VEHICLE LIST =====\n";
    cout << "Total vehicles: " << vehicles.size() << endl << endl;
    
    for (size_t i = 0; i < vehicles.size(); i++) {
        cout << "Vehicle #" << (i + 1) << ":" << endl;
        vehicles[i].displayDetails();
        cout << "------------------------" << endl;
    }
}

// Add a new vehicle
void addVehicle() {
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    Vehicle newVehicle;
    string input;
    int yearInput;
    double rateInput;
    
    cout << "\n===== ADD NEW VEHICLE =====\n";
    
    // Generate a vehicle ID (could be made more sophisticated)
    string vehicleId = "V" + to_string(vehicles.size() + 1);
    newVehicle.setVehicleId(vehicleId);
    
    // Get vehicle details from user
    cout << "Make and Model: ";
    getline(cin, input);
    newVehicle.setMakeModel(input);
    
    cout << "Year: ";
    cin >> yearInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newVehicle.setYear(yearInput);
    
    cout << "Type (Sedan, SUV, etc.): ";
    getline(cin, input);
    newVehicle.setType(input);
    
    cout << "Registration Number: ";
    getline(cin, input);
    newVehicle.setRegistrationNumber(input);
    
    cout << "Status (Available, In maintenance, etc.): ";
    getline(cin, input);
    newVehicle.setStatus(input);
    
    cout << "Rate per day ($): ";
    cin >> rateInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newVehicle.setRatePerDay(rateInput);
    
    // Add the new vehicle to the vector
    vehicles.push_back(newVehicle);
    
    // Save the updated vehicles to file
    saveVehiclesToFile(vehicles);
    
    cout << "\nVehicle added successfully with ID: " << vehicleId << endl;
}

// Update an existing vehicle
void updateVehicle() {
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    
    if (vehicles.empty()) {
        cout << "No vehicles found in the system." << endl;
        return;
    }
    
    string searchId;
    cout << "\n===== UPDATE VEHICLE =====\n";
    cout << "Enter Vehicle ID to update: ";
    getline(cin, searchId);
    
    auto it = find_if(vehicles.begin(), vehicles.end(),
                     [searchId](const Vehicle& v) { return v.getVehicleId() == searchId; });
    
    if (it != vehicles.end()) {
        // Vehicle found
        int index = distance(vehicles.begin(), it);
        Vehicle& vehicle = vehicles[index];
        
        cout << "\nCurrent vehicle details:" << endl;
        vehicle.displayDetails();
        cout << "\n------------------------\n";
        
        string input;
        int yearInput;
        double rateInput;
        
        // Update vehicle details
        cout << "New Make and Model (press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) {
            vehicle.setMakeModel(input);
        }
        
        cout << "New Year (enter 0 to keep current): ";
        cin >> yearInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (yearInput != 0) {
            vehicle.setYear(yearInput);
        }
        
        cout << "New Type (press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) {
            vehicle.setType(input);
        }
        
        cout << "New Registration Number (press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) {
            vehicle.setRegistrationNumber(input);
        }
        
        cout << "New Status (press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) {
            vehicle.setStatus(input);
        }
        
        cout << "New Rate per day (enter 0 to keep current): ";
        cin >> rateInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (rateInput != 0) {
            vehicle.setRatePerDay(rateInput);
        }
        
        // Save the updated vehicles to file
        saveVehiclesToFile(vehicles);
        
        cout << "\nVehicle updated successfully!" << endl;
    } else {
        cout << "\nVehicle not found with ID: " << searchId << endl;
    }
}

// Delete a vehicle
void deleteVehicle() {
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    
    if (vehicles.empty()) {
        cout << "No vehicles found in the system." << endl;
        return;
    }
    
    string searchId;
    char confirmation;
    
    cout << "\n===== DELETE VEHICLE =====\n";
    cout << "Enter Vehicle ID to delete: ";
    getline(cin, searchId);
    
    auto it = find_if(vehicles.begin(), vehicles.end(),
                     [searchId](const Vehicle& v) { return v.getVehicleId() == searchId; });
    
    if (it != vehicles.end()) {
        // Vehicle found
        cout << "\nVehicle to delete:" << endl;
        it->displayDetails();
        
        cout << "\nAre you sure you want to delete this vehicle? (Y/N): ";
        cin >> confirmation;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (tolower(confirmation) == 'y') {
            vehicles.erase(it);
            saveVehiclesToFile(vehicles);
            cout << "\nVehicle deleted successfully!" << endl;
        } else {
            cout << "\nDeletion cancelled." << endl;
        }
    } else {
        cout << "\nVehicle not found with ID: " << searchId << endl;
    }
}

// Search for vehicles
void searchVehicle() {
    vector<Vehicle> vehicles = loadVehiclesFromFile();
    
    if (vehicles.empty()) {
        cout << "No vehicles found in the system." << endl;
        return;
    }
    
    int searchOption;
    string searchTerm;
    bool found = false;
    
    cout << "\n===== SEARCH VEHICLE =====\n";
    cout << "Search by:\n";
    cout << "1. Vehicle ID\n";
    cout << "2. Make/Model\n";
    cout << "3. Registration Number\n";
    cout << "4. Type\n";
    cout << "5. Status\n";
    cout << "Enter your choice: ";
    
    cin >> searchOption;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter search term: ";
    getline(cin, searchTerm);
    
    cout << "\nSearch Results:\n";
    
    for (const auto& vehicle : vehicles) {
        bool match = false;
        
        switch (searchOption) {
            case 1: // Vehicle ID
                match = vehicle.getVehicleId() == searchTerm;
                break;
            case 2: // Make/Model
                match = vehicle.getMakeModel().find(searchTerm) != string::npos;
                break;
            case 3: // Registration Number
                match = vehicle.getRegistrationNumber() == searchTerm;
                break;
            case 4: // Type
                match = vehicle.getType() == searchTerm;
                break;
            case 5: // Status
                match = vehicle.getStatus() == searchTerm;
                break;
            default:
                cout << "Invalid search option." << endl;
                return;
        }
        
        if (match) {
            found = true;
            cout << "------------------------" << endl;
            vehicle.displayDetails();
        }
    }
    
    if (!found) {
        cout << "No matching vehicles found." << endl;
    }
}