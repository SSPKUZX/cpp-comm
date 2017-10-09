#include <memory>
#include <algorithm>

#include "noncopyable.h"

namespace utl{ 

	/*
	 * problem description: for pointers stored in stl containers, user has to 
	 *				delete them manually.
	 * solutions: provide container or container-alike type to delete pointers 
	 *			automatically
	 * */

	/*
	 *	solution A: std::unique_ptr<ContainerType, DefaultPtrsDeleter>
	 * */
	template<class T>
	struct SafeUptr_t;
	template<class T>
	using SafeUptr = typename SafeUptr_t<T>::type;	

	template<class T, class... Args>
	inline SafeUptr<T> MakeSafeUptr(Args&&... args){
		return SafeUptr<T>( new T(std::forward<Args>(args)...) );	
	}

	// support automatic deletion for elements of vector/list/deque/set
	struct ValuesDeleter{
		template< template<class, class... > class ValuesType, class T, class... Args >
		void operator()( ValuesType<T*,Args...>* valuePtr){
			if( valuePtr ){
				std::for_each(valuePtr->begin(), valuePtr->end(), std::default_delete<T>() );	
			}	
		}	
	};
	template< template<class, class... > class ValuesType, class T, class... Args >
	struct SafeUptr_t<ValuesType<T*,Args...>> {
		using type = std::unique_ptr< ValuesType<T*,Args...>, ValuesDeleter>;
	};

	// support automatic mapped type's deletion of map/multimap/unordered_map/unordered_multimap
	struct MappedTypesDeleter{
		template< template<class, class, class...> class MapType, class KeyType, class ValueType, class... Args>
		void operator()(MapType<KeyType, ValueType*, Args...>* mapPtr){
			if(mapPtr){ 
				std::for_each( mapPtr->begin(), mapPtr->end(), 
						[]( std::pair<const KeyType, ValueType*>& k2v){ delete k2v.second; }
					); 
			}
		}
	};
	template< template<class, class, class...> class MapType, class KeyType, class ValueType, class... Args>
	struct SafeUptr_t< MapType<KeyType,ValueType*,Args...> >{
		using type = std::unique_ptr< MapType<KeyType,ValueType*,Args...>, MappedTypesDeleter >; 
	};



	/*
	 * solution B: provide containers with the same functional methods as stl containers 
	 *	but user must manage ptrs outside of containers, like delete them manually
	 *
	 * abandomed solution:
	 	Providing the same functionality as stl containers by private inheritance and operator-> :
		by making inheritance private, upcasting which causes memory leaks would be avoided in a certain degree
		but even in C++11 a C-style cast can "break through" access protection and perform a formally correct upcast
		like this: auto ptr = (T*)(&SafePtrs<T>); delete ptr; This should be avoided. 
		but no way to stop following undefined behaviors, b' deleting objects on stack is always incorrect:
		thought above can be avoided by following delete statement:
			SafePtrs<ValuesType<T*, Args...>>*	operator&() = delete;
			const SafePtrs<ValuesType<T*, Args...>>*	operator&() const = delete;
		but still, no one could save someone who wants to commit suicide like following:
			auto values = (ValuesType<T*, Args...>&)(safePtrs);
			auto ptr	= (ValuesType<T*, Args...>*)(&values);
			or: auto ptr	= (ValuesType<T*, Args...>*)(std::addressof(safePtrs));
			delete ptr;
		in the other hand, by not providing [operator*], unexpected upcasting could be avoided like following:
			std::vector<int*>* ptr = &*SafePtrs<std::vector<int*>>;
		also copy will be avoided like following:
			std::vector<int*> copyed = *SafePtrs<std::vector<int*>>;
		but of course, if user does want, copy is still possible like following:
			std::copy(safePtrs->begin(), safePtrs->end(), std::back_inserter(copyed));
			std::vector<int*> copyed2(safePtrs->begin(), safePtrs->end());
		for those occasions, pointers should NOT be deleted manually
	 *		
	 *	currently, forbidding allocation SafePtrs on heap through [operator new] is the best way to stop conversion
	 *	from SafePtrs to STL containers which may cause memory leaks by deleting stl containers ptr. 
	 * */
	template<class T>
	struct SafePtrs;
	// support for vector/list/deque/set
	template< template<class, class...> class ValuesType, class T, class... Args >
	struct SafePtrs<ValuesType<T*, Args...>> final: public ValuesType<T*, Args...>, Noncopyable{
		template<class... RArgs>
		SafePtrs(RArgs&&... rargs) : ValuesType<T*, Args...>( std::forward<RArgs>(rargs)...){} 
		// initializer_list can't be forwarded as args
		SafePtrs( std::initializer_list<T*> il) : ValuesType<T*, Args...>(std::move(il)){}

		// forbid allocation on heap to avoid following potential memory leaks
		// auto ptr = (ValuesType<T*, Args...>*)( new SafePtrs<ValuesType<T*,Args...>>(RArgs...) );
		// ...;   delete ptr;
		void* operator new( std::size_t) = delete;

		~SafePtrs(){ 
			ValuesDeleter()(static_cast<ValuesType<T*, Args...>*>(this)); // --> vector/list/deque need static_cast, set doesn't --->why ???
		}
	};
	//support for (unordered_)(mulit)map
	template< template<class, class, class...> class MapType, class KeyType, class ValueType, class... Args>
	struct SafePtrs< MapType<KeyType,ValueType*,Args...> > final: public MapType<KeyType,ValueType*,Args...>, Noncopyable{
		template<class... RArgs>
		SafePtrs(RArgs&&... rargs) : MapType<KeyType,ValueType*,Args...>( std::forward<RArgs>(rargs)...){} 
		// initializer_list can't be forwarded as rargs
		SafePtrs( std::initializer_list<std::pair<const KeyType, ValueType*>> il) : MapType<KeyType,ValueType*,Args...>(std::move(il)){}

		// forbid allocation on heap to avoid potential memory leaks
		void* operator new( std::size_t) = delete;
		~SafePtrs(){ 
			MappedTypesDeleter()(this);
		}
	};

	/*
	 * solution C: user specify pointers as smart_ptrs like std::vector<std::unique_ptr<T>>
	 *			or std::map<K,std::shared_ptr<V>>, then pointers are totally managed automatically
	 * */

} // end of utl

