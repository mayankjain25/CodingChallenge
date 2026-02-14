@echo off
echo Building Extra Credit - Best Fit Ellipse...
g++ -std=c++17 -mwindows main.cpp -o ExtraCredit.exe -lgdi32 -luser32
if %errorlevel% equ 0 (
    echo Build successful! Run ExtraCredit.exe
) else (
    echo Build failed!
)
