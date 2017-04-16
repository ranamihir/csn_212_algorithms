#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x, y;
};

int orientation(Point, Point, Point);
void convexHull(Point [], int);

int main() {
    Point points[] = {{2, 1}, {3, 0}, {0, 0}, {3, 3}, {0, 3}, {2, 2}, {1, 1}};
    int n = sizeof(points)/sizeof(points[0]);
    convexHull(points, n);
    return 0;
}

// Find orientation of the ordered triplet (a, b, c)
int orientation(Point a, Point b, Point c) {
    int val = ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));

    if(!val)
		return 0;  // colinear
    return (val > 0) ? 1 : 2; // clock- / counterclock-wise respectively
}

// Print convex hull of n points
void convexHull(Point points[], int n) {
    // There must be at least 3 points
    if(n < 3)
		return;
	
	// Initialize result
    vector<Point> hull;

    // Find leftmost point
    int l = 0, i;
    for(i = 1; i < n; i++)
        if(points[i].x < points[l].x)
            l = i;

    // Start from leftmost point, keep moving counterclockwise until we are at the start point again
	// This loop runs O(h) times; h = number of points in result
    int a = l, b;
    do {
		// Add current point to result
        hull.push_back(points[a]);

        // Search for a point 'b' such that orientation(a, x, b) is counterclockwise for all points 'x'
        b = (a+1)%n;
        for(i = 0; i < n; i++) {
           // If i is more counterclockwise than current b, then update b
           if(orientation(points[a], points[i], points[b]) == 2)
               b = i;
        }

        // Now b is the most counterclockwise with respect to a
        // Set a as b for next iteration, so that b is added to result
        a = b;

    } while(a != l);  // While first point is not reached

    // Print result
    for(i = 0; i < hull.size(); i++)
		cout << "(" << hull[i].x << ", " << hull[i].y << ")" << endl;
}
