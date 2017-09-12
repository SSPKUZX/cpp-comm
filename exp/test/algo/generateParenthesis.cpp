//https://leetcode.com/problems/generate-parentheses/#/description
//
//
//
#include <string>
#include <vector>
#include <stack>
#include <tuple>
#include <iostream>

using namespace std;

void explore( int const idx, int const n, int const lefts, char* line, vector<string>& lines){
	if(idx==n) {
		lines.push_back(line);
	} else { 
		if(idx-lefts<lefts){ // less right braces than left ones 
			line[idx] =')';
			explore(idx+1, n, lefts, line, lines);
		}
		if(lefts<n/2){
			line[idx]='(';	
			explore(idx+1, n, lefts+1, line, lines);
		}
	} 
}
vector<string> generateParenthesis(int n) {
	vector<string> lines;	    
	if(n<1) return lines;

	n*=2;
	char line[n+1];
	line[n] ='\0';
	size_t lefts = 0, index=0;
	explore(index, n, lefts, line, lines);
	return lines;
}

/* non-recursively
 * */
vector<string> generateParenthesis2(int n) {
	vector<string> lines;	    
	if(n<1) return lines;

	n*=2;
	char line[n+1];
	line[n] ='\0';
	size_t left_count = 0, right_count=0, idx=0;
	stack<tuple<size_t,size_t,size_t>> context;//lefts,rights,idxes;
	while(true){	
		if(idx==n){
			lines.push_back(line);
			if(context.empty()) break;
			// recover saved scenario 
			auto scenario = context.top();
			context.pop();
			left_count = get<0>(scenario);
			right_count = get<1>(scenario)+1;
			idx = get<2>(scenario);
			// choose right brace this time
			line[idx++]=')';
		}else{
			// choose left brace
			if(left_count<n/2){
				// save decision point scenario 
				if(right_count<left_count){
					context.push({left_count,right_count,idx});
				}
				line[idx++]='(';
				++left_count;
			// choose right brace
			} else if(right_count<left_count){
				line[idx++]=')';
				++right_count;
			}
		}
	}

	return lines;
}


void print(vector<string> const& lines){
	for( auto& line : lines){
		cout << line << '\n'; 
	}
}

int main(){
	print( generateParenthesis2(1) ); 
	cout << '\n';
	print( generateParenthesis2(2) ); 
	cout << '\n';
	print( generateParenthesis2(3) ); 
	cout << '\n';
	print( generateParenthesis2(4) ); 
	cout << '\n';

	return 0;
}
