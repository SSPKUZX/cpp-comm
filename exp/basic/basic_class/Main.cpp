#include "Date.h"
#include <iostream>


int main()
{
	Date date(2016, 8, 8), otherDate(2019 ,8 ,9);

	std::cout << "obtaining static variable: " << Date::getDAYS() << std::endl;
	
	std::cout << "call isEqual:" << isEqual(date, otherDate) << std::endl;
	
	std::cout << "call daysLater:" << date.daysLater(3).getDay() << std::endl;

	std::cout << "call operator+:" << (date + 3).getDay() << std::endl;

	std::cout << "call operator== :" << (date == otherDate) << std::endl;
	
	std::cin >> date;
	std::cout << "call operator << : " << date ;

	std::cout << "calling member function unary operator+: " << +date << std::endl;
	++date;
	std::cout << "call ++date:" << date;
	std::cout << "call date++:" << date;
	std::cout << "call date[2]:" << date[2] <<  std::endl;
	std::cout << "typecast into int: " << (int)date << std::endl;
	date = otherDate;
	std::cout << "call operator=: " << date;
	return 0;
}
