#pragma once

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

}