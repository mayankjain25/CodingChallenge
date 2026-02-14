/**
 * Problem 1 - Circle Rasterization Program
 * 
 * This program demonstrates circle rasterization on a discrete 20x20 grid.
 * Users can click and drag to define circles, which are then rasterized to
 * show which grid points best represent the circle boundary.
 */

#include "Config.h"
#include "Geometry.h"
#include "Grid.h"
#include "Rasterizer.h"
#include "Renderer.h"

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * Application state manager.
 * Encapsulates all application state and behavior.
 */
class Application {
private:
    Grid grid;
    
    // Mouse interaction state
    bool isDragging;
    Point2D dragStartCanvas;    // Where the drag started (canvas space)
    Point2D dragCurrentCanvas;  // Current mouse position during drag (canvas space)
    
    // Rasterization results
    bool hasRasterizedCircle;
    Circle userCircleGrid;      // User's circle in grid space
    Circle innerBoundGrid;      // Inner bound circle in grid space
    Circle outerBoundGrid;      // Outer bound circle in grid space
    
public:
    Application()
        : grid(Config::GRID_SIZE, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::GRID_PADDING),
          isDragging(false),
          hasRasterizedCircle(false) {
    }
    
    /**
     * Handle mouse button down event - start circle definition.
     */
    void onMouseDown(int x, int y) {
        // Reset to clean state for new circle
        grid.resetHighlights();
        hasRasterizedCircle = false;
        
        // Start dragging
        isDragging = true;
        dragStartCanvas = Point2D(x, y);
        dragCurrentCanvas = dragStartCanvas;
    }
    
    /**
     * Handle mouse move event - update circle preview.
     */
    void onMouseMove(int x, int y) {
        if (isDragging) {
            dragCurrentCanvas = Point2D(x, y);
        }
    }
    
    /**
     * Handle mouse button up event - finalize and rasterize circle.
     */
    void onMouseUp(int x, int y) {
        if (!isDragging) {
            return;
        }
        
        isDragging = false;
        dragCurrentCanvas = Point2D(x, y);
        
        const CoordinateTransform& transform = grid.getTransform();
        Point2D centerGrid = transform.canvasToGrid(dragStartCanvas);
        Point2D edgeGrid = transform.canvasToGrid(dragCurrentCanvas);
        double radiusGrid = centerGrid.distanceTo(edgeGrid);
        
        // Only rasterize if circle has meaningful size
        if (radiusGrid > 0.1) {
            userCircleGrid = Circle(centerGrid, radiusGrid);
            
            // Rasterize the circle
            CircleRasterizer::rasterizeOptimized(grid, userCircleGrid);
            
            // Calculate bounding circles
            if (grid.calculateBoundingCircles(centerGrid, innerBoundGrid, outerBoundGrid)) {
                hasRasterizedCircle = true;
            }
        }
    }
    
    /**
     * Render the entire application.
     */
    void render(HDC hdc) {
        Renderer renderer(hdc);
        
        // Clear background
        RECT rect = {0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT};
        renderer.clearCanvas(rect);
        
        // Draw grid points
        renderer.drawGrid(grid);
        
        // Draw preview circle while dragging
        if (isDragging) {
            renderer.drawPreviewCircle(dragStartCanvas, dragCurrentCanvas);
        }
        
        // Draw final circles after rasterization
        if (hasRasterizedCircle) {
            renderer.drawFinalCircles(
                userCircleGrid,
                innerBoundGrid,
                outerBoundGrid,
                grid.getTransform()
            );
        }
    }
    
    const Grid& getGrid() const { return grid; }
};

// Global application instance
Application* g_pApp = nullptr;

/**
 * Application entry point.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    const wchar_t CLASS_NAME[] = L"CircleRasterizerWindow";
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;      
    RegisterClass(&wc);
    
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    
    RECT windowRect = {0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT};
    AdjustWindowRect(&windowRect, style, FALSE);
    
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Circle Rasterization - Meril Coding Challenge",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (hwnd == nullptr) {
        return 0;
    }
    
    // Create application instance
    g_pApp = new Application();
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Cleanup
    delete g_pApp;
    
    return 0;
}

/**
 * Window procedure - handles all window messages.
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            if (g_pApp) {
                // Create off-screen buffer for double buffering
                HDC memDC = CreateCompatibleDC(hdc);
                HBITMAP memBitmap = CreateCompatibleBitmap(hdc, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
                HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
                
                // Render to off-screen buffer
                g_pApp->render(memDC);
                
                // Copy the complete buffer to screen in one operation
                BitBlt(hdc, 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);
                
                // Clean up
                SelectObject(memDC, oldBitmap);
                DeleteObject(memBitmap);
                DeleteDC(memDC);
            }
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (g_pApp) {
                g_pApp->onMouseDown(x, y);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        
        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (g_pApp && (wParam & MK_LBUTTON)) {
                g_pApp->onMouseMove(x, y);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        
        case WM_LBUTTONUP: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (g_pApp) {
                g_pApp->onMouseUp(x, y);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        
        case WM_ERASEBKGND:
            // Prevent flickering by handling erase ourselves
            return 1;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
