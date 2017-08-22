//https://leetcode.com/problems/evaluate-reverse-polish-notation/#/description

#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

int evalRPN(vector<string>& tokens) {
	stack<int> stk;
	int left=0,right=0;
	for(auto token : tokens){
		if(token=="+"){
			right=stk.top();
			stk.pop();
			left=stk.top();
			stk.pop();
			stk.push(left+right);
		}else if(token=="-"){
			right=stk.top();
			stk.pop();
			left=stk.top();
			stk.pop();
			stk.push(left-right);
		}else if(token=="/"){
			right=stk.top();
			stk.pop();
			left=stk.top();
			stk.pop();
			stk.push(left/right);
		}else if(token=="*"){
			right=stk.top();
			stk.pop();
			left=stk.top();
			stk.pop();
			stk.push(left*right);
		}else{
			stk.push(stol(token));
		}
	}

	return stk.top();
}

int main(int argc, char* argv[])
{
	vector<string> input={"2", "1", "+", "3", "*" };	
	vector<string> input2={"4", "13", "5", "/", "+"};	
	cout << evalRPN(input) << '\n';
	cout << evalRPN(input2) << '\n';
	return 0;
}
