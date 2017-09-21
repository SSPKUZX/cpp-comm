#include "Dog.cpp"
#include "Animal.cpp"
#include "Cat.cpp"

int main()
{
	Animal *dog = new Dog();
	//dog->speak();
	Animal *cat = new Cat();
	//cat->speak();
	Animal* animals[2];
	animals[0] = dog;
	animals[1] = cat;
	int idx = 0 ;

	for( ; idx < 2; ++idx)
		animals[idx]->speak();
	delete animals[0];

	animals[0] = animals[1]->copy();	
	for( idx=0; idx < 2; ++idx)
		animals[idx]->speak();

	delete animals[0];
	delete animals[1];

	//std::cout << "animal's name:" << ani->getName() << std::endl; 
	return 0;
}
