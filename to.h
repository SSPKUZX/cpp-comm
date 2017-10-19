#include <sstream>

#include "variadic.h" 

namespace utl{

	template<class StrLike>
	struct is_string_like : std::integral_constant<bool,std::is_same<StrLike,std::string>::value || std::is_same<StrLike, char*>::value >{};

	template<std::size_t N>
	struct is_string_like<char[N]> : std::true_type{};

	//case 1: integral or float -> string 
	template<class Dst,class Src>
	inline auto To(Src const& src)
	-> typename std::enable_if<(std::is_integral<Src>::value||std::is_floating_point<Src>::value)&&is_string_like<Dst>::value, std::string>::type {
		return std::to_string(src);	
	}

	// case 2: string -> integral or float
	// string -> int
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,int>::value, int>::type{
		return std::stoi(str);	
	}
	// string -> unsigned int
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,unsigned int>::value, unsigned int>::type{
		auto l = std::stol(str);
		if(l>std::numeric_limits<unsigned int>::max() || l<std::numeric_limits<unsigned int>::min()){
			throw std::out_of_range(str+" is greater than maximal uint or less than minimal uint");
		}
		return static_cast<unsigned int>(l);	
	}
	// string -> long
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,long>::value, long>::type{
		return std::stol(str);	
	}
	// string -> unsigned long
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,unsigned long>::value, unsigned long>::type{
		return std::stoul(str);	
	}
	// string -> long long
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,long long>::value, long long>::type{
		return std::stoll(str);	
	}
	// string -> unsigned long long
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,unsigned long long>::value, unsigned long long>::type{
		return std::stoull(str);	
	}
	// string -> float 
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,float>::value, float>::type{
		return std::stof(str);	
	}
	// string -> double 
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,double>::value, double>::type{
		return std::stod(str);	
	}
	// string -> long double 
	template<class Dst,class Src>
	inline auto To(Src const& str)
	->typename std::enable_if<is_string_like<Src>::value&&std::is_same<Dst,long double>::value, long double>::type{
		return std::stold(str);	
	}

	// case 3: enum to integral or float
	template<class Dst,class Src>
	auto To(Src const& src)
	-> typename std::enable_if<std::is_enum<Src>::value && (std::is_integral<Dst>::value
									||std::is_floating_point<Dst>::value), Dst>::type {
		using UT = typename std::underlying_type<Src>::type;	
		UT e = static_cast<UT>(src);
		if(e>std::numeric_limits<Dst>::max()){
			throw std::out_of_range("enum value="+std::to_string(e)+" is greater than maximal Dst type");	
		}
		return static_cast<Dst>(e);	
	}

	// case 4: integral to enum
	template<class Dst, class Src>
	inline auto To(Src const& src)
	-> typename std::enable_if<std::is_integral<Src>::value && std::is_enum<Dst>::value, Dst>::type {
		return static_cast<Dst>(src);
	}

	// case 5: Src can implicitly convert to Dst
	template<class Dst, class Src>
	inline auto To(Src&& src)
	-> typename std::enable_if<std::is_convertible<Src,Dst>::value, Dst>::type {
		return std::forward<Src>(src);
	}

	// case 6: concat srcs... to string
	struct ToHook{
		ToHook(std::ostringstream& oss_) : oss(oss_){}
		template<class T>	
		void operator()(T const& val){
			oss << To<std::string>(val);	
		} 

		std::ostringstream&	oss;
	};
	template<class... Args>
	auto To(Args&&... args)
	-> typename std::enable_if<(sizeof...(Args)>1), std::string>::type{
		std::ostringstream oss;	
		vparams(ToHook(oss), std::forward<Args>(args)...);	
		return oss.str();
	}

	// case 7: for T,if ostream& operator<<( ostream& os, T const&) has defined,
	//  not integral, not floating point and not string like
	// then to<std::string> is enabled
	template<class T>
	struct is_out_streamable{
		template<class U>	
		static auto test(U* ptr)->decltype( std::declval<std::ostream>() <<*ptr, std::true_type{});
		template<class U>	
		static auto test(...)->std::false_type;

		enum{ value = decltype(test<T>(nullptr)){}, };
	};

	template<class Dst, class Src>
	auto To(Src const& src)
	-> typename std::enable_if<is_out_streamable<Src>::value && not std::is_integral<Src>::value && not std::is_floating_point<Src>::value && not is_string_like<Src>::value && std::is_same<std::string,Dst>::value, std::string>::type{
		std::ostringstream oss;	
		oss<<src;
		return oss.str();	
	}

} // end of utl
