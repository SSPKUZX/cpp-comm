#include <iostream>
#include <cstdlib>
#include <ctime>
//#include <random>

int main()
{
	using namespace std;
	
	int seed = static_cast<unsigned int>( time(0) );
	srand(seed);

	//cout << rand() << endl;	
	//cout << "ha, first random\n";
	
	//generate a random number in a range
	int min=1, max=6;
	static const double fraction = 1.0 / ( static_cast<double>(RAND_MAX) + 1.0);
	
	for(int count=0; count < 10; ++count)
		cout << static_cast<int>( fraction * (max - min) * rand() + min) << endl;

	//cout << "here is r:" << r << "\t here is fraction:" << fraction << "\tr * fraction:" << r*fraction << "\trange:" << range << "\tval:" << val << endl;

	/*random_device rd;
	mt19937 mersenne( rd() );

	{	
		cout << "\t" << mersenne();
		if( （count + 1） % 5 ==0 )
			count << endl;
	}*/
}

