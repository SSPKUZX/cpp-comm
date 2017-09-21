#include <utility>


template<typename T>
class Holder
{
	private:
		T*	m_p;

	public:
		Holder( T* const _p = nullptr) :  m_p(_p) {}
		Holder(const Holder& rhs) : m_p( rhs.m_p ==nullptr ? nullptr : new T(*rhs.m_p)) {}
		~Holder(){ delete m_p;}

		/** copy and swap display***/
		friend void swap(Holder& lhs, Holder& rhs)
		{
			using std::swap;
			swap(lhs.m_p,  rhs.m_p);
		}

		// work for both Holder const& and Holder&&
		Holder& operator= ( Holder other)   // by value, so the compiler will copy value through default constructor into "other"
		{
			swap(*this, other);
			return *this;
		}

		//for c++0x
		Holder(Holder&& other): Holder()   // construct a default value
		{
			swap(*this, other);
		}
};

int main()
{
	Holder<int>	h(new int(4)), h7;
	auto h8 = h7; // segment default
	Holder<int> h2 = h, h3( Holder<int>(new int(5)));
	h = h3;
	h = Holder<int>(new int(9));
}
