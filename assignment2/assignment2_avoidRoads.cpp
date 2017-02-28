#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class AvoidRoads {
    private:
		vector<vector<long> > map;
		
	    bool blocked(int wS, int wE, int hS, int hE, vector<string> bad) {
	    	string str = "";
	    	stringstream Stream;
			Stream.str(str);
			Stream << wS;
			str += Stream.str() + " ";
			Stream.clear();
			Stream << wE;
			str += Stream.str() + " ";
			Stream.clear();
			Stream << hS;
			str += Stream.str() + " ";
			Stream.clear();
			Stream << hE;
			str += Stream.str() + " ";
			Stream.clear();
	        for (int i = 0; i < bad.size(); i++)
	            if(bad[i] == str)
	                return true;
	        return false;
	    }

    	long calculatePaths(int width, int height, vector<string> bad, vector<vector<long> > map) {
	        if((width == 1 && height == 0 && !blocked(width-1, height, width, height, bad)) ||
	            (height == 1 && width == 0 && !blocked(width, height-1, width, height, bad)))
	            return 1;

	        int lowWidth = width-1, lowHeight = height-1;
	        long result = 0;

	        if (lowWidth >= 0 && !blocked(lowWidth, height, width, height, bad)
	                && !blocked(width, height, lowWidth, height, bad))
	           result += map[height][lowWidth];
	           
	        if (lowHeight >= 0 && !blocked(width, lowHeight, width, height, bad)
	                && !blocked(width, height, width, lowHeight, bad))
	           result += map[lowHeight][width];

	        return result;
    	}

    public:
		long numWays(int width, int height, vector<string> bad) {
			int i, levelWidth = 0, levelHeight = 0;
	        map = vector<vector<long> >(width+1, vector<long>(height+1));
	        map[0][0] = 0;
	        while(levelWidth <= width && levelHeight <= height) {
	            for (i = 0; i <= height && levelWidth <= width; i++)
	               map[i][levelWidth] = calculatePaths(levelWidth, i, bad, map);
	            levelWidth++;
	            
	            for (i = 0; i <= width && levelHeight <= height; i++)
	               map[levelHeight][i] = calculatePaths(i, levelHeight, bad, map);
	            levelHeight++;
	        }
	        return map[height][width];
	    }
};

int main() {
    AvoidRoads avoid_roads;
    vector<string> bad;
	bad.push_back("0 0 0 1");
	bad.push_back("6 6 5 6");
    
    cout << avoid_roads.numWays(6, 6, bad) << endl;
    
	return 0;
}
