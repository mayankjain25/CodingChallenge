# Extra Credit - Best Fit Ellipse

## Objective
This program allows users to click grid points and generates the best fit ellipse through the selected points.

## Features
- 20x20 grid display
- Click grid points to toggle between blue (selected) and gray (unselected)
- Press **G** to generate and display the best fit ellipse (red)
- Press **C** to clear all selections and return to the original state

## Building
Run the build script:
```batch
build.bat
```

This will compile the program using g++ (MinGW).

## Running
After building, run:
```batch
ExtraCredit.exe
```

## Requirements
- Windows OS
- MinGW with g++ compiler
- At least 5 points must be selected to generate an ellipse

## Usage
1. Click on grid intersection points to select them (they will turn blue)
2. Click again on a selected point to deselect it (it will turn gray)
3. Once you have at least 5 points selected, press **G** to generate the best fit ellipse
4. The ellipse (in red) will be drawn with the optimal fit for all selected points
5. Press **C** to clear all selections and start over

## Algorithm
The program uses a **covariance-based ellipse fitting method**:
1. Calculates the centroid (mean) of all selected points
2. Computes the covariance matrix to determine the principal axes
3. Finds the rotation angle from the covariance
4. Calculates the semi-major (a) and semi-minor (b) axes
5. Renders the rotated ellipse

The algorithm handles:
- Rotated ellipses (not just axis-aligned)
- Varying eccentricities (from nearly circular to highly elongated)
- Robust fitting that minimizes errors across all points

## Files
- `main.cpp` - Main program with Win32 window handling
- `Config.h` - Configuration constants
- `Geometry.h` - Geometric structures and ellipse fitting algorithm
- `Grid.h` - Grid point management
- `Rasterizer.h` - Drawing primitives for ellipses
- `Renderer.h` - Rendering system
- `build.bat` - Build script
