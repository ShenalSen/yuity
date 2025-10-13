@echo off
echo === Running Tourmate Authentication Module Test ===
echo.

if not exist "build\tourmate_auth_test.exe" (
    echo Error: Authentication test executable not found. Please build the project first.
    echo Run: build.bat
    pause
    exit /b 1
)

echo Starting authentication module test...
echo This is an interactive test with a menu system.
echo.
build\tourmate_auth_test.exe

echo.
echo === Authentication Test Completed ===
pause