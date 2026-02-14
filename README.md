# Coding Challenge

## Overview
This repository contains solutions to three computational geometry problems involving interactive grid-based visualizations using Windows GDI and Win32 API.

## Problems

### Problem 1 - Circle Rasterization
Interactive program that allows users to draw circles and see which grid points best represent the circle boundary on a discrete 20×20 grid.

**Algorithm**: Distance-based rasterization with adaptive threshold (√2/2 ≈ 0.707 grid units)

### Problem 2 - Best-Fit Circle
Users select grid points by clicking, and the program computes the best-fit circle through those points using least squares optimization.

**Algorithm**: Pratt algebraic circle fitting method with Newton's method iteration

### Extra Credit - Best-Fit Ellipse
Similar to Problem 2, but fits an ellipse instead of a circle, supporting rotation and varying eccentricities.

**Algorithm**: Covariance-based ellipse fitting using Principal Component Analysis (PCA)

## Getting Started

### Pre-built Binaries
**Compiled executables are available in the [Releases](../../releases) section of this repository.**

Download and run the `.exe` files directly on Windows without needing to compile.

### Building from Source
Each problem directory contains:
- Source code (`.cpp` and `.h` files)
- `build.bat` script for compilation
- Detailed `README.md` with build instructions

**Requirements**:
- Windows OS
- MinGW with g++

Navigate to each problem directory and run:
```batch
build.bat
```

## Documentation
- Each problem directory has detailed documentation

## Project Structure
```
Coding Challenge/
├── Problem1/           # Circle rasterization
├── Problem2/           # Best-fit circle
├── extracredit/        # Best-fit ellipse
```
