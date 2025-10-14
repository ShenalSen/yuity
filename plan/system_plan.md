# TOUR MATE - Vehicle Management System Plan

## 1. System Components

### Data Structures
- Vehicle class with attributes:
  - Vehicle ID
  - Make/Model
  - Year
  - Type (Sedan, SUV, etc.)
  - Registration Number
  - Status (Available, In maintenance, etc.)
  - Rate per hour/day

- User class with attributes:
  - Username
  - Password
  - Role (Admin/Staff)

- Sales record with attributes:
  - Sale ID
  - Vehicle ID
  - Customer details
  - Start date/time
  - End date/time
  - Amount
  - Payment status

### File Storage
- vehicles.txt - Store vehicle details
- users.txt - Store user login credentials
- sales.txt - Store sales records
- company_info.txt - Store company details

## 2. Program Modules

### Authentication Module
- Login function
- Logout function
- User validation

### Vehicle Management Module
- View all vehicles
- Add new vehicle
- Update vehicle details
- Delete vehicle
- Search vehicle by ID or other criteria

### Sales Management Module
- Record new sale
- View all sales
- Search sales by date, vehicle, or customer
- Generate sales reports

### Main Module
- Main menu display
- User input handling
- Navigation between modules
- Exit program

## 3. Program Flow

1. Start program
2. Display welcome screen
3. Prompt for login
4. Based on login, show appropriate menu
5. Handle user's menu selection
6. Execute selected function
7. Return to menu or logout
8. Exit program

## 4. Error Handling

- Input validation for all user inputs
- File operation error handling
- Login attempt limitations
- Data integrity checks

## 5. User Interface

- Console-based menu system
- Clear prompts and messages
- Consistent formatting
- Informative error messages
- Confirmation for critical operations (delete, update)