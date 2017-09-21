#include <iostream>
#include <vector>
#include <fstream>

#include "pipelize.h"

using namespace std;
using namespace utl;

// read records from file
vector<string> read_file(std::string const& filePath){ 
	fstream input(filePath);
	if(input) cout << "succ in reading " << filePath << '\n';
	else cout << "fail in reading " << filePath << '\n'; 
	vector<string> ips;
	string line;
	while( input >> line )
	{
		ips.push_back(line);
	}
	input.close();
	return std::move(ips);
}

// sort elements
struct sorter{
	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
	inline SeqContainer<T> operator()( SeqContainer<T> const& records) const{
		SeqContainer<T> ret(records);
		std::sort(ret.begin(), ret.end());	
		return std::move(ret);
	}

	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
	inline SeqContainer<T>& operator()( SeqContainer<T>& records) const{
		std::sort(records.begin(), records.end());	
		return records;
	}

	template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T>
	inline SeqContainer<T> operator()( SeqContainer<T>&& records) const{
		std::sort(records.begin(), records.end());	
		return std::move(records);
	}

};

inline sorter sort(){ return sorter(); }

// calcalute difference set
template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
struct difference{
	difference( SeqContainer<T> && records) : m_right_records( std::move(records) ){
		sort(m_right_records.begin(), m_right_records.end());
	}

	template< template<class T2, class alloc = std::allocator<T2>> class SeqContainer2, class T2> 
	inline SeqContainer<T> operator()(SeqContainer2<T2> const& leftRecords) const{
		SeqContainer<T> diff;
		set_difference(m_right_records.begin(), m_right_records.end(), leftRecords.begin(), leftRecords.end(), back_inserter(diff));	
		return std::move(diff);
	}	

	private:
		SeqContainer<T>	m_right_records;
};


template< template<class T, class alloc = std::allocator<T>> class SeqContainer, class T> 
inline difference<SeqContainer, T> differenciate(SeqContainer<T> records){
	return difference<SeqContainer,T>(std::move(records));
}

int main(){
	read_file("online_ips.txt") | sort() | differenciate( read_file("all_ips.txt") ) | utl::log();
	return 0;
}
