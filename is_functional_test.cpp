#include "traits_ext.h"

#include <iostream>

// lambda
auto lam_i2ii = [](int const idx, int const idx2){ return 2+idx; };	
// raw function
int i2ii(int, int){ return 0;}
void v2ii(int, int){}
// member function
struct m_i2ii
{
	int i2ii(int, int){ return 9;}
	int operator()(int, int){ return 8;}
};
struct m_i2ii_{};  // non-function object

int main(int argc, char* argv[])
{
	using namespace comm;
	std::cout << "is lam_i2ii functional=" << is_functional< decltype(lam_i2ii), int(int, int) >::value << std::endl;
	std::cout << "is lam_i2ii functional=" << is_functional< decltype(lam_i2ii), double(int, double) >::value << std::endl;
	std::cout << "is lam_i2ii functional=" << is_functional< decltype(lam_i2ii), int, int >::value << std::endl;
	std::cout << "is lam_i2ii functional=" << is_functional< decltype(lam_i2ii), int >::value << std::endl;

	std::cout << "is i2ii functional=" << is_functional< decltype(i2ii), int(int, int) >::value << std::endl;
	std::cout << "is i2ii functional=" << is_functional< decltype(i2ii), void(int, int) >::value << std::endl;
	std::cout << "is i2ii functional=" << is_functional< decltype(i2ii), int, int >::value << std::endl;
	std::cout << "is i2ii functional=" << is_functional< decltype(i2ii), int, int, double >::value << std::endl;

	std::cout << "is v2ii functional=" << is_functional< decltype(v2ii), void(int, int) >::value << std::endl;
	std::cout << "is v2ii functional=" << is_functional< decltype(v2ii), int(int, int) >::value << std::endl;
	std::cout << "is v2ii functional=" << is_functional< decltype(v2ii), int, int >::value << std::endl;
	std::cout << "is v2ii functional=" << is_functional< decltype(v2ii), int, int, float >::value << std::endl;

	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), int(m_i2ii*, int, int) >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), double(m_i2ii*, int, int) >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), int(m_i2ii, int, int) >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), std::string(m_i2ii, int, int) >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), m_i2ii, int, int >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), m_i2ii  >::value << std::endl;
	m_i2ii m_obj;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), m_i2ii*, int, int >::value << std::endl;
	std::cout << "is m_i2ii::i2ii functional=" << is_functional< decltype(&m_i2ii::i2ii), m_i2ii*, std::string, int >::value << std::endl;


	std::cout << "is m_i2ii functional=" << is_functional< m_i2ii, int(int,int) >::value << std::endl;
	std::cout << "is m_i2ii functional=" << is_functional< m_i2ii, void(int,int) >::value << std::endl;
	std::cout << "is m_i2ii functional=" << is_functional< m_i2ii, int,int >::value << std::endl;
	std::cout << "is m_i2ii functional=" << is_functional< m_i2ii, std::string>::value << std::endl;
	std::cout << "is m_i2ii functional=" << is_functional<m_i2ii >::value << std::endl;
	std::cout << "is m_i2ii functional=" << is_functional<m_i2ii_ >::value << std::endl;

	
};
