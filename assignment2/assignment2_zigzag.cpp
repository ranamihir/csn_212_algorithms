#include <iostream>
using namespace std;

int max(int, int);
int zigzag(int [], int);

int main() {
	int n = 0, i = 0;
	
	cout << "Enter length of array: ";
	cin >> n;
	
	int *a = new int[n];
	
	cout << "Enter elements of array:" << endl;
	for(i = 0; i < n; i++)
		cin >> a[i];
	
	cout << "Length of longest zig-zag sequence is: " << zigzag(a, n) << endl;
	
	delete [] a;

	return 0;
}

// Return maximum of two numbers
int max(int a, int b) {
	return (a > b) ? a : b;
}
 
// Return length of longest zig-zag subsequence
int zigzag(int a[], int n) {
    int sequence[n][2], i = 0, j = 0, result = 1;
    
    for (i = 0; i < n; i++)
		sequence[i][0] = sequence[i][1] = 1;

    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && sequence[i][0] < sequence[j][1] + 1)
                sequence[i][0] = sequence[j][1] + 1;
 
            if( a[j] > a[i] && sequence[i][1] < sequence[j][0] + 1)
                sequence[i][1] = sequence[j][0] + 1;
        }

        if (result < max(sequence[i][0], sequence[i][1]))
            result = max(sequence[i][0], sequence[i][1]);
    }

    return result;
}
