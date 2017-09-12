#pragma once

#include <sys/ipc.h>
#include <stdexcept>

namespace sys
{
	template<class Tp>
	bool SharedMemory<Tp>::Create( bool useOnExist)//SHM_CS const cs)
	{
		if( !exists() ){
			return doCreate();
		}else if( !useOnExist ){
			return clear() ? doCreate() : false;
		}else { // USE_ON_EXIST
			// do nothing, just use the existed one
			return true;	
		}
	}

	template<class Tp>
	bool SharedMemory<Tp>::Attach()
	{
   		void* memory_addr = shmat(m_shmId, NULL, 0);
   		if (memory_addr == reinterpret_cast<void*>(-1)) {
   		    return false;
		}else {
			m_pObject = reinterpret_cast<Tp*>(memory_addr);
			return true;	
		}
	}

	template<class Tp>
	Tp& SharedMemory<Tp>::Get() &
	{
		if( nullptr == m_pObject)			
			throw std::logic_error("the object in shared memory is not attached, so unable to obtained"); 
		else
			return *m_pObject;
	}
	
	template<class Tp>
	const struct shmid_ds SharedMemory<Tp>::State() const
	{
		struct shmid_ds ds;	
		shmctl( m_shmId, IPC_STAT, &ds);
		return ds;
	}

	template<class Tp>
	bool SharedMemory<Tp>::Detach()
	{
		if( nullptr == m_pObject)			
			throw std::logic_error("the object in shared memory is not attached, so unable to detached"); 
		return -1 != shmdt(m_pObject);	
	}

	template<class Tp>
	bool SharedMemory<Tp>::Release()
	{
		return -1 != shmctl( m_shmId , IPC_RMID , NULL );
	}

	template<class Tp>
	bool SharedMemory<Tp>::exists()
	{
		m_shmId = shmget( m_shmKey , sizeof(Tp), 0640);     
		return -1 != m_shmId;
	}

	template<class Tp>
	bool SharedMemory<Tp>::doCreate()
	{
		m_shmId = shmget( m_shmKey, sizeof(Tp), 0640|IPC_CREAT|IPC_EXCL); 
		return -1 != m_shmId;
	}

	template<class Tp>
	bool SharedMemory<Tp>::clear()
	{
		return Attach() && Detach() && Release();				
	}
}
