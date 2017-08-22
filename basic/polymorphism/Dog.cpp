#ifndef DOG_H_
#define DOG_H_

#include "Animal.cpp"

class Dog : public Animal
{
	public:
	virtual void speak()
	{
		std::cout << "Dog" << std::endl;
	}

	const char* getName() const
	{
		return "Dog";
	}

	virtual ~ Dog(){}
	
	virtual Animal* copy() const
	{
		return new Dog(*this);
	}
};

#endif
