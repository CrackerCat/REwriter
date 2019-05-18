#pragma once
#pragma once

#include <Windows.h>
#include "cs_ptr32.hpp"
static inline void* slow_alloc32(unsigned size) {
	unsigned offs = 4096;

	void* result = nullptr;
	do {
		result = VirtualAlloc((void*)offs, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		offs += 4096;
	} while (result == nullptr);
	return result;
}


template<unsigned TAG, unsigned POOL_SIZE, unsigned MIN_POOL_ALLOC_QUANTAS = 16>
struct pool32_t {

	//
	// Memory manager: dynamically allocates memory from 
	// a fixed pool that is allocated statically at link-time.
	// 
	// Usage: after calling memmgr_init() in your 
	// initialization routine, just use memmgr_alloc() instead
	// of malloc() and memmgr_free() instead of free().
	// Naturally, you can use the preprocessor to define 
	// malloc() and free() as aliases to memmgr_alloc() and 
	// memmgr_free(). This way the manager will be a drop-in 
	// replacement for the standard C library allocators, and can
	// be useful for debugging memory allocation problems and 
	// leaks.
	//
	// Preprocessor flags you can define to customize the 
	// memory manager:
	//
	// DEBUG_MEMMGR_FATAL
	//    Allow printing out a message when allocations fail
	//
	// DEBUG_MEMMGR_SUPPORT_STATS
	//    Allow printing out of stats in function 
	//    memmgr_print_stats When this is disabled, 
	//    memmgr_print_stats does nothing.
	//
	// Note that in production code on an embedded system 
	// you'll probably want to keep those undefined, because
	// they cause printf to be called.
	//
	// POOL_SIZE
	//    Size of the pool for new allocations. This is 
	//    effectively the heap size of the application, and can 
	//    be changed in accordance with the available memory 
	//    resources.
	//
	// MIN_POOL_ALLOC_QUANTAS
	//    Internally, the memory manager allocates memory in
	//    quantas roughly the size of two ulong objects. To
	//    minimize pool fragmentation in case of multiple allocations
	//    and deallocations, it is advisable to not allocate
	//    blocks that are too small.
	//    This flag sets the minimal ammount of quantas for 
	//    an allocation. If the size of a ulong is 4 and you
	//    set this flag to 16, the minimal size of an allocation
	//    will be 4 * 2 * 16 = 128 bytes
	//    If you have a lot of small allocations, keep this value
	//    low to conserve memory. If you have mostly large 
	//    allocations, it is best to make it higher, to avoid 
	//    fragmentation.
	//
	// Notes:
	// 1. This memory manager is *not thread safe*. Use it only
	//    for single thread/task applications.
	// 




	typedef unsigned char byte;
	typedef unsigned long ulong;

	static_assert(sizeof(ulong) == 4);

	typedef ulong Align;

	union mem_header_union
	{
		struct
		{
			// Pointer to the next block in the free list
			//
			ptr32_t<union mem_header_union> next;

			// Size of the block (in quantas of sizeof(mem_header_t))
			//
			ulong size;
		} s;

		// Used to align headers in memory to a boundary
		//
		Align align_dummy;
	};

	//typedef union mem_header_union mem_header_t;

	using mem_header_t = mem_header_union;

	struct mem_base_t {
		ptr32_t<mem_header_t> freep;
		ulong pool_free_pos;
		mem_header_t base;
	};

	// Initial empty list
	//
	//static inline ptr32_t<mem_header_t> base;

	// Start of free list
	//
	//static inline ptr32_t<mem_header_t> freep = 0;

	// Static pool for new allocations
	//
	//static byte pool[POOL_SIZE] = { 0 };
	static inline ptr32_t<byte> pool;

	static inline ptr32_t<mem_base_t> pool_base = { 0 };
	//static inline ulong pool_free_pos = 0;

	static void memmgr_init()
	{
		pool = (byte*)slow_alloc32(POOL_SIZE + 4096);

		pool_base = (mem_base_t*)(void*)pool;
		pool = &pool[sizeof(mem_base_t)];

		memset(&pool[0], 0, POOL_SIZE);
		pool_base->base.s.next = 0;
		pool_base->base.s.size = 0;
		pool_base->freep = 0;
		pool_base->pool_free_pos = 0;
	}

	static void deinit() {

		VirtualFree((void*)pool_base, 0, MEM_DECOMMIT);
	}

	static ptr32_t<mem_header_t> get_mem_from_pool(ulong nquantas)
	{
		ulong total_req_size;

		ptr32_t<mem_header_t> h;

		if (nquantas < MIN_POOL_ALLOC_QUANTAS)
			nquantas = MIN_POOL_ALLOC_QUANTAS;

		total_req_size = nquantas * sizeof(mem_header_t);

		if (pool_base->pool_free_pos + total_req_size <= POOL_SIZE)
		{
			h = (ptr32_t<mem_header_t>)(mem_header_t*)(void*)(pool + pool_base->pool_free_pos);
			h->s.size = nquantas;
			memmgr_free((void*)(h + 1));
			pool_base->pool_free_pos += total_req_size;
		}
		else
		{
			return 0;
		}

		return pool_base->freep;
	}


	// Allocations are done in 'quantas' of header size.
	// The search for a free block of adequate size begins at the point 'freep'
	// where the last block was found.
	// If a too-big block is found, it is split and the tail is returned (this
	// way the header of the original needs only to have its size adjusted).
	// The pointer returned to the user points to the free space within the block,
	// which begins one quanta after the header.
	//
	static void* memmgr_alloc(ulong nbytes)
	{
		ptr32_t<mem_header_t> p;
		ptr32_t<mem_header_t> prevp;

		// Calculate how many quantas are required: we need enough to house all
		// the requested bytes, plus the header. The -1 and +1 are there to make sure
		// that if nbytes is a multiple of nquantas, we don't allocate too much
		//
		ulong nquantas = (nbytes + sizeof(mem_header_t) - 1) / sizeof(mem_header_t) + 1;

		// First alloc call, and no free list yet ? Use 'base' for an initial
		// denegerate block of size 0, which points to itself
		//
		if ((prevp = pool_base->freep) == 0)
		{
			pool_base->base.s.next = pool_base->freep = prevp = &pool_base->base;
			pool_base->base.s.size = 0;
		}

		for (p = prevp->s.next; ; prevp = p, p = p->s.next)
		{
			// big enough ?
			if (p->s.size >= nquantas)
			{
				// exactly ?
				if (p->s.size == nquantas)
				{
					// just eliminate this block from the free list by pointing
					// its prev's next to its next
					//
					prevp->s.next = p->s.next;
				}
				else // too big
				{
					p->s.size -= nquantas;
					p += p->s.size;
					p->s.size = nquantas;
				}

				pool_base->freep = prevp;
				return (void*)(p + 1);
			}
			// Reached end of free list ?
			// Try to allocate the block from the pool. If that succeeds,
			// get_mem_from_pool adds the new block to the free list and
			// it will be found in the following iterations. If the call
			// to get_mem_from_pool doesn't succeed, we've run out of
			// memory
			//
			else if (p == pool_base->freep)
			{
				if ((p = get_mem_from_pool(nquantas)) == 0)
				{
#ifdef DEBUG_MEMMGR_FATAL
					printf("!! Memory allocation failed !!\n");
#endif
					return 0;
				}
			}
		}
	}


	// Scans the free list, starting at freep, looking the the place to insert the
	// free block. This is either between two existing blocks or at the end of the
	// list. In any case, if the block being freed is adjacent to either neighbor,
	// the adjacent blocks are combined.
	//
	static void memmgr_free(void* ap)
	{
		ptr32_t<mem_header_t> block;
		ptr32_t<mem_header_t> p;

		// acquire pointer to block header
		block = ((ptr32_t<mem_header_t>)(mem_header_t*)ap) - 1;

		// Find the correct place to place the block in (the free list is sorted by
		// address, increasing order)
		//
		for (p = pool_base->freep; !(block > p && block < p->s.next); p = p->s.next)
		{
			// Since the free list is circular, there is one link where a
			// higher-addressed block points to a lower-addressed block.
			// This condition checks if the block should be actually
			// inserted between them
			//
			if (p >= p->s.next && (block > p || block < p->s.next))
				break;
		}

		// Try to combine with the higher neighbor
		//
		if (block + block->s.size == p->s.next)
		{
			block->s.size += p->s.next->s.size;
			block->s.next = p->s.next->s.next;
		}
		else
		{
			block->s.next = p->s.next;
		}

		// Try to combine with the lower neighbor
		//
		if (p + p->s.size == block)
		{
			p->s.size += block->s.size;
			p->s.next = block->s.next;
		}
		else
		{
			p->s.next = block;
		}

		pool_base->freep = p;
	}

	template<typename T>
	static inline ptr32_t<T> allocate(unsigned n = 1) {
		return ptr32_t<T>((T*)memmgr_alloc(sizeof(T) * n));
	}

	template<typename T>
	static inline void deallocate(ptr32_t<T> p) {
		memmgr_free((void*)& p[0]);
	}


};