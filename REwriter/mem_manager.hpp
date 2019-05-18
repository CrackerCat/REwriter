#pragma once
#include <memory_resource>
#include "cs_ptr32.hpp"
namespace cs::mem {

	void init_memmanager();
	void shutdown_memmanager();
	void* _alloc32(unsigned sz);

	void _dealloc32(void* p);


	template<typename T>
	static inline ptr32_t<T> allocate32(unsigned n = 1) {
		return ptr32_t<T>((T*)_alloc32(sizeof(T) * n));
	}

	template<typename T>
	static inline void deallocate32(ptr32_t<T> p) {
		_dealloc32((void*)& p[0]);
	}
	// See StackOverflow replies to this answer for important commentary about inheriting from std::allocator before replicating this code.
template <typename T>
class low32_allocator : public std::allocator<T>
{
public:
	typedef unsigned size_type;
	typedef ptr32_t<T> pointer;
	typedef  const ptr32_t< T> const_pointer;

	template<typename _Tp1>
	struct rebind
	{
		typedef low32_allocator<_Tp1> other;
	};

	pointer allocate(size_type n, const void* hint = 0)
	{
		return allocate32<T>(n);//pool32_t::allocate<T>(n);//std::allocator<T>::allocate(n, hint);
	}

	void deallocate(pointer p, size_type n)
	{
		deallocate32(p);
		//return pool32_t::deallocate(p);//std::allocator<T>::deallocate(p, n);
	}

	low32_allocator() throw() : std::allocator<T>() { }
	low32_allocator(const low32_allocator& a) throw() : std::allocator<T>(a) { }
	template <class U>
	low32_allocator(const low32_allocator<U>& a) throw() : std::allocator<T>(a) { }
	~low32_allocator() throw() { }
};

}