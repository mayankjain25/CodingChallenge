#ifndef RENDERER_H
#define RENDERER_H

#include "Config.h"
#include "Grid.h"
#include "Geometry.h"

/**
 * Handles all rendering operations for the application.
 * Separates drawing logic from application logic.
 */
class Renderer {
private:
    HDC hdc;
    
    /**
     * Draw a filled circle at the specified position.
     */
    void drawFilledCircle(int centerX, int centerY, int radius, COLORREF color) {
        HBRUSH brush = CreateSolidBrush(color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, 1, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        
        Ellipse(hdc, centerX - radius, centerY - radius,
                centerX + radius, centerY + radius);
        
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
        DeleteObject(brush);
    }
    
    /**
     * Draw a circle outline with specified pen width.
     */
    void drawCircleOutline(int centerX, int centerY, int radius, COLORREF color, int penWidth) {
        HPEN pen = CreatePen(PS_SOLID, penWidth, color);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        
        Ellipse(hdc, centerX - radius, centerY - radius,
                centerX + radius, centerY + radius);
        
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }
    
public:
    Renderer(HDC deviceContext) : hdc(deviceContext) {}
    
    /**
     * Clear the entire canvas with background color.
     */
    void clearCanvas(RECT rect) {
        HBRUSH brush = CreateSolidBrush(Config::COL_BACKGROUND);
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
    
    /**
     * Draw all grid points with their current states.
     */
    void drawGrid(const Grid& grid) {
        const auto& points = grid.getPoints();
        
        for (const auto& point : points) {
            COLORREF color = point.highlighted ? Config::COL_BLUE : Config::COL_GRAY;
            drawFilledCircle(
                static_cast<int>(point.canvasPosition.x),
                static_cast<int>(point.canvasPosition.y),
                Config::POINT_RADIUS,
                color
            );
        }
    }
    
    /**
     * Draw a preview circle while user is dragging (not yet rasterized).
     */
    void drawPreviewCircle(const Point2D& center, const Point2D& current) {
        double radius = center.distanceTo(current);
        if (radius > 1.0) {  // Only draw if radius is meaningful
            drawCircleOutline(
                static_cast<int>(center.x),
                static_cast<int>(center.y),
                static_cast<int>(radius),
                Config::COL_PREVIEW,
                Config::CIRCLE_THIN_WIDTH
            );
        }
    }
    
    /**
     * Draw the final circle visualization with all three circles.
     * 
     * @param userCircle The original circle specified by the user (blue, thick)
     * @param innerCircle The inner bounding circle (red, thin)
     * @param outerCircle The outer bounding circle (red, thin)
     * @param transform Coordinate transform for converting grid to canvas space
     */
    void drawFinalCircles(const Circle& userCircle, const Circle& innerCircle,
                          const Circle& outerCircle, const CoordinateTransform& transform) {
        // Draw user's original circle in blue with thick pen
        if (userCircle.isValid()) {
            Point2D centerCanvas = transform.gridToCanvas(userCircle.center);
            double radiusCanvas = transform.gridDistanceToCanvas(userCircle.radius);
            
            drawCircleOutline(
                static_cast<int>(centerCanvas.x),
                static_cast<int>(centerCanvas.y),
                static_cast<int>(radiusCanvas),
                Config::COL_BLUE,
                Config::CIRCLE_THICK_WIDTH
            );
        }
        
        // Draw inner bounding circle in red with thin pen
        if (innerCircle.isValid()) {
            Point2D centerCanvas = transform.gridToCanvas(innerCircle.center);
            double radiusCanvas = transform.gridDistanceToCanvas(innerCircle.radius);
            
            drawCircleOutline(
                static_cast<int>(centerCanvas.x),
                static_cast<int>(centerCanvas.y),
                static_cast<int>(radiusCanvas),
                Config::COL_RED,
                Config::CIRCLE_THIN_WIDTH
            );
        }
        
        // Draw outer bounding circle in red with thin pen
        if (outerCircle.isValid()) {
            Point2D centerCanvas = transform.gridToCanvas(outerCircle.center);
            double radiusCanvas = transform.gridDistanceToCanvas(outerCircle.radius);
            
            drawCircleOutline(
                static_cast<int>(centerCanvas.x),
                static_cast<int>(centerCanvas.y),
                static_cast<int>(radiusCanvas),
                Config::COL_RED,
                Config::CIRCLE_THIN_WIDTH
            );
        }
    }
};

#endif // RENDERER_H
