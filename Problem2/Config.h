/**
 * Configuration Constants
 * 
 * Centralizes all configuration constants to eliminate hard-coded values
 * throughout the codebase
 * 
 */

#pragma once
#include <windows.h>

constexpr int GRID_SIZE = 20;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

constexpr int CELL_SIZE = WINDOW_WIDTH / GRID_SIZE;

inline COLORREF GetBackgroundColor() { return RGB(255, 255, 255); }  // White
inline COLORREF GetGridLineColor() { return RGB(200, 200, 200); }    // Light gray
inline COLORREF GetUnselectedColor() { return RGB(220, 220, 220); }  // Light gray (unselected)
inline COLORREF GetSelectedColor() { return RGB(0, 0, 255); }        // Blue
inline COLORREF GetCircleColor() { return RGB(255, 0, 0); }          // Red

constexpr int POINT_RADIUS = 5;
