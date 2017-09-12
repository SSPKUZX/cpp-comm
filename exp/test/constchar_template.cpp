#include <iostream>

template<class Tp>
class is_module_name_defined
{
	private:
		template<class U>
		static auto check(int) -> decltype( U::KModuleName, std::true_type());// { return U::KModuleName; }  

		template<class U>
		static std::false_type check(...);// { return typeid(U).name(); }

	public:
		static constexpr bool value = decltype(check<Tp>(0))();//)::value ? Tp::KModuleName : typeid(Tp).name();  
};

/* functional */
template<class Tp>
auto ModuleNameOf() -> typename std::enable_if< !is_module_name_defined<Tp>::value, const char*>::type{
	return typeid(Tp).name();
}

template<class Tp>
auto ModuleNameOf() -> typename std::enable_if< is_module_name_defined<Tp>::value, const char*>::type{
	return Tp::KModuleName;
}

/* typetrait */
template<class Tp, bool B = false>
struct ModuleNameChooser
{
	static const char* name() {
		return typeid(Tp).name(); // const char* name() const
	}
};

/** test cases */
template<class Tp>
struct ModuleNameChooser<Tp, true>
{
	static constexpr const char* name() {
		return Tp::KModuleName;
	}
};

template<class Tp>
using ModuleName = ModuleNameChooser<Tp, is_module_name_defined<Tp>::value>;

//static char moduleId[] = "mrv2appmaster";
//template<const char* moduleId> // ---> moduleId here must be global variable
template<class Module>
class SharedMemory
{
	public:
		void operator()() {
			std::cout << "current module [" << ModuleName<Module>::name() << "] is using shared memory\n";	
		}
};

class WithModuleName
{
	public:
		static constexpr const char* KModuleName = "mrv2appmaster on yard";
};

class WithModuleName2
{
	public:
		static const char* KModuleName;
};
const char* WithModuleName2::KModuleName = "mrv2appmaster on yard";

class NoModuleName{};

int main()
{
	std::cout << ModuleNameOf<WithModuleName>() << '\n';
	std::cout << ModuleNameOf<NoModuleName>() << '\n';

	SharedMemory<WithModuleName>()();
	SharedMemory<WithModuleName2>()();
	SharedMemory<NoModuleName>()();
		
	return 0;
}
