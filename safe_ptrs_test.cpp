#include "safe_ptrs.h"

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <iostream>

int main(){
	using namespace utl;	

	// safe seqs for SafeUptr
	SafeUptr<std::vector<int*>>	arr( new std::vector<int*>{new int(3)});
	arr->push_back(new int(10));
	auto dq = MakeSafeUptr<std::deque<int*>>();
	dq->push_back(new int(99));
	SafeUptr<std::list<int*>>	lt;
	SafeUptr<std::set<int*>>	st;
	// safe maps for SafeUptr
	SafeUptr<std::map<int,int*>>	pairs( new std::map<int,int*>{ {3,new int(9)} });
	SafeUptr<std::multimap<int,int*>> mt;
	SafeUptr<std::unordered_map<int,int*>> um;
	SafeUptr<std::unordered_multimap<int,int*>> umt;

	// safe seqs for SafePtrs
	SafePtrs<std::vector<int*>>	safeVec{new int(6)};
	safeVec.push_back(new int(8));
	SafePtrs<std::list<int*>>	safeLst;
	safeLst.push_back(new int(7));
	SafePtrs<std::deque<int*>>	safeDeq;
	safeDeq.push_back(new int(6));
	SafePtrs<std::set<int*>>	safeSet;
	safeSet.insert(new int(4));
	// safe maps for SafePtrs
	SafePtrs<std::map<int,int*>>	spairs{ {3,new int(9)} };
	SafePtrs<std::multimap<int,int*>> smt;
	smt.emplace(3, new int(1));
	SafePtrs<std::unordered_map<int,int*>> sum;
	sum.emplace(3, new int(2));
	SafePtrs<std::unordered_multimap<int,int*>> sumt;
	sumt.emplace(3, new int(4));

	return 0;
}
