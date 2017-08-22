#include "Date.h"

int Date::DAYS = 0;

int Date::getDAYS() 
{
	return DAYS;
}

Date::Date(int y, int m, int d):year(y), month(m), day(d)
{
	++DAYS;
}

Date::Date(const Date& date)
{
	year = date.year;
	month = date.month;
	day = date.day;
}

const int& Date::getYear() const
{
	return year;
}

const int& Date::getMonth() const
{
	return month;
}

const int& Date::getDay() const
{
	return day;
}

void Date::set(int y, int m, int d)
{
	year = y;
	month = m;
	day = d;
}

Date Date::daysLater(int days)
{
	return Date(year,month,day+days);
}

Date Date::operator+ ()
{
	return *this;	
}

Date& Date::operator++()
{
	++day;
	return *this;
}

Date Date::operator++(int dumy)
{
	Date old(*this);
	++day;

	return old;
}

int& Date::operator[](const int index)
{
	switch(index)
	{
		case 0: return year;
		case 1: return month;
		case 2: return day;
	}
}

Date::operator int()
{
	return year;
}

Date& Date::operator= (const Date& date)
{
	if(*this == date)
		return *this;
	
	// presume that there is allocated pointer 'ptr'
	// then first of all is to dealloate the pointer like following:
	// delete ptr;
	year = date.year;
	month = date.month;
	day = date.day;

	return *this;	
}

bool isEqual(const Date &date, const Date &other)
{
	return (date.year == other.year) && (date.month == other.month) && (date.day == other.day);
}


Date operator+ (const Date &date, int days)
{
	return Date(date.year, date.month, date.day + days);
}

bool operator< (const Date &date, const Date &other)
{
	if (date.year < other.year) 	return true;
	if (date.year > other.year) 	return false;
	if (date.month < other.month)  	return true;
	if (date.month > other.month)	return false;
	return date.day < other.day;
}

bool operator> (const Date &date, const Date &other)
{
	return other < date;
}

bool operator<= (const Date &date, const Date &other)
{
	return !(other < date);
}

bool operator>= (const Date &date, const Date &other)
{
	return !(date < other);
}

bool operator!= (const Date &date, const Date &other)
{
	return (date < other) || (other < date); 
}

bool operator== (const Date &date, const Date &other)
{
	return !(date != other);
}
	
std::ostream& operator<< (std::ostream &out, const Date &date)
{
	out << date.year << "\\" << date.month << "\\" << date.day << std::endl;
	
	return out;
}

std::istream& operator>> (std::istream &in, Date &date)
{
	in >> date.year;
	in >> date.month;
	in >> date.day;

	return in;
}
