#include <iostream>
#include <cassert>

#pragma pack(push,8)
struct eight
{
	uint8_t		byte8;
	uint64_t	byte64;  // if this does not exist, size of eight is 1
};
#pragma pack(pop)

#pragma pack(4)   // same as push 4
struct four 
{
	uint8_t		byte8;
	uint32_t	byte32; // if following fields do not exist, size of four is 1
	uint8_t		byte8_;
	uint16_t	byte16;	
};
#pragma pack()

// compare to struct 'four', 'four2' has a better field order
// in which its size won't change when alignment length changes.
// for 'four', alignment length --> size is 1->8,2->10,4/8/16->12 
#pragma pack(push,2)
struct four2 
{
	uint8_t		byte8;
	uint8_t		byte8_;
	uint16_t	byte16;  // the align boundary is min(sizeof(uint16_t), N), ie min(2,4)=2	
	uint32_t	byte32; 
};
#pragma pack(pop)


int main()
{
	std::cout << "sizeof eight is=" << sizeof(eight) << "\n";
	std::cout << "sizeof four  is=" << sizeof(four) << "\n";
	std::cout << "sizeof four2 is=" << sizeof(four2) << "\n";
	assert( sizeof(eight) == 16);
	assert( sizeof(four)  == 12);
	assert( sizeof(four2) == 8);
	std::cout << "struct eight and four got aligned\n";
}
