# Tourmate Vehicle Management System - System Analysis & Planning

## 1. PROJECT OVERVIEW

**Project Name:** Tourmate Vehicle Management System
**Purpose:** Create a comprehensive application to manage TOUR MATE cab service's main processes
**Technology:** C++ with GUI/Console interface
**Assessment:** CSE4002 WRIT1 (100% weighting)

## 2. STAKEHOLDER ANALYSIS

### Primary Stakeholders:
- **Tour Mate Management:** Company executives requiring system oversight
- **System Administrators:** Personnel managing vehicle and sales data
- **Operational Staff:** Users performing daily operations (vehicle management, sales recording)

### Secondary Stakeholders:
- **IT Support:** Technical maintenance personnel
- **Auditors:** Personnel requiring access to sales and vehicle reports

## 3. FUNCTIONAL REQUIREMENTS ANALYSIS

### 3.1 Core Functional Requirements

#### A. Vehicle Management Module
- **FR1.1:** View Available Vehicle Details
  - Display all registered vehicles with complete information
  - Filter and search capabilities
  - Real-time availability status

- **FR1.2:** Add Vehicle Details
  - Register new vehicles in the system
  - Capture vehicle identification (license plate, model, year, etc.)
  - Set initial availability status

- **FR1.3:** Update Vehicle Details
  - Modify existing vehicle information
  - Change availability status
  - Update maintenance records

- **FR1.4:** Delete Vehicle Details
  - Remove vehicles from active fleet
  - Maintain audit trail for deleted records
  - Prevent deletion of vehicles with active sales

#### B. Sales Management Module
- **FR2.1:** Record New Sales
  - Create sales transactions
  - Link sales to specific vehicles
  - Capture customer and trip details

- **FR2.2:** View Sales Details
  - Display sales history and reports
  - Filter by date, vehicle, or customer
  - Generate summary statistics

- **FR2.3:** Update Sales Information
  - Modify existing sales records
  - Handle cancellations and refunds
  - Update payment status

#### C. User Authentication Module
- **FR3.1:** User Login
  - Secure authentication process
  - Role-based access control
  - Session management

- **FR3.2:** User Logout
  - Secure session termination
  - Data persistence before logout

#### D. Company Information Module
- **FR4.1:** View Company Details
  - Display company profile and contact information
  - System information and version details

#### E. System Navigation
- **FR5.1:** Menu-Driven Interface
  - Intuitive navigation structure
  - Consistent user experience
  - Help and guidance features

- **FR5.2:** Exit Functionality
  - Safe system shutdown
  - Data backup before exit

### 3.2 Data Requirements

#### Vehicle Entity Attributes:
- Vehicle ID (Primary Key)
- License Plate Number
- Vehicle Model
- Vehicle Year
- Vehicle Type (Sedan, SUV, Hatchback, etc.)
- Seating Capacity
- Current Status (Available, In-Service, Maintenance)
- Registration Date
- Last Service Date

#### Sales Entity Attributes:
- Sales ID (Primary Key)
- Vehicle ID (Foreign Key)
- Customer Name
- Customer Contact
- Trip Start Location
- Trip End Location
- Trip Date and Time
- Trip Duration
- Fare Amount
- Payment Status
- Sales Representative

#### User Entity Attributes:
- User ID (Primary Key)
- Username
- Password (Encrypted)
- User Role (Admin, Operator, Viewer)
- Last Login Time
- Account Status

## 4. NON-FUNCTIONAL REQUIREMENTS ANALYSIS

### 4.1 Usability Requirements
- **NFR1:** User-friendly interface with intuitive navigation
- **NFR2:** Clear error messages and help documentation
- **NFR3:** Consistent design patterns across all modules
- **NFR4:** Maximum 3 clicks to reach any functionality

### 4.2 Reliability Requirements
- **NFR5:** System availability of 99% during operational hours
- **NFR6:** Data integrity with automatic backup mechanisms
- **NFR7:** Error recovery without data loss
- **NFR8:** Input validation to prevent data corruption

### 4.3 Performance Requirements
- **NFR9:** Response time under 2 seconds for all operations
- **NFR10:** Support for minimum 100 concurrent vehicle records
- **NFR11:** Support for minimum 1000 sales transactions

### 4.4 Security Requirements
- **NFR12:** Password-protected user authentication
- **NFR13:** Data encryption for sensitive information
- **NFR14:** Audit trail for all data modifications
- **NFR15:** Role-based access control

### 4.5 Maintainability Requirements
- **NFR16:** Modular code structure for easy maintenance
- **NFR17:** Comprehensive code documentation
- **NFR18:** Standardized coding conventions
- **NFR19:** Configuration management for system settings

## 5. SYSTEM CONSTRAINTS

### 5.1 Technical Constraints
- **TC1:** Must be developed in C++ programming language only
- **TC2:** File-based data storage (no database required)
- **TC3:** Console/GUI interface (no web interface)
- **TC4:** Single-user system (no concurrent multi-user support)

### 5.2 Operational Constraints
- **TC5:** Menu-driven interface mandatory
- **TC6:** Must include comprehensive error handling
- **TC7:** Data persistence through files or arrays
- **TC8:** Backup functionality required

### 5.3 Development Constraints
- **TC9:** Individual project (no team collaboration)
- **TC10:** 3000-word equivalent documentation
- **TC11:** Must include flowcharts and test documentation
- **TC12:** Submission deadline constraints

## 6. RISK ANALYSIS

### 6.1 Technical Risks
- **R1:** File corruption leading to data loss
  - *Mitigation:* Implement robust backup mechanisms and file validation

- **R2:** Memory management issues in C++
  - *Mitigation:* Use proper memory allocation/deallocation practices

- **R3:** Input validation bypass leading to system crashes
  - *Mitigation:* Comprehensive input validation and error handling

### 6.2 Project Risks
- **R4:** Time management and scope creep
  - *Mitigation:* Detailed project planning with milestone tracking

- **R5:** Requirement misunderstanding
  - *Mitigation:* Regular review against assessment criteria

## 7. SUCCESS CRITERIA

### 7.1 Functional Success Criteria
- All required modules implemented and working correctly
- Error-free program execution
- Complete data persistence and backup functionality
- User-friendly interface with intuitive navigation

### 7.2 Academic Success Criteria
- Meet all learning outcomes (LO1-LO4)
- Achieve excellent grade (70-100%) based on marking criteria
- Comprehensive documentation and testing
- Proper modularization and control structure implementation

## 8. PROJECT APPROACH STRATEGY

### 8.1 Development Methodology
- **Waterfall Approach:** Due to clear requirements and academic constraints
- **Phases:** Analysis → Design → Implementation → Testing → Documentation

### 8.2 Modularization Strategy
- **Separate modules** for each major functionality
- **Header files** for class declarations
- **Source files** for implementation
- **Utility functions** for common operations

### 8.3 Data Storage Strategy
- **Text files** for persistent data storage
- **CSV format** for easy data manipulation
- **Backup files** for data recovery
- **Configuration files** for system settings

## 9. NEXT STEPS

1. **Complete System Architecture Design** (Task 2)
2. **Create Detailed Flowcharts** (Task 3 - 25 marks)
3. **Setup Development Environment** (Task 4)
4. **Begin Implementation** (Tasks 5-12 - 50 marks)
5. **Testing and Documentation** (Tasks 13-17 - 25 marks)

---

**Document Status:** Complete - System Analysis & Planning Phase
**Next Phase:** System Architecture Design
**Date:** October 11, 2025