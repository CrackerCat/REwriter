#pragma once
#include "saner_instruction_range.hpp"
#include "reglist.hpp"
namespace opcode_classes {
	bool is_conditional_branch(cs_x86_op_e op);
	bool is_flag_based_conditional_branch(cs_x86_op_e op);

	bool is_gpreg_cmov(cs_x86_op_e op);
	bool is_conditional_set_byte(cs_x86_op_e op);

	bool is_flag_based_conditional_op(cs_x86_op_e op);


	bool spoils_carry_flag(cs_x86_op_e op);
	bool spoils_zero_flag(cs_x86_op_e op);
	bool spoils_sign_flag(cs_x86_op_e op);
	bool spoils_overflow_flag(cs_x86_op_e op);
	bool spoils_parity_flag(cs_x86_op_e op);


	bool uses_carry_flag(cs_x86_op_e op);
	bool uses_zero_flag(cs_x86_op_e op);
	bool uses_sign_flag(cs_x86_op_e op);
	bool uses_overflow_flag(cs_x86_op_e op);
	bool uses_parity_flag(cs_x86_op_e op);

	bool potentially_has_nonflag_hidden_operands(cs_x86_op_e op);

	bool get_hidden_usedefs(insn_t* CS_RESTRICT ins,  csreglist_t* CS_RESTRICT uses = nullptr, csreglist_t* CS_RESTRICT defs = nullptr);
	bool spoils_all_baseflags(cs_x86_op_e op);
}