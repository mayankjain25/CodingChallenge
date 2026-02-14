/**
 * Renderer.h - Double-Buffered Rendering System
 * 
 * Manages off-screen rendering to eliminate flicker. Coordinates
 * the rendering of grid, points, and ellipses.
 * 
 */

#pragma once
#include <windows.h>
#include "Config.h"
#include "Grid.h"
#include "Rasterizer.h"
#include "Geometry.h"

class Renderer {
private:
    HWND hwnd;
    HDC hdcMem;
    HBITMAP hbmMem;
    HBITMAP hbmOld;
    int width;
    int height;
    
public:
    Renderer(HWND hwnd, int width, int height) 
        : hwnd(hwnd), width(width), height(height) {
        HDC hdc = GetDC(hwnd);
        hdcMem = CreateCompatibleDC(hdc);
        hbmMem = CreateCompatibleBitmap(hdc, width, height);
        hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
        ReleaseDC(hwnd, hdc);
    }
    
    ~Renderer() {
        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
    }
    
    void Render(const Grid& grid, const EllipseShape* bestFitEllipse = nullptr) {
        // Clear background
        RECT rect = {0, 0, width, height};
        HBRUSH bgBrush = CreateSolidBrush(GetBackgroundColor());
        FillRect(hdcMem, &rect, bgBrush);
        DeleteObject(bgBrush);
        
        // Draw grid lines
        Rasterizer::DrawGrid(hdcMem, GRID_SIZE, CELL_SIZE, GetGridLineColor());
        
        // Draw all grid points
        for (int i = 0; i < grid.GetSize(); i++) {
            for (int j = 0; j < grid.GetSize(); j++) {
                const GridPoint& gp = grid.GetPoint(i, j);
                Point pixelPos = gp.GetPixelCoords();
                
                COLORREF color = gp.selected ? GetSelectedColor() : GetUnselectedColor();
                Rasterizer::DrawFilledCircle(
                    hdcMem, 
                    static_cast<int>(pixelPos.x), 
                    static_cast<int>(pixelPos.y), 
                    POINT_RADIUS, 
                    color
                );
            }
        }
        
        // Draw best fit ellipse if available
        if (bestFitEllipse && bestFitEllipse->valid) {
            Rasterizer::DrawEllipseOutline(hdcMem, *bestFitEllipse, GetEllipseColor(), 2);
        }
    }
    
    void Present() {
        HDC hdc = GetDC(hwnd);
        BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, hdc);
    }
};
