#include<iostream>
#include<string>

using namespace std;

template<typename T>
struct type_trait
{
	enum {	is_integral = false,
			is_float = false, };
};

template<> 
struct type_trait<char>
{
	enum {	is_integral = true,
			is_float = false, }; 
};

template<> 
struct type_trait<unsigned char>
{
	enum { is_integral = true,
		   is_float = false, }; 
};

template<>
struct type_trait<int>
{
	enum { is_integral = true,
		   is_float = false, }; 
};

template<>
struct type_trait<unsigned int>
{
	enum { is_integral = true,
		   is_float = false, }; 
};

template<>
struct type_trait<float>
{
	enum { is_integral = false,
	       is_float = true, };
};

////////////////////////////////////////////////////
/////////////////define new type////////////////////
////////////////////////////////////////////////////
struct big_integer 
{
	big_integer(const std::string&) { }
};

// tag it as integral
template<>
struct type_trait<big_integer>
{ 
	enum { is_integral = true,
		   is_float = false, }; 
};

/*************************************************/
/*********** the interface **********************/
/************************************************/
template<typename T, typename Trait = type_trait<T> >
bool is_integral(T) 
{
	return Trait::is_integral;
}

template<typename T, typename Trait = type_trait<T> >
bool is_float(T)
{
	return Trait::is_float;
}

/*
 * test the interface for different data type
 *
int main()
{
	cout << "is_integral(123) = " << ::is_integral(123) << endl; // int-> yes
	cout << "is_integral(big_integer(\"123\")) = " <<::is_integral(big_integer("123")) << endl; // big_integer-> yes
	cout << "is_integral(123.) = " << ::is_integral(123.) << endl; // double-> no
	cout << "is_integral(\"123\") = " << ::is_integral("123") << endl; // string-> no
	cout << "is_float(123.) = " << ::is_float((float)123.) << endl; // double-> no
	
}
*/ 
