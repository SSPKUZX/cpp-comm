#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
	unordered_map<int,int> num_store(nums.size());
	sort(nums.begin(), nums.end());		        
	for( auto idx = 0; idx<nums.size(); ++idx) num_store[nums[idx]]=idx;

	vector<vector<int>> threes;
	for(int first =0; first < (int)(nums.size()-2); ++first){  // size_t will overflow
		for(int second= first+1; second< nums.size()-1; ++second){
			auto it = num_store.find(-nums[first]-nums[second]);
			if( num_store.end() != it && it->second > second){ 
				threes.push_back({nums[first], nums[second],-nums[first]-nums[second]});
			}
			while(second<nums.size()-2 && nums[second]==nums[second+1]) ++second;
		}
		while(first<nums.size()-3 && nums[first]==nums[first+1]) ++first;
	}

	return threes;
}

void print( vector<int>& input){
	for( auto three : threeSum(input) )
	{
		copy( three.begin(), three.end(), ostream_iterator<int>(cout, "\t"));
		cout << '\n';
	}
	cout << "\n\n";
}

int main()
{
	vector<int> input{-1, 0, 1, 2, -1, -4};
	vector<int> input2{0,0,0,0,0};
	vector<int> input3{-1,0,1,1,1};
	vector<int> input4;

	print(input);
	print(input2);
	print(input3);
	print(input4);

	return 0;
}
