#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int max_participants(vector<int> &, int, int);

int main() {
	int n, i, result = 0;
	
	cout << "Enter length of array: ";
	cin >> n;
	
	vector<int> a(2*n);
	vector<pair<int,int> > num(n);
	
	cout << "Enter elements of array:" << endl;
	for(i = 0; i < n; i++) {
		cin >> a[i];
		num[i].first = a[i];
		num[i].second = i;
		a[i+n] = a[i];
	}
	
	sort(num.begin(), num.end());
	
	for(i = 0; i < n; i++)
		result = max(result, max_participants(a, num[i].second, n));

	cout << result << endl;
}

int max_participants(vector<int> &a, int id, int n) {
	vector<int> dp_matrix(n, 0);
	int result = 0, i, j;
	
	dp_matrix[0] = 1;
	for(i = id+1; i < id+n; i++) {
		for(j = id; j < i; j++)
			if(a[j] < a[i] && dp_matrix[i-id] < dp_matrix[j-id])
				dp_matrix[i-id] = dp_matrix[j-id];
		dp_matrix[i-id]++;
		if(result < dp_matrix[i-id])
		    result = dp_matrix[i-id];
	}
	
	return result;
}
