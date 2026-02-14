# Circle Rasterization Program

## Overview
This program demonstrates circle rasterization on a discrete 20×20 grid. Users can interactively define circles by clicking and dragging, and the program displays which grid points best represent the circle boundary.

## Features
- **Interactive Circle Definition**: Click and drag to define circle center and radius
- **Real-time Preview**: See circle outline while dragging
- **Smart Rasterization**: Algorithm determines which grid points are nearest to the circle boundary
- **Visual Feedback**: 
  - Gray points: Default state
  - Blue points: Rasterized circle representation
  - Blue thick circle: Original user-specified circle
  - Red thin circles: Inner and outer bounds of rasterized points
- **Continuous Coordinate System**: Circle centers are not snapped to grid points, maintaining precision

## Requirements
- **Platform**: Windows (Win32 API)
- **Compiler**: MinGW with g++
- **C++ Standard**: C++11 or later

## Project Structure
```
Meril Coding Challenge/
├── Config.h          - Configuration constants and settings
├── Geometry.h        - Point, Circle, and coordinate transformation classes
├── Grid.h            - Grid management and bounding circle calculations
├── Rasterizer.h      - Circle rasterization algorithm
├── Renderer.h        - Rendering/drawing functions
├── main.cpp          - Application entry point and window management
├── README.md         - This file
└── build.bat         - Build script (optional)
```

## Building the PrograM

### Option 1: Using MinGW/g++

```cmd
g++ -std=c++11 -O2 -Wall main.cpp -o Problem1.exe -lgdi32 -luser32
```

### Option 2: Using Build Script
Execute the `build.bat` file present in the directory and it will compile and create an executable.

## Running the Program

After successful compilation:
```cmd
Problem1.exe
```

Or double-click `Problem1.exe` in File Explorer.

## Usage Instructions

1. **Launch the program** - A window with a 20×20 grid of gray points appears
2. **Draw a circle**:
   - Click and hold the left mouse button to set the circle center
   - Drag to define the radius (you'll see a preview circle)
   - Release the mouse button to rasterize
3. **View results**:
   - Blue points show the rasterized circle
   - Blue thick circle shows your original specification
   - Red thin circles show the inner and outer bounds
4. **Draw another circle**: Simply click and drag again to reset and draw a new circle

## Algorithm Explanation

### Rasterization Approach

The program uses a **distance-based threshold algorithm**:

1. **For each grid point**, calculate the distance from the point to the circle's boundary
2. **Highlight the point** if this distance is within a threshold value
3. **Threshold value**: `sqrt(2)/2 ≈ 0.707` grid units

### Why This Threshold?

- Grid points are spaced 1 unit apart
- The maximum distance from a circle to the nearest grid point occurs when the circle passes through the center of a grid cell
- This distance is `sqrt(0.5² + 0.5²) = 0.707`
- Using this threshold ensures:
  - No gaps in the rasterized circle
  - No excessive over-rasterization
  - Proper handling of non-grid-aligned circles

### Coordinate Systems

The program maintains **two separate coordinate systems**:

1. **Canvas Space**: Pixel coordinates on the window
2. **Grid Space**: Logical coordinates (0-19 for each axis)

The `CoordinateTransform` class handles conversions, ensuring circle centers can be placed at arbitrary positions, not just at grid points.

### Bounding Circles

After rasterization:
- **Inner Circle**: Radius = minimum distance from center to any highlighted point
- **Outer Circle**: Radius = maximum distance from center to any highlighted point

These provide visual feedback on the accuracy of the rasterization.

## Customization

You can modify behavior by editing `Config.h`:

```cpp
// Change grid size
constexpr int GRID_SIZE = 30;  // For a 30×30 grid

// Adjust colors
constexpr COLORREF COLOR_BLUE = RGB(0, 0, 255);  // Pure blue

// Modify rasterization sensitivity
constexpr double RASTERIZATION_THRESHOLD = 0.5;  // Tighter threshold
```
