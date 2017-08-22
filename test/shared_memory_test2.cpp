#include "shared_memory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define IPCKEY 0x366378

typedef struct{
		char agen[10];
		unsigned char file_no;
} st_setting;

int main(int argc, char** argv)
{ 
	sys::SharedMemory<st_setting> shm(IPCKEY);
	shm.Create(false); //sys::SHM_CS::CLEAR_ON_EXIST ); // RETURN_ON_EXIST
	shm.Attach();

	// manipulate shm
	auto obj = shm.Get();		
	obj.file_no = 9;
	system("ipcs -m");

	shm.Detach();
	shm.Release();
	system("ipcs -m");

	return 0;
}

