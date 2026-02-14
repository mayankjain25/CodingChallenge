#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <vector>
#include <algorithm>

/**
 * Represents a 2D point in either canvas space or grid space.
 * 
 * The program maintains two coordinate systems:
 * - Canvas space: Pixel coordinates on the window
 * - Grid space: Logical grid coordinates (0-19 for a 20x20 grid)
 */
class Point2D {
public:
    double x;
    double y;
    
    Point2D() : x(0.0), y(0.0) {}
    Point2D(double x_, double y_) : x(x_), y(y_) {}
    
    /**
     * Calculate Euclidean distance to another point.
     */
    double distanceTo(const Point2D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    /**
     * Calculate squared distance.
     */
    double distanceSquaredTo(const Point2D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return dx * dx + dy * dy;
    }
};

class Circle {
public:
    Point2D center;
    double radius;
    
    Circle() : radius(0.0) {}
    Circle(const Point2D& c, double r) : center(c), radius(r) {}
    Circle(double cx, double cy, double r) : center(cx, cy), radius(r) {}
    
    bool isValid() const {
        return radius > 0.0;
    }
    
    /**
     * Calculate the distance from a point to the circle's boundary.
     * Returns 0 if point is on the circle, negative if inside, positive if outside.
     */
    double distanceFromBoundary(const Point2D& point) const {
        double distToCenter = center.distanceTo(point);
        return std::abs(distToCenter - radius);
    }
    
    /**
     * Check if a point is approximately on the circle boundary within a threshold.
     */
    bool isPointNearBoundary(const Point2D& point, double threshold) const {
        return distanceFromBoundary(point) <= threshold;
    }
};

/**
 * Represents a grid point that can be highlighted or not.
 */
struct GridPoint {
    Point2D gridPosition;    // Position in grid space (0-19)
    Point2D canvasPosition;  // Position in canvas/pixel space
    bool highlighted;
    
    GridPoint() : highlighted(false) {}
    GridPoint(const Point2D& grid, const Point2D& canvas)
        : gridPosition(grid), canvasPosition(canvas), highlighted(false) {}
};

/**
 * Helper class for coordinate transformations between canvas and grid space.
 */
class CoordinateTransform {
private:
    double gridSpacing;      // Pixel spacing between grid points
    Point2D gridOrigin;      // Top-left corner of grid in canvas space
    int gridSize;            // Number of points per dimension
    
public:
    CoordinateTransform(int gridSz, int canvasWidth, int canvasHeight, int padding)
        : gridSize(gridSz) {
        // Calculate available space and spacing
        int availableWidth = canvasWidth - 2 * padding;
        int availableHeight = canvasHeight - 2 * padding;
        
        // Use the smaller dimension to ensure grid fits
        int minDimension = std::min(availableWidth, availableHeight);
        gridSpacing = static_cast<double>(minDimension) / (gridSize - 1);
        
        // Center the grid in the canvas
        gridOrigin.x = padding + (availableWidth - gridSpacing * (gridSize - 1)) / 2.0;
        gridOrigin.y = padding + (availableHeight - gridSpacing * (gridSize - 1)) / 2.0;
    }
    
    /**
     * Convert canvas coordinates to grid coordinates.
     */
    Point2D canvasToGrid(const Point2D& canvas) const {
        return Point2D(
            (canvas.x - gridOrigin.x) / gridSpacing,
            (canvas.y - gridOrigin.y) / gridSpacing
        );
    }
    
    /**
     * Convert grid coordinates to canvas coordinates.
     */
    Point2D gridToCanvas(const Point2D& grid) const {
        return Point2D(
            gridOrigin.x + grid.x * gridSpacing,
            gridOrigin.y + grid.y * gridSpacing
        );
    }
    
    /**
     * Convert a distance in canvas space to grid space.
     */
    double canvasDistanceToGrid(double canvasDistance) const {
        return canvasDistance / gridSpacing;
    }
    
    /**
     * Convert a distance in grid space to canvas space.
     */
    double gridDistanceToCanvas(double gridDistance) const {
        return gridDistance * gridSpacing;
    }
    
    /**
     * Get the grid spacing in pixels.
     */
    double getGridSpacing() const {
        return gridSpacing;
    }
};

#endif // GEOMETRY_H
