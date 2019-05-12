#pragma once
#include "cs_core.hpp"
#include "cs_ida.hpp"

namespace cs::patchops {

	void fill_nopbuffer(uint8_t* out, unsigned size);

	void nop_ea(ea_t ea, unsigned size);

	void replace_code(uint8_t* codebuffer, unsigned repl_size, ea_t ea, unsigned original_insn_size);

	void make_short_branch_uncond(ea_t ea);

	
}