#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "Grid.h"
#include "Geometry.h"
#include "Config.h"

/**
 * Circle rasterization algorithm.
 * The goal is to find grid points that best represent a continuous circle on a discrete grid.
 * 
 * Approach:
 * 1. For each grid point, calculate its distance to the circle's boundary
 * 2. A point is considered "on" the circle if the distance from the point to the 
 *    circle boundary is within a threshold
 * 
 * Threshold Selection:
 * - The threshold is set to sqrt(2)/2 ≈ 0.707 in grid units
 * - This value is chosen because:
 *   * Grid points are 1 unit apart
 *   * The farthest a circle can be from any grid point is when it passes through
 *     the center of a grid cell (distance 0.5 in each direction, sqrt(0.5²+0.5²) = 0.707)
 *   * This ensures we capture all points that should visually represent the circle
 *   * It's conservative enough to avoid over-rasterization but inclusive enough
 *     to avoid gaps in the circle
 * 
 */
class CircleRasterizer {
public:
    /**
     * Rasterize a circle onto the grid by highlighting points near the boundary.
     * 
     * @param grid The grid to rasterize onto
     * @param circle The circle to rasterize (in grid space)
     */
    static void rasterize(Grid& grid, const Circle& circle) {
        if (!circle.isValid()) {
            return;
        }
        
        const int size = grid.getSize();
        const double threshold = Config::RASTERIZATION_THRESHOLD;
        
        // Check each grid point
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                GridPoint& point = grid.getPointMutable(row, col);
                
                double distToCenter = circle.center.distanceTo(point.gridPosition);
                double distToBoundary = std::abs(distToCenter - circle.radius);
                
                point.highlighted = (distToBoundary <= threshold);
            }
        }
    }
    
    /**
     * Alternative rasterization using optimized bounding box.
     * This version only checks points within a bounding box around the circle.
     * 
     * @param grid The grid to rasterize onto
     * @param circle The circle to rasterize (in grid space)
     */
    static void rasterizeOptimized(Grid& grid, const Circle& circle) {
        if (!circle.isValid()) {
            return;
        }
        
        const int size = grid.getSize();
        const double threshold = Config::RASTERIZATION_THRESHOLD;
        
        // Calculate bounding box in grid space
        int minRow = std::max(0, static_cast<int>(std::floor(circle.center.y - circle.radius - threshold)));
        int maxRow = std::min(size - 1, static_cast<int>(std::ceil(circle.center.y + circle.radius + threshold)));
        int minCol = std::max(0, static_cast<int>(std::floor(circle.center.x - circle.radius - threshold)));
        int maxCol = std::min(size - 1, static_cast<int>(std::ceil(circle.center.x + circle.radius + threshold)));
        
        // Only check points within bounding box
        for (int row = minRow; row <= maxRow; ++row) {
            for (int col = minCol; col <= maxCol; ++col) {
                GridPoint& point = grid.getPointMutable(row, col);
                
                double distToCenter = circle.center.distanceTo(point.gridPosition);
                double distToBoundary = std::abs(distToCenter - circle.radius);
                
                point.highlighted = (distToBoundary <= threshold);
            }
        }
        
        // Clear points outside the bounding box
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                if (row < minRow || row > maxRow || col < minCol || col > maxCol) {
                    grid.getPointMutable(row, col).highlighted = false;
                }
            }
        }
    }
};

#endif // RASTERIZER_H
