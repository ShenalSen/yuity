@echo off
echo === Running Tourmate Vehicle Management Test ===

REM Check if executable exists
if not exist "build\tourmate_vehicle_test.exe" (
    echo Error: Vehicle test executable not found.
    echo Please run build.bat first to compile the project.
    pause
    exit /b 1
)

echo Starting Vehicle Management System...
echo.

REM Run the vehicle management test
build\tourmate_vehicle_test.exe

echo.
echo Vehicle Management Test completed.
pause