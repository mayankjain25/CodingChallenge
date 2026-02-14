# Problem 2 - Best Fit Circle

## Objective
This program allows users to click grid points and generates the best fit circle through the selected points.

## Features
- 20x20 grid display
- Click grid points to toggle between blue (selected) and gray (unselected)
- Press **G** to generate and display the best fit circle (red)
- Press **C** to clear all selections and return to the original state

## Building
Run the build script:
```batch
build.bat
```

This will compile the program using g++

## Running
After building, run:
```batch
Problem2.exe
```

## Requirements
- Windows OS
- G++
- At least 3 points must be selected to generate a circle

## Usage
1. Click on grid intersection points to select them (they will turn blue)
2. Click again on a selected point to deselect it (it will turn gray)
3. Once you have at least 3 points selected, press **G** to generate the best fit circle
4. The circle (in red) will be drawn through the selected points
5. Press **C** to clear all selections and start over

## Algorithm
The program uses the Pratt algebraic circle fitting method, which:
1. Translates points to the centroid
2. Calculates statistical moments
3. Uses Newton's method to solve for the circle parameters
4. Returns the circle with center and radius that best fits all selected points

## Files
- `main.cpp` - Main program with Win32 window handling
- `Config.h` - Configuration constants
- `Geometry.h` - Geometric structures and circle fitting algorithm
- `Grid.h` - Grid point management
- `Rasterizer.h` - Drawing primitives
- `Renderer.h` - Rendering system
- `build.bat` - Build script
