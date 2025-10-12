# Tourmate Vehicle Management System - Flowcharts Design (25 marks)

## 1. FLOWCHART OVERVIEW

This document presents comprehensive flowcharts for the Tourmate Vehicle Management System, demonstrating the use of controlling structures (selection and repetition) and showcasing the advantages of modularization in program design.

## 2. CONTROL STRUCTURES EXPLANATION

### 2.1 Selection Structures (Decision Making)

**Definition:** Selection structures allow programs to choose different paths of execution based on conditions.

**Types Used in Tourmate System:**
- **Simple IF-ELSE:** User input validation, login verification
- **Multiple IF-ELSE IF:** Menu choice handling, status checking
- **SWITCH-CASE:** Main menu navigation, operation selection

**Advantages:**
- **Conditional Logic:** Enables different behaviors based on user input or system state
- **Error Handling:** Allows graceful handling of invalid inputs and edge cases
- **Business Rules:** Implements complex business logic with clear decision points
- **User Experience:** Provides different paths for different user roles and scenarios

**Examples in System:**
```
IF (username exists AND password matches)
    THEN: Grant access to system
    ELSE: Display error message and retry
```

### 2.2 Repetition Structures (Loops)

**Definition:** Repetition structures allow programs to execute code blocks multiple times.

**Types Used in Tourmate System:**
- **DO-WHILE Loops:** Menu systems that continue until user chooses to exit
- **FOR Loops:** Iterating through vehicle lists, sales records
- **WHILE Loops:** File reading operations, data validation loops

**Advantages:**
- **Menu Systems:** Keep displaying menus until user exits
- **Data Processing:** Process multiple records efficiently
- **Input Validation:** Repeat input requests until valid data is entered
- **File Operations:** Read/write multiple records from/to files

**Examples in System:**
```
DO
    Display main menu
    Get user choice
    Process choice
WHILE (choice != EXIT)
```

### 2.3 Modularization Advantages

**Definition:** Breaking down complex programs into smaller, manageable modules or functions.

**Key Benefits:**

1. **Reduced Complexity**
   - Each module handles a specific task
   - Easier to understand and maintain
   - Clear separation of concerns

2. **Code Reusability**
   - Common functions can be used multiple times
   - Reduces code duplication
   - Consistent behavior across the system

3. **Easier Testing**
   - Individual modules can be tested separately
   - Isolated testing of specific functionality
   - Simplified debugging process

4. **Team Development**
   - Different developers can work on different modules
   - Parallel development possible
   - Clear interfaces between modules

5. **Maintainability**
   - Changes to one module don't affect others
   - Easier to add new features
   - Simplified bug fixing

**Modularization in Tourmate System:**
- **Authentication Module:** Handles all login/logout operations
- **Vehicle Module:** Manages all vehicle-related operations
- **Sales Module:** Handles all sales transactions
- **File Module:** Manages all data persistence operations
- **Menu Module:** Handles all user interface operations

## 3. SYSTEM FLOWCHARTS

### 3.1 Main System Flowchart

```
START
  ↓
Initialize System
  ↓
Load Configuration Files
  ↓
Display Welcome Screen
  ↓
[User Authentication Required?] → YES → Call Authentication Module
  ↓ NO                                        ↓
Display Main Menu ←──────────────────────────┘
  ↓
Get User Choice
  ↓
[Valid Choice?] → NO → Display Error Message
  ↓ YES               ↓
[Choice Analysis]     ↓
  ↓                   ↓
Vehicle Management → Call Vehicle Module
  ↓                   ↓
Sales Management → Call Sales Module
  ↓                   ↓
Company Info → Call Company Module
  ↓                   ↓
Exit → Call Exit Module
  ↓                   ↓
[Continue?] → YES → ←┘
  ↓ NO
Save Data
  ↓
Create Backup
  ↓
END
```

### 3.2 Authentication Module Flowchart

```
START Authentication
  ↓
Initialize Login Attempts = 0
  ↓
Display Login Screen
  ↓
Get Username and Password
  ↓
[Username Empty?] → YES → Display "Username Required"
  ↓ NO                      ↓
[Password Empty?] → YES → Display "Password Required"
  ↓ NO                      ↓
Validate User Credentials ←┘
  ↓
[Valid Credentials?] → NO → Increment Login Attempts
  ↓ YES                       ↓
Set User Session            [Attempts > 3?] → YES → Lock Account
  ↓                           ↓ NO                      ↓
Display Welcome Message     Display Error Message    Display Lock Message
  ↓                           ↓                        ↓
RETURN SUCCESS             [Try Again?] → YES → ←─────→ RETURN LOCKED
                            ↓ NO
                           RETURN FAILED
```

### 3.3 Vehicle Management Module Flowchart

```
START Vehicle Management
  ↓
Display Vehicle Menu
  ↓
Get User Choice
  ↓
[Choice = 1: View All] → YES → Call Display All Vehicles
  ↓                              ↓
[Choice = 2: Add New] → YES → Call Add Vehicle Function
  ↓                              ↓
[Choice = 3: Update] → YES → Call Update Vehicle Function
  ↓                              ↓
[Choice = 4: Delete] → YES → Call Delete Vehicle Function
  ↓                              ↓
[Choice = 5: Search] → YES → Call Search Vehicle Function
  ↓                              ↓
[Choice = 6: Back] → YES → RETURN TO MAIN
  ↓                              ↓
Display Invalid Choice Error   ←─┘
  ↓
[Continue?] → YES → ←───────────┘
  ↓ NO
RETURN TO MAIN

SUB-FUNCTION: Add Vehicle
START
  ↓
Display Add Vehicle Form
  ↓
Get Vehicle Details
  ↓
[License Plate Empty?] → YES → Display Error, Retry
  ↓ NO
[License Plate Exists?] → YES → Display Duplicate Error
  ↓ NO
[Model Empty?] → YES → Display Error, Retry
  ↓ NO
[Year Valid?] → NO → Display Error, Retry
  ↓ YES
Create Vehicle Record
  ↓
Save to File
  ↓
[Save Successful?] → NO → Display Save Error
  ↓ YES
Display Success Message
  ↓
Create Backup
  ↓
RETURN
```

### 3.4 Sales Management Module Flowchart

```
START Sales Management
  ↓
Display Sales Menu
  ↓
Get User Choice
  ↓
[Choice = 1: View All] → YES → Call Display All Sales
  ↓                              ↓
[Choice = 2: Add New] → YES → Call Add Sale Function
  ↓                              ↓
[Choice = 3: Update] → YES → Call Update Sale Function
  ↓                              ↓
[Choice = 4: Delete] → YES → Call Delete Sale Function
  ↓                              ↓
[Choice = 5: Reports] → YES → Call Generate Reports
  ↓                              ↓
[Choice = 6: Search] → YES → Call Search Sales Function
  ↓                              ↓
[Choice = 7: Back] → YES → RETURN TO MAIN
  ↓                              ↓
Display Invalid Choice Error   ←─┘
  ↓
[Continue?] → YES → ←───────────┘
  ↓ NO
RETURN TO MAIN

SUB-FUNCTION: Add Sale
START
  ↓
Display Available Vehicles
  ↓
Get Vehicle Selection
  ↓
[Vehicle Available?] → NO → Display Error, Retry
  ↓ YES
Get Customer Details
  ↓
[Customer Name Empty?] → YES → Display Error, Retry
  ↓ NO
Get Trip Details
  ↓
[Start Location Empty?] → YES → Display Error, Retry
  ↓ NO
[End Location Empty?] → YES → Display Error, Retry
  ↓ NO
Calculate Fare
  ↓
Display Sale Summary
  ↓
[Confirm Sale?] → NO → Cancel Operation
  ↓ YES
Create Sale Record
  ↓
Update Vehicle Status
  ↓
Save to File
  ↓
Display Success Message
  ↓
RETURN
```

### 3.5 File Operations Module Flowchart

```
START File Operations
  ↓
[Operation Type?]
  ↓
LOAD DATA → Check File Exists
  ↓             ↓
SAVE DATA → [File Exists?] → NO → Display File Not Found
  ↓             ↓ YES              ↓
BACKUP → Open File              Create Empty Records
  ↓             ↓                   ↓
[File Open?] → NO → Display File Error
  ↓ YES           ↓
Read/Write Data   Log Error
  ↓               ↓
[Operation Success?] → NO → Display Operation Error
  ↓ YES                       ↓
Close File                    Retry Operation?
  ↓                           ↓
[Backup Required?] → YES → Create Backup Copy
  ↓ NO                        ↓
RETURN SUCCESS               [Backup Success?] → NO → Log Backup Error
                              ↓ YES
                             RETURN SUCCESS
```

### 3.6 Input Validation Module Flowchart

```
START Input Validation
  ↓
Get Input Value and Type
  ↓
[Type = INTEGER?] → YES → Validate Integer Range
  ↓                         ↓
[Type = STRING?] → YES → Validate String Length/Format
  ↓                         ↓
[Type = DATE?] → YES → Validate Date Format
  ↓                         ↓
[Type = EMAIL?] → YES → Validate Email Pattern
  ↓                         ↓
[Type = PHONE?] → YES → Validate Phone Format
  ↓                         ↓
[Validation Passed?] → NO → Generate Error Message
  ↓ YES                       ↓
RETURN VALID                Display Error to User
                              ↓
                            [Retry?] → YES → Get New Input
                              ↓ NO
                             RETURN INVALID
```

## 4. FLOWCHART SYMBOLS REFERENCE

### 4.1 Standard Flowchart Symbols Used

| Symbol | Name | Purpose | Usage in System |
|--------|------|---------|-----------------|
| Oval | Terminal | Start/End points | System startup, module entry/exit |
| Rectangle | Process | Processing steps | Data manipulation, calculations |
| Diamond | Decision | Conditional branches | Input validation, menu choices |
| Parallelogram | Input/Output | Data input/output | User input, display output |
| Circle | Connector | Connection points | Module connections, flow continuation |
| Rectangle with double sides | Predefined Process | Module calls | Function calls, subroutines |

### 4.2 Logical Instruction Flow Principles

1. **Top-to-Bottom Flow:** All flowcharts follow top-to-bottom logical progression
2. **Left-to-Right Branching:** Decision branches flow left (NO/FALSE) and right (YES/TRUE)
3. **Clear Labels:** All decision points clearly labeled with conditions
4. **Single Entry/Exit:** Each module has one entry and one exit point
5. **Minimal Crossing Lines:** Flow lines minimize crossings for clarity

## 5. MODULARIZATION IMPLEMENTATION

### 5.1 Module Hierarchy

```
Main System
├── Authentication Module
│   ├── ValidateUser()
│   ├── LoginUser()
│   └── LogoutUser()
├── Vehicle Management Module
│   ├── DisplayVehicles()
│   ├── AddVehicle()
│   ├── UpdateVehicle()
│   ├── DeleteVehicle()
│   └── SearchVehicles()
├── Sales Management Module
│   ├── DisplaySales()
│   ├── AddSale()
│   ├── UpdateSale()
│   ├── DeleteSale()
│   └── GenerateReports()
├── File Operations Module
│   ├── LoadData()
│   ├── SaveData()
│   ├── CreateBackup()
│   └── ValidateFile()
└── Utility Modules
    ├── InputValidator()
    ├── ErrorHandler()
    └── MenuDisplay()
```

### 5.2 Module Interface Design

Each module follows standardized interface patterns:

**Input Parameters:**
- Clearly defined data types
- Validation requirements
- Optional parameters with defaults

**Return Values:**
- Success/failure status codes
- Error messages for failures
- Processed data for successful operations

**Error Handling:**
- Consistent error code system
- Graceful failure handling
- User-friendly error messages

## 6. CONTROL STRUCTURE ANALYSIS

### 6.1 Selection Structure Usage

**Menu Navigation (Switch-Case):**
- Main menu: 6 options with default case
- Vehicle menu: 6 options with validation
- Sales menu: 7 options with error handling

**Data Validation (If-Else):**
- Input format checking
- Business rule enforcement
- Security validation

**Error Handling (Nested If-Else):**
- File operation errors
- Network connectivity issues
- Data integrity checks

### 6.2 Repetition Structure Usage

**Menu Systems (Do-While):**
- Continue until user selects exit
- Redisplay after each operation
- Handle invalid choices gracefully

**Data Processing (For Loops):**
- Process vehicle collections
- Generate sales reports
- File reading operations

**Input Validation (While Loops):**
- Repeat until valid input received
- Handle multiple retry attempts
- Provide helpful error messages

## 7. ADVANTAGES DEMONSTRATION

### 7.1 Complexity Reduction

**Without Modularization:**
- Single large function: 500+ lines
- Complex nested conditions
- Difficult to understand and maintain

**With Modularization:**
- Multiple small functions: 20-50 lines each
- Clear purpose and responsibility
- Easy to understand and test

### 7.2 Code Reusability

**Common Functions:**
- Input validation used across all modules
- File operations shared by all data modules
- Error handling consistent throughout system

**Benefits:**
- Reduced development time
- Consistent behavior
- Easier maintenance

### 7.3 Testing Benefits

**Unit Testing:**
- Each module tested independently
- Specific test cases for each function
- Isolated bug identification

**Integration Testing:**
- Module interaction testing
- Data flow validation
- System-wide functionality verification

---

**Document Status:** Complete - Flowcharts Design Phase
**Marks Target:** 25 marks (LO1: 10 marks, LO2: 15 marks)
**Next Phase:** Setup Development Environment
**Date:** October 12, 2025