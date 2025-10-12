# Tourmate Vehicle Management System - System Architecture Design

## 1. SYSTEM OVERVIEW

**Architecture Type:** Layered Architecture with Modular Design
**Interface Type:** Console-based Menu-driven Application
**Data Storage:** File-based Persistence with CSV format
**Programming Language:** C++ with Object-Oriented Design

## 2. SYSTEM ARCHITECTURE LAYERS

### 2.1 Presentation Layer (Console Interface)
- **Menu System:** Main menu and sub-menus for navigation
- **Input/Output Handlers:** Console input validation and formatted output
- **User Interface Components:** Text-based forms and displays

### 2.2 Business Logic Layer
- **Authentication Manager:** User login/logout and session management
- **Vehicle Manager:** Vehicle CRUD operations and business rules
- **Sales Manager:** Sales transaction processing and reporting
- **Company Manager:** Company information management
- **Validation Engine:** Input validation and business rule enforcement

### 2.3 Data Access Layer
- **File I/O Manager:** File operations and data persistence
- **Data Parser:** CSV file parsing and writing
- **Backup Manager:** Data backup and recovery operations
- **Error Handler:** File operation error management

## 3. MODULE ARCHITECTURE

### 3.1 Core Modules

#### A. Authentication Module
```
AuthenticationManager
├── validateUser()
├── loginUser()
├── logoutUser()
├── checkSession()
└── encryptPassword()
```

#### B. Vehicle Management Module
```
VehicleManager
├── addVehicle()
├── viewAllVehicles()
├── viewVehicleById()
├── updateVehicle()
├── deleteVehicle()
├── searchVehicles()
└── validateVehicleData()
```

#### C. Sales Management Module
```
SalesManager
├── addSale()
├── viewAllSales()
├── viewSaleById()
├── updateSale()
├── deleteSale()
├── generateSalesReport()
├── searchSales()
└── validateSalesData()
```

#### D. Company Information Module
```
CompanyManager
├── viewCompanyInfo()
├── updateCompanyInfo()
└── displaySystemInfo()
```

#### E. File Operations Module
```
FileManager
├── loadVehicleData()
├── saveVehicleData()
├── loadSalesData()
├── saveSalesData()
├── loadUserData()
├── saveUserData()
├── createBackup()
├── restoreBackup()
└── validateFileIntegrity()
```

#### F. Menu System Module
```
MenuManager
├── displayMainMenu()
├── displayVehicleMenu()
├── displaySalesMenu()
├── displayUserMenu()
├── handleUserChoice()
├── clearScreen()
└── pauseScreen()
```

### 3.2 Utility Modules

#### A. Input Validation Module
```
InputValidator
├── validateInteger()
├── validateString()
├── validateDate()
├── validateEmail()
├── validatePhoneNumber()
└── validateChoice()
```

#### B. Error Handling Module
```
ErrorHandler
├── logError()
├── displayError()
├── handleFileError()
├── handleValidationError()
└── gracefulExit()
```

## 4. DATA STRUCTURES DESIGN

### 4.1 Vehicle Entity Structure
```cpp
struct Vehicle {
    int vehicleId;           // Primary Key
    string licensePlate;     // Unique identifier
    string model;           // Vehicle model
    int year;               // Manufacturing year
    string type;            // Vehicle type (Sedan, SUV, etc.)
    int seatingCapacity;    // Number of seats
    string status;          // Available, In-Service, Maintenance
    string registrationDate; // Date of registration
    string lastServiceDate; // Last maintenance date
};
```

### 4.2 Sales Entity Structure
```cpp
struct Sales {
    int salesId;            // Primary Key
    int vehicleId;          // Foreign Key to Vehicle
    string customerName;    // Customer information
    string customerContact; // Phone/Email
    string startLocation;   // Trip start point
    string endLocation;     // Trip end point
    string tripDate;        // Date of trip
    string tripTime;        // Time of trip
    float tripDuration;     // Duration in hours
    float fareAmount;       // Total fare
    string paymentStatus;   // Paid, Pending, Cancelled
    string salesRep;        // Sales representative
};
```

### 4.3 User Entity Structure
```cpp
struct User {
    int userId;             // Primary Key
    string username;        // Login username
    string password;        // Encrypted password
    string role;            // Admin, Operator, Viewer
    string lastLogin;       // Last login timestamp
    string accountStatus;   // Active, Inactive, Locked
};
```

### 4.4 Company Information Structure
```cpp
struct CompanyInfo {
    string companyName;     // "TOUR MATE"
    string address;         // Company address
    string phone;           // Contact number
    string email;           // Company email
    string website;         // Company website
    string establishedYear; // Year of establishment
    int totalVehicles;      // Fleet size
    string systemVersion;   // Application version
};
```

## 5. FILE STORAGE DESIGN

### 5.1 File Structure
```
/data/
├── vehicles.csv        // Vehicle data storage
├── sales.csv          // Sales transaction data
├── users.csv          // User account data
├── company.csv        // Company information
└── /backup/           // Backup directory
    ├── vehicles_backup.csv
    ├── sales_backup.csv
    └── users_backup.csv
```

### 5.2 CSV File Formats

#### A. vehicles.csv Format
```
VehicleID,LicensePlate,Model,Year,Type,SeatingCapacity,Status,RegistrationDate,LastServiceDate
1,ABC-123,Toyota Camry,2020,Sedan,5,Available,2020-01-15,2024-09-01
```

#### B. sales.csv Format
```
SalesID,VehicleID,CustomerName,CustomerContact,StartLocation,EndLocation,TripDate,TripTime,Duration,FareAmount,PaymentStatus,SalesRep
1,1,John Smith,+1234567890,Airport,Hotel,2024-10-10,14:30,1.5,25.50,Paid,Sarah
```

#### C. users.csv Format
```
UserID,Username,Password,Role,LastLogin,AccountStatus
1,admin,encrypted_password,Admin,2024-10-10 09:00,Active
```

## 6. CONSOLE INTERFACE DESIGN

### 6.1 Main Menu Structure
```
======================================
    TOUR MATE MANAGEMENT SYSTEM
======================================
1. Vehicle Management
2. Sales Management  
3. Company Information
4. User Management
5. System Backup
6. Exit System
======================================
Enter your choice (1-6): 
```

### 6.2 Vehicle Management Menu
```
======================================
      VEHICLE MANAGEMENT
======================================
1. View All Vehicles
2. Add New Vehicle
3. Update Vehicle Details
4. Delete Vehicle
5. Search Vehicles
6. Back to Main Menu
======================================
Enter your choice (1-6): 
```

### 6.3 Sales Management Menu
```
======================================
       SALES MANAGEMENT
======================================
1. View All Sales
2. Add New Sale
3. Update Sale Details
4. Delete Sale
5. Sales Reports
6. Search Sales
7. Back to Main Menu
======================================
Enter your choice (1-7): 
```

## 7. CONTROL FLOW DESIGN

### 7.1 System Startup Flow
```
Start Application
    ↓
Initialize System
    ↓
Load Configuration
    ↓
Verify Data Files
    ↓
Display Login Screen
    ↓
Authenticate User
    ↓
Load Main Menu
```

### 7.2 Menu Navigation Flow
```
Main Menu
    ↓
User Selection
    ↓
Validate Choice
    ↓
Execute Module Function
    ↓
Display Results
    ↓
Return to Menu/Exit
```

### 7.3 Data Operation Flow
```
User Input
    ↓
Validate Input
    ↓
Process Business Logic
    ↓
Update Data Storage
    ↓
Create Backup
    ↓
Display Confirmation
```

## 8. ERROR HANDLING STRATEGY

### 8.1 Input Validation Errors
- Invalid menu choices
- Invalid data formats
- Missing required fields
- Out-of-range values

### 8.2 File Operation Errors
- File not found
- Permission denied
- Disk space full
- File corruption

### 8.3 Business Logic Errors
- Duplicate entries
- Foreign key violations
- Business rule violations
- Concurrent access issues

## 9. MODULARIZATION BENEFITS

### 9.1 Code Maintainability
- **Separation of Concerns:** Each module handles specific functionality
- **Easy Debugging:** Isolated modules simplify error tracking
- **Code Reusability:** Common functions can be shared across modules
- **Scalability:** New features can be added without affecting existing modules

### 9.2 Development Efficiency
- **Parallel Development:** Different modules can be developed simultaneously
- **Testing Isolation:** Each module can be tested independently
- **Code Organization:** Clear structure improves code readability
- **Version Control:** Changes can be tracked at module level

## 10. SECURITY CONSIDERATIONS

### 10.1 Authentication Security
- Password encryption using simple encryption algorithm
- Session management with timeout
- Role-based access control
- Login attempt monitoring

### 10.2 Data Security
- File access permissions
- Data validation to prevent injection
- Backup encryption (optional)
- Audit trail for critical operations

## 11. PERFORMANCE CONSIDERATIONS

### 11.1 File I/O Optimization
- Lazy loading of data
- Efficient file parsing
- Minimal file access operations
- Cached data for frequently accessed information

### 11.2 Memory Management
- Proper memory allocation/deallocation
- Efficient data structures
- Minimal memory footprint
- Resource cleanup on exit

## 12. IMPLEMENTATION GUIDELINES

### 12.1 Coding Standards
- Use descriptive variable and function names
- Consistent indentation and formatting
- Comprehensive comments and documentation
- Error handling in all functions

### 12.2 File Organization
```
/src/
├── main.cpp              // Main application entry point
├── /headers/             // Header files directory
│   ├── Vehicle.h
│   ├── Sales.h
│   ├── User.h
│   ├── MenuManager.h
│   ├── FileManager.h
│   └── Utilities.h
├── /source/              // Source files directory
│   ├── Vehicle.cpp
│   ├── Sales.cpp
│   ├── User.cpp
│   ├── MenuManager.cpp
│   ├── FileManager.cpp
│   └── Utilities.cpp
└── /data/                // Data files directory
    ├── vehicles.csv
    ├── sales.csv
    ├── users.csv
    └── company.csv
```

---

**Document Status:** Complete - System Architecture Design Phase
**Next Phase:** Create Flowcharts
**Date:** October 11, 2025