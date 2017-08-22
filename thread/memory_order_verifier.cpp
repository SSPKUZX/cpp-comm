#include<atomic>
#include<memory>
#include<iostream>

int main(int argc, char* argv[])
{
	// atomic load/release of shared_ptr
	auto spInt = std::make_shared<int>(0);
	std::cout << "atomic_load of spInt="  << *std::atomic_load( &spInt ) << std::endl;
	std::cout << "atomic_storing  spInt\n";
	std::atomic_store( &spInt, std::make_shared<int>(3) );
	std::cout << "atomic_load of spInt="  << *std::atomic_load( &spInt ) << std::endl;

	// verifying memory_order_relaxed
	// for different atomic variables x,y,z, the holder thread will keep their local x,y,z
	// in thread local cache and refresh them to raw address at any time independently. 
	// what we can only ensure is the relative order of a certain value for a certain thread.
	// the value list for a certain value from different threads  may interleave in any manner
	// example as follows,  no order exists between $atom_a and $atom_b in loading and storing 
	// but (1) and (3) have a order constraint on variable atom_a which means if thread#2 has  
	// ever loaded '9' set in (2), thread#2 can only load '9' later( loading '3' will never happen ) 
	// std::atomic<int> atom_a(0), atom_b(0);; 
	// atom_a.store( 3, std::memory_order_relaxed); // thread #1 (1)
	// atom_b.store( 3, std::memory_order_relaxed); // thread #1 (2)
	// atom_a.store( 9, std::memory_order_relaxed); // thread #1 (3)
	// int a = atom_a.load( std::memory_order_relaxed); // thread #2 (4)							   
	// int b = atom_b.load( std::memory_order_relaxed); // thread #2 (5)							   

	// verifying memory_order_release && ~acquire -----> no dependency between different variables
	std::atomic<bool>	x(false), y(false);
	std::atomic<int>	z(0);
	std::thread writer_a( [&x](){ x.store(true, std::memory_order_release); } );	
	std::thread writer_b( [&y](){ y.store(true, std::memory_order_release); } );	
	std::thread reader_a( [&x, &y, &z]()
						{	while( !x.load(std::memory_order_acquire ) ); 
							if( y.load(std::memory_order_acquire) )
								++z;
						} 
					);	
	std::thread reader_b( [&x, &y, &z]()
						{	while( !y.load(std::memory_order_acquire ) ); 
							if( x.load(std::memory_order_acquire) )
								++z;
						} 
					);	
	writer_a.join();
	writer_b.join();
	reader_a.join();
	reader_b.join();
	std::cout << "the final z is " << z.load();
	// analysis : there is no ordering between variable x and y, 
	// so here is the outcome :
	// from reader_a's perspective, x is true while y is false or true finally
	// from reader_b's perspective, y is true while x is false or true finally
	// so z's value could be 0,1 or 2;
	
	// verifying memory_order_release && ~acquire/consume ----->  transitive synchronization 
	std::atomic<int>	m(0),n(0);
	std::atomic<bool>	sync(false);
	std::thread before( [&m,&n,&sync]()
					{
						m.store( 2, std::memory_order_relaxed);	// does fetch_sub still work here?
						n.store( 4, std::memory_order_relaxed);					
						sync.store( true, std::memory_order_release); 
					} );
	std::thread after( [&m,&n,&sync]()
					{
						while( !sync.load(std::memory_order_acquire ) );
						//while( !sync.load(std::memory_order_consume) );
						std::cout << "the final m is " << m.load( std::memory_order_relaxed) << std::endl;					
						std::cout << "the final n is " << m.load( std::memory_order_relaxed) << std::endl;					
					} );
	before.join();
	after.join();
	// analysis : for thread before, store-release on sync makes m&n&sync as a batch and get updated together 
	//			  so the final value of m&n in thread after will be 2&4 for sure
	// ####### for 'consume', 'sync' will only synchronize itself, leaving m&n unsynchronized
	//			  so the final value of m&n in thread after will be 0|2&0|4 for uncertainty 


	


	return 0;
}
