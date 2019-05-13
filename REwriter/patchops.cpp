
#include "patchops.hpp"

/*
	adapted from some python code i wrote way back
*/
CS_NOINLINE
static void fill_nopbuffer_impl(uint8_t* out, unsigned size) {
	auto put = [out](auto... fks) {
		unsigned char nfks[] = { fks... };
		for (unsigned i = 0; i < sizeof...(fks); ++i) {
			out[i] = nfks[i];
		}
	};

	if (size == 1)
		put(0x90);
	else if (size == 2)
		put(0x66, 0x90);
	else if (size == 3)
		put(0x0F, 0x1F, 0x00);
	else if (size == 4)
		put(0x0F, 0x1F, 0x40, 0x00);
	else if (size == 5)
		put(0x0F, 0x1F, 0x44, 0x00, 0x00);
	else if (size == 6)
		put(0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00);
	else if (size == 7)
		put(0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00);
	else if (size == 8)
		put(0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00);
	else if (size == 9)
		put(0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00);
	else if (size == 10)
		put(0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00);
	else if (size == 11)
		put(0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00);
	else {
		put(0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00);
		size -= 11;

		fill_nopbuffer_impl(&out[11], size);

	}
}
void cs::patchops::fill_nopbuffer(uint8_t* out, unsigned size) {
	fill_nopbuffer_impl(out, size);
}

void cs::patchops::nop_ea(ea_t ea, unsigned size) {
	uint8_t* buf = (uint8_t*)alloca(size);

	fill_nopbuffer(buf, size);

	patch_bytes(ea, (void*)buf, size);

}
void cs::patchops::replace_code(uint8_t* codebuffer, unsigned repl_size, ea_t ea, unsigned original_insn_size) {

	if (repl_size == original_insn_size) {
		patch_bytes(ea, (void*)codebuffer, repl_size);
	}
	else {
		unsigned diff = original_insn_size - repl_size;

		ea_t nopat_ea = ea + repl_size;
		patch_bytes(ea, (void*)codebuffer, repl_size);

		nop_ea(nopat_ea, diff);
	}
}

void cs::patchops::make_short_branch_uncond(ea_t ea) {
	patch_byte(ea, 0xeb);
}