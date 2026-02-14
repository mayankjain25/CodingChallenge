/**
 * Problem 2: Best-Fit Circle Through Selected Points
 * 
 * This program allows users to interactively select grid points and generates
 * the best-fit circle that minimizes the squared distance to all selected points.
 * 
 * Features:
 * - Interactive point selection via mouse click
 * - Pratt algebraic circle fitting algorithm
 * - Validation for collinear points
 * - Real-time visualization
 * 
 * Controls:
 * - Click: Toggle point selection
 * - G key: Generate best-fit circle
 * - C key: Clear all selections
 * 
 */

#include <windows.h>
#include "Config.h"
#include "Grid.h"
#include "Renderer.h"
#include "Geometry.h"
#include <memory>

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Application {
private:
    Grid grid;
    std::unique_ptr<Renderer> renderer;
    Circle bestFitCircle;
    bool showCircle;

public:
    Application() : showCircle(false) {}

    /**
     * Initialize renderer after window creation.
     * @param hwnd Window handle
     */
    void InitializeRenderer(HWND hwnd) {
        renderer = std::make_unique<Renderer>(hwnd, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    /**
     * Render the current state.
     */
    void Render() {
        if (renderer) {
            renderer->Render(grid, showCircle ? &bestFitCircle : nullptr);
            renderer->Present();
        }
    }

    /**
     * Handle mouse button down event.
     * @param x Mouse x coordinate
     * @param y Mouse y coordinate
     */
    void OnMouseDown(int x, int y) {
        int i, j;
        if (Grid::PixelToGrid(x, y, i, j)) {
            grid.TogglePoint(i, j);
            showCircle = false;  // Hide circle when grid changes
            Render();
        }
    }

    /**
     * Generate best-fit circle from selected points.
     * @param hwnd Window handle for message boxes
     */
    void GenerateCircle(HWND hwnd) {
        std::vector<Point> selectedPoints = grid.GetSelectedPoints();
        
        if (selectedPoints.size() < 3) {
            showCircle = false;
            MessageBox(hwnd, 
                      "Please select at least 3 points to fit a circle.", 
                      "Not Enough Points", 
                      MB_OK | MB_ICONINFORMATION);
        } else {
            bestFitCircle = FitCircle(selectedPoints);
            if (bestFitCircle.radius > 0) {
                showCircle = true;
            } else {
                showCircle = false;
                MessageBox(hwnd, 
                          "Cannot fit a circle through collinear points.\n"
                          "Please select points that are not in a straight line.", 
                          "Invalid Point Configuration", 
                          MB_OK | MB_ICONWARNING);
            }
        }
        
        Render();
    }

    /**
     * Clear all selected points and hide circle.
     */
    void Clear() {
        grid.Clear();
        showCircle = false;
        Render();
    }
};

Application* g_app = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create application instance
    Application app;
    g_app = &app;
    
    // Register window class
    const char CLASS_NAME[] = "Problem2WindowClass";
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    
    // Calculate window size to account for borders
    RECT windowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    
    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Problem 2 - Click Points, Press G for Circle, C to Clear",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if (hwnd == NULL) {
        return 0;
    }
    
    app.InitializeRenderer(hwnd);
    
    ShowWindow(hwnd, nCmdShow);
    
    app.Render();
    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (g_app) {
                g_app->Render();
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (g_app) {
                g_app->OnMouseDown(x, y);
            }
            return 0;
        }
        
        case WM_CHAR: {
            char key = static_cast<char>(wParam);
            
            if (key == 'g' || key == 'G') {
                if (g_app) {
                    g_app->GenerateCircle(hwnd);
                }
            }
            else if (key == 'c' || key == 'C') {
                if (g_app) {
                    g_app->Clear();
                }
            }
            return 0;
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
