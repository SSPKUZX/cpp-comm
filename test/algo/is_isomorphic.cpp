//https://leetcode.com/problems/isomorphic-strings/#/description
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

bool isIsomorphic(string s, string t) {
	unordered_map<char, char> mapper;
	unordered_set<char>       sets;
	for(auto idx=0; idx<s.size(); ++idx){
		if(mapper.find(s[idx])==mapper.end()){
			if(sets.end()==sets.find(t[idx])){
				mapper.emplace(s[idx], t[idx]);
				sets.insert(t[idx]);
			} else {
				return false;
			}
		} else if(mapper[s[idx]]!=t[idx]) {
			return false;
		}
	}

	return true;
}

int main()
{
	cout << boolalpha <<  isIsomorphic("title", "paper") << '\n';
	cout << boolalpha <<  isIsomorphic("egg", "add") << '\n';
	cout << boolalpha <<  isIsomorphic("foo", "bar") << '\n';
	cout << boolalpha <<  isIsomorphic("ab", "aa") << '\n';

	return 0;
}
