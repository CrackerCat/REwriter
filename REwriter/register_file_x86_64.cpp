#include "register_file_x86_64.hpp"

using namespace cs::register_file;
#if 1
static constexpr auto xregister_file = _build_register_file::register_file_x86_64;


#define	BADREG		0xff

using idareg_remaptable_t = std::array<uint8_t, ida_registers::NIDAREGS>;

constexpr unsigned find_reg_for_idareg(unsigned idareg) {
	//for (const auto& reg : xregister_file) {
	for (size_t i = 0; i < _build_register_file::register_file_x86_64_length; ++i) {
		if (xregister_file.operator[](i).idareg() == idareg) {
			return i;

		}
	}
	//}
	return BADREG;
}


constexpr idareg_remaptable_t build_remaptable() {
	idareg_remaptable_t result{};

	for (unsigned i = 0; i < ida_registers::NIDAREGS; ++i) {
		unsigned p = find_reg_for_idareg(i);
		result[i] = p;
	}
	return result;
}

static constexpr idareg_remaptable_t g_remap = build_remaptable();
  
const contigreg_t* cs::register_file::idareg_to_contigreg(unsigned idr) {
	unsigned remap = g_remap[idr];
	if (remap == BADREG) {
		return nullptr;
	}
	return &xregister_file[remap];
}

#endif