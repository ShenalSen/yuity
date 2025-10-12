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

echo - Compiling test_core.cpp...
g++ -c -std=c++11 -I"src/headers" "test_core.cpp" -o "build/test_core.o"
if errorlevel 1 goto error

echo.
echo Linking executable...
g++ -std=c++11 build/Vehicle.o build/Sales.o build/User.o build/Utilities.o build/test_core.o -o build/tourmate_test.exe
if errorlevel 1 goto error

echo.
echo === Build Successful! ===
echo Executable created: build/tourmate_test.exe
echo.
echo To run the test: build\tourmate_test.exe
echo.
pause
exit /b 0

:error
echo.
echo === Build Failed! ===
echo Check the error messages above.
pause
exit /b 1