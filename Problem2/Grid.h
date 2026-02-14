/**
 * Grid Point Management
 * 
 * Manages a 2D grid of interactive points, handling selection state
 * and coordinate transformations between pixel and grid space.
 * 
 */

#pragma once
#include "Config.h"
#include "Geometry.h"
#include <vector>

struct GridPoint {
    int i;  // Grid row
    int j;  // Grid column
    bool selected;
    
    GridPoint() : i(0), j(0), selected(false) {}
    GridPoint(int i, int j) : i(i), j(j), selected(false) {}
    
    Point GetPixelCoords() const {
        return Point(
            j * CELL_SIZE + CELL_SIZE / 2.0,
            i * CELL_SIZE + CELL_SIZE / 2.0
        );
    }
};

class Grid {
private:
    std::vector<std::vector<GridPoint>> points;
    
public:
    Grid() {
        // Initialize grid with all points unselected
        points.resize(GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; i++) {
            points[i].resize(GRID_SIZE);
            for (int j = 0; j < GRID_SIZE; j++) {
                points[i][j] = GridPoint(i, j);
            }
        }
    }
    
    void TogglePoint(int i, int j) {
        if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
            points[i][j].selected = !points[i][j].selected;
        }
    }
    
    bool IsSelected(int i, int j) const {
        if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
            return points[i][j].selected;
        }
        return false;
    }
    
    void Clear() {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                points[i][j].selected = false;
            }
        }
    }
    
    std::vector<Point> GetSelectedPoints() const {
        std::vector<Point> selectedPoints;
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (points[i][j].selected) {
                    selectedPoints.push_back(points[i][j].GetPixelCoords());
                }
            }
        }
        return selectedPoints;
    }
    
    // Convert pixel coordinates to grid indices
    static bool PixelToGrid(int x, int y, int& i, int& j) {
        j = x / CELL_SIZE;
        i = y / CELL_SIZE;
        
        if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
            return true;
        }
        return false;
    }
    
    int GetSize() const { return GRID_SIZE; }
    
    const GridPoint& GetPoint(int i, int j) const {
        return points[i][j];
    }
};
