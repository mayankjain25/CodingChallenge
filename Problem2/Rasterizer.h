/**
 * Drawing Primitives
 * 
 * Provides low-level drawing functions for circles and grid lines.
 * 
 */

#pragma once
#include <windows.h>
#include "Geometry.h"
#include <cmath>

class Rasterizer {
public:
    static void DrawFilledCircle(HDC hdc, int cx, int cy, int radius, COLORREF color) {
        HBRUSH brush = CreateSolidBrush(color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, 1, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        
        Ellipse(hdc, cx - radius, cy - radius, cx + radius, cy + radius);
        
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
        DeleteObject(brush);
    }
    
    static void DrawCircleOutline(HDC hdc, const Circle& circle, COLORREF color, int thickness = 2) {
        int cx = static_cast<int>(circle.center.x);
        int cy = static_cast<int>(circle.center.y);
        int radius = static_cast<int>(circle.radius);
        
        HPEN pen = CreatePen(PS_SOLID, thickness, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        
        Arc(hdc, 
            cx - radius, cy - radius, 
            cx + radius, cy + radius,
            cx + radius, cy,  // Start point
            cx + radius, cy); // End point (same as start = full circle)
        
        Ellipse(hdc, cx - radius, cy - radius, cx + radius + 1, cy + radius + 1);
        
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
    
    // Draw grid lines
    static void DrawGrid(HDC hdc, int gridSize, int cellSize, COLORREF color) {
        HPEN pen = CreatePen(PS_SOLID, 1, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        
        int width = gridSize * cellSize;
        int height = gridSize * cellSize;
        
        // Draw vertical lines
        for (int i = 0; i <= gridSize; i++) {
            int x = i * cellSize;
            MoveToEx(hdc, x, 0, NULL);
            LineTo(hdc, x, height);
        }
        
        // Draw horizontal lines
        for (int i = 0; i <= gridSize; i++) {
            int y = i * cellSize;
            MoveToEx(hdc, 0, y, NULL);
            LineTo(hdc, width, y);
        }
        
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
};
