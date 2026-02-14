/**
 * Geometric Structures and Algorithms
 * 
 * Defines geometric primitives (Point, Circle) and implements the
 * Pratt algebraic circle fitting algorithm for best-fit calculations.
 * 
 * Algorithm: Pratt Method
 * - Uses least squares optimization
 * - Minimizes algebraic distance to circle
 * - Handles degenerate cases (collinear points)
 * 
 */

#pragma once
#include <cmath>
#include <vector>
#include <algorithm>

struct Point {
    double x;
    double y;
    
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

struct Circle {
    Point center;
    double radius;
    
    Circle() : center(), radius(0) {}
    Circle(const Point& c, double r) : center(c), radius(r) {}
    Circle(double cx, double cy, double r) : center(cx, cy), radius(r) {}
};

// Best fit circle using algebraic fit (Pratt method)
// This uses least squares to find the circle that best fits a set of points
Circle FitCircle(const std::vector<Point>& points) {
    if (points.size() < 3) {
        return Circle();  // Need at least 3 points for a circle
    }
    
    // Calculate centroid
    double sum_x = 0, sum_y = 0;
    for (const auto& p : points) {
        sum_x += p.x;
        sum_y += p.y;
    }
    double cx = sum_x / points.size();
    double cy = sum_y / points.size();
    
    // Translate points to centroid
    std::vector<Point> translated;
    translated.reserve(points.size());
    for (const auto& p : points) {
        translated.push_back(Point(p.x - cx, p.y - cy));
    }
    
    // Calculate statistical moments
    double Mxx = 0, Myy = 0, Mxy = 0, Mxz = 0, Myz = 0, Mzz = 0;
    for (const auto& p : translated) {
        double zi = p.x * p.x + p.y * p.y;
        Mxx += p.x * p.x;
        Myy += p.y * p.y;
        Mxy += p.x * p.y;
        Mxz += p.x * zi;
        Myz += p.y * zi;
        Mzz += zi * zi;
    }
    Mxx /= points.size();
    Myy /= points.size();
    Mxy /= points.size();
    Mxz /= points.size();
    Myz /= points.size();
    Mzz /= points.size();
    
    // Calculate coefficients for circle equation
    double Mz = Mxx + Myy;
    double Cov_xy = Mxx * Myy - Mxy * Mxy;
    double Var_z = Mzz - Mz * Mz;
    
    double A2 = 4 * Cov_xy - 3 * Mz * Mz - Mzz;
    double A1 = Var_z * Mz + 4 * Cov_xy * Mz + Mxz * Mxz + Myz * Myz;
    double A0 = Mxz * (Mxz * Myy - Myz * Mxy) + Myz * (Myz * Mxx - Mxz * Mxy) - Var_z * Cov_xy;
    double A22 = A2 + A2;
    
    double epsilon = 1e-12;
    double ynew = 1e+20;
    int IterMax = 20;
    double xnew = 0;
    
    // Newton's method to solve for the circle
    for (int iter = 0; iter < IterMax; iter++) {
        double yold = ynew;
        ynew = A0 + xnew * (A1 + xnew * (A2 + 4 * xnew * xnew));
        if (std::abs(ynew) > std::abs(yold)) {
            xnew = 0;
            break;
        }
        double Dy = A1 + xnew * (A22 + 16 * xnew * xnew);
        double xold = xnew;
        xnew = xold - ynew / Dy;
        if (std::abs((xnew - xold) / xnew) < epsilon) break;
        if (iter >= IterMax - 1) {
            xnew = 0;
        }
        if (xnew < 0) {
            xnew = 0;
            break;
        }
    }
    
    // Calculate circle center and radius
    double DET = xnew * xnew - xnew * Mz + Cov_xy;
    
    // Check if DET is too small (collinear or nearly collinear points)
    if (std::abs(DET) < 1e-10) {
        return Circle();  // Invalid circle - points are collinear
    }
    
    double center_x = (Mxz * (Myy - xnew) - Myz * Mxy) / DET / 2;
    double center_y = (Myz * (Mxx - xnew) - Mxz * Mxy) / DET / 2;
    
    // Transform back to original coordinate system
    center_x += cx;
    center_y += cy;
    
    // Calculate radius as average distance from center to all points
    double sum_r = 0;
    for (const auto& p : points) {
        double dx = p.x - center_x;
        double dy = p.y - center_y;
        sum_r += std::sqrt(dx * dx + dy * dy);
    }
    double radius = sum_r / points.size();
    
    if (std::isnan(center_x) || std::isnan(center_y) || std::isnan(radius) ||
        std::isinf(center_x) || std::isinf(center_y) || std::isinf(radius) ||
        radius <= 0 || radius > 10000) {
        return Circle();  // Invalid circle
    }
    
    return Circle(center_x, center_y, radius);
}
