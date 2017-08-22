#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

template<typename T>
struct iterator_trait {
	typedef typename T::iterator iterator;
	typedef typename T::value_type value_type;
	static iterator begin(T& c) { return c.begin(); }
	static iterator end(T& c) { return c.end(); }
};

template<typename T, size_t N>
struct iterator_trait<T[N]> {
	typedef T* iterator;
	typedef T value_type;
	static iterator begin(T arr[N]) { return arr; }
	static iterator end(T arr[N]) { return arr + N; }
};

template<typename T, typename Trait = iterator_trait<T>>
// use reference to keep array from decaying to pointer
void print_each(T& container) {
	std::copy(Trait::begin(container), Trait::end(container), std::ostream_iterator<typename Trait::value_type>(std::cout, "\t"));
}

int main() {
		
	int arr_i[] = { 1, 2 };
	std::string arr_s[] = { "3", "4" };
	std::vector<float> vec_f = { 5.f, 6.f };
	std::list<double> lst_d = { 7., 8. };
	print_each(arr_i); // 1 2
	print_each(arr_s); // 3 4
	print_each(vec_f); // 5 6
	print_each(lst_d); // 7 8
}

