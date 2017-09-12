//https://leetcode.com/problems/course-schedule/#/description
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class GNode{
	public:
		vector<size_t> post_indexes;
};

bool canFinish(int numCourses, vector<pair<int, int>> const& prerequisites) {
	auto graph = new GNode[numCourses];
	for( auto& p : prerequisites ){        
		graph[p.second].post_indexes.push_back(p.first);
	}
	auto taken_count = 0;
	auto taken = new bool[numCourses];
	stack<int> postNodes;	
	int taken_idx=0; 

	while(taken_count < numCourses){
		while(taken[taken_idx]) ++taken_idx;
		postNodes.push(taken_idx);

		fill(taken, taken+numCourses, false);
		while(!postNodes.empty()){
			auto cur = postNodes.top();
			if(taken[cur]) return false;
			taken[cur]=true;
			++taken_count;
			postNodes.pop();
			for(auto k : graph[cur].post_indexes){
				postNodes.push(k);
			}
		}
	}

	delete[] taken;
	delete[] graph;

	return true;
}

int main()
{
	cout << boolalpha << canFinish(2, {{0,1},{1,0}}) << '\n';
	cout << boolalpha << canFinish(2, {{0,1}}) << '\n';
	cout << boolalpha << canFinish(6, {{2,1},{3,2},{3,1},{3,4},{5,3}}) << '\n';
	cout << boolalpha << canFinish(5, {{2,1},{3,2},{4,3},{2,4}}) << '\n';

	return 0;
}
