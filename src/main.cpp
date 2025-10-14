#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include "vehicle.h"
#include "user.h"
#include "sales.h"

using namespace std;

// Global variables
bool isLoggedIn = false;
string currentUser = "";
string currentRole = "";

// Function prototypes
void displayMainMenu();
void handleMainMenu();
bool displayLoginMenu();  // Changed return type to bool
bool login(string username, string password);
void logout();
void displayVehicleMenu();
void handleVehicleMenu();
void displaySalesMenu();
void handleSalesMenu();
void viewCompanyDetails();
void clearScreen();
void pressEnterToContinue();

int main() {
    // Start the program
    cout << "\n\n";
    cout << "===============================================\n";
    cout << "      WELCOME TO TOUR MATE VEHICLE SYSTEM      \n";
    cout << "===============================================\n\n";
    
    bool exitProgram = false;
    
    while (!exitProgram) {
        if (!isLoggedIn) {
            exitProgram = displayLoginMenu(); // Use the return value to determine if we should exit
        } else {
            displayMainMenu();
            handleMainMenu();
        }
    }
    
    cout << "\nThank you for using TOUR MATE VEHICLE SYSTEM!\n";
    return 0;
}

// Display the login menu
bool displayLoginMenu() {
    string username, password;
    int choice;
    
    cout << "\n===== LOGIN MENU =====\n";
    cout << "1. Login\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        pressEnterToContinue();
        return false; // Return to main loop without exiting
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            cout << "\nUsername: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            
            if (login(username, password)) {
                cout << "\nLogin successful! Welcome, " << currentUser << "!\n";
                isLoggedIn = true;
                pressEnterToContinue();
                return false; // Return to main loop without exiting
            } else {
                cout << "\nInvalid username or password. Please try again.\n";
                pressEnterToContinue();
                return false; // Return to main loop without exiting
            }
            break;
        case 2:
            return true; // Signal to exit the program
        default:
            cout << "Invalid choice. Please try again.\n";
            pressEnterToContinue();
            return false; // Return to main loop without exiting
    }
    
    return false; // Default return to continue the program
}


// Login function - placeholder 
bool login(string username, string password) {
//harcoded password and username
    if (username == "admin" && password == "admin123") {
        currentUser = username;
        currentRole = "admin";
        return true;
    }
    return false;
}

// Display the main menu
void displayMainMenu() {
    clearScreen();
    cout << "\n===== MAIN MENU =====\n";
    cout << "Current User: " << currentUser << " (" << currentRole << ")\n\n";
    cout << "1. Vehicle Management\n";
    cout << "2. Sales Management\n";
    cout << "3. View Company Details\n";
    cout << "4. Logout\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// Handle main menu selections
void handleMainMenu() {
    int choice;
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        pressEnterToContinue();
        return;
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            displayVehicleMenu();
            handleVehicleMenu();
            break;
        case 2:
            displaySalesMenu();
            handleSalesMenu();
            break;
        case 3:
            viewCompanyDetails();
            break;
        case 4:
            logout();
            break;
        case 5:
            logout();
            currentUser = "";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            pressEnterToContinue();
    }
}

// Logout function
void logout() {
    isLoggedIn = false;
    currentUser = "";
    currentRole = "";
    cout << "Logged out successfully.\n";
    pressEnterToContinue();
}

// Display vehicle menu - placeholder
void displayVehicleMenu() {
    clearScreen();
    cout << "\n===== VEHICLE MANAGEMENT =====\n";
    cout << "1. View All Vehicles\n";
    cout << "2. Add New Vehicle\n";
    cout << "3. Update Vehicle\n";
    cout << "4. Delete Vehicle\n";
    cout << "5. Search Vehicle\n";
    cout << "6. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

// Handle vehicle menu - placeholder
void handleVehicleMenu() {
    int choice;
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        pressEnterToContinue();
        return;
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            viewAllVehicles();
            pressEnterToContinue();
            break;
        case 2:
            addVehicle();
            pressEnterToContinue();
            break;
        case 3:
            updateVehicle();
            pressEnterToContinue();
            break;
        case 4:
            deleteVehicle();
            pressEnterToContinue();
            break;
        case 5:
            searchVehicle();
            pressEnterToContinue();
            break;
        case 6:
            // Return to main menu
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            pressEnterToContinue();
    }
}

// Display sales menu - placeholder
void displaySalesMenu() {
    clearScreen();
    cout << "\n===== SALES MANAGEMENT =====\n";
    cout << "1. Record New Sale\n";
    cout << "2. View All Sales\n";
    cout << "3. Search Sales\n";
    cout << "4. Generate Sales Report\n";
    cout << "5. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

// Handle sales menu - placeholder
void handleSalesMenu() {
    int choice;
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        pressEnterToContinue();
        return;
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            addSale();
            pressEnterToContinue();
            break;
        case 2:
            viewAllSales();
            pressEnterToContinue();
            break;
        case 3:
            searchSales();
            pressEnterToContinue();
            break;
        case 4:
            generateSalesReport();
            pressEnterToContinue();
            break;
        case 5:
            // Return to main menu
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            pressEnterToContinue();
    }
}

// View company details - placeholder
void viewCompanyDetails() {
    clearScreen();
    cout << "\n===== COMPANY DETAILS =====\n";
    cout << "Name: TOUR MATE Cab Services\n";
    cout << "Address: 123 City Road, Metro City\n";
    cout << "Contact: +1234567890\n";
    cout << "Email: info@tourmate.com\n";
    cout << "Website: www.tourmate.com\n";
    pressEnterToContinue();
}

// Clear screen function
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

// Press enter to continue function
void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}