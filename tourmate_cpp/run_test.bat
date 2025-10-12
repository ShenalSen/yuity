@echo off
echo === Running Tourmate Core Data Structures Test ===
echo.

if not exist "build\tourmate_test.exe" (
    echo Error: Executable not found. Please build the project first.
    echo Run: build.bat
    pause
    exit /b 1
)

echo Running test...
echo.
build\tourmate_test.exe

echo.
echo === Test Completed ===
pause