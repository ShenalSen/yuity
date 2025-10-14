# Test Plan for TOUR MATE Vehicle Management System

## 1. Introduction

This document outlines the test plan for the TOUR MATE vehicle management system. The goal of testing is to ensure that the application meets all requirements specified in the assignment brief and functions correctly without errors.

## 2. Test Objectives

- Verify that all core functionalities work as expected
- Ensure proper error handling is implemented
- Validate the user interface for usability and clarity
- Confirm data persistence through file operations
- Test boundary conditions and edge cases

## 3. Test Environment

- Operating System: Windows
- Compiler: g++
- Testing Tools: Manual testing

## 4. Features to Test

### 4.1. Authentication Module
- User login
- Logout functionality
- Invalid login attempts handling

### 4.2. Vehicle Management Module
- View all vehicles
- Add new vehicle
- Update vehicle details
- Delete vehicle
- Search for vehicles

### 4.3. Sales Management Module
- Record new sales
- View all sales
- Search sales
- Generate sales report

### 4.4. Other Features
- View company details
- Main menu navigation
- Error handling

## 5. Test Cases

### 5.1. Authentication Test Cases

| ID | Test Case | Test Steps | Expected Result | Actual Result | Status |
|----|-----------|------------|-----------------|--------------|--------|
| TC-AUTH-01 | Valid Login | 1. Start application<br>2. Select login option<br>3. Enter valid username and password | User should be logged in and main menu displayed | | |
| TC-AUTH-02 | Invalid Login | 1. Start application<br>2. Select login option<br>3. Enter invalid username or password | Error message should be displayed and login should fail | | |
| TC-AUTH-03 | Logout | 1. Login to the system<br>2. Select logout option from main menu | User should be logged out and login menu should be displayed | | |

### 5.2. Vehicle Management Test Cases

| ID | Test Case | Test Steps | Expected Result | Actual Result | Status |
|----|-----------|------------|-----------------|--------------|--------|
| TC-VEH-01 | View All Vehicles (Empty) | 1. Login to the system<br>2. Select Vehicle Management<br>3. Select View All Vehicles option | Message "No vehicles found" should be displayed | | |
| TC-VEH-02 | Add New Vehicle | 1. Login to the system<br>2. Select Vehicle Management<br>3. Select Add New Vehicle<br>4. Enter all required details | Vehicle should be added successfully with confirmation message | | |
| TC-VEH-03 | View All Vehicles (After Addition) | 1. Add a vehicle<br>2. Select View All Vehicles option | The added vehicle should be displayed in the list | | |
| TC-VEH-04 | Update Vehicle | 1. Add a vehicle<br>2. Select Update Vehicle<br>3. Enter vehicle ID<br>4. Update some fields | Vehicle details should be updated with confirmation message | | |
| TC-VEH-05 | Delete Vehicle | 1. Add a vehicle<br>2. Select Delete Vehicle<br>3. Enter vehicle ID<br>4. Confirm deletion | Vehicle should be removed with confirmation message | | |
| TC-VEH-06 | Search Vehicle | 1. Add multiple vehicles<br>2. Select Search Vehicle<br>3. Choose search criteria<br>4. Enter search term | Matching vehicles should be displayed | | |

### 5.3. Sales Management Test Cases

| ID | Test Case | Test Steps | Expected Result | Actual Result | Status |
|----|-----------|------------|-----------------|--------------|--------|
| TC-SALE-01 | Record New Sale (No Vehicles) | 1. Login to system with no vehicles<br>2. Select Sales Management<br>3. Select Record New Sale | Error message about no vehicles available | | |
| TC-SALE-02 | Record New Sale | 1. Add a vehicle<br>2. Select Sales Management<br>3. Select Record New Sale<br>4. Enter all required details | Sale should be added successfully with confirmation message | | |
| TC-SALE-03 | View All Sales | 1. Add a sale<br>2. Select View All Sales | The added sale should be displayed in the list | | |
| TC-SALE-04 | Search Sales | 1. Add multiple sales<br>2. Select Search Sales<br>3. Choose search criteria<br>4. Enter search term | Matching sales should be displayed | | |
| TC-SALE-05 | Generate Sales Report | 1. Add multiple sales<br>2. Select Generate Sales Report | Report should be generated and saved to file | | |

### 5.4. Other Test Cases

| ID | Test Case | Test Steps | Expected Result | Actual Result | Status |
|----|-----------|------------|-----------------|--------------|--------|
| TC-OTH-01 | View Company Details | 1. Login to the system<br>2. Select View Company Details option | Company details should be displayed | | |
| TC-OTH-02 | Exit Application | 1. Select Exit option from main menu | Application should close properly | | |
| TC-OTH-03 | Input Validation | 1. Enter invalid inputs (like letters where numbers are expected) | Appropriate error messages should be displayed | | |

## 6. User Acceptance Testing

### 6.1. Questionnaire Template

1. How would you rate the ease of navigation through the application? (1-5)
2. Did you encounter any issues during login or logout?
3. Were you able to successfully add, update, and delete vehicles?
4. How intuitive was the process of recording a new sale? (1-5)
5. Did the search functionality work as expected?
6. Did you encounter any errors or unexpected behavior?
7. Was the application responsive to your inputs?
8. How would you rate the overall user experience? (1-5)
9. Any suggestions for improvement?

## 7. Test Schedule

- Unit Testing: [Date]
- Integration Testing: [Date]
- System Testing: [Date]
- User Acceptance Testing: [Date]

## 8. Deliverables

- Test cases with results
- Test report summary
- User acceptance testing feedback
- List of defects found and fixed