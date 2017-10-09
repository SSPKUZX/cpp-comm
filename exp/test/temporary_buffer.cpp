#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>

int main(){

	int numbers[] = {30,50,10,20,60,40};

	// use temporary buffer to sort and show the numbers:
	std::pair <int*,std::ptrdiff_t> result = std::get_temporary_buffer<int>(6);
	if (result.second>0)
	{
		std::uninitialized_copy (numbers,numbers+result.second,result.first);
		std::sort (result.first,result.first+result.second);
		std::cout << "sorted numbers  : ";
		for (int i=0;i<result.second;i++)
			std::cout << result.first[i] << " ";
		std::cout << '\n';
		std::return_temporary_buffer (result.first);
	}
	
	// show original numbers:
	std::cout << "unsorted numbers: ";
	for (int i=0;i<6;i++)
	std::cout << numbers[i] << " ";
	std::cout << '\n';

	return 0;
}
