#ifndef ANIMAL_H_
#define ANIMAL_H_	

#include <iostream>

class Animal
{
	public:
	virtual void speak()
	{
		std::cout << "???" << std::endl;
	}
	
	virtual ~ Animal(){}

	virtual Animal*  copy() const{}	
};

#endif
