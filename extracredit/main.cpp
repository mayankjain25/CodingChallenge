/**
 * Extra Credit: Best-Fit Ellipse Through Selected Points
 * 
 * This program allows users to interactively select grid points and generates
 * the best-fit ellipse using covariance-based fitting. Unlike circles, ellipses
 * can accommodate varying eccentricities and rotations.
 * 
 * Features:
 * - Interactive point selection via mouse click
 * - Covariance-based ellipse fitting algorithm
 * - Support for rotated ellipses
 * - Validation for collinear points
 * - Real-time visualization
 * 
 * Controls:
 * - Click: Toggle point selection
 * - G key: Generate best-fit ellipse
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

/**
 * Application state manager for Extra Credit.
 * Encapsulates all application state and behavior to avoid global variables.
 */
class Application {
private:
    Grid grid;
    std::unique_ptr<Renderer> renderer;
    EllipseShape bestFitEllipse;
    bool showEllipse;

public:
    Application() : showEllipse(false) {}

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
            renderer->Render(grid, showEllipse ? &bestFitEllipse : nullptr);
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
            showEllipse = false;  // Hide ellipse when grid changes
            Render();
        }
    }

    /**
     * Generate best-fit ellipse from selected points.
     * @param hwnd Window handle for message boxes
     */
    void GenerateEllipse(HWND hwnd) {
        std::vector<Point> selectedPoints = grid.GetSelectedPoints();
        
        if (selectedPoints.size() < 5) {
            showEllipse = false;
            MessageBox(hwnd, 
                      "Please select at least 5 points to fit an ellipse.", 
                      "Not Enough Points", 
                      MB_OK | MB_ICONINFORMATION);
        } else {
            bestFitEllipse = FitEllipse(selectedPoints);
            if (bestFitEllipse.valid) {
                showEllipse = true;
            } else {
                showEllipse = false;
                MessageBox(hwnd, 
                          "Cannot fit an ellipse through collinear points.\n"
                          "Please select points that are not in a straight line.", 
                          "Invalid Point Configuration", 
                          MB_OK | MB_ICONWARNING);
            }
        }
        
        Render();
    }

    /**
     * Clear all selected points and hide ellipse.
     */
    void Clear() {
        grid.Clear();
        showEllipse = false;
        Render();
    }
};

Application* g_app = nullptr;

/**
 * Windows entry point.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create application instance
    Application app;
    g_app = &app;
    
    const char CLASS_NAME[] = "ExtraCreditWindowClass";
    
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
        "Extra Credit - Best Fit Ellipse (Press G for Ellipse, C to Clear)",
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
    
    // Initialize application renderer
    app.InitializeRenderer(hwnd);
    
    ShowWindow(hwnd, nCmdShow);
    
    // Initial render
    app.Render();
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

/**
 * Window message handler.
 */
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
                    g_app->GenerateEllipse(hwnd);
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
