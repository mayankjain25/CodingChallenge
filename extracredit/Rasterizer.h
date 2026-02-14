/**
 * Drawing Primitives
 * 
 * Provides low-level drawing functions for rotated ellipses, circles,
 * and grid lines using Windows GDI. Implements parametric ellipse
 * rendering with rotation support.
 * 
 */

#pragma once
#include <windows.h>
#include "Geometry.h"
#include <cmath>

class Rasterizer {
public:
    // Draw a filled circle at the specified position
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
    
    // Draw an ellipse outline with rotation
    static void DrawEllipseOutline(HDC hdc, const EllipseShape& ellipse, COLORREF color, int thickness = 2) {
        if (!ellipse.valid || ellipse.a <= 0 || ellipse.b <= 0) {
            return;
        }
        
        HPEN pen = CreatePen(PS_SOLID, thickness, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        
        // Draw rotated ellipse using parametric form
        const int numPoints = 360;
        POINT* pts = new POINT[numPoints + 1];
        
        double cos_angle = std::cos(ellipse.angle);
        double sin_angle = std::sin(ellipse.angle);
        
        for (int i = 0; i <= numPoints; i++) {
            double t = 2.0 * 3.14159265358979323846 * i / numPoints;
            
            // Parametric ellipse: (a*cos(t), b*sin(t))
            double x_ellipse = ellipse.a * std::cos(t);
            double y_ellipse = ellipse.b * std::sin(t);
            
            // Rotate by angle
            double x_rot = x_ellipse * cos_angle - y_ellipse * sin_angle;
            double y_rot = x_ellipse * sin_angle + y_ellipse * cos_angle;
            
            // Translate to center
            pts[i].x = static_cast<LONG>(ellipse.center.x + x_rot);
            pts[i].y = static_cast<LONG>(ellipse.center.y + y_rot);
        }
        
        Polyline(hdc, pts, numPoints + 1);
        
        delete[] pts;
        
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
