@echo off
REM Build script for Problem1
REM Requires MinGW

echo Building Problem1...
echo.
echo Using MinGW g++ compiler...
g++ -std=c++11 -O2 -Wall main.cpp -o Problem1.exe -lgdi32 -luser32
if %ERRORLEVEL% EQU 0 (
	echo.
	echo ======================================
	echo Build successful with g++!
	echo Executable: Problem1.exe
	echo ======================================
	echo.
	echo Run the program with: Problem1.exe
) else (
	echo.
	echo Build failed!
	exit /b 1
)
 
