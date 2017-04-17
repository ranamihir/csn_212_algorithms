#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

struct Point {
    int x, y;
};

bool comp(Point, Point);
int determinant(Point, Point, Point);
void findPartialHull(vector<Point> &, vector<Point> &, Point, Point);

int main() {
    int n;
    cin >> n;
    Point p;
    // Hold pointers of raw data, result of upper convex hull, and result of lower convex hull respectively
    vector<Point> P = vector<Point>(), upper = vector<Point>(), lower = vector<Point>();

    for(int i = 0; i < n; i++) {
        cin >> p.x >> p.y;
        P.push_back(p);
    }

    float t0 = clock();
    sort(P.begin(), P.end(), comp);             // Sort raw data in increasing order of x
    findPartialHull(P, upper, P[0], P[n-1]);    // Upper hull, line direction p1->pn
    findPartialHull(P, lower, P[n-1], P[0]);    // Lower hull, line direction pn->p1
    sort(upper.begin(), upper.end(), comp);     // Sort result in increasing order of x
    sort(lower.begin(), lower.end(), comp);     // Sort result in increasing order of x

    float t1 = clock();
    for(vector<Point>::iterator it = upper.begin(); it < upper.end(); it++) {
        // Upper part (clockwise)
        cout << "(" << (*it).x << ", " << (*it).y << ")" << endl;
    }

    for(vector<Point>::iterator it = lower.end()-1; it >= lower.begin(); it--) {
        // Reverse iterating order for lower part (clockwise)
        cout << "(" << (*it).x << ", " << (*it).y << ")" << endl;
    }

    cout << "Total time: " << (t1-t0)/CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}

bool comp(Point a, Point b) {
    return (a.x < b.x);
}

int determinant(Point a, Point b, Point c) {
    return ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));
}

void findPartialHull(vector<Point> &a, vector<Point> &result, Point p1, Point pn) {
    vector<Point> s1 = vector<Point>();   // Holds points of a part
    for(vector<Point>::iterator it = a.begin(); it < a.end(); it++) {
        // If the point is on the left of p1-->pn line then push it into the vector
        if(determinant(p1, pn, *it) > 0)
            s1.push_back(*it);
    }

    // If s1 is empty, write p1 to result array
    // Base case
    if(s1.empty())
        result.push_back(p1);
    else {
        float maxDist = 0, dist, length = sqrt(pow(p1.x - pn.x, 2) + pow(p1.y - pn.y, 2));
        Point pmax;    // Holds the pointer of pmax
        for(vector<Point>::iterator it = s1.begin(); it < s1.end(); it++) {
            dist = determinant(p1,pn,*it) / length; // Distance (height) = area / length
            if(dist > maxDist) {
                // Find point with maximum distance
                maxDist = dist;
                pmax = *it;
            }
        }
        // Find points left to p1-->pmax and pmax-->pn
        // Hold points left to p1-->pmax and left to pmax-->pn respectively
        vector<Point> s11 = vector<Point>(), s12 = vector<Point>();
        for(vector<Point>::iterator it = s1.begin(); it < s1.end(); it++) {
            // Left to p1-->pmax
            if(determinant(p1,pmax,*it) > 0)
                s11.push_back(*it);
            // Left to pmax-->pn
            else if(determinant(pmax,pn,*it) > 0)
                s12.push_back(*it);
        }

        findPartialHull(s11, result, p1, pmax); // Recursively search in s11
        findPartialHull(s12, result, pmax, pn); // Recursively search in s12
    }
}

//Test case1-Total time: 1.2e-05 seconds
//Test case2-Total time: 7.3e-05 seconds
//Test case3-Total time: 0.000537 seconds
//Test case4-Total time: 0.00032 seconds
//Test case5-Total time: 0.000623 seconds
