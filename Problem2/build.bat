@echo off
echo Building Problem 2...
g++ -std=c++17 -mwindows main.cpp -o Problem2.exe -lgdi32 -luser32
if %errorlevel% equ 0 (
    echo Build successful! Run Problem2.exe
) else (
    echo Build failed!
)
