#include <iostream>

void swap(int *x, int *y)
{
	int temp= *x;
	*x = *y;
	*y = temp;
}

int main()
{
	int left = 0, right =1;

	swap(&left, &right);

	std::cout << "left = " <<left <<"\t right = " << right << std::endl;

	return 0;
}
