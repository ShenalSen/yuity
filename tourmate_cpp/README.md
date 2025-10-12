# Tourmate Vehicle Management System - Development Environment Setup

## Project Overview
**Project Name:** Tourmate Vehicle Management System  
**Language:** C++  
**Interface:** Console-based Menu-driven Application  
**Assignment:** CSE4002 WRIT1 (100% weighting)  

## Directory Structure

```
tourmate_cpp/
├── src/                     # Source code directory
│   ├── headers/            # Header files (.h)
│   │   ├── Vehicle.h
│   │   ├── Sales.h
│   │   ├── User.h
│   │   ├── AuthManager.h
│   │   ├── VehicleManager.h
│   │   ├── SalesManager.h
│   │   ├── CompanyManager.h
│   │   ├── FileManager.h
│   │   ├── MenuManager.h
│   │   ├── InputValidator.h
│   │   ├── ErrorHandler.h
│   │   └── Utilities.h
│   ├── source/             # Source files (.cpp)
│   │   ├── main.cpp
│   │   ├── Vehicle.cpp
│   │   ├── Sales.cpp
│   │   ├── User.cpp
│   │   ├── AuthManager.cpp
│   │   ├── VehicleManager.cpp
│   │   ├── SalesManager.cpp
│   │   ├── CompanyManager.cpp
│   │   ├── FileManager.cpp
│   │   ├── MenuManager.cpp
│   │   ├── InputValidator.cpp
│   │   ├── ErrorHandler.cpp
│   │   └── Utilities.cpp
├── data/                   # Data storage directory
│   ├── vehicles.csv        # Vehicle records
│   ├── sales.csv          # Sales transactions
│   ├── users.csv          # User accounts
│   ├── company.csv        # Company information
│   └── backup/            # Backup files
│       ├── vehicles_backup.csv
│       ├── sales_backup.csv
│       └── users_backup.csv
├── docs/                  # Documentation
│   ├── README.md
│   ├── USER_MANUAL.md
│   ├── TECHNICAL_DOCS.md
│   └── API_REFERENCE.md
├── tests/                 # Test files
│   ├── test_data/
│   ├── unit_tests/
│   └── integration_tests/
├── build/                 # Compiled binaries
│   └── tourmate.exe
├── Makefile              # Build configuration
├── CMakeLists.txt        # CMake configuration
└── README.md             # Project overview
```

## Development Environment Requirements

### Compiler Requirements
- **C++ Compiler:** GCC 7.0+ or MSVC 2017+ or Clang 5.0+
- **C++ Standard:** C++11 or later
- **Platform:** Windows/Linux/macOS compatible

### IDE Recommendations
- **Primary:** Visual Studio Code with C++ extension
- **Alternative:** Visual Studio Community, Code::Blocks, Dev-C++
- **Command Line:** GCC/G++ with Makefile

### Required Libraries
- **Standard Library:** iostream, fstream, string, vector, algorithm
- **Additional:** sstream, iomanip, ctime, cstdlib

## File Naming Conventions

### Header Files (.h)
- **Classes:** CamelCase (e.g., `VehicleManager.h`)
- **Structs:** CamelCase (e.g., `Vehicle.h`)
- **Utilities:** Descriptive names (e.g., `InputValidator.h`)

### Source Files (.cpp)
- **Implementation:** Match header names (e.g., `VehicleManager.cpp`)
- **Main:** `main.cpp`
- **Tests:** `test_[module].cpp`

### Data Files (.csv)
- **Lowercase:** snake_case (e.g., `vehicles.csv`)
- **Backups:** `[filename]_backup.csv`
- **Timestamps:** `[filename]_YYYYMMDD_HHMMSS.csv`

### Documentation Files
- **Markdown:** UPPERCASE for main docs (e.g., `README.md`)
- **Technical:** Descriptive names (e.g., `API_REFERENCE.md`)

## Coding Standards

### General Guidelines
- **Indentation:** 4 spaces (no tabs)
- **Line Length:** Maximum 100 characters
- **Encoding:** UTF-8
- **Line Endings:** LF (Unix style)

### Naming Conventions
```cpp
// Classes: PascalCase
class VehicleManager {
    // Public methods: camelCase
    public:
        void addVehicle();
        bool validateInput();
    
    // Private members: camelCase with underscore prefix
    private:
        int vehicleCount_;
        string fileName_;
};

// Constants: UPPER_SNAKE_CASE
const int MAX_VEHICLES = 1000;
const string DATA_FILE = "vehicles.csv";

// Variables: camelCase
int vehicleId;
string licensePlate;
bool isValid;

// Functions: camelCase
void displayMenu();
bool saveToFile();
```

### Header Guards
```cpp
#ifndef VEHICLE_MANAGER_H
#define VEHICLE_MANAGER_H

// Header content here

#endif // VEHICLE_MANAGER_H
```

### Include Order
```cpp
// System includes
#include <iostream>
#include <fstream>
#include <string>

// Project includes
#include "Vehicle.h"
#include "FileManager.h"
```

## Build Configuration

### Makefile Structure
```makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCDIR = src/source
HEADERDIR = src/headers
BUILDDIR = build
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/tourmate

# Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/*

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install
```

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)
project(TourmateMgmtSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(src/headers)

# Source files
file(GLOB SOURCES "src/source/*.cpp")

# Executable
add_executable(tourmate ${SOURCES})

# Compiler flags
target_compile_options(tourmate PRIVATE -Wall -Wextra -O2)
```

## Development Workflow

### Phase 1: Setup and Core Structures
1. Create all header files with class declarations
2. Implement basic data structures (Vehicle, Sales, User)
3. Set up file I/O operations
4. Create basic menu system

### Phase 2: Module Implementation
1. Authentication module
2. Vehicle management module
3. Sales management module
4. Company information module

### Phase 3: Integration and Testing
1. Integrate all modules
2. Implement error handling
3. Add input validation
4. Create test cases

### Phase 4: Documentation and Finalization
1. Code documentation
2. User manual
3. Technical documentation
4. Final testing and cleanup

## Git Workflow (Optional)

### Branch Structure
- `main`: Production-ready code
- `development`: Integration branch
- `feature/[module-name]`: Individual feature development

### Commit Guidelines
```
feat: Add vehicle management module
fix: Resolve file reading error in CSV parser
docs: Update API documentation
test: Add unit tests for authentication
refactor: Improve error handling structure
```

## Quality Assurance

### Code Review Checklist
- [ ] Follows coding standards
- [ ] Proper error handling
- [ ] Memory management (no leaks)
- [ ] Input validation
- [ ] Documentation completeness
- [ ] Test coverage

### Testing Strategy
- **Unit Tests:** Individual function testing
- **Integration Tests:** Module interaction testing
- **System Tests:** End-to-end functionality
- **User Acceptance Tests:** Real-world usage scenarios

## Performance Considerations

### File I/O Optimization
- Minimize file operations
- Use buffered reading/writing
- Implement lazy loading for large datasets
- Cache frequently accessed data

### Memory Management
- Use stack allocation when possible
- Proper cleanup of dynamic allocations
- Avoid memory leaks with RAII principles
- Efficient data structures selection

## Security Guidelines

### Input Validation
- Validate all user inputs
- Sanitize file paths
- Check data ranges and formats
- Prevent buffer overflows

### Data Protection
- Basic password encryption
- File access permissions
- Audit trail for critical operations
- Secure default configurations

---

**Setup Status:** Complete - Development Environment Ready
**Next Phase:** Implement Core Data Structures
**Date:** October 12, 2025