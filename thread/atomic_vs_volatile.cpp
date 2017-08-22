#include <atomic>

int main()
{
	// std::atomic will guarantee that
	// 1> operations on atomic objects are atomic
	std::atomic<int>	ai(0);
	++ai;    //READ&MODIFY&WRITE are grouped as a atomic action
	// 2> prevent compilers to reordering code sequences
	int x = 0;
	ai = 3;
	int y = 4;
	// if ai is not atomic, the above three sentences may be 
	// reordered to any oreder
	
	// volatile tells compilers to optimise code
	volatile int vi = 3;
	vi = 4;
	// if vi is not volatile, compilers will optimize it 
	// as int vi = 4 even vi is a atomic object;
	int temperature = 4;
	vi = temperature;
	vi = temperature;
	// volatile is suitable for special memory  
	// like memory mapped I/O. like the code above
	// temperature comes from a sensor, its value changes
	// any time, so multiple assignments make sense.
	// optimization by compilers is wrong here

	return 0;
}
