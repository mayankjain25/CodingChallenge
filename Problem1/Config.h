#ifndef CONFIG_H
#define CONFIG_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

/**
 * Configuration header containing all constants and settings for the circle rasterization program.
 */

namespace Config {
    // Grid Configuration
    constexpr int GRID_SIZE = 20;                    
    constexpr int WINDOW_WIDTH = 800;                
    constexpr int WINDOW_HEIGHT = 800;               
    constexpr int GRID_PADDING = 50;                 
    
    // Visual Configuration
    constexpr int POINT_RADIUS = 4;                  
    constexpr int CIRCLE_THICK_WIDTH = 3;            
    constexpr int CIRCLE_THIN_WIDTH = 1;             
    
    // Colors (RGB)
    constexpr COLORREF COL_GRAY = RGB(220, 220, 220);      
    constexpr COLORREF COL_BLUE = RGB(0, 100, 255);        
    constexpr COLORREF COL_RED = RGB(255, 0, 0);           
    constexpr COLORREF COL_BACKGROUND = RGB(255, 255, 255); 
    constexpr COLORREF COL_PREVIEW = RGB(150, 150, 255);   
    
    // Algorithm Configuration
    // Threshold for determining if a point is "nearest" to the circle boundary
    // A point is considered on the circle if its distance from the circle boundary
    // is within RASTERIZATION_THRESHOLD units in grid space
    constexpr double RASTERIZATION_THRESHOLD = 0.7071;  // sqrt(2)/2 for diagonal neighbors
}

#endif // CONFIG_H
