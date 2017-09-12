//https://leetcode.com/problems/count-of-smaller-numbers-after-self/#/description


#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

vector<int> countSmaller(vector<int> const& nums) {
	if(nums.empty()) return vector<int>();	

	vector<int> smallers(nums.size(),0);
	for(int idx=nums.size()-2; idx>=0; --idx){
		auto smaller_idx=idx+1;	
		while(smaller_idx<nums.size() && nums[idx]<=nums[smaller_idx]) ++smaller_idx;	
		smallers[idx]=smallers[smaller_idx]+1;
	}

	return smallers;
}

template<class T>
void print( vector<T> const& list)
{
	copy( list.begin(), list.end(), ostream_iterator<T>(cout, "\t"));
	cout << '\n';
}

//https://leetcode.com/problems/queue-reconstruction-by-height/#/description
vector<pair<int, int>> reconstructQueue(vector<pair<int, int>> & people) {
	sort(people.begin(), people.end(), [](pair<int,int> const& lhs, pair<int,int> const& rhs)
						{ return lhs.first>rhs.first || (lhs.first==rhs.first && lhs.second < rhs.second); });

	for(auto idx=1; idx<people.size(); ++idx){
		auto c_idx = people[idx].second;
		if(idx!=c_idx){
			while( people[c_idx].second==people[idx].second && people[c_idx].first<people[idx].second) ++c_idx;
		}
	}

	for( auto& p : people)
	{
		cout << p.first << '\t' <<  p.second << '\t';	
	}
	cout << '\n';

	return people;
}


int main()
{
	print( countSmaller({5, 2, 6, 1}) );
	vector<pair<int,int>> people({{7,0}, {4,4}, {7,1}, {5,0}, {6,1}, {5,2}});
	reconstructQueue( people);

	return 0;
}

