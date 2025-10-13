@echo off
echo === Running Tourmate Core Data Structures Test ===
echo.

if not exist "build\tourmate_core_test.exe" (
    echo Error: Core test executable not found. Please build the project first.
    echo Run: build.bat
    pause
    exit /b 1
)

echo Running core data structures test...
echo.
build\tourmate_core_test.exe

echo.
echo === Core Test Completed ===
pause