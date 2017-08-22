#include  <iostream>

//using namespace std;

std::ostream& manipulate(std::ostream& out)
{
	out << "flushed\t";
	return out;
}

//typedef std::ostream& (*manipulator)(std::ostream&);
using manipulator = decltype( manipulate );
using outType	  = decltype( std::cout );
outType& operator<<(outType& out, manipulator m)
{
	return (*m)(out);
}

int main()
{
	std::cout << "before manipulation:";
	manipulate(std::cout);
	//std::cout << manipulate;

	return 0;
}


