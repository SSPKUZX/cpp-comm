//https://leetcode.com/problems/distinct-subsequences/#/description
//

#include <string>
#include <vector>
#include <iostream>

using namespace std;

/*
 * space and time as O(mn)
 * */
int numDistinct(string s, string t) {
	if( t.empty() || s.size() < t.size() )
		return 0;

	// initialize the walls
	size_t dists[s.size()][t.size()];
	for(auto count=0,idx=0; idx<s.size();++idx){
		if( s[idx]==t[0]) ++count;
		dists[idx][0]=count;
	}
	auto match=1;
	for(auto idx=0; idx<t.size(); ++idx){
		if(s[idx]!=t[idx]) match=0;
		dists[idx][idx]=match;
	}

	// dynamic scheduling
	for( auto col=1; col < t.size(); ++col)
		for(auto row=col+1; row < s.size(); ++row)
		{
			dists[row][col] = dists[row-1][col];
			if( s[row]==t[col] ){	
				dists[row][col]+=dists[row-1][col-1];
			}
		}

	// printing
	for(auto r=0; r<s.size(); ++r){
		for( auto c=0; c<t.size(); ++c)
			cout << dists[r][c] << '\t';
		cout << '\n';
	}

	auto d = dists[s.size()-1][t.size()-1];
	cout << "s1=" << s << "\tt1=" << t << "\t distincts="  << d << '\n';
	return d;
}

/*
 *  memory and time as O(n(m-n))
 * */
int numDistinct2(string s, string t) {
	if( t.empty() || s.size() < t.size() )
		return 0;

	// initialize the walls
	auto width=s.size()-t.size()+1;
	size_t dists[width][t.size()];
	for(auto count=0,idx=0; idx<width;++idx){
		if( s[idx]==t[0]) ++count;
		dists[idx][0]=count;
	}
	auto match=1;
	for(auto idx=0; idx<t.size(); ++idx){
		if(s[idx]!=t[idx]) match=0;
		dists[0][idx]=match;
	}

	// dynamic scheduling
	for(auto row=1; row < width; ++row)
		for( auto col=1; col < t.size(); ++col)
		{
			dists[row][col] = dists[row-1][col];
			if( s[row+col]==t[col] ){	
				dists[row][col]+=dists[row][col-1];
			}
		}

	// printing
	for(auto r=0; r<width; ++r){
		for( auto c=0; c<t.size(); ++c)
			cout << dists[r][c] << '\t';
		cout << '\n';
	}

	auto d = dists[width-1][t.size()-1];
	cout << "s1=" << s << "\tt1=" << t << "\t distincts="  << d << '\n';
	return d;
}

/*
 *  time as O(n(m-n))
 *  memory as O(n)
 * */
int numDistinct3(string s, string t) {
	if( t.empty() || s.size() < t.size() ) return 0;
	// initialize the walls
	vector<size_t> dists(t.size()+1, 0);
	dists[0]=1;
	for(auto row=0; row < s.size()-t.size()+1; ++row){
		for( auto col=1; col < t.size()+1; ++col){
			dists[col]+=(s[row+col-1]==t[col-1]?dists[col-1]:0);
		}
	}

	// printing
	for( auto c=0; c<t.size()+1; ++c)
		cout << dists[c] << '\t';
	cout << "\ns1=" << s << "\tt1=" << t << "\t distincts="  << dists[t.size()] << '\n';
	return dists[t.size()];
}
int main()
{
	numDistinct("rabbbit","rabbit");
	numDistinct2("rabbbit","rabbit");
	numDistinct3("rabbbit","rabbit");
	numDistinct("hah", "h");
	numDistinct2("hah", "h");
	numDistinct3("hah", "h");
	numDistinct("hah", "ha");
	numDistinct2("hah", "ha");
	numDistinct3("hah", "ha");
	numDistinct("hah", "ah");
	numDistinct2("hah", "ah");
	numDistinct3("hah", "ah");
	numDistinct("hahqaqcj", "haj");
	numDistinct2("hahqaqcj", "haj");
	numDistinct3("hahqaqcj", "haj");

	return 0;
}
