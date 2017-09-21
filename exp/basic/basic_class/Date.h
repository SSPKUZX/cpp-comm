#ifndef DATE_H_
#define DATE_H_

#include <iostream>

class Date 
{
	private:
	int year;
	int month;
	int day;
	static int DAYS;

	public:
	Date(int, int, int);
	const int& getYear() const;
	const int& getMonth() const;
	const int& getDay() const;
	void set(int, int, int);
	Date daysLater(int);
	static int getDAYS();

	Date& operator++ ();
	Date  operator++ (int);
	int&  operator[](const int index);
	//type cast into int
	operator int();
	
	//copy constructor
	Date(const Date& );
	Date& operator= (const Date&);

	friend bool isEqual(const Date&, const Date&);	

	friend bool operator< (const Date&, const Date&);
	friend bool operator> (const Date&, const Date&);
	friend bool operator<= (const Date&, const Date&);
	friend bool operator>= (const Date&, const Date&);
	friend bool operator== (const Date&, const Date&);
	friend bool operator!= (const Date&, const Date&);
	
	friend Date operator+ (const Date&, int);
	//member function: overloading the unary operator+
	Date operator+ ();	
	friend std::ostream& operator<< (std::ostream&, const Date&);
	friend std::istream& operator>> (std::istream&, Date&);
 
};

#endif
