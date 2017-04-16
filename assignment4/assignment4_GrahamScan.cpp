#include <iostream>
#include <stack>
#include <cstdlib>
using namespace std;

struct Point {
	int x, y;
};

Point nextToTop(stack<Point> &);
int swap(Point &, Point &);
int distSq(Point, Point);
int orientation(Point, Point, Point);
int compare(const void *, const void *);
void convexHull(Point [], int);

// A globle point for sorting points with reference to the first point
// Used in compare function of library function qsort()
Point p0;

int main() {
	Point points[] = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
	int n = sizeof(points)/sizeof(points[0]);
	convexHull(points, n);
	return 0;
}

// Find next to top in a stack
Point nextToTop(stack<Point> &S) {
	Point p = S.top();
	S.pop();
	Point result = S.top();
	S.push(p);
	return result;
}

// Swap two points
int swap(Point &p1, Point &p2) {
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

// Find square of distance between p1 and p2
int distSq(Point p1, Point p2) {
	return ((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y)*(p1.y - p2.y));
}

// Find orientation of the ordered triplet (a, b, c)
int orientation(Point a, Point b, Point c) {
	int val = ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));
 
	if(!val)
		return 0;  // colinear
	return (val > 0) ? 1: 2; // clock- / counterclock-wise respectively
}

// Function used by library function qsort() to sort an array of points with respect to the first point
int compare(const void *vp1, const void *vp2) {
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;

   // Find orientation
   int o = orientation(p0, *p1, *p2);
   if(!o)
	 return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;
   return (o == 2) ? -1: 1;
}

// Prints convex hull of n points
void convexHull(Point points[], int n) {
   // Find the bottommost point
   int ymin = points[0].y, min = 0, i, y;
   for(i = 1; i < n; i++) {
	 y = points[i].y;
	
	 // Pick bottom-most or choose the left-most point in case of tie
	 if((y < ymin) || (ymin == y && points[i].x < points[min].x))
		ymin = points[i].y, min = i;
   }

   // Place bottom-most point at first position
   swap(points[0], points[min]);

   // Sort n-1 points with respect to the first point (based on polar angle in counterclockwise direction)
   p0 = points[0];
   qsort(&points[1], n-1, sizeof(Point), compare);

   // If two or more points make same angle with p0, remove all but the one that is farthest from p0
   int m = 1; // Initialize size of modified array
   for(i = 1; i < n; i++) {
	   // Keep removing i while angle of i and i+1 is same with respect to p0
	   while(i < n-1 && orientation(p0, points[i], points[i+1]) == 0)
		  i++;

	   points[m] = points[i];
	   m++;  // Update size of modified array
   }

   // If modified array of points has less than 3 points, convex hull is not possible
   if(m < 3)
   	return;
 
   // Create an empty stack and push first three points to it
   stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);

   // Process remaining n-3 points
   for(i = 3; i < m; i++) {
	  // Keep removing top while the angle formed by points next-to-top, top, and points[i] makes a non-left turn
	  while(orientation(nextToTop(S), S.top(), points[i]) != 2)
		 S.pop();
	  S.push(points[i]);
   }

   // Now stack has the output points, print contents of stack
   while(!S.empty()) {
	   Point p = S.top();
	   cout << "(" << p.x << ", " << p.y <<")" << endl;
	   S.pop();
   }
}
