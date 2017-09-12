#pragma once

//#include <mutex>
#include <type_traits>
#include <sys/types.h>
#include <sys/shm.h>

namespace sys
{
	// creation strategy of shared memory
/*	enum class SHM_CS
	{
		CLEAR_ON_EXIST, // when the specified shared memory exists, clear it and create a new one	
		USE_ON_EXIST // when exists already, just use it
	};
*/
	// this is designed for managing a specific object in shared memory 
	// the type of object must be trivial( all data member are trivial, not pointer) 
	// so far, no synchronization is guaranteed so it's single-threaded
	template<class Tp>	
	class SharedMemory
	{
		public:
			using value_type = Tp;

			static_assert( std::is_trivial<Tp>::value, "type of SharedMemory must be trivial");

			SharedMemory( key_t const shmKey) : m_pObject(nullptr), m_shmId(-1), m_shmKey( shmKey){}
			//create the shared memory from system 
			bool Create(bool useOnExist = true); // SHM_CS const cs = SHM_CS::USE_ON_EXIST);
			//attach shared memory to current process		
			bool Attach();
			//obtain object of Tp in the shared memory 
			Tp&  Get() &; 	
			//display content of shared memory
			const struct shmid_ds State() const;	
			//detach shared memory from current process		
			bool Detach();
			//release the attached shared memory
			bool Release();

			//noncopyable
			SharedMemory( SharedMemory const&) = delete;
			SharedMemory& operator=( SharedMemory const&) = delete;
	
		private:
			// does shmId exists already in shared memory
			bool exists();
			// create the shared memory in any case
			bool doCreate();
			// clear the existed shared memroy with shmKey
			bool clear();	

		private:
			Tp*			m_pObject;
			int			m_shmId;
			key_t		m_shmKey;
//			std::mutex	m_mutex; // should used for synchronization 
	}; // end of class SharedMemory

} // end of namespace sys

#include "shared_memory_inl.h"
