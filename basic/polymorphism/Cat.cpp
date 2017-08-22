#include "Animal.cpp"

class Cat : public Animal
{
	public:
	virtual void speak()
	{
		std::cout << "Cat" << std::endl;
	}
	
	virtual Animal* copy() const
	{
		return new Cat(*this);
	}
};
