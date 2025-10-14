# TOUR MATE Vehicle Management System - Flow Charts

## 1. Main System Flow Chart

```
START
|
V
Display Welcome Screen
|
V
Display Login Menu
|
V
< Login Successful? > --- No ---> Display Error Message
|                                 |
| Yes                             V
|                                 Return to Login Menu
V
Display Main Menu
|
V
< User Selection > --- Exit ---> Logout and Exit Program
|                                |
| Other Options                   V
|                                 END
V
Execute Selected Function
|
V
Return to Main Menu (unless Logout/Exit selected)
```

## 2. Login Module Flow Chart

```
START
|
V
Display Login Menu
|
V
< User Selection > --- Exit ---> Return to Main Program
|
| Login
V
Get Username and Password
|
V
< Valid Credentials? > --- No ---> Display Error Message
|                                 |
| Yes                             V
|                                 Return to Login Menu
V
Set Current User and Role
|
V
Set isLoggedIn = true
|
V
Return to Main Program
|
V
END
```

## 3. Vehicle Management Flow Chart

```
START
|
V
Display Vehicle Menu
|
V
< User Selection >
|
|--- View All Vehicles ---> Load Vehicles from File ---> Display Vehicles ---> Return to Vehicle Menu
|
|--- Add Vehicle ---> Get Vehicle Details ---> Add to Vehicle List ---> Save to File ---> Return to Vehicle Menu
|
|--- Update Vehicle ---> Get Vehicle ID ---> < Vehicle Exists? > --- No ---> Display Error ---> Return to Vehicle Menu
|                                             | 
|                                             | Yes
|                                             V
|                                           Get New Details ---> Update Vehicle ---> Save to File ---> Return to Vehicle Menu
|
|--- Delete Vehicle ---> Get Vehicle ID ---> < Vehicle Exists? > --- No ---> Display Error ---> Return to Vehicle Menu
|                                             |
|                                             | Yes
|                                             V
|                                           Confirm Deletion ---> < Confirmed? > --- No ---> Return to Vehicle Menu
|                                                                  |
|                                                                  | Yes
|                                                                  V
|                                                                Delete Vehicle ---> Save to File ---> Return to Vehicle Menu
|
|--- Search Vehicle ---> Get Search Criteria ---> Search in Vehicle List ---> Display Results ---> Return to Vehicle Menu
|
|--- Return to Main Menu ---> END
```

## 4. Sales Management Flow Chart

```
START
|
V
Display Sales Menu
|
V
< User Selection >
|
|--- Record New Sale ---> < Any Vehicles Available? > --- No ---> Display Error ---> Return to Sales Menu
|                            |
|                            | Yes
|                            V
|                          Get Sale Details ---> Add to Sales List ---> Update Vehicle Status ---> Save to Files ---> Return to Sales Menu
|
|--- View All Sales ---> Load Sales from File ---> Display Sales ---> Return to Sales Menu
|
|--- Search Sales ---> Get Search Criteria ---> Search in Sales List ---> Display Results ---> Return to Sales Menu
|
|--- Generate Sales Report ---> Calculate Statistics ---> Display Report ---> Export Report to File ---> Return to Sales Menu
|
|--- Return to Main Menu ---> END
```

## 5. Data File Operations Flow Chart

```
START
|
V
< Operation Type >
|
|--- Load Data ---> Open File ---> < File Exists? > --- No ---> Return Empty List
|                                    |
|                                    | Yes
|                                    V
|                                  Read Line by Line ---> Parse Data ---> Add to List ---> Return List
|
|--- Save Data ---> Open File ---> < File Opened Successfully? > --- No ---> Display Error Message ---> END
|                                    |
|                                    | Yes
|                                    V
|                                  For Each Item in List ---> Convert to String ---> Write to File ---> Close File ---> END
```

## 6. Error Handling Flow Chart

```
START
|
V
< Error Type >
|
|--- Input Validation Error ---> Display Specific Error Message ---> Clear Input Buffer ---> Request Input Again
|
|--- File Operation Error ---> Display Error Message ---> Suggest Alternative Action
|
|--- Invalid Selection Error ---> Display Error Message ---> Show Menu Options Again
|
V
END
```

## Explanation of Controlling Structures and Modularization

### Controlling Structures

1. **Selection Structures** used in the system:
   - If-else statements for conditional logic (e.g., checking if credentials are valid)
   - Switch statements for menu selections (e.g., handling different menu options)
   - Conditional operators for simple checks (e.g., checking if a list is empty)

2. **Repetition Structures** used in the system:
   - While loops for menu navigation (e.g., main program loop)
   - For loops for iterating through lists (e.g., displaying all vehicles)
   - Do-while loops for input validation (ensuring valid input before proceeding)

### Advantages of Modularization

1. **Improved Readability**: Breaking the system into modules (authentication, vehicle management, sales management) makes the code more readable and easier to understand.

2. **Easier Maintenance**: When changes are needed, they can be made to specific modules without affecting the entire system.

3. **Reusability**: Functions and classes can be reused across different parts of the system (e.g., file operations).

4. **Collaboration**: Different team members can work on different modules simultaneously.

5. **Testing**: Individual modules can be tested separately, making it easier to identify and fix issues.

6. **Scalability**: New features can be added as new modules without disrupting existing functionality.

The TOUR MATE system utilizes modularization by separating concerns into different classes and functions:
- Vehicle class for vehicle management
- User class for authentication
- Sales class for sales management
- Separate functions for file operations
- Menu handling functions for user interface

This approach reduces complexity by breaking the system into manageable components that can be developed, tested, and maintained independently.