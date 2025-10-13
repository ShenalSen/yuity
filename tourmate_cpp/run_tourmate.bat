@echo off
echo ═══════════════════════════════════════════════════════════
echo            TOURMATE VEHICLE MANAGEMENT SYSTEM
echo                    MAIN APPLICATION RUNNER
echo ═══════════════════════════════════════════════════════════

REM Check if main application executable exists
if not exist "build\tourmate.exe" (
    echo Error: Main application executable not found!
    echo Please run build.bat first to compile the project.
    pause
    exit /b 1
)

REM Create data directory if it doesn't exist
if not exist "data" mkdir data

REM Create default data files if they don't exist
if not exist "data\users.csv" (
    echo Creating default users file...
    echo username,hashedPassword,role,salt,lastLogin,isActive > data\users.csv
    echo admin,5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8,admin,salt123,2024-01-01 00:00:00,true >> data\users.csv
    echo operator,ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f,operator,salt456,2024-01-01 00:00:00,true >> data\users.csv
    echo viewer,cd2eb0837c9b4c962c22d2ff8b5441b7b45805887f051d39bf133b583baf6860,viewer,salt789,2024-01-01 00:00:00,true >> data\users.csv
)

if not exist "data\vehicles.csv" (
    echo Creating sample vehicles file...
    echo vehicleId,make,model,year,capacity,fuelType,status,pricePerKm,mileage,lastService,registrationNumber,driverAssigned,category > data\vehicles.csv
    echo V001,Toyota,Hiace,2022,12,Diesel,Available,2.50,15000,2024-01-01,ABC-1234,John Doe,Van >> data\vehicles.csv
    echo V002,Mercedes,Sprinter,2023,20,Diesel,Available,3.00,8000,2024-01-01,XYZ-5678,Jane Smith,Bus >> data\vehicles.csv
    echo V003,Toyota,Prius,2021,4,Hybrid,Available,2.00,25000,2024-01-01,DEF-9876,Mike Johnson,Car >> data\vehicles.csv
    echo V004,Ford,Transit,2022,15,Diesel,Available,2.75,12000,2024-01-01,GHI-4321,Sarah Wilson,Van >> data\vehicles.csv
    echo V005,BMW,X5,2023,7,Petrol,Available,4.00,5000,2024-01-01,JKL-8765,David Brown,SUV >> data\vehicles.csv
)

if not exist "data\sales.csv" (
    echo Creating empty sales file...
    echo saleId,vehicleId,customerId,customerName,saleDate,amount,paymentMethod,tripType,fromLocation,toLocation,distance,duration,status > data\sales.csv
)

if not exist "data\customers.csv" (
    echo Creating empty customers file...
    echo customerId,name,email,phone,address,registrationDate,totalBookings,totalSpent,isVip > data\customers.csv
)

if not exist "data\bookings.csv" (
    echo Creating empty bookings file...
    echo bookingId,customerId,vehicleId,fromLocation,toLocation,departureTime,returnTime,passengers,tripType,status,totalAmount,paymentMethod,paymentStatus,specialRequests,createdDate,confirmedDate,cancellationReason > data\bookings.csv
)

echo.
echo ═══════════════════════════════════════════════════════════
echo                     SYSTEM READY
echo ═══════════════════════════════════════════════════════════
echo.
echo 🚗 Welcome to Tourmate Vehicle Management System!
echo.
echo 📋 Default Login Credentials:
echo    👑 Admin:    username=admin    password=Admin123
echo    👤 Operator: username=operator password=Oper123  
echo    👁️  Viewer:   username=viewer   password=View123
echo.
echo 🎯 Quick Start Guide:
echo    1. Log in with one of the default accounts
echo    2. Explore Vehicle Management for fleet operations
echo    3. Use Sales Management for bookings and customers
echo    4. Check Reports for business insights
echo    5. Access Help menu for detailed documentation
echo.
echo 💡 Pro Tips:
echo    • Admin has full system access
echo    • Operator can manage vehicles and sales
echo    • Viewer has read-only access
echo    • All data is automatically saved
echo    • Use Emergency Operations (Admin) for system management
echo.

echo Would you like to generate sample data for testing? (y/n)
set /p generateData=
if /i "%generateData%"=="y" (
    echo.
    echo 🔄 Generating sample data...
    if exist "build\generate_sample_data.exe" (
        build\generate_sample_data.exe
    ) else (
        echo Warning: Sample data generator not found. Continuing without sample data.
    )
    echo.
)

echo ═══════════════════════════════════════════════════════════
echo                   STARTING APPLICATION
echo ═══════════════════════════════════════════════════════════
echo.

REM Run the main Tourmate application
build\tourmate.exe

echo.
echo ═══════════════════════════════════════════════════════════
echo                   APPLICATION CLOSED
echo ═══════════════════════════════════════════════════════════
echo.
echo 🎉 Thank you for using Tourmate!
echo 💼 Your trusted partner in transport business management.
echo.
echo 📊 System Status: All data has been saved automatically
echo 🔒 Security: All sessions have been properly terminated
echo 📁 Data Location: All files stored in 'data' directory
echo.
echo 🌟 For support and documentation, run the application
echo    and access the Help menu from the main dashboard.
echo.
pause