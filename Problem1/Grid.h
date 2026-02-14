#ifndef GRID_H
#define GRID_H

#include "Geometry.h"
#include "Config.h"
#include <vector>
#include <limits>

/**
 * Manages the 20x20 grid of points and their states.
 * Handles grid initialization, point highlighting, and bound calculations.
 */
class Grid {
private:
    std::vector<GridPoint> points;
    int size;
    CoordinateTransform transform;
    
public:
    /**
     * Initialize the grid with specified canvas dimensions.
     */
    Grid(int gridSize, int canvasWidth, int canvasHeight, int padding)
        : size(gridSize),
          transform(gridSize, canvasWidth, canvasHeight, padding) {
        
        // Create all grid points
        points.reserve(size * size);
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                Point2D gridPos(col, row);
                Point2D canvasPos = transform.gridToCanvas(gridPos);
                points.emplace_back(gridPos, canvasPos);
            }
        }
    }
    
    /**
     * Reset all points to non-highlighted state.
     */
    void resetHighlights() {
        for (auto& point : points) {
            point.highlighted = false;
        }
    }
    
    /**
     * Get all grid points.
     */
    const std::vector<GridPoint>& getPoints() const {
        return points;
    }
    
    /**
     * Get grid point at specific row and column.
     */
    const GridPoint& getPoint(int row, int col) const {
        return points[row * size + col];
    }
    
    /**
     * Get mutable reference to grid point.
     */
    GridPoint& getPointMutable(int row, int col) {
        return points[row * size + col];
    }
    
    /**
     * Get coordinate transform.
     */
    const CoordinateTransform& getTransform() const {
        return transform;
    }
    
    /**
     * Get grid size.
     */
    int getSize() const {
        return size;
    }
    
    /**
     * Calculate the bounding circles (inner and outer) for all highlighted points.
     * 
     * Algorithm:
     * - Inner circle: Largest circle that fits inside all highlighted points
     * - Outer circle: Smallest circle that contains all highlighted points
     * 
     * These circles provide bounds for the rasterized representation.
     * 
     * @param center Circle center in grid space
     * @param innerCircle Output parameter for inner bounding circle
     * @param outerCircle Output parameter for outer bounding circle
     * @return true if bounds were calculated successfully
     */
    bool calculateBoundingCircles(const Point2D& center, Circle& innerCircle, Circle& outerCircle) const {
        double minDistance = std::numeric_limits<double>::max();
        double maxDistance = 0.0;
        bool hasHighlightedPoints = false;
        
        // Find the minimum and maximum distances from center to highlighted points
        for (const auto& point : points) {
            if (point.highlighted) {
                hasHighlightedPoints = true;
                double dist = center.distanceTo(point.gridPosition);
                minDistance = std::min(minDistance, dist);
                maxDistance = std::max(maxDistance, dist);
            }
        }
        
        if (!hasHighlightedPoints) {
            return false;
        }
        
        // Inner circle: radius is the minimum distance to any highlighted point
        innerCircle = Circle(center, minDistance);
        
        // Outer circle: radius is the maximum distance to any highlighted point
        outerCircle = Circle(center, maxDistance);
        
        return true;
    }
    
    /**
     * Get highlighted points for visualization or analysis.
     */
    std::vector<Point2D> getHighlightedPoints() const {
        std::vector<Point2D> highlighted;
        for (const auto& point : points) {
            if (point.highlighted) {
                highlighted.push_back(point.gridPosition);
            }
        }
        return highlighted;
    }
};

#endif // GRID_H
