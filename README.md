# TOUR MATE Vehicle Management System

## Overview

This is a console-based vehicle management system for TOUR MATE cab services. The application allows management of vehicle details and sales records, with user authentication for security.

## Features

- **User Authentication**
  - Login/logout functionality
  - Role-based access

- **Vehicle Management**
  - View all vehicles
  - Add new vehicles
  - Update vehicle details
  - Delete vehicles
  - Search vehicles

- **Sales Management**
  - Record new sales
  - View all sales
  - Search sales
  - Generate sales reports

- **Other Features**
  - View company details
  - User-friendly menus and navigation
  - Data persistence using file storage

## Project Structure

- `src/` - Contains all source code files
  - `main.cpp` - Main program file
  - `vehicle.h/cpp` - Vehicle class and vehicle management functions
  - `user.h/cpp` - User class and authentication functions
  - `sales.h/cpp` - Sales class and sales management functions
  - `Makefile` - Compilation instructions

- `plan/` - System design documents
  - `system_plan.md` - System components and modules
  - `flowcharts.md` - Flow diagrams for program logic

- `testing/` - Testing documentation
  - `test_plan.md` - Test plan, cases, and user acceptance testing

## How to Compile

1. Navigate to the `src` directory
2. Run `make` command to compile the program
3. Execute `tourmate` to run the program

## Default Login

- Username: admin
- Password: admin123

## File Storage

The application uses text files for data storage:
- `vehicles.txt` - Stores vehicle information
- `users.txt` - Stores user credentials
- `sales.txt` - Stores sales records

## Assessment Information

This project is created for the CSE4002 - Fundamentals in Programming module assessment. The requirements include:

1. Design flow charts for the system (Task 1)
2. Develop a functional C++ program (Task 2)
3. Prepare a test document (Task 3)

## Author

[Your Name]
Student ID: [Your Student ID]