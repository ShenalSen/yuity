@echo off
echo === Building Tourmate C++ Project ===

REM Check if g++ is available
g++ --version >nul 2>&1
if errorlevel 1 (
    echo Error: g++ compiler not found. Please install MinGW or MSYS2.
    echo Download from: https://www.msys2.org/
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build

echo.
echo Compiling source files...

REM Compile each source file to object files
echo - Compiling Vehicle.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/Vehicle.cpp" -o "build/Vehicle.o"
if errorlevel 1 goto error

echo - Compiling Sales.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/Sales.cpp" -o "build/Sales.o"
if errorlevel 1 goto error

echo - Compiling User.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/User.cpp" -o "build/User.o"
if errorlevel 1 goto error

echo - Compiling Utilities.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/Utilities.cpp" -o "build/Utilities.o"
if errorlevel 1 goto error

echo - Compiling AuthenticationManager.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/AuthenticationManager.cpp" -o "build/AuthenticationManager.o"
if errorlevel 1 goto error

echo - Compiling VehicleManager.cpp...
g++ -c -std=c++11 -I"src/headers" "src/source/VehicleManager.cpp" -o "build/VehicleManager.o"
if errorlevel 1 goto error

echo.
echo Linking executables...

REM Core test executable
echo - Linking core test...
g++ -c -std=c++11 -I"src/headers" "test_core.cpp" -o "build/test_core.o"
if errorlevel 1 goto error

g++ -std=c++11 build/Vehicle.o build/Sales.o build/User.o build/Utilities.o build/test_core.o -o build/tourmate_core_test.exe
if errorlevel 1 goto error

REM Authentication test executable
echo - Linking authentication test...
g++ -c -std=c++11 -I"src/headers" "test_authentication.cpp" -o "build/test_authentication.o"
if errorlevel 1 goto error

g++ -std=c++11 build/Vehicle.o build/Sales.o build/User.o build/Utilities.o build/AuthenticationManager.o build/test_authentication.o -o build/tourmate_auth_test.exe
if errorlevel 1 goto error

REM Vehicle management test executable
echo - Linking vehicle management test...
g++ -c -std=c++11 -I"src/headers" "test_vehicle.cpp" -o "build/test_vehicle.o"
if errorlevel 1 goto error

g++ -std=c++11 build/Vehicle.o build/Sales.o build/User.o build/Utilities.o build/AuthenticationManager.o build/VehicleManager.o build/test_vehicle.o -o build/tourmate_vehicle_test.exe
if errorlevel 1 goto error

echo.
echo === Build Successful! ===
echo Executables created:
echo   - build/tourmate_core_test.exe (Core data structures test)
echo   - build/tourmate_auth_test.exe (Authentication module test)
echo   - build/tourmate_vehicle_test.exe (Vehicle management test)
echo.
echo To run tests:
echo   - Core test:           build\tourmate_core_test.exe
echo   - Authentication test: build\tourmate_auth_test.exe
echo   - Vehicle test:        build\tourmate_vehicle_test.exe
echo.
pause
exit /b 0

:error
echo.
echo === Build Failed! ===
echo Check the error messages above.
pause
exit /b 1