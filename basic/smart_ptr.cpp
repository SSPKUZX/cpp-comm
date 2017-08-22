#include <memory>
#include <iostream>

auto custom_deleter = [](int* p){ std::cout << "customed deleter\n"; delete p; };

std::unique_ptr<int, decltype(custom_deleter)> create()
{
	std::unique_ptr<int, decltype(custom_deleter)> product(nullptr, custom_deleter); 
	product.reset( new int(5));

	return product;
}

/*std:: enable_shared_from_this. That’s a template for a base class you inherit from 
 *if you want a class managed by std::shared_ptrs to be able to safely create 
 *a std::shared_ptr from a this pointer.
 * */
class SharedThisEnabler : public std::enable_shared_from_this< SharedThisEnabler>
{
	public:
		template<class... Args>
		static std::shared_ptr< SharedThisEnabler> create( Args&&... args)
		{
			return std::shared_ptr<SharedThisEnabler>( new SharedThisEnabler( std::forward<Args>(args)... ));
			//std::make_shared< SharedThisEnabler>( std::forward<Args>(args)... );	
			// make_shared won't work b' it requires public constructor for type
		};

		std::shared_ptr<SharedThisEnabler> useThis()
		{
			// shared_from_this will search corresonding control block for the 'this' pointer
			// which requires that s shared_ptr of this type has already existed
			// if not, undefined behavior happens
			// to prevent from this, it's often a good practice to qualify constructors
			// as private and have a static method to create shared_ptr of this type
			return shared_from_this();// member from enable_shared_from_this	
		}

	private:
		SharedThisEnabler() = default;
};


int main()
{
	//unique_ptr ensures exclusive ownership for resource
	//it's move-only objects
	// it's a good choice in factory method	
	auto ui = create();	
	std::cout << "value of ui=" << *ui << "\n";
	std::shared_ptr<int> si = std::move(ui);
	std::cout << "value of si=" << *si << "\n";

	//shared_ptr shares ownership
	//it differs in assigning customed deleter
	si = std::shared_ptr<int>( new int(6), [](int* p) { delete p; });
	// when passing raw pointer as constructor parameter for sp
	// it must ensure that no other smart ptrs should point to it
	// otherwise undefined behavior will happen
	//
	// shared_ptr  does not support T[]

	auto ste = SharedThisEnabler::create();

	// weak_ptr can't be dereferenced or tested for nullness
	// it's an augumentation to shared_ptr, the resource it 
	// points to may no longer exists and it does not affect
	// the reference count
	std::weak_ptr<int> wi(si);
	si = nullptr;
	if( wi.expired() )
	{
		std::cout <<"weak ptr expired\n";	
	}
	// get a shared ptr from weak ptr when dereference for 
	// weak ptr is needed
	// if weak ptr expires, shared ptr has nullptr inside
	auto swi = wi.lock();
	// if wi expires, bad_weak_ptr throws
	// so lock is preferred
//	std::shared_ptr<int> swi2(wi); 

	//weak ptr can be used when there is only necessity to check whether
	//a ptr expires like cache&release, subject&observer pattern  and cyclic
	//shared ptr example in item20 of book <<effective modern c++>> p137 around


	// prefer the 'make' functions in creating smart ptrs because of efficiency&
	// exception safety, but there are times that 'make' can't do like assigning
	// customed deleter or using brace initializer or creating types with class-specific
	// operator new&delete
	// a good practice to use customed deleter without make_shared
	std::shared_ptr<int> spi( new int(2), [](int* p){ delete p;} );
	// useSp( std::move(spi), computeOtherArgs() );
	
	// unique_ptr requires that Tp must be complete while shared_ptr doesn't
	// for more detail, please refer to ../pimpl idiom

	return 0;
}

