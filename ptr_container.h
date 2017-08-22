#include <vector>

namespace wxg{

	template<class T>
	class PtrContainer{
		static_assert(std::is_object<T>::value&&!std::is_pointer<T>::value, \
				"type T in PtrContainer can't be void, function, reference or pointer");
		public:
			using value_type				= T;
			using reference					= T&;
			using const_reference			= T const&;
			using pointer					= T*;	
			using const_pointer				= const T*;	
			using difference_type			= ptrdiff_t;
			using size_type					= size_t;	

		private:
			class ptr_iterator : public std::iterator<std::random_access_iterator_tag, T>{
				public:
					using raw_iterator		= T**;	

					ptr_iterator( raw_iterator pi) : m_ptr_it(pi){}
					ptr_iterator& operator++() { ++m_ptr_it; return *this;}
					ptr_iterator operator++(int) { auto tmp=*this; operator++(); return tmp;}
					ptr_iterator operator+(size_type n) { return ptr_iterator(m_ptr_it+n); }
					ptr_iterator& operator+=(size_type n) { m_ptr_it+=n; return *this;}
					ptr_iterator& operator--() { --m_ptr_it; return *this;}
					ptr_iterator operator--(int) { auto tmp=*this; operator--(); return tmp;}
					ptr_iterator operator-(size_type n) { return ptr_iterator(m_ptr_it-n); }
					ptr_iterator& operator-=(size_type n) { m_ptr_it-=n; return *this;}
					bool operator==( ptr_iterator const& rhs) const{ return m_ptr_it== rhs.m_ptr_it;}
					bool operator!=( ptr_iterator const& rhs) const{ return !(*this==rhs);}
					reference operator*(){ return **m_ptr_it; }
					const_reference operator*() const{ return **m_ptr_it; }
					pointer	operator->() { return std::addressof(operator*()); }
					const_pointer	operator->() const{ return std::addressof(operator*()); }
					reference operator[](difference_type n) { return **(m_ptr_it+n); }
					const_reference operator[](difference_type n) const{ return **(m_ptr_it+n); }

				private:
					raw_iterator	m_ptr_it;
			};

		public:		
			using iterator					= ptr_iterator;
			using const_iterator			= const ptr_iterator;
			using reverse_iterator			= std::reverse_iterator<iterator>;
			using const_reverse_iterator	= const reverse_iterator;

			PtrContainer() : m_inner_container(){}
			explicit PtrContainer( std::initializer_list<T*> il ) : m_inner_container(il){}
			~PtrContainer(){ clear(); }
			PtrContainer( const PtrContainer&) = delete;
			PtrContainer& operator=( const PtrContainer&) = delete;
			PtrContainer( PtrContainer&&) = default;
			PtrContainer& operator=( PtrContainer&& rhs){
				if(&rhs != this){	
//					clear();
//					m_inner_container = move(rhs.m_inner_container);
					this->~PtrContainer();
					new (this)PtrContainer(std::move(rhs));
				}
				return *this;
			}

			// capacity	
			size_type size() const { return m_inner_container.size(); }
			size_type max_size() const { return m_inner_container.max_size(); }
			size_type capacity() const { return m_inner_container.capacity(); }
			bool empty() const { return m_inner_container.empty(); }
			void  reserve( size_type n) { m_inner_container.reserve(n); }
			void  shrift_to_fit() { m_inner_container.shrift_to_fit(); }
			// resize is forbidden, because no good default new value for ptr


			// iterators
			iterator begin() { return iterator(m_inner_container.data()); }
			const_iterator cbegin() const { return iterator(const_cast<T**>(m_inner_container.data())); }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator crbegin() const { return reverse_iterator(cend()); }
			iterator end() { return iterator(m_inner_container.data()+m_inner_container.size()); }
			const_iterator cend() const{ return iterator(const_cast<T**>(m_inner_container.data()+m_inner_container.size())); }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator crend() const { return reverse_iterator(cbegin()); }


			// element access
			reference operator[] ( size_type index){ return *m_inner_container[index]; }  
			const_reference operator[] ( size_type index) const{ return *m_inner_container[index]; }
			reference at( size_type index){ return *m_inner_container.at(index); }
			const_reference at( size_type index) const{ return *m_inner_container.at(index); }
			reference front() { return *m_inner_container.front(); }
			const_reference front() const{ return *m_inner_container.front(); }
			reference back() { return *m_inner_container.back(); }
			const_reference back() const{ return *m_inner_container.back(); }

			// modification
			// only movable ptr is supported
			void push_back( T* && ptr){ m_inner_container.push_back(ptr); }
			template<class... Args>
			void emplace_back(Args&&... args){
				m_inner_container.emplace_back( new value_type(std::forward<Args>(args)... ) );
			} 	
			void clear(){
				for( auto&& ptr : m_inner_container){ delete ptr; }
				m_inner_container.clear();
			}

		private:
			std::vector<T*>	m_inner_container;
	};

}
