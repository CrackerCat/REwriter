
#include "cs_core.hpp"
#include "cs_intrin.hpp"

#include "low32_allocator.hpp"
#include "mem_manager.hpp"


static pool32_t<'PO01', 128 * 1024 * 1024, 32> main_view;


void cs::mem::init_memmanager() {
	main_view.memmgr_init();

}


void cs::mem::shutdown_memmanager() {
	main_view.deinit();
}

void* cs::mem::_alloc32(unsigned sz) {
	return main_view.memmgr_alloc(sz);
}

void cs::mem::_dealloc32(void* p) {
	main_view.memmgr_free(p);
}
