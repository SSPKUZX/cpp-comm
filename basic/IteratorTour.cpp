#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	using namespace std;

	vector<int> vect;
	for(int idx=0; idx < 6; ++idx)
	{
		vect.push_back( idx );
	}

	vector<int>::const_iterator it = vect.begin();

	it = max_element(vect.begin(), vect.end());
	cout << *it << endl;
	it = min_element(vect.begin(), vect.end());
	cout << *it << endl;
	
	it = find(vect.begin(), vect.end(), 3);
	//vect.insert(it, 9);
	while( it != vect.end())
	{
		cout << *(it++) << "\t";	
	}	
	cout << endl;

	sort(vect.begin(), vect.end());
	it = vect.begin();
	while( it != vect.end())
	{
		cout << *(it++) << "\t";	
	}	
	cout << endl;

	reverse(vect.begin(), vect.end());
	it = vect.begin();
	while( it != vect.end())
	{
		cout << *(it++) << "\t";	
	}	
	cout << endl;

	return 0;
}
