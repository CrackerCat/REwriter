#include "cs_core.hpp"

#include "x86_64_ops.hpp"

#include "cs_ida.hpp"
static constexpr unsigned g_oplut_length = (NN_S_last / 32) + ((NN_S_last % 32) == 0 ? 0 : 1);
using oplut_t = std::array<unsigned, g_oplut_length>;

static constexpr void init_oplut(oplut_t& lut) {
	for (unsigned i = 0; i < g_oplut_length; ++i) {
		lut[i] = 0;
	}
}

template<cs_x86_op_e op, cs_x86_op_e... ops> 
static constexpr void oplut_make_impl(oplut_t* lut) {
	lut->operator[](op / 32) |= 1 << (op & 0x1F);
	if constexpr (sizeof...(ops) != 0) {
		oplut_make_impl<ops...>(lut);
	}
	else {

	}
}

template<cs_x86_op_e... ops>
static constexpr oplut_t make_oplut() {
	oplut_t result{};
	init_oplut(result);
	oplut_make_impl<ops...>(&result);
	return result;
}


static constexpr oplut_t g_iset_is_conditional_branch = make_oplut <
	NN_S_ja,                  // Jump if Above (CF=0 & ZF=0)
	NN_S_jae,                 // Jump if Above or Equal (CF=0)
	NN_S_jb,                  // Jump if Below (CF=1)
	NN_S_jbe,                 // Jump if Below or Equal (CF=1 | ZF=1)
	NN_S_jc,                  // Jump if Carry (CF=1)
	NN_S_jcxz,                // Jump if CX is 0
	NN_S_jecxz,               // Jump if ECX is 0
	NN_S_jrcxz,               // Jump if RCX is 0
	NN_S_je,                  // Jump if Equal (ZF=1)
	NN_S_jg,                  // Jump if Greater (ZF=0 & SF=OF)
	NN_S_jge,                 // Jump if Greater or Equal (SF=OF)
	NN_S_jl,                  // Jump if Less (SF!=OF)
	NN_S_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)
	NN_S_jna,                 // Jump if Not Above (CF=1 | ZF=1)
	NN_S_jnae,                // Jump if Not Above or Equal (CF=1)
	NN_S_jnb,                 // Jump if Not Below (CF=0)
	NN_S_jnbe,                // Jump if Not Below or Equal (CF=0 & ZF=0)
	NN_S_jnc,                 // Jump if Not Carry (CF=0)
	NN_S_jne,                 // Jump if Not Equal (ZF=0)
	NN_S_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)
	NN_S_jnge,                // Jump if Not Greater or Equal (ZF=1)
	NN_S_jnl,                 // Jump if Not Less (SF=OF)
	NN_S_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_jno,                 // Jump if Not Overflow (OF=0)
	NN_S_jnp,                 // Jump if Not Parity (PF=0)
	NN_S_jns,                 // Jump if Not Sign (SF=0)
	NN_S_jnz,                 // Jump if Not Zero (ZF=0)
	NN_S_jo,                  // Jump if Overflow (OF=1)
	NN_S_jp,                  // Jump if Parity (PF=1)
	NN_S_jpe,                 // Jump if Parity Even (PF=1)
	NN_S_jpo,                 // Jump if Parity Odd  (PF=0)
	NN_S_js,                  // Jump if Sign (SF=1)
	NN_S_jz                  // Jump if Zero (ZF=1)

>();

static constexpr oplut_t g_iset_is_flag_based_conditional_branch = make_oplut <
	NN_S_ja,                  // Jump if Above (CF=0 & ZF=0)
	NN_S_jae,                 // Jump if Above or Equal (CF=0)
	NN_S_jb,                  // Jump if Below (CF=1)
	NN_S_jbe,                 // Jump if Below or Equal (CF=1 | ZF=1)
	NN_S_jc,                  // Jump if Carry (CF=1)
	NN_S_je,                  // Jump if Equal (ZF=1)
	NN_S_jg,                  // Jump if Greater (ZF=0 & SF=OF)
	NN_S_jge,                 // Jump if Greater or Equal (SF=OF)
	NN_S_jl,                  // Jump if Less (SF!=OF)
	NN_S_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)
	NN_S_jna,                 // Jump if Not Above (CF=1 | ZF=1)
	NN_S_jnae,                // Jump if Not Above or Equal (CF=1)
	NN_S_jnb,                 // Jump if Not Below (CF=0)
	NN_S_jnbe,                // Jump if Not Below or Equal (CF=0 & ZF=0)
	NN_S_jnc,                 // Jump if Not Carry (CF=0)
	NN_S_jne,                 // Jump if Not Equal (ZF=0)
	NN_S_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)
	NN_S_jnge,                // Jump if Not Greater or Equal (ZF=1)
	NN_S_jnl,                 // Jump if Not Less (SF=OF)
	NN_S_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_jno,                 // Jump if Not Overflow (OF=0)
	NN_S_jnp,                 // Jump if Not Parity (PF=0)
	NN_S_jns,                 // Jump if Not Sign (SF=0)
	NN_S_jnz,                 // Jump if Not Zero (ZF=0)
	NN_S_jo,                  // Jump if Overflow (OF=1)
	NN_S_jp,                  // Jump if Parity (PF=1)
	NN_S_jpe,                 // Jump if Parity Even (PF=1)
	NN_S_jpo,                 // Jump if Parity Odd  (PF=0)
	NN_S_js,                  // Jump if Sign (SF=1)
	NN_S_jz                  // Jump if Zero (ZF=1)

> ();

static constexpr oplut_t g_iset_is_gpreg_cmov = make_oplut<
	NN_S_cmova,               // Move if Above (CF=0 & ZF=0)
	NN_S_cmovb,               // Move if Below (CF=1)
	NN_S_cmovbe,              // Move if Below or Equal (CF=1 | ZF=1)
	NN_S_cmovg,               // Move if Greater (ZF=0 & SF=OF)
	NN_S_cmovge,              // Move if Greater or Equal (SF=OF)
	NN_S_cmovl,               // Move if Less (SF!=OF)
	NN_S_cmovle,              // Move if Less or Equal (ZF=1 | SF!=OF)
	NN_S_cmovnb,              // Move if Not Below (CF=0)
	NN_S_cmovno,              // Move if Not Overflow (OF=0)
	NN_S_cmovnp,              // Move if Not Parity (PF=0)
	NN_S_cmovns,              // Move if Not Sign (SF=0)
	NN_S_cmovnz,              // Move if Not Zero (ZF=0)
	NN_S_cmovo,               // Move if Overflow (OF=1)
	NN_S_cmovp,               // Move if Parity (PF=1)
	NN_S_cmovs,               // Move if Sign (SF=1)
	NN_S_cmovz               // Move if Zero (ZF=1)
>();


static constexpr oplut_t g_iset_is_set_based_on_flag = make_oplut <
	NN_S_seta,                // Set Byte if Above (CF=0 & ZF=0)
	NN_S_setae,               // Set Byte if Above or Equal (CF=0)
	NN_S_setb,                // Set Byte if Below (CF=1)
	NN_S_setbe,               // Set Byte if Below or Equal (CF=1 | ZF=1)
	NN_S_setc,                // Set Byte if Carry (CF=1)
	NN_S_sete,                // Set Byte if Equal (ZF=1)
	NN_S_setg,                // Set Byte if Greater (ZF=0 & SF=OF)
	NN_S_setge,               // Set Byte if Greater or Equal (SF=OF)
	NN_S_setl,                // Set Byte if Less (SF!=OF)
	NN_S_setle,               // Set Byte if Less or Equal (ZF=1 | SF!=OF)
	NN_S_setna,               // Set Byte if Not Above (CF=1 | ZF=1)
	NN_S_setnae,              // Set Byte if Not Above or Equal (CF=1)
	NN_S_setnb,               // Set Byte if Not Below (CF=0)
	NN_S_setnbe,              // Set Byte if Not Below or Equal (CF=0 & ZF=0)
	NN_S_setnc,               // Set Byte if Not Carry (CF=0)
	NN_S_setne,               // Set Byte if Not Equal (ZF=0)
	NN_S_setng,               // Set Byte if Not Greater (ZF=1 | SF!=OF)
	NN_S_setnge,              // Set Byte if Not Greater or Equal (ZF=1)
	NN_S_setnl,               // Set Byte if Not Less (SF=OF)
	NN_S_setnle,              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_setno,               // Set Byte if Not Overflow (OF=0)
	NN_S_setnp,               // Set Byte if Not Parity (PF=0)
	NN_S_setns,               // Set Byte if Not Sign (SF=0)
	NN_S_setnz,               // Set Byte if Not Zero (ZF=0)
	NN_S_seto,                // Set Byte if Overflow (OF=1)
	NN_S_setp,                // Set Byte if Parity (PF=1)
	NN_S_setpe,               // Set Byte if Parity Even (PF=1)
	NN_S_setpo,               // Set Byte if Parity Odd  (PF=0)
	NN_S_sets,                // Set Byte if Sign (SF=1)
	NN_S_setz                // Set Byte if Zero (ZF=1)
> ();
static constexpr oplut_t g_iset_uses_carry_flag = make_oplut<
	NN_S_seta,                // Set Byte if Above (CF=0 & ZF=0)
	NN_S_setae,               // Set Byte if Above or Equal (CF=0)
	NN_S_setb,                // Set Byte if Below (CF=1)
	NN_S_setbe,               // Set Byte if Below or Equal (CF=1 | ZF=1)
	NN_S_setc,                // Set Byte if Carry (CF=1)
	NN_S_setna,               // Set Byte if Not Above (CF=1 | ZF=1)
	NN_S_setnae,              // Set Byte if Not Above or Equal (CF=1)
	NN_S_setnb,               // Set Byte if Not Below (CF=0)
	NN_S_setnbe,              // Set Byte if Not Below or Equal (CF=0 & ZF=0)
	NN_S_setnc,               // Set Byte if Not Carry (CF=0)

	NN_S_cmova,               // Move if Above (CF=0 & ZF=0)
	NN_S_cmovb,               // Move if Below (CF=1)
	NN_S_cmovbe,              // Move if Below or Equal (CF=1 | ZF=1)
	NN_S_cmovnb,              // Move if Not Below (CF=0)
	NN_S_ja,                  // Jump if Above (CF=0 & ZF=0)
	NN_S_jae,                 // Jump if Above or Equal (CF=0)
	NN_S_jb,                  // Jump if Below (CF=1)
	NN_S_jbe,                 // Jump if Below or Equal (CF=1 | ZF=1)
	NN_S_jc,                  // Jump if Carry (CF=1)
	NN_S_jna,                 // Jump if Not Above (CF=1 | ZF=1)
	NN_S_jnae,                // Jump if Not Above or Equal (CF=1)
	NN_S_jnb,                 // Jump if Not Below (CF=0)
	NN_S_jnbe,                // Jump if Not Below or Equal (CF=0 & ZF=0)
	NN_S_jnc,                 // Jump if Not Carry (CF=0)
	NN_S_adc,
	NN_S_cmc,
	NN_S_sbb,
	NN_S_lahf,
	NN_S_rcr,
	NN_S_rcl,
	NN_S_pushfq,
	NN_S_adcx

>();

static constexpr oplut_t g_iset_uses_zero_flag = make_oplut <
	NN_S_pushfq,
	NN_S_lahf, 

	NN_S_ja,                  // Jump if Above (CF=0 & ZF=0)
	NN_S_jbe,                 // Jump if Below or Equal (CF=1 | ZF=1)
	NN_S_je,                  // Jump if Equal (ZF=1)
	NN_S_jg,                  // Jump if Greater (ZF=0 & SF=OF)
	NN_S_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)
	NN_S_jna,                 // Jump if Not Above (CF=1 | ZF=1)
	NN_S_jnbe,                // Jump if Not Below or Equal (CF=0 & ZF=0)
	NN_S_jne,                 // Jump if Not Equal (ZF=0)
	NN_S_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)
	NN_S_jnge,                // Jump if Not Greater or Equal (ZF=1)
	NN_S_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_jnz,                 // Jump if Not Zero (ZF=0)
	NN_S_jz,                  // Jump if Zero (ZF=1)

	NN_S_cmova,               // Move if Above (CF=0 & ZF=0)
	NN_S_cmovbe,              // Move if Below or Equal (CF=1 | ZF=1)
	NN_S_cmovg,               // Move if Greater (ZF=0 & SF=OF)
	NN_S_cmovle,              // Move if Less or Equal (ZF=1 | SF!=OF)
	NN_S_cmovnz,              // Move if Not Zero (ZF=0)
	NN_S_cmovz,               // Move if Zero (ZF=1)


	NN_S_seta,                // Set Byte if Above (CF=0 & ZF=0)
	NN_S_setbe,               // Set Byte if Below or Equal (CF=1 | ZF=1)
	NN_S_sete,                // Set Byte if Equal (ZF=1)
	NN_S_setg,                // Set Byte if Greater (ZF=0 & SF=OF)
	NN_S_setle,               // Set Byte if Less or Equal (ZF=1 | SF!=OF)
	NN_S_setna,               // Set Byte if Not Above (CF=1 | ZF=1)
	NN_S_setnbe,              // Set Byte if Not Below or Equal (CF=0 & ZF=0)
	NN_S_setne,               // Set Byte if Not Equal (ZF=0)
	NN_S_setng,               // Set Byte if Not Greater (ZF=1 | SF!=OF)
	NN_S_setnge,              // Set Byte if Not Greater or Equal (ZF=1)
	NN_S_setnle,              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_setnz,               // Set Byte if Not Zero (ZF=0)
	NN_S_setz                // Set Byte if Zero (ZF=1)

>();


static constexpr oplut_t g_iset_uses_sign_flag = make_oplut <
	NN_S_pushfq,
	NN_S_lahf,
	NN_S_jg,                  // Jump if Greater (ZF=0 & SF=OF)
	NN_S_jge,                 // Jump if Greater or Equal (SF=OF)
	NN_S_jl,                  // Jump if Less (SF!=OF)

	NN_S_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)

	NN_S_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)
	NN_S_jnl,                 // Jump if Not Less (SF=OF)
	NN_S_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_jns,                 // Jump if Not Sign (SF=0)
	NN_S_js,                  // Jump if Sign (SF=1)


	NN_S_cmovg,               // Move if Greater (ZF=0 & SF=OF)
	NN_S_cmovge,              // Move if Greater or Equal (SF=OF)
	NN_S_cmovl,               // Move if Less (SF!=OF)
	NN_S_cmovle,              // Move if Less or Equal (ZF=1 | SF!=OF)
	NN_S_cmovns,              // Move if Not Sign (SF=0)
	NN_S_cmovs,               // Move if Sign (SF=1)

	NN_S_setg,                // Set Byte if Greater (ZF=0 & SF=OF)
	NN_S_setge,               // Set Byte if Greater or Equal (SF=OF)
	NN_S_setl,                // Set Byte if Less (SF!=OF)
	NN_S_setle,               // Set Byte if Less or Equal (ZF=1 | SF!=OF)

	NN_S_setng,               // Set Byte if Not Greater (ZF=1 | SF!=OF)
	NN_S_setnl,               // Set Byte if Not Less (SF=OF)
	NN_S_setnle,              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_setns,               // Set Byte if Not Sign (SF=0)
	NN_S_sets                // Set Byte if Sign (SF=1)

>();

static constexpr oplut_t g_iset_spoils_zcaspo_flags = make_oplut <
	NN_S_popfq,
	NN_S_bextr,
	NN_S_blsi,
	NN_S_blsmsk,
	NN_S_blsr,
	NN_S_bsf,
	NN_S_bsr,
	NN_S_bzhi,
	NN_S_cmp,
	NN_S_cmps,
	NN_S_cmpxchg,
	NN_S_cmpxchg16b,
	NN_S_cmpxchg8b,
	NN_S_comisd,
	NN_S_comiss,
	NN_S_div,
	NN_S_lzcnt,
	NN_S_idiv,
	NN_S_imul,
	NN_S_mul,
	NN_S_neg,
	NN_S_or,
	NN_S_popcnt,
	NN_S_ptest,
	NN_S_vptest,
	NN_S_sbb,
	NN_S_sar,
	NN_S_shl,
	NN_S_sal,
	NN_S_shr,
	NN_S_scas,
	NN_S_shld,
	NN_S_shrd,
	NN_S_sub,
	NN_S_test,
	NN_S_tzcnt,
	NN_S_ucomisd,
	NN_S_ucomiss,
	NN_S_vucomisd,
	NN_S_vucomiss,
	NN_S_xor,
	NN_S_xadd,

	NN_S_kortestb,
	NN_S_kortestw,
	NN_S_kortestq,
	NN_S_kortestd,

	NN_S_ktestb,
	NN_S_ktestw,
	NN_S_ktestq,
	NN_S_ktestd
>();
static constexpr oplut_t g_iset_spoils_carry_flag = make_oplut <
	NN_S_adc,
	NN_S_adcx,
	NN_S_add,
	NN_S_and,
	NN_S_bsr,
	NN_S_bsf,
	NN_S_bt,                  // Bit Test
	NN_S_btc,                 // Bit Test and Complement
	NN_S_btr,                 // Bit Test and Reset
	NN_S_bts,                 // Bit Test and Set
	NN_S_clc,
	NN_S_cmc,
	NN_S_cmp,
	NN_S_div,
	NN_S_idiv,
	NN_S_test,
	NN_S_xor,
	NN_S_sub,
	NN_S_stc,
	NN_S_sbb,
	NN_S_sahf,
	NN_S_rcl,
	NN_S_rol,
	NN_S_rcr,
	NN_S_ror,
	NN_S_popfq,
	NN_S_or,
	NN_S_mul,
	NN_S_imul
	//incomplete list

>();
static constexpr oplut_t g_iset_spoils_zero_flag = make_oplut <
	NN_S_adc,
	NN_S_add,
	NN_S_and,
	NN_S_bsr,
	NN_S_bsf,

	NN_S_cmp,
	NN_S_div,
	NN_S_idiv,
	NN_S_test,
	NN_S_xor,
	NN_S_sub,
	NN_S_sbb,
	NN_S_sahf,

	NN_S_popfq,
	NN_S_or,
	NN_S_mul,
	NN_S_imul,
	NN_S_dec,
	NN_S_inc

	//incomplete list
>();

static constexpr oplut_t g_iset_spoils_sign_flag = make_oplut <
	NN_S_adc,
	NN_S_add,
	NN_S_and,
	NN_S_bsr,
	NN_S_bsf,
	NN_S_bt,                  // Bit Test
	NN_S_btc,                 // Bit Test and Complement
	NN_S_btr,                 // Bit Test and Reset
	NN_S_bts,                 // Bit Test and Set
	NN_S_cmp,
	NN_S_div,
	NN_S_idiv,
	NN_S_test,
	NN_S_xor,
	NN_S_sub,
	NN_S_sbb,
	NN_S_sahf,

	NN_S_popfq,
	NN_S_or,
	NN_S_mul,
	NN_S_imul,
	NN_S_dec,
	NN_S_inc
	//incomplete list
> ();
static constexpr oplut_t g_iset_spoils_parity_flag = make_oplut <
	NN_S_sahf,
	NN_S_popfq,
	NN_S_test,
	NN_S_cmp,

	NN_S_adc,
	NN_S_add,
	NN_S_sub,
	NN_S_sbb


>();
//NOT SAHF
static constexpr oplut_t g_iset_spoils_overflow_flag = make_oplut <
	NN_S_popfq,
	NN_S_test,
	NN_S_cmp,
	NN_S_adc,
	NN_S_add,
	NN_S_sub,
	NN_S_sbb
>();
static constexpr oplut_t g_iset_uses_parity_flag = make_oplut<
	NN_S_jnp,                 // Jump if Not Parity (PF=0)
	NN_S_jp,                  // Jump if Parity (PF=1)
	NN_S_jpe,                 // Jump if Parity Even (PF=1)
	NN_S_jpo,                 // Jump if Parity Odd  (PF=0)


	NN_S_cmovnp,              // Move if Not Parity (PF=0)
	NN_S_cmovp,               // Move if Parity (PF=1)

	NN_S_setnp,               // Set Byte if Not Parity (PF=0)
	NN_S_setp,                // Set Byte if Parity (PF=1)
	NN_S_setpe,               // Set Byte if Parity Even (PF=1)
	NN_S_setpo,               // Set Byte if Parity Odd  (PF=0)
	NN_S_pushfq,
	NN_S_lahf
>();

//NOT USED BY LAHF
static constexpr oplut_t g_iset_uses_overflow_flag = make_oplut<
	NN_S_adox,
	NN_S_pushfq,

	NN_S_jg,                  // Jump if Greater (ZF=0 & SF=OF)
	NN_S_jge,                 // Jump if Greater or Equal (SF=OF)
	NN_S_jl,                  // Jump if Less (SF!=OF)
	NN_S_jle,                 // Jump if Less or Equal (ZF=1 | SF!=OF)


	NN_S_jng,                 // Jump if Not Greater (ZF=1 | SF!=OF)

	NN_S_jnl,                 // Jump if Not Less (SF=OF)
	NN_S_jnle,                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_jno,                 // Jump if Not Overflow (OF=0)


	NN_S_jo,                  // Jump if Overflow (OF=1)


	NN_S_cmovg,               // Move if Greater (ZF=0 & SF=OF)
	NN_S_cmovge,              // Move if Greater or Equal (SF=OF)
	NN_S_cmovl,               // Move if Less (SF!=OF)
	NN_S_cmovle,              // Move if Less or Equal (ZF=1 | SF!=OF)
	NN_S_cmovno,              // Move if Not Overflow (OF=0)
	NN_S_cmovo,               // Move if Overflow (OF=1)


	NN_S_setg,                // Set Byte if Greater (ZF=0 & SF=OF)
	NN_S_setge,               // Set Byte if Greater or Equal (SF=OF)
	NN_S_setl,                // Set Byte if Less (SF!=OF)
	NN_S_setle,               // Set Byte if Less or Equal (ZF=1 | SF!=OF)


	NN_S_setng,               // Set Byte if Not Greater (ZF=1 | SF!=OF)
	NN_S_setnl,               // Set Byte if Not Less (SF=OF)
	NN_S_setnle,              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
	NN_S_setno,               // Set Byte if Not Overflow (OF=0)
	NN_S_seto                // Set Byte if Overflow (OF=1)



>();




static inline bool instruction_set_is_member(const oplut_t* __restrict lut, cs_x86_op_e op) {
	return _bittest((const long*)&lut->operator[](op / 32), op & 0x1F);
}

bool opcode_classes::is_conditional_branch(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_is_conditional_branch, op);
}
bool opcode_classes::is_flag_based_conditional_branch(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_is_conditional_branch, op);
}
bool opcode_classes::is_gpreg_cmov(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_is_gpreg_cmov, op);
}

bool opcode_classes::is_conditional_set_byte(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_is_set_based_on_flag, op);
}
bool opcode_classes::is_flag_based_conditional_op(cs_x86_op_e op) {
	return is_conditional_branch(op) || is_gpreg_cmov(op) || is_conditional_set_byte(op);
}

bool opcode_classes::spoils_carry_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_carry_flag, op);
}

bool opcode_classes::spoils_zero_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_zero_flag, op);
}

bool opcode_classes::spoils_sign_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_sign_flag, op);
}


bool opcode_classes::uses_carry_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_uses_carry_flag, op);
}

bool opcode_classes::uses_zero_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_uses_zero_flag, op);
}

bool opcode_classes::uses_sign_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_uses_sign_flag, op);
}

bool opcode_classes::spoils_overflow_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_overflow_flag, op);
}

bool opcode_classes::spoils_parity_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_parity_flag, op);
}

bool opcode_classes::uses_overflow_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_uses_overflow_flag, op);
}

bool opcode_classes::uses_parity_flag(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_uses_parity_flag, op);

}

static constexpr oplut_t g_potentially_has_nonflag_hidden_operands = make_oplut <

	NN_S_cwd,                 // AX -> DX:AX (with sign)
	NN_S_cdq,                 // EAX -> EDX:EAX (with sign)
	NN_S_cqo,                 // RAX -> RDX:RAX (with sign)

	NN_S_cbw,                 // AL -> AX (with sign)
	NN_S_cwde,                // AX -> EAX (with sign)
	NN_S_cdqe,                // EAX -> RAX (with sign)
	//hidden use/def - ax,dx,eax,edx,rax,rdx
	NN_S_idiv,

	//Hidden use/def - cx,ecx,rcx
	NN_S_jcxz,
	NN_S_jecxz,
	NN_S_jrcxz,

	//hidden use/def - RSP
	NN_S_push,
	NN_S_pop,
	NN_S_pushfq,
	NN_S_popfq,

	NN_S_sahf,
	NN_S_lahf,
	NN_S_call

> ();

bool opcode_classes::potentially_has_nonflag_hidden_operands(cs_x86_op_e op) {
	return instruction_set_is_member(&g_potentially_has_nonflag_hidden_operands, op);
}

bool opcode_classes::get_hidden_usedefs(insn_t* CS_RESTRICT ins, csreglist_t* CS_RESTRICT uses, csreglist_t* CS_RESTRICT defs ) {
	using namespace cs::x86regs;
	bool changed_any = false;
	auto try_add_use = [uses, &changed_any](unsigned reg, unsigned size) {
		if (uses) {
			uses->add(reg, size);
			changed_any = true;
		}
	};
	auto try_add_def = [defs, &changed_any](unsigned reg, unsigned size) {
		if (defs) {
			defs->add(reg, size);
			changed_any = true;
		}
	};


	auto gen_sxrax_to_rdx = [&try_add_def, &try_add_use](unsigned size) {
		try_add_use(RAX, size);
		try_add_def(RAX, size);
		try_add_def(RDX, size);
	};

	auto gen_sxrax_to_rax = [&try_add_def, &try_add_use](unsigned size) {
		try_add_use(RAX, size);
		try_add_def(RAX, size*2);
	};

	auto gen_nonbyte_idiv = [&try_add_def, &try_add_use](unsigned size) {
		try_add_use(RAX, size);
		try_add_use(RDX, size);

		try_add_def(RAX, size);
		try_add_def(RDX, size);
	};
	switch (ins->itype) {
	case NN_push:
	case NN_pop:
	case NN_pushfq:
	case NN_popfq:
		try_add_use(RSP, 8);
		try_add_def(RSP, 8);
		break;

	case NN_cwd:
		gen_sxrax_to_rdx(2);
		break;
	case NN_cdq:
		gen_sxrax_to_rdx(4);
		break;
	case NN_cqo:
		gen_sxrax_to_rdx(8);
		break;
	case NN_cbw:
		gen_sxrax_to_rax(1);
		break;
	case NN_cwde:
		gen_sxrax_to_rax(2);
		break;
	case NN_cdqe:
		gen_sxrax_to_rax(4);
		break;
	case NN_idiv:
	{
		
		auto dsize = dtype_to_size(ins->ops[0].dtype);

		if (dsize == 1) {
			try_add_use(RAX, 2);
			try_add_def(AH, 1);
			try_add_def(RAX, 1);

		}
		else {
			gen_nonbyte_idiv(dsize);
		}
		break;
	}

	case NN_jcxz:
		try_add_use(RCX, 2);
		break;
	case NN_jecxz:
		try_add_use(RCX, 4);
		break;
	case NN_jrcxz:
		try_add_use(RCX, 8);
		break;
	case NN_sahf:
		try_add_use(AH, 1);
		break;
	case NN_lahf:
		try_add_def(AH, 1);
		break;
	case NN_call:
		// RAX, RCX, RDX, R8, R9, R10, R11 
		try_add_def(RAX, 8);
		try_add_def(RCX, 8);
		try_add_def(RDX, 8);
		try_add_def(R8, 8);
		try_add_def(R9, 8);
		try_add_def(R10, 8);
		try_add_def(R11, 8);


		try_add_def(ZF, 1);
		try_add_def(CF, 1);
		try_add_def(SF, 1);
		try_add_def(OF, 1);
		try_add_def(PF, 1);
		break;
	}
	
	return changed_any;
}

bool opcode_classes::spoils_all_baseflags(cs_x86_op_e op) {
	return instruction_set_is_member(&g_iset_spoils_zcaspo_flags, op);
}