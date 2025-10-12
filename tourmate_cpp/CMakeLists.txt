# Tourmate Vehicle Management System - CMake Configuration
# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project information
project(TourmateMgmtSystem 
        VERSION 1.0.0 
        DESCRIPTION "Tourmate Vehicle Management System"
        LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(src/headers)

# Find all source files
file(GLOB_RECURSE SOURCES "src/source/*.cpp")
file(GLOB_RECURSE HEADERS "src/headers/*.h")

# Create executable
add_executable(tourmate ${SOURCES} ${HEADERS})

# Set output directory
set_target_properties(tourmate PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build"
)

# Platform-specific settings
if(WIN32)
    # Windows specific settings
    set_target_properties(tourmate PROPERTIES
        OUTPUT_NAME "tourmate.exe"
    )
elseif(UNIX)
    # Unix/Linux specific settings
    set_target_properties(tourmate PROPERTIES
        OUTPUT_NAME "tourmate"
    )
endif()

# Custom targets for data management
add_custom_target(init-data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data/backup
    COMMAND ${CMAKE_COMMAND} -E echo "VehicleID,LicensePlate,Model,Year,Type,SeatingCapacity,Status,RegistrationDate,LastServiceDate" > ${CMAKE_BINARY_DIR}/data/vehicles.csv
    COMMAND ${CMAKE_COMMAND} -E echo "SalesID,VehicleID,CustomerName,CustomerContact,StartLocation,EndLocation,TripDate,TripTime,Duration,FareAmount,PaymentStatus,SalesRep" > ${CMAKE_BINARY_DIR}/data/sales.csv
    COMMAND ${CMAKE_COMMAND} -E echo "UserID,Username,Password,Role,LastLogin,AccountStatus" > ${CMAKE_BINARY_DIR}/data/users.csv
    COMMAND ${CMAKE_COMMAND} -E echo "CompanyName,Address,Phone,Email,Website,EstablishedYear,TotalVehicles,SystemVersion" > ${CMAKE_BINARY_DIR}/data/company.csv
    COMMENT "Initializing data files..."
)

# Custom target for running the application
add_custom_target(run
    COMMAND tourmate
    DEPENDS tourmate
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build
    COMMENT "Running Tourmate Vehicle Management System..."
)

# Installation rules
install(TARGETS tourmate
    RUNTIME DESTINATION bin
)

# CPack configuration for creating packages
include(CPack)
set(CPACK_PACKAGE_NAME "TourmateMgmtSystem")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tourmate Vehicle Management System")
set(CPACK_PACKAGE_VENDOR "Student")
set(CPACK_PACKAGE_CONTACT "student@example.com")

# Enable testing
enable_testing()

# Add a simple test
add_test(NAME BasicTest
    COMMAND tourmate --test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

# Print build information
message(STATUS "=== Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCES}")
message(STATUS "=============================")