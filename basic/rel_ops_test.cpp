#include <utility>
#include <iostream>

struct Item{
	Item(int _k):k(_k){}
	int k;
};

bool operator< ( Item const& lhs, Item const& rhs){
	return lhs.k<rhs.k;
}
bool operator== ( Item const& lhs, Item const& rhs){
	return lhs.k==rhs.k;
}

using namespace std;
using namespace std::rel_ops;

int main(){
	Item i1=3, i2 = 4;
	cout << boolalpha << (i1 > i2) << '\n';
	cout << boolalpha << (i1 >= i2) << '\n';
	cout << boolalpha << (i1 <= i2) << '\n';
	cout << boolalpha << (i1 != i2) << '\n';

	return 0;
}

