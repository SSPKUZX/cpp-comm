//https://leetcode.com/problems/largest-number/#/description

#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iostream>

using namespace std;

string largestNumber(vector<int>& nums) {
	sort(nums.begin(), nums.end(), [](int lhs, int rhs){
				return strcmp((to_string(lhs)+to_string(rhs)).c_str(), (to_string(rhs)+to_string(lhs)).c_str())>0;
			});
	ostringstream oss;
	for_each( nums.begin(), nums.end(), [&oss]( int ele){ oss << ele;} );
	return all_of(nums.begin(),nums.end(),[](int ele){return ele==0;})?"0":oss.str();
}

int main()
{
	vector<int> nums={3, 30, 34, 5, 9};
	cout << largestNumber(nums) << '\n';
	vector<int> nums1={0,0,0};
	cout << largestNumber(nums1) << '\n';

	return 0;
}


