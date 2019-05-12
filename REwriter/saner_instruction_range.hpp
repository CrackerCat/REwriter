#pragma once
#include "idaheaders.hpp"
enum cs_x86_op_e : uint16_t
{
	NN_S_null = 0,            // Unknown Operation
//	NN_S_aaa,                 // ASCII Adjust after Addition
	//NN_S_aad,                 // ASCII Adjust AX before Division
	//NN_S_aam,                 // ASCII Adjust AX after Multiply
	//NN_S_aas,                 // ASCII Adjust AL after Subtraction
	NN_S_adc,                 // Add with Carry
	NN_S_add,                 // Add
	NN_S_and,                 // Logical AND

	//NN_S_arpl,                // Adjust RPL Field of Selector

	//NN_S_bound,               // Check Array Index Against Bounds

	NN_S_bsf,                 // Bit Scan Forward
	NN_S_bsr,                 // Bit Scan Reverse
	NN_S_bt,                  // Bit Test
	NN_S_btc,                 // Bit Test and Complement
	NN_S_btr,                 // Bit Test and Reset
	NN_S_bts,                 // Bit Test and Set
	NN_S_call,                // Call Procedure
	NN_S_callfi,              // Indirect Call Far Procedure
	NN_S_callni,              // Indirect Call Near Procedure

	NN_S_cbw,                 // AL -> AX (with sign)
	NN_S_cwde,                // AX -> EAX (with sign)
	NN_S_cdqe,                // EAX -> RAX (with sign)

	NN_S_clc,                 // Clear Carry Flag
	NN_S_cld,                 // Clear Direction Flag
//	NN_S_cli,                 // Clear Interrupt Flag
	//NN_S_clts,                // Clear Task-Switched Flag in CR0
	NN_S_cmc,                 // Complement Carry Flag
	NN_S_cmp,                 // Compare Two Operands
	NN_S_cmps,                // Compare Strings

	NN_S_cwd,                 // AX -> DX:AX (with sign)
	NN_S_cdq,                 // EAX -> EDX:EAX (with sign)
	NN_S_cqo,                 // RAX -> RDX:RAX (with sign)

	NN_S_daa,                 // Decimal Adjust AL after Addition
	NN_S_das,                 // Decimal Adjust AL after Subtraction
	NN_S_dec,                 // Decrement by 1
	NN_S_div,                 // Unsigned Divide
	NN_S_enterw,              // Make Stack Frame for Procedure Parameters
	NN_S_enter,               // Make Stack Frame for Procedure Parameters
	NN_S_enterd,              // Make Stack Frame for Procedure Parameters
	NN_S_enterq,              // Make Stack Frame for Procedure Parameters
	//NN_S_hlt,                 // Halt
	NN_S_idiv,                // Signed Divide
	NN_S_imul,                // Signed Multiply
	//NN_S_in,                  // Input from Port
	NN_S_inc,                 // Increment by 1
	NN_S_ins,                 // Input Byte(s) from Port to String
	NN_S_int,                 // Call to Interrupt Procedure
	NN_S_into,                // Call to Interrupt Procedure if Overflow Flag = 1
	NN_S_int3,                // Trap to Debugger
	//NN_S_iretw,               // Interrupt Return
	//NN_S_iret,                // Interrupt Return
//	NN_S_iretd,               // Interrupt Return (use32)
	//NN_S_iretq,               // Interrupt Return (use64)
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
	NN_S_jz,                  // Jump if Zero (ZF=1)
	NN_S_jmp,                 // Jump
	NN_S_jmpfi,               // Indirect Far Jump
	NN_S_jmpni,               // Indirect Near Jump
	NN_S_jmpshort,            // Jump Short (not used)
	NN_S_lahf,                // Load Flags into AH Register
	//NN_S_lar,                 // Load Access Right Byte
	NN_S_lea,                 // Load Effective Address
	NN_S_leavew,              // High Level Procedure Exit
	NN_S_leave,               // High Level Procedure Exit
	NN_S_leaved,              // High Level Procedure Exit
	NN_S_leaveq,              // High Level Procedure Exit
	//NN_S_lgdt,                // Load Global Descriptor Table Register
	//NN_S_lidt,                // Load Interrupt Descriptor Table Register
	NN_S_lgs,                 // Load Full Pointer to GS:xx
	NN_S_lss,                 // Load Full Pointer to SS:xx
	NN_S_lds,                 // Load Full Pointer to DS:xx
	NN_S_les,                 // Load Full Pointer to ES:xx
	NN_S_lfs,                 // Load Full Pointer to FS:xx
	//NN_S_lldt,                // Load Local Descriptor Table Register
	//NN_S_lmsw,                // Load Machine Status Word
	NN_S_lock,                // Assert LOCK# Signal Prefix
	NN_S_lods,                // Load String
	/*NN_S_loopw,               // Loop while ECX != 0
	NN_S_loop,                // Loop while CX != 0
	NN_S_loopd,               // Loop while ECX != 0
	NN_S_loopq,               // Loop while RCX != 0
	NN_S_loopwe,              // Loop while CX != 0 and ZF=1
	NN_S_loope,               // Loop while rCX != 0 and ZF=1
	NN_S_loopde,              // Loop while ECX != 0 and ZF=1
	NN_S_loopqe,              // Loop while RCX != 0 and ZF=1
	NN_S_loopwne,             // Loop while CX != 0 and ZF=0
	NN_S_loopne,              // Loop while rCX != 0 and ZF=0
	NN_S_loopdne,             // Loop while ECX != 0 and ZF=0
	NN_S_loopqne,             // Loop while RCX != 0 and ZF=0*/
	//NN_S_lsl,                 // Load Segment Limit
	//NN_S_ltr,                 // Load Task Register
	NN_S_mov,                 // Move Data
	NN_S_movsp,               // Move to/from Special Registers
	NN_S_movs,                // Move Byte(s) from String to String
	NN_S_movsx,               // Move with Sign-Extend
	NN_S_movzx,               // Move with Zero-Extend
	NN_S_mul,                 // Unsigned Multiplication of AL or AX
	NN_S_neg,                 // Two's Complement Negation
	NN_S_nop,                 // No Operation
	NN_S_not,                 // One's Complement Negation
	NN_S_or,                  // Logical Inclusive OR
	//NN_S_out,                 // Output to Port
	//NN_S_outs,                // Output Byte(s) to Port
	NN_S_pop,                 // Pop a word from the Stack
	//NN_S_popaw,               // Pop all General Registers
	//NN_S_popa,                // Pop all General Registers
	//NN_S_popad,               // Pop all General Registers (use32)
	NN_S_popaq,               // Pop all General Registers (use64)
	//NN_S_popfw,               // Pop Stack into Flags Register
//	NN_S_popf,                // Pop Stack into Flags Register
	//NN_S_popfd,               // Pop Stack into Eflags Register
	NN_S_popfq,               // Pop Stack into Rflags Register
	NN_S_push,                // Push Operand onto the Stack
	//NN_S_pushaw,              // Push all General Registers
	//NN_S_pusha,               // Push all General Registers
	//NN_S_pushad,              // Push all General Registers (use32)
	NN_S_pushaq,              // Push all General Registers (use64)
	//NN_S_pushfw,              // Push Flags Register onto the Stack
//	NN_S_pushf,               // Push Flags Register onto the Stack
	//NN_S_pushfd,              // Push Flags Register onto the Stack (use32)
	NN_S_pushfq,              // Push Flags Register onto the Stack (use64)
	NN_S_rcl,                 // Rotate Through Carry Left
	NN_S_rcr,                 // Rotate Through Carry Right
	NN_S_rol,                 // Rotate Left
	NN_S_ror,                 // Rotate Right
	NN_S_rep,                 // Repeat String Operation
	NN_S_repe,                // Repeat String Operation while ZF=1
	NN_S_repne,               // Repeat String Operation while ZF=0
	NN_S_retn,                // Return Near from Procedure
	NN_S_retf,                // Return Far from Procedure
	NN_S_sahf,                // Store AH into Flags Register
	NN_S_sal,                 // Shift Arithmetic Left
	NN_S_sar,                 // Shift Arithmetic Right
	NN_S_shl,                 // Shift Logical Left
	NN_S_shr,                 // Shift Logical Right
	NN_S_sbb,                 // Integer Subtraction with Borrow
	NN_S_scas,                // Compare String
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
	NN_S_setz,                // Set Byte if Zero (ZF=1)
	//NN_S_sgdt,                // Store Global Descriptor Table Register
//	NN_S_sidt,                // Store Interrupt Descriptor Table Register
	NN_S_shld,                // Double Precision Shift Left
	NN_S_shrd,                // Double Precision Shift Right
	//NN_S_sldt,                // Store Local Descriptor Table Register
	//NN_S_smsw,                // Store Machine Status Word
	NN_S_stc,                 // Set Carry Flag
	NN_S_std,                 // Set Direction Flag
	//NN_S_sti,                 // Set Interrupt Flag
	NN_S_stos,                // Store String
	//NN_S_str,                 // Store Task Register
	NN_S_sub,                 // Integer Subtraction
	NN_S_test,                // Logical Compare
//	NN_S_verr,                // Verify a Segment for Reading
	//NN_S_verw,                // Verify a Segment for Writing
///	NN_S_wait,                // Wait until BUSY# Pin is Inactive (HIGH)
	NN_S_xchg,                // Exchange Register/Memory with Register
	//NN_S_xlat,                // Table Lookup Translation
	NN_S_xor,                 // Logical Exclusive OR

	//
	//      486 instructions
	//

	NN_S_cmpxchg,             // Compare and Exchange
	NN_S_bswap,               // Swap bits in EAX
	NN_S_xadd,                // t<-dest; dest<-src+dest; src<-t
	//NN_S_invd,                // Invalidate Data Cache
	//NN_S_wbinvd,              // Invalidate Data Cache (write changes)
	//NN_S_invlpg,              // Invalidate TLB entry

	//
	//      Pentium instructions
	//

	//NN_S_rdmsr,               // Read Machine Status Register
	//NN_S_wrmsr,               // Write Machine Status Register
	NN_S_cpuid,               // Get CPU ID
	NN_S_cmpxchg8b,           // Compare and Exchange Eight Bytes
	NN_S_rdtsc,               // Read Time Stamp Counter
	//NN_S_rsm,                 // Resume from System Management Mode

	//
	//      Pentium Pro instructions
	//

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
	NN_S_cmovz,               // Move if Zero (ZF=1)
	NN_S_fcmovb,              // Floating Move if Below
	NN_S_fcmove,              // Floating Move if Equal
	NN_S_fcmovbe,             // Floating Move if Below or Equal
	NN_S_fcmovu,              // Floating Move if Unordered
	NN_S_fcmovnb,             // Floating Move if Not Below
	NN_S_fcmovne,             // Floating Move if Not Equal
	NN_S_fcmovnbe,            // Floating Move if Not Below or Equal
	NN_S_fcmovnu,             // Floating Move if Not Unordered
	NN_S_fcomi,               // FP Compare, result in EFLAGS
	NN_S_fucomi,              // FP Unordered Compare, result in EFLAGS
	NN_S_fcomip,              // FP Compare, result in EFLAGS, pop stack
	NN_S_fucomip,             // FP Unordered Compare, result in EFLAGS, pop stack
	NN_S_rdpmc,               // Read Performance Monitor Counter

	//
	//      FPP instructuions
	//

	NN_S_fld,                 // Load Real
	NN_S_fst,                 // Store Real
	NN_S_fstp,                // Store Real and Pop
	NN_S_fxch,                // Exchange Registers
	NN_S_fild,                // Load Integer
	NN_S_fist,                // Store Integer
	NN_S_fistp,               // Store Integer and Pop
	NN_S_fbld,                // Load BCD
	NN_S_fbstp,               // Store BCD and Pop
	NN_S_fadd,                // Add Real
	NN_S_faddp,               // Add Real and Pop
	NN_S_fiadd,               // Add Integer
	NN_S_fsub,                // Subtract Real
	NN_S_fsubp,               // Subtract Real and Pop
	NN_S_fisub,               // Subtract Integer
	NN_S_fsubr,               // Subtract Real Reversed
	NN_S_fsubrp,              // Subtract Real Reversed and Pop
	NN_S_fisubr,              // Subtract Integer Reversed
	NN_S_fmul,                // Multiply Real
	NN_S_fmulp,               // Multiply Real and Pop
	NN_S_fimul,               // Multiply Integer
	NN_S_fdiv,                // Divide Real
	NN_S_fdivp,               // Divide Real and Pop
	NN_S_fidiv,               // Divide Integer
	NN_S_fdivr,               // Divide Real Reversed
	NN_S_fdivrp,              // Divide Real Reversed and Pop
	NN_S_fidivr,              // Divide Integer Reversed
	NN_S_fsqrt,               // Square Root
	NN_S_fscale,              // Scale:  st(0) <- st(0) * 2^st(1)
	NN_S_fprem,               // Partial Remainder
	NN_S_frndint,             // Round to Integer
	NN_S_fxtract,             // Extract exponent and significand
	NN_S_fabs,                // Absolute value
	NN_S_fchs,                // Change Sign
	NN_S_fcom,                // Compare Real
	NN_S_fcomp,               // Compare Real and Pop
	NN_S_fcompp,              // Compare Real and Pop Twice
	NN_S_ficom,               // Compare Integer
	NN_S_ficomp,              // Compare Integer and Pop
	NN_S_ftst,                // Test
	NN_S_fxam,                // Examine
	NN_S_fptan,               // Partial tangent
	NN_S_fpatan,              // Partial arctangent
	NN_S_f2xm1,               // 2^x - 1
	NN_S_fyl2x,               // Y * lg2(X)
	NN_S_fyl2xp1,             // Y * lg2(X+1)
	NN_S_fldz,                // Load +0.0
	NN_S_fld1,                // Load +1.0
	NN_S_fldpi,               // Load PI=3.14...
	NN_S_fldl2t,              // Load lg2(10)
	NN_S_fldl2e,              // Load lg2(e)
	NN_S_fldlg2,              // Load lg10(2)
	NN_S_fldln2,              // Load ln(2)
	NN_S_finit,               // Initialize Processor
	NN_S_fninit,              // Initialize Processor (no wait)
	NN_S_fsetpm,              // Set Protected Mode
	NN_S_fldcw,               // Load Control Word
	NN_S_fstcw,               // Store Control Word
	NN_S_fnstcw,              // Store Control Word (no wait)
	NN_S_fstsw,               // Store Status Word
	NN_S_fnstsw,              // Store Status Word (no wait)
	NN_S_fclex,               // Clear Exceptions
	NN_S_fnclex,              // Clear Exceptions (no wait)
	NN_S_fstenv,              // Store Environment
	NN_S_fnstenv,             // Store Environment (no wait)
	NN_S_fldenv,              // Load Environment
	NN_S_fsave,               // Save State
	NN_S_fnsave,              // Save State (no wait)
	NN_S_frstor,              // Restore State
	NN_S_fincstp,             // Increment Stack Pointer
	NN_S_fdecstp,             // Decrement Stack Pointer
	NN_S_ffree,               // Free Register
	NN_S_fnop,                // No Operation
	NN_S_feni,                // (8087 only)
	NN_S_fneni,               // (no wait) (8087 only)
	NN_S_fdisi,               // (8087 only)
	NN_S_fndisi,              // (no wait) (8087 only)

	//
	//      80387 instructions
	//

	NN_S_fprem1,              // Partial Remainder ( < half )
	NN_S_fsincos,             // t<-cos(st); st<-sin(st); push t
	NN_S_fsin,                // Sine
	NN_S_fcos,                // Cosine
	NN_S_fucom,               // Compare Unordered Real
	NN_S_fucomp,              // Compare Unordered Real and Pop
	NN_S_fucompp,             // Compare Unordered Real and Pop Twice

	//
	//      Instructions added 28.02.96
	//

	//NN_S_setalc,              // Set AL to Carry Flag
	/*NN_S_svdc,                // Save Register and Descriptor
	NN_S_rsdc,                // Restore Register and Descriptor
	NN_S_svldt,               // Save LDTR and Descriptor
	NN_S_rsldt,               // Restore LDTR and Descriptor
	NN_S_svts,                // Save TR and Descriptor
	NN_S_rsts,                // Restore TR and Descriptor*/
	//NN_S_icebp,               // ICE Break Point
	//NN_S_loadall,             // Load the entire CPU state from ES:EDI

	//
	//      MMX instructions
	//

	NN_S_emms,                // Empty MMX state
	NN_S_movd,                // Move 32 bits
	NN_S_movq,                // Move 64 bits
	NN_S_packsswb,            // Pack with Signed Saturation (Word->Byte)
	NN_S_packssdw,            // Pack with Signed Saturation (Dword->Word)
	NN_S_packuswb,            // Pack with Unsigned Saturation (Word->Byte)
	NN_S_paddb,               // Packed Add Byte
	NN_S_paddw,               // Packed Add Word
	NN_S_paddd,               // Packed Add Dword
	NN_S_paddsb,              // Packed Add with Saturation (Byte)
	NN_S_paddsw,              // Packed Add with Saturation (Word)
	NN_S_paddusb,             // Packed Add Unsigned with Saturation (Byte)
	NN_S_paddusw,             // Packed Add Unsigned with Saturation (Word)
	NN_S_pand,                // Bitwise Logical And
	NN_S_pandn,               // Bitwise Logical And Not
	NN_S_pcmpeqb,             // Packed Compare for Equal (Byte)
	NN_S_pcmpeqw,             // Packed Compare for Equal (Word)
	NN_S_pcmpeqd,             // Packed Compare for Equal (Dword)
	NN_S_pcmpgtb,             // Packed Compare for Greater Than (Byte)
	NN_S_pcmpgtw,             // Packed Compare for Greater Than (Word)
	NN_S_pcmpgtd,             // Packed Compare for Greater Than (Dword)
	NN_S_pmaddwd,             // Packed Multiply and Add
	NN_S_pmulhw,              // Packed Multiply High
	NN_S_pmullw,              // Packed Multiply Low
	NN_S_por,                 // Bitwise Logical Or
	NN_S_psllw,               // Packed Shift Left Logical (Word)
	NN_S_pslld,               // Packed Shift Left Logical (Dword)
	NN_S_psllq,               // Packed Shift Left Logical (Qword)
	NN_S_psraw,               // Packed Shift Right Arithmetic (Word)
	NN_S_psrad,               // Packed Shift Right Arithmetic (Dword)
	NN_S_psrlw,               // Packed Shift Right Logical (Word)
	NN_S_psrld,               // Packed Shift Right Logical (Dword)
	NN_S_psrlq,               // Packed Shift Right Logical (Qword)
	NN_S_psubb,               // Packed Subtract Byte
	NN_S_psubw,               // Packed Subtract Word
	NN_S_psubd,               // Packed Subtract Dword
	NN_S_psubsb,              // Packed Subtract with Saturation (Byte)
	NN_S_psubsw,              // Packed Subtract with Saturation (Word)
	NN_S_psubusb,             // Packed Subtract Unsigned with Saturation (Byte)
	NN_S_psubusw,             // Packed Subtract Unsigned with Saturation (Word)
	NN_S_punpckhbw,           // Unpack High Packed Data (Byte->Word)
	NN_S_punpckhwd,           // Unpack High Packed Data (Word->Dword)
	NN_S_punpckhdq,           // Unpack High Packed Data (Dword->Qword)
	NN_S_punpcklbw,           // Unpack Low Packed Data (Byte->Word)
	NN_S_punpcklwd,           // Unpack Low Packed Data (Word->Dword)
	NN_S_punpckldq,           // Unpack Low Packed Data (Dword->Qword)
	NN_S_pxor,                // Bitwise Logical Exclusive Or

	//
	//      Undocumented Deschutes processor instructions
	//

	//NN_S_fxsave,              // Fast save FP context
	//NN_S_fxrstor,             // Fast restore FP context

	//      Pentium II instructions

	NN_S_sysenter,            // Fast Transition to System Call Entry Point
	//NN_S_sysexit,             // Fast Transition from System Call Entry Point

	//      3DNow! instructions

/*	NN_S_pavgusb,             // Packed 8-bit Unsigned Integer Averaging
	NN_S_pfadd,               // Packed Floating-Point Addition
	NN_S_pfsub,               // Packed Floating-Point Subtraction
	NN_S_pfsubr,              // Packed Floating-Point Reverse Subtraction
	NN_S_pfacc,               // Packed Floating-Point Accumulate
	NN_S_pfcmpge,             // Packed Floating-Point Comparison, Greater or Equal
	NN_S_pfcmpgt,             // Packed Floating-Point Comparison, Greater
	NN_S_pfcmpeq,             // Packed Floating-Point Comparison, Equal
	NN_S_pfmin,               // Packed Floating-Point Minimum
	NN_S_pfmax,               // Packed Floating-Point Maximum
	NN_S_pi2fd,               // Packed 32-bit Integer to Floating-Point
	NN_S_pf2id,               // Packed Floating-Point to 32-bit Integer
	NN_S_pfrcp,               // Packed Floating-Point Reciprocal Approximation
	NN_S_pfrsqrt,             // Packed Floating-Point Reciprocal Square Root Approximation
	NN_S_pfmul,               // Packed Floating-Point Multiplication
	NN_S_pfrcpit1,            // Packed Floating-Point Reciprocal First Iteration Step
	NN_S_pfrsqit1,            // Packed Floating-Point Reciprocal Square Root First Iteration Step
	NN_S_pfrcpit2,            // Packed Floating-Point Reciprocal Second Iteration Step
	NN_S_pmulhrw,             // Packed Floating-Point 16-bit Integer Multiply with rounding
	NN_S_femms,               // Faster entry/exit of the MMX or floating-point state
	NN_S_prefetch,            // Prefetch at least a 32-byte line into L1 data cache
	NN_S_prefetchw,           // Prefetch processor cache line into L1 data cache (mark as modified)
*/

	//      Pentium III instructions

	NN_S_addps,               // Packed Single-FP Add
	NN_S_addss,               // Scalar Single-FP Add
	NN_S_andnps,              // Bitwise Logical And Not for Single-FP
	NN_S_andps,               // Bitwise Logical And for Single-FP
	NN_S_cmpps,               // Packed Single-FP Compare
	NN_S_cmpss,               // Scalar Single-FP Compare
	NN_S_comiss,              // Scalar Ordered Single-FP Compare and Set EFLAGS
	NN_S_cvtpi2ps,            // Packed signed INT32 to Packed Single-FP conversion
	NN_S_cvtps2pi,            // Packed Single-FP to Packed INT32 conversion
	NN_S_cvtsi2ss,            // Scalar signed INT32 to Single-FP conversion
	NN_S_cvtss2si,            // Scalar Single-FP to signed INT32 conversion
	NN_S_cvttps2pi,           // Packed Single-FP to Packed INT32 conversion (truncate)
	NN_S_cvttss2si,           // Scalar Single-FP to signed INT32 conversion (truncate)
	NN_S_divps,               // Packed Single-FP Divide
	NN_S_divss,               // Scalar Single-FP Divide
	NN_S_ldmxcsr,             // Load Streaming SIMD Extensions Technology Control/Status Register
	NN_S_maxps,               // Packed Single-FP Maximum
	NN_S_maxss,               // Scalar Single-FP Maximum
	NN_S_minps,               // Packed Single-FP Minimum
	NN_S_minss,               // Scalar Single-FP Minimum
	NN_S_movaps,              // Move Aligned Four Packed Single-FP
	NN_S_movhlps,             // Move High to Low Packed Single-FP
	NN_S_movhps,              // Move High Packed Single-FP
	NN_S_movlhps,             // Move Low to High Packed Single-FP
	NN_S_movlps,              // Move Low Packed Single-FP
	NN_S_movmskps,            // Move Mask to Register
	NN_S_movss,               // Move Scalar Single-FP
	NN_S_movups,              // Move Unaligned Four Packed Single-FP
	NN_S_mulps,               // Packed Single-FP Multiply
	NN_S_mulss,               // Scalar Single-FP Multiply
	NN_S_orps,                // Bitwise Logical OR for Single-FP Data
	NN_S_rcpps,               // Packed Single-FP Reciprocal
	NN_S_rcpss,               // Scalar Single-FP Reciprocal
	NN_S_rsqrtps,             // Packed Single-FP Square Root Reciprocal
	NN_S_rsqrtss,             // Scalar Single-FP Square Root Reciprocal
	NN_S_shufps,              // Shuffle Single-FP
	NN_S_sqrtps,              // Packed Single-FP Square Root
	NN_S_sqrtss,              // Scalar Single-FP Square Root
	NN_S_stmxcsr,             // Store Streaming SIMD Extensions Technology Control/Status Register
	NN_S_subps,               // Packed Single-FP Subtract
	NN_S_subss,               // Scalar Single-FP Subtract
	NN_S_ucomiss,             // Scalar Unordered Single-FP Compare and Set EFLAGS
	NN_S_unpckhps,            // Unpack High Packed Single-FP Data
	NN_S_unpcklps,            // Unpack Low Packed Single-FP Data
	NN_S_xorps,               // Bitwise Logical XOR for Single-FP Data
	NN_S_pavgb,               // Packed Average (Byte)
	NN_S_pavgw,               // Packed Average (Word)
	NN_S_pextrw,              // Extract Word
	NN_S_pinsrw,              // Insert Word
	NN_S_pmaxsw,              // Packed Signed Integer Word Maximum
	NN_S_pmaxub,              // Packed Unsigned Integer Byte Maximum
	NN_S_pminsw,              // Packed Signed Integer Word Minimum
	NN_S_pminub,              // Packed Unsigned Integer Byte Minimum
	NN_S_pmovmskb,            // Move Byte Mask to Integer
	NN_S_pmulhuw,             // Packed Multiply High Unsigned
	NN_S_psadbw,              // Packed Sum of Absolute Differences
	NN_S_pshufw,              // Packed Shuffle Word
	NN_S_maskmovq,            // Byte Mask write
	NN_S_movntps,             // Move Aligned Four Packed Single-FP Non Temporal
	NN_S_movntq,              // Move 64 Bits Non Temporal
	NN_S_prefetcht0,          // Prefetch to all cache levels
	NN_S_prefetcht1,          // Prefetch to all cache levels
	NN_S_prefetcht2,          // Prefetch to L2 cache
	NN_S_prefetchnta,         // Prefetch to L1 cache
	NN_S_sfence,              // Store Fence

	// Pentium III Pseudo instructions

	NN_S_cmpeqps,             // Packed Single-FP Compare EQ
	NN_S_cmpltps,             // Packed Single-FP Compare LT
	NN_S_cmpleps,             // Packed Single-FP Compare LE
	NN_S_cmpunordps,          // Packed Single-FP Compare UNORD
	NN_S_cmpneqps,            // Packed Single-FP Compare NOT EQ
	NN_S_cmpnltps,            // Packed Single-FP Compare NOT LT
	NN_S_cmpnleps,            // Packed Single-FP Compare NOT LE
	NN_S_cmpordps,            // Packed Single-FP Compare ORDERED
	NN_S_cmpeqss,             // Scalar Single-FP Compare EQ
	NN_S_cmpltss,             // Scalar Single-FP Compare LT
	NN_S_cmpless,             // Scalar Single-FP Compare LE
	NN_S_cmpunordss,          // Scalar Single-FP Compare UNORD
	NN_S_cmpneqss,            // Scalar Single-FP Compare NOT EQ
	NN_S_cmpnltss,            // Scalar Single-FP Compare NOT LT
	NN_S_cmpnless,            // Scalar Single-FP Compare NOT LE
	NN_S_cmpordss,            // Scalar Single-FP Compare ORDERED

	// AMD K7 instructions
/*
	NN_S_pf2iw,               // Packed Floating-Point to Integer with Sign Extend
	NN_S_pfnacc,              // Packed Floating-Point Negative Accumulate
	NN_S_pfpnacc,             // Packed Floating-Point Mixed Positive-Negative Accumulate
	NN_S_pi2fw,               // Packed 16-bit Integer to Floating-Point
	NN_S_pswapd,              // Packed Swap Double Word
*/
	// Undocumented FP instructions (thanks to norbert.juffa@amd.com)

	NN_S_fstp1,               // Alias of Store Real and Pop
	NN_S_fcom2,               // Alias of Compare Real
	NN_S_fcomp3,              // Alias of Compare Real and Pop
	NN_S_fxch4,               // Alias of Exchange Registers
	NN_S_fcomp5,              // Alias of Compare Real and Pop
	NN_S_ffreep,              // Free Register and Pop
	NN_S_fxch7,               // Alias of Exchange Registers
	NN_S_fstp8,               // Alias of Store Real and Pop
	NN_S_fstp9,               // Alias of Store Real and Pop

	// Pentium 4 instructions

	NN_S_addpd,               // Add Packed Double-Precision Floating-Point Values
	NN_S_addsd,               // Add Scalar Double-Precision Floating-Point Values
	NN_S_andnpd,              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
	NN_S_andpd,               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
	//NN_S_clflush,             // Flush Cache Line
	NN_S_cmppd,               // Compare Packed Double-Precision Floating-Point Values
	NN_S_cmpsd,               // Compare Scalar Double-Precision Floating-Point Values
	NN_S_comisd,              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	NN_S_cvtdq2pd,            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
	NN_S_cvtdq2ps,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	NN_S_cvtpd2dq,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvtpd2pi,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvtpd2ps,            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
	NN_S_cvtpi2pd,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	NN_S_cvtps2dq,            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvtps2pd,            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
	NN_S_cvtsd2si,            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
	NN_S_cvtsd2ss,            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
	NN_S_cvtsi2sd,            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
	NN_S_cvtss2sd,            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
	NN_S_cvttpd2dq,           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvttpd2pi,           // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvttps2dq,           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_cvttsd2si,           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
	NN_S_divpd,               // Divide Packed Double-Precision Floating-Point Values
	NN_S_divsd,               // Divide Scalar Double-Precision Floating-Point Values
	NN_S_lfence,              // Load Fence
	NN_S_maskmovdqu,          // Store Selected Bytes of Double Quadword
	NN_S_maxpd,               // Return Maximum Packed Double-Precision Floating-Point Values
	NN_S_maxsd,               // Return Maximum Scalar Double-Precision Floating-Point Value
	NN_S_mfence,              // Memory Fence
	NN_S_minpd,               // Return Minimum Packed Double-Precision Floating-Point Values
	NN_S_minsd,               // Return Minimum Scalar Double-Precision Floating-Point Value
	NN_S_movapd,              // Move Aligned Packed Double-Precision Floating-Point Values
	NN_S_movdq2q,             // Move Quadword from XMM to MMX Register
	NN_S_movdqa,              // Move Aligned Double Quadword
	NN_S_movdqu,              // Move Unaligned Double Quadword
	NN_S_movhpd,              // Move High Packed Double-Precision Floating-Point Values
	NN_S_movlpd,              // Move Low Packed Double-Precision Floating-Point Values
	NN_S_movmskpd,            // Extract Packed Double-Precision Floating-Point Sign Mask
	NN_S_movntdq,             // Store Double Quadword Using Non-Temporal Hint
	NN_S_movnti,              // Store Doubleword Using Non-Temporal Hint
	NN_S_movntpd,             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
	NN_S_movq2dq,             // Move Quadword from MMX to XMM Register
	NN_S_movsd,               // Move Scalar Double-Precision Floating-Point Values
	NN_S_movupd,              // Move Unaligned Packed Double-Precision Floating-Point Values
	NN_S_mulpd,               // Multiply Packed Double-Precision Floating-Point Values
	NN_S_mulsd,               // Multiply Scalar Double-Precision Floating-Point Values
	NN_S_orpd,                // Bitwise Logical OR of Double-Precision Floating-Point Values
	NN_S_paddq,               // Add Packed Quadword Integers
	NN_S_pause,               // Spin Loop Hint
	NN_S_pmuludq,             // Multiply Packed Unsigned Doubleword Integers
	NN_S_pshufd,              // Shuffle Packed Doublewords
	NN_S_pshufhw,             // Shuffle Packed High Words
	NN_S_pshuflw,             // Shuffle Packed Low Words
	NN_S_pslldq,              // Shift Double Quadword Left Logical
	NN_S_psrldq,              // Shift Double Quadword Right Logical
	NN_S_psubq,               // Subtract Packed Quadword Integers
	NN_S_punpckhqdq,          // Unpack High Data
	NN_S_punpcklqdq,          // Unpack Low Data
	NN_S_shufpd,              // Shuffle Packed Double-Precision Floating-Point Values
	NN_S_sqrtpd,              // Compute Square Roots of Packed Double-Precision Floating-Point Values
	NN_S_sqrtsd,              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
	NN_S_subpd,               // Subtract Packed Double-Precision Floating-Point Values
	NN_S_subsd,               // Subtract Scalar Double-Precision Floating-Point Values
	NN_S_ucomisd,             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	NN_S_unpckhpd,            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
	NN_S_unpcklpd,            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
	NN_S_xorpd,               // Bitwise Logical OR of Double-Precision Floating-Point Values

	// AMD syscall/sysret instructions

	NN_S_syscall,             // Low latency system call
	//NN_S_sysret,              // Return from system call

	// AMD64 instructions

	//NN_S_swapgs,              // Exchange GS base with KernelGSBase MSR

	// New Pentium instructions (SSE3)

	NN_S_movddup,             // Move One Double-FP and Duplicate
	NN_S_movshdup,            // Move Packed Single-FP High and Duplicate
	NN_S_movsldup,            // Move Packed Single-FP Low and Duplicate

	// Missing AMD64 instructions

	NN_S_movsxd,              // Move with Sign-Extend Doubleword
	NN_S_cmpxchg16b,          // Compare and Exchange 16 Bytes

	// SSE3 instructions

	NN_S_addsubpd,            // Add /Sub packed DP FP numbers
	NN_S_addsubps,            // Add /Sub packed SP FP numbers
	NN_S_haddpd,              // Add horizontally packed DP FP numbers
	NN_S_haddps,              // Add horizontally packed SP FP numbers
	NN_S_hsubpd,              // Sub horizontally packed DP FP numbers
	NN_S_hsubps,              // Sub horizontally packed SP FP numbers
	//NN_S_monitor,             // Set up a linear address range to be monitored by hardware
	//NN_S_mwait,               // Wait until write-back store performed within the range specified by the MONITOR instruction
	NN_S_fisttp,              // Store ST in intXX (chop) and pop
	NN_S_lddqu,               // Load unaligned integer 128-bit

	// SSSE3 instructions

	NN_S_psignb,              // Packed SIGN Byte
	NN_S_psignw,              // Packed SIGN Word
	NN_S_psignd,              // Packed SIGN Doubleword
	NN_S_pshufb,              // Packed Shuffle Bytes
	NN_S_pmulhrsw,            // Packed Multiply High with Round and Scale
	NN_S_pmaddubsw,           // Multiply and Add Packed Signed and Unsigned Bytes
	NN_S_phsubsw,             // Packed Horizontal Subtract and Saturate
	NN_S_phaddsw,             // Packed Horizontal Add and Saturate
	NN_S_phaddw,              // Packed Horizontal Add Word
	NN_S_phaddd,              // Packed Horizontal Add Doubleword
	NN_S_phsubw,              // Packed Horizontal Subtract Word
	NN_S_phsubd,              // Packed Horizontal Subtract Doubleword
	NN_S_palignr,             // Packed Align Right
	NN_S_pabsb,               // Packed Absolute Value Byte
	NN_S_pabsw,               // Packed Absolute Value Word
	NN_S_pabsd,               // Packed Absolute Value Doubleword

	// VMX instructions

	/*NN_S_vmcall,              // Call to VM Monitor
	NN_S_vmclear,             // Clear Virtual Machine Control Structure
	NN_S_vmlaunch,            // Launch Virtual Machine
	NN_S_vmresume,            // Resume Virtual Machine
	NN_S_vmptrld,             // Load Pointer to Virtual Machine Control Structure
	NN_S_vmptrst,             // Store Pointer to Virtual Machine Control Structure
	NN_S_vmread,              // Read Field from Virtual Machine Control Structure
	NN_S_vmwrite,             // Write Field from Virtual Machine Control Structure
	NN_S_vmxoff,              // Leave VMX Operation
	NN_S_vmxon,               // Enter VMX Operation

	// Undefined Instruction

	NN_S_ud2,                 // Undefined Instruction
*/
	// Added with x86-64

	//NN_S_rdtscp,              // Read Time-Stamp Counter and Processor ID

	// Geode LX 3DNow! extensions

	//NN_S_pfrcpv,              // Reciprocal Approximation for a Pair of 32-bit Floats
//	NN_S_pfrsqrtv,            // Reciprocal Square Root Approximation for a Pair of 32-bit Floats

	// SSE2 pseudoinstructions

	NN_S_cmpeqpd,             // Packed Double-FP Compare EQ
	NN_S_cmpltpd,             // Packed Double-FP Compare LT
	NN_S_cmplepd,             // Packed Double-FP Compare LE
	NN_S_cmpunordpd,          // Packed Double-FP Compare UNORD
	NN_S_cmpneqpd,            // Packed Double-FP Compare NOT EQ
	NN_S_cmpnltpd,            // Packed Double-FP Compare NOT LT
	NN_S_cmpnlepd,            // Packed Double-FP Compare NOT LE
	NN_S_cmpordpd,            // Packed Double-FP Compare ORDERED
	NN_S_cmpeqsd,             // Scalar Double-FP Compare EQ
	NN_S_cmpltsd,             // Scalar Double-FP Compare LT
	NN_S_cmplesd,             // Scalar Double-FP Compare LE
	NN_S_cmpunordsd,          // Scalar Double-FP Compare UNORD
	NN_S_cmpneqsd,            // Scalar Double-FP Compare NOT EQ
	NN_S_cmpnltsd,            // Scalar Double-FP Compare NOT LT
	NN_S_cmpnlesd,            // Scalar Double-FP Compare NOT LE
	NN_S_cmpordsd,            // Scalar Double-FP Compare ORDERED

	// SSSE4.1 instructions

	NN_S_blendpd,              // Blend Packed Double Precision Floating-Point Values
	NN_S_blendps,              // Blend Packed Single Precision Floating-Point Values
	NN_S_blendvpd,             // Variable Blend Packed Double Precision Floating-Point Values
	NN_S_blendvps,             // Variable Blend Packed Single Precision Floating-Point Values
	NN_S_dppd,                 // Dot Product of Packed Double Precision Floating-Point Values
	NN_S_dpps,                 // Dot Product of Packed Single Precision Floating-Point Values
	NN_S_extractps,            // Extract Packed Single Precision Floating-Point Value
	NN_S_insertps,             // Insert Packed Single Precision Floating-Point Value
	NN_S_movntdqa,             // Load Double Quadword Non-Temporal Aligned Hint
	NN_S_mpsadbw,              // Compute Multiple Packed Sums of Absolute Difference
	NN_S_packusdw,             // Pack with Unsigned Saturation
	NN_S_pblendvb,             // Variable Blend Packed Bytes
	NN_S_pblendw,              // Blend Packed Words
	NN_S_pcmpeqq,              // Compare Packed Qword Data for Equal
	NN_S_pextrb,               // Extract Byte
	NN_S_pextrd,               // Extract Dword
	NN_S_pextrq,               // Extract Qword
	NN_S_phminposuw,           // Packed Horizontal Word Minimum
	NN_S_pinsrb,               // Insert Byte
	NN_S_pinsrd,               // Insert Dword
	NN_S_pinsrq,               // Insert Qword
	NN_S_pmaxsb,               // Maximum of Packed Signed Byte Integers
	NN_S_pmaxsd,               // Maximum of Packed Signed Dword Integers
	NN_S_pmaxud,               // Maximum of Packed Unsigned Dword Integers
	NN_S_pmaxuw,               // Maximum of Packed Word Integers
	NN_S_pminsb,               // Minimum of Packed Signed Byte Integers
	NN_S_pminsd,               // Minimum of Packed Signed Dword Integers
	NN_S_pminud,               // Minimum of Packed Unsigned Dword Integers
	NN_S_pminuw,               // Minimum of Packed Word Integers
	NN_S_pmovsxbw,             // Packed Move with Sign Extend
	NN_S_pmovsxbd,             // Packed Move with Sign Extend
	NN_S_pmovsxbq,             // Packed Move with Sign Extend
	NN_S_pmovsxwd,             // Packed Move with Sign Extend
	NN_S_pmovsxwq,             // Packed Move with Sign Extend
	NN_S_pmovsxdq,             // Packed Move with Sign Extend
	NN_S_pmovzxbw,             // Packed Move with Zero Extend
	NN_S_pmovzxbd,             // Packed Move with Zero Extend
	NN_S_pmovzxbq,             // Packed Move with Zero Extend
	NN_S_pmovzxwd,             // Packed Move with Zero Extend
	NN_S_pmovzxwq,             // Packed Move with Zero Extend
	NN_S_pmovzxdq,             // Packed Move with Zero Extend
	NN_S_pmuldq,               // Multiply Packed Signed Dword Integers
	NN_S_pmulld,               // Multiply Packed Signed Dword Integers and Store Low Result
	NN_S_ptest,                // Logical Compare
	NN_S_roundpd,              // Round Packed Double Precision Floating-Point Values
	NN_S_roundps,              // Round Packed Single Precision Floating-Point Values
	NN_S_roundsd,              // Round Scalar Double Precision Floating-Point Values
	NN_S_roundss,              // Round Scalar Single Precision Floating-Point Values

	// SSSE4.2 instructions

	NN_S_crc32,                // Accumulate CRC32 Value
	NN_S_pcmpestri,            // Packed Compare Explicit Length Strings, Return Index
	NN_S_pcmpestrm,            // Packed Compare Explicit Length Strings, Return Mask
	NN_S_pcmpistri,            // Packed Compare Implicit Length Strings, Return Index
	NN_S_pcmpistrm,            // Packed Compare Implicit Length Strings, Return Mask
	NN_S_pcmpgtq,              // Compare Packed Data for Greater Than
	NN_S_popcnt,               // Return the Count of Number of Bits Set to 1

	// AMD SSE4a instructions
/*
	NN_S_extrq,                // Extract Field From Register
	NN_S_insertq,              // Insert Field
	NN_S_movntsd,              // Move Non-Temporal Scalar Double-Precision Floating-Point
	NN_S_movntss,              // Move Non-Temporal Scalar Single-Precision Floating-Point
	
*/
NN_S_lzcnt,                // Leading Zero Count
	// xsave/xrstor instructions

	/*NN_S_xgetbv,               // Get Value of Extended Control Register
	NN_S_xrstor,               // Restore Processor Extended States
	NN_S_xsave,                // Save Processor Extended States
	NN_S_xsetbv,               // Set Value of Extended Control Register
*/
	// Intel Safer Mode Extensions (SMX)

	//NN_S_getsec,               // Safer Mode Extensions (SMX) Instruction

	// AMD-V Virtualization ISA Extension

	/*NN_S_clgi,                 // Clear Global Interrupt Flag
	NN_S_invlpga,              // Invalidate TLB Entry in a Specified ASID
	NN_S_skinit,               // Secure Init and Jump with Attestation
	NN_S_stgi,                 // Set Global Interrupt Flag
	NN_S_vmexit,               // Stop Executing Guest, Begin Executing Host
	NN_S_vmload,               // Load State from VMCB
	NN_S_vmmcall,              // Call VMM
	NN_S_vmrun,                // Run Virtual Machine
	NN_S_vmsave,               // Save State to VMCB

	// VMX+ instructions

	NN_S_invept,               // Invalidate Translations Derived from EPT
	NN_S_invvpid,              // Invalidate Translations Based on VPID
*/
	// Intel Atom instructions

	NN_S_movbe,                // Move Data After Swapping Bytes

	// Intel AES instructions

	NN_S_aesenc,                // Perform One Round of an AES Encryption Flow
	NN_S_aesenclast,            // Perform the Last Round of an AES Encryption Flow
	NN_S_aesdec,                // Perform One Round of an AES Decryption Flow
	NN_S_aesdeclast,            // Perform the Last Round of an AES Decryption Flow
	NN_S_aesimc,                // Perform the AES InvMixColumn Transformation
	NN_S_aeskeygenassist,       // AES Round Key Generation Assist

	// Carryless multiplication

	NN_S_pclmulqdq,            // Carry-Less Multiplication Quadword

	// Returns modifies by operand size prefixes

//	NN_S_retnw,               // Return Near from Procedure (use16)
	//NN_S_retnd,               // Return Near from Procedure (use32)
	NN_S_retnq,               // Return Near from Procedure (use64)
	//NN_S_retfw,               // Return Far from Procedure (use16)
//	NN_S_retfd,               // Return Far from Procedure (use32)
	NN_S_retfq,               // Return Far from Procedure (use64)

	// RDRAND support

	NN_S_rdrand,              // Read Random Number

	// new GPR instructions

	NN_S_adcx,                 // Unsigned Integer Addition of Two Operands with Carry Flag
	NN_S_adox,                 // Unsigned Integer Addition of Two Operands with Overflow Flag
	NN_S_andn,                 // Logical AND NOT
	NN_S_bextr,                // Bit Field Extract
	NN_S_blsi,                 // Extract Lowest Set Isolated Bit
	NN_S_blsmsk,               // Get Mask Up to Lowest Set Bit
	NN_S_blsr,                 // Reset Lowest Set Bit
	NN_S_bzhi,                 // Zero High Bits Starting with Specified Bit Position
	NN_S_clac,                 // Clear AC Flag in EFLAGS Register
	NN_S_mulx,                 // Unsigned Multiply Without Affecting Flags
	NN_S_pdep,                 // Parallel Bits Deposit
	NN_S_pext,                 // Parallel Bits Extract
	NN_S_rorx,                 // Rotate Right Logical Without Affecting Flags
	NN_S_sarx,                 // Shift Arithmetically Right Without Affecting Flags
	NN_S_shlx,                 // Shift Logically Left Without Affecting Flags
	NN_S_shrx,                 // Shift Logically Right Without Affecting Flags
	NN_S_stac,                 // Set AC Flag in EFLAGS Register
	NN_S_tzcnt,                // Count the Number of Trailing Zero Bits
	//NN_S_xsaveopt,             // Save Processor Extended States Optimized
	//NN_S_invpcid,              // Invalidate Processor Context ID
	NN_S_rdseed,               // Read Random Seed
	NN_S_rdfsbase,             // Read FS Segment Base
	NN_S_rdgsbase,             // Read GS Segment Base
	NN_S_wrfsbase,             // Write FS Segment Base
	NN_S_wrgsbase,             // Write GS Segment Base

	// new AVX instructions

	NN_S_vaddpd,               // Add Packed Double-Precision Floating-Point Values
	NN_S_vaddps,               // Packed Single-FP Add
	NN_S_vaddsd,               // Add Scalar Double-Precision Floating-Point Values
	NN_S_vaddss,               // Scalar Single-FP Add
	NN_S_vaddsubpd,            // Add /Sub packed DP FP numbers
	NN_S_vaddsubps,            // Add /Sub packed SP FP numbers
	NN_S_vaesdec,              // Perform One Round of an AES Decryption Flow
	NN_S_vaesdeclast,          // Perform the Last Round of an AES Decryption Flow
	NN_S_vaesenc,              // Perform One Round of an AES Encryption Flow
	NN_S_vaesenclast,          // Perform the Last Round of an AES Encryption Flow
	NN_S_vaesimc,              // Perform the AES InvMixColumn Transformation
	NN_S_vaeskeygenassist,     // AES Round Key Generation Assist
	NN_S_vandnpd,              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
	NN_S_vandnps,              // Bitwise Logical And Not for Single-FP
	NN_S_vandpd,               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
	NN_S_vandps,               // Bitwise Logical And for Single-FP
	NN_S_vblendpd,             // Blend Packed Double Precision Floating-Point Values
	NN_S_vblendps,             // Blend Packed Single Precision Floating-Point Values
	NN_S_vblendvpd,            // Variable Blend Packed Double Precision Floating-Point Values
	NN_S_vblendvps,            // Variable Blend Packed Single Precision Floating-Point Values
	NN_S_vbroadcastf128,       // Broadcast 128 Bits of Floating-Point Data
	NN_S_vbroadcasti128,       // Broadcast 128 Bits of Integer Data
	NN_S_vbroadcastsd,         // Broadcast Double-Precision Floating-Point Element
	NN_S_vbroadcastss,         // Broadcast Single-Precision Floating-Point Element
	NN_S_vcmppd,               // Compare Packed Double-Precision Floating-Point Values
	NN_S_vcmpps,               // Packed Single-FP Compare
	NN_S_vcmpsd,               // Compare Scalar Double-Precision Floating-Point Values
	NN_S_vcmpss,               // Scalar Single-FP Compare
	NN_S_vcomisd,              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	NN_S_vcomiss,              // Scalar Ordered Single-FP Compare and Set EFLAGS
	NN_S_vcvtdq2pd,            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
	NN_S_vcvtdq2ps,            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	NN_S_vcvtpd2dq,            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_vcvtpd2ps,            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
	NN_S_vcvtph2ps,            // Convert 16-bit FP Values to Single-Precision FP Values
	NN_S_vcvtps2dq,            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_vcvtps2pd,            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
	NN_S_vcvtps2ph,            // Convert Single-Precision FP value to 16-bit FP value
	NN_S_vcvtsd2si,            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
	NN_S_vcvtsd2ss,            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
	NN_S_vcvtsi2sd,            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
	NN_S_vcvtsi2ss,            // Scalar signed INT32 to Single-FP conversion
	NN_S_vcvtss2sd,            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
	NN_S_vcvtss2si,            // Scalar Single-FP to signed INT32 conversion
	NN_S_vcvttpd2dq,           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_vcvttps2dq,           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	NN_S_vcvttsd2si,           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
	NN_S_vcvttss2si,           // Scalar Single-FP to signed INT32 conversion (truncate)
	NN_S_vdivpd,               // Divide Packed Double-Precision Floating-Point Values
	NN_S_vdivps,               // Packed Single-FP Divide
	NN_S_vdivsd,               // Divide Scalar Double-Precision Floating-Point Values
	NN_S_vdivss,               // Scalar Single-FP Divide
	NN_S_vdppd,                // Dot Product of Packed Double Precision Floating-Point Values
	NN_S_vdpps,                // Dot Product of Packed Single Precision Floating-Point Values
	NN_S_vextractf128,         // Extract Packed Floating-Point Values
	NN_S_vextracti128,         // Extract Packed Integer Values
	NN_S_vextractps,           // Extract Packed Floating-Point Values
	NN_S_vfmadd132pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmadd132ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfmadd132sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfmadd132ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfmadd213pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmadd213ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfmadd213sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfmadd213ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfmadd231pd,          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmadd231ps,          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfmadd231sd,          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfmadd231ss,          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfmaddsub132pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmaddsub132ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmaddsub213pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmaddsub213ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmaddsub231pd,       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmaddsub231ps,       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmsub132pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmsub132ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmsub132sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfmsub132ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vfmsub213pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmsub213ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmsub213sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfmsub213ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vfmsub231pd,          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfmsub231ps,          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfmsub231sd,          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfmsub231ss,          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vfmsubadd132pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmsubadd132ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	NN_S_vfmsubadd213pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmsubadd213ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	NN_S_vfmsubadd231pd,       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	NN_S_vfmsubadd231ps,       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	NN_S_vfnmadd132pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfnmadd132ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfnmadd132sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmadd132ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfnmadd213pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfnmadd213ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfnmadd213sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmadd213ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfnmadd231pd,         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	NN_S_vfnmadd231ps,         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	NN_S_vfnmadd231sd,         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmadd231ss,         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	NN_S_vfnmsub132pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfnmsub132ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfnmsub132sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmsub132ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vfnmsub213pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfnmsub213ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfnmsub213sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmsub213ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vfnmsub231pd,         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	NN_S_vfnmsub231ps,         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	NN_S_vfnmsub231sd,         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	NN_S_vfnmsub231ss,         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	NN_S_vgatherdps,           // Gather Packed SP FP Values Using Signed Dword Indices
	NN_S_vgatherdpd,           // Gather Packed DP FP Values Using Signed Dword Indices
	NN_S_vgatherqps,           // Gather Packed SP FP Values Using Signed Qword Indices
	NN_S_vgatherqpd,           // Gather Packed DP FP Values Using Signed Qword Indices
	NN_S_vhaddpd,              // Add horizontally packed DP FP numbers
	NN_S_vhaddps,              // Add horizontally packed SP FP numbers
	NN_S_vhsubpd,              // Sub horizontally packed DP FP numbers
	NN_S_vhsubps,              // Sub horizontally packed SP FP numbers
	NN_S_vinsertf128,          // Insert Packed Floating-Point Values
	NN_S_vinserti128,          // Insert Packed Integer Values
	NN_S_vinsertps,            // Insert Packed Single Precision Floating-Point Value
	NN_S_vlddqu,               // Load Unaligned Packed Integer Values
	NN_S_vldmxcsr,             // Load Streaming SIMD Extensions Technology Control/Status Register
	NN_S_vmaskmovdqu,          // Store Selected Bytes of Double Quadword with NT Hint
	NN_S_vmaskmovpd,           // Conditionally Load Packed Double-Precision Floating-Point Values
	NN_S_vmaskmovps,           // Conditionally Load Packed Single-Precision Floating-Point Values
	NN_S_vmaxpd,               // Return Maximum Packed Double-Precision Floating-Point Values
	NN_S_vmaxps,               // Packed Single-FP Maximum
	NN_S_vmaxsd,               // Return Maximum Scalar Double-Precision Floating-Point Value
	NN_S_vmaxss,               // Scalar Single-FP Maximum
	NN_S_vminpd,               // Return Minimum Packed Double-Precision Floating-Point Values
	NN_S_vminps,               // Packed Single-FP Minimum
	NN_S_vminsd,               // Return Minimum Scalar Double-Precision Floating-Point Value
	NN_S_vminss,               // Scalar Single-FP Minimum
	NN_S_vmovapd,              // Move Aligned Packed Double-Precision Floating-Point Values
	NN_S_vmovaps,              // Move Aligned Four Packed Single-FP
	NN_S_vmovd,                // Move 32 bits
	NN_S_vmovddup,             // Move One Double-FP and Duplicate
	NN_S_vmovdqa,              // Move Aligned Double Quadword
	NN_S_vmovdqu,              // Move Unaligned Double Quadword
	NN_S_vmovhlps,             // Move High to Low Packed Single-FP
	NN_S_vmovhpd,              // Move High Packed Double-Precision Floating-Point Values
	NN_S_vmovhps,              // Move High Packed Single-FP
	NN_S_vmovlhps,             // Move Low to High Packed Single-FP
	NN_S_vmovlpd,              // Move Low Packed Double-Precision Floating-Point Values
	NN_S_vmovlps,              // Move Low Packed Single-FP
	NN_S_vmovmskpd,            // Extract Packed Double-Precision Floating-Point Sign Mask
	NN_S_vmovmskps,            // Move Mask to Register
	NN_S_vmovntdq,             // Store Double Quadword Using Non-Temporal Hint
	NN_S_vmovntdqa,            // Load Double Quadword Non-Temporal Aligned Hint
	NN_S_vmovntpd,             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
	NN_S_vmovntps,             // Move Aligned Four Packed Single-FP Non Temporal
	NN_S_vmovq,                // Move 64 bits
	NN_S_vmovsd,               // Move Scalar Double-Precision Floating-Point Values
	NN_S_vmovshdup,            // Move Packed Single-FP High and Duplicate
	NN_S_vmovsldup,            // Move Packed Single-FP Low and Duplicate
	NN_S_vmovss,               // Move Scalar Single-FP
	NN_S_vmovupd,              // Move Unaligned Packed Double-Precision Floating-Point Values
	NN_S_vmovups,              // Move Unaligned Four Packed Single-FP
	NN_S_vmpsadbw,             // Compute Multiple Packed Sums of Absolute Difference
	NN_S_vmulpd,               // Multiply Packed Double-Precision Floating-Point Values
	NN_S_vmulps,               // Packed Single-FP Multiply
	NN_S_vmulsd,               // Multiply Scalar Double-Precision Floating-Point Values
	NN_S_vmulss,               // Scalar Single-FP Multiply
	NN_S_vorpd,                // Bitwise Logical OR of Double-Precision Floating-Point Values
	NN_S_vorps,                // Bitwise Logical OR for Single-FP Data
	NN_S_vpabsb,               // Packed Absolute Value Byte
	NN_S_vpabsd,               // Packed Absolute Value Doubleword
	NN_S_vpabsw,               // Packed Absolute Value Word
	NN_S_vpackssdw,            // Pack with Signed Saturation (Dword->Word)
	NN_S_vpacksswb,            // Pack with Signed Saturation (Word->Byte)
	NN_S_vpackusdw,            // Pack with Unsigned Saturation
	NN_S_vpackuswb,            // Pack with Unsigned Saturation (Word->Byte)
	NN_S_vpaddb,               // Packed Add Byte
	NN_S_vpaddd,               // Packed Add Dword
	NN_S_vpaddq,               // Add Packed Quadword Integers
	NN_S_vpaddsb,              // Packed Add with Saturation (Byte)
	NN_S_vpaddsw,              // Packed Add with Saturation (Word)
	NN_S_vpaddusb,             // Packed Add Unsigned with Saturation (Byte)
	NN_S_vpaddusw,             // Packed Add Unsigned with Saturation (Word)
	NN_S_vpaddw,               // Packed Add Word
	NN_S_vpalignr,             // Packed Align Right
	NN_S_vpand,                // Bitwise Logical And
	NN_S_vpandn,               // Bitwise Logical And Not
	NN_S_vpavgb,               // Packed Average (Byte)
	NN_S_vpavgw,               // Packed Average (Word)
	NN_S_vpblendd,             // Blend Packed Dwords
	NN_S_vpblendvb,            // Variable Blend Packed Bytes
	NN_S_vpblendw,             // Blend Packed Words
	NN_S_vpbroadcastb,         // Broadcast a Byte Integer
	NN_S_vpbroadcastd,         // Broadcast a Dword Integer
	NN_S_vpbroadcastq,         // Broadcast a Qword Integer
	NN_S_vpbroadcastw,         // Broadcast a Word Integer
	NN_S_vpclmulqdq,           // Carry-Less Multiplication Quadword
	NN_S_vpcmpeqb,             // Packed Compare for Equal (Byte)
	NN_S_vpcmpeqd,             // Packed Compare for Equal (Dword)
	NN_S_vpcmpeqq,             // Compare Packed Qword Data for Equal
	NN_S_vpcmpeqw,             // Packed Compare for Equal (Word)
	NN_S_vpcmpestri,           // Packed Compare Explicit Length Strings, Return Index
	NN_S_vpcmpestrm,           // Packed Compare Explicit Length Strings, Return Mask
	NN_S_vpcmpgtb,             // Packed Compare for Greater Than (Byte)
	NN_S_vpcmpgtd,             // Packed Compare for Greater Than (Dword)
	NN_S_vpcmpgtq,             // Compare Packed Data for Greater Than
	NN_S_vpcmpgtw,             // Packed Compare for Greater Than (Word)
	NN_S_vpcmpistri,           // Packed Compare Implicit Length Strings, Return Index
	NN_S_vpcmpistrm,           // Packed Compare Implicit Length Strings, Return Mask
	NN_S_vperm2f128,           // Permute Floating-Point Values
	NN_S_vperm2i128,           // Permute Integer Values
	NN_S_vpermd,               // Full Doublewords Element Permutation
	NN_S_vpermilpd,            // Permute Double-Precision Floating-Point Values
	NN_S_vpermilps,            // Permute Single-Precision Floating-Point Values
	NN_S_vpermpd,              // Permute Double-Precision Floating-Point Elements
	NN_S_vpermps,              // Permute Single-Precision Floating-Point Elements
	NN_S_vpermq,               // Qwords Element Permutation
	NN_S_vpextrb,              // Extract Byte
	NN_S_vpextrd,              // Extract Dword
	NN_S_vpextrq,              // Extract Qword
	NN_S_vpextrw,              // Extract Word
	NN_S_vpgatherdd,           // Gather Packed Dword Values Using Signed Dword Indices
	NN_S_vpgatherdq,           // Gather Packed Qword Values Using Signed Dword Indices
	NN_S_vpgatherqd,           // Gather Packed Dword Values Using Signed Qword Indices
	NN_S_vpgatherqq,           // Gather Packed Qword Values Using Signed Qword Indices
	NN_S_vphaddd,              // Packed Horizontal Add Doubleword
	NN_S_vphaddsw,             // Packed Horizontal Add and Saturate
	NN_S_vphaddw,              // Packed Horizontal Add Word
	NN_S_vphminposuw,          // Packed Horizontal Word Minimum
	NN_S_vphsubd,              // Packed Horizontal Subtract Doubleword
	NN_S_vphsubsw,             // Packed Horizontal Subtract and Saturate
	NN_S_vphsubw,              // Packed Horizontal Subtract Word
	NN_S_vpinsrb,              // Insert Byte
	NN_S_vpinsrd,              // Insert Dword
	NN_S_vpinsrq,              // Insert Qword
	NN_S_vpinsrw,              // Insert Word
	NN_S_vpmaddubsw,           // Multiply and Add Packed Signed and Unsigned Bytes
	NN_S_vpmaddwd,             // Packed Multiply and Add
	NN_S_vpmaskmovd,           // Conditionally Store Dword Values Using Mask
	NN_S_vpmaskmovq,           // Conditionally Store Qword Values Using Mask
	NN_S_vpmaxsb,              // Maximum of Packed Signed Byte Integers
	NN_S_vpmaxsd,              // Maximum of Packed Signed Dword Integers
	NN_S_vpmaxsw,              // Packed Signed Integer Word Maximum
	NN_S_vpmaxub,              // Packed Unsigned Integer Byte Maximum
	NN_S_vpmaxud,              // Maximum of Packed Unsigned Dword Integers
	NN_S_vpmaxuw,              // Maximum of Packed Word Integers
	NN_S_vpminsb,              // Minimum of Packed Signed Byte Integers
	NN_S_vpminsd,              // Minimum of Packed Signed Dword Integers
	NN_S_vpminsw,              // Packed Signed Integer Word Minimum
	NN_S_vpminub,              // Packed Unsigned Integer Byte Minimum
	NN_S_vpminud,              // Minimum of Packed Unsigned Dword Integers
	NN_S_vpminuw,              // Minimum of Packed Word Integers
	NN_S_vpmovmskb,            // Move Byte Mask to Integer
	NN_S_vpmovsxbd,            // Packed Move with Sign Extend
	NN_S_vpmovsxbq,            // Packed Move with Sign Extend
	NN_S_vpmovsxbw,            // Packed Move with Sign Extend
	NN_S_vpmovsxdq,            // Packed Move with Sign Extend
	NN_S_vpmovsxwd,            // Packed Move with Sign Extend
	NN_S_vpmovsxwq,            // Packed Move with Sign Extend
	NN_S_vpmovzxbd,            // Packed Move with Zero Extend
	NN_S_vpmovzxbq,            // Packed Move with Zero Extend
	NN_S_vpmovzxbw,            // Packed Move with Zero Extend
	NN_S_vpmovzxdq,            // Packed Move with Zero Extend
	NN_S_vpmovzxwd,            // Packed Move with Zero Extend
	NN_S_vpmovzxwq,            // Packed Move with Zero Extend
	NN_S_vpmuldq,              // Multiply Packed Signed Dword Integers
	NN_S_vpmulhrsw,            // Packed Multiply High with Round and Scale
	NN_S_vpmulhuw,             // Packed Multiply High Unsigned
	NN_S_vpmulhw,              // Packed Multiply High
	NN_S_vpmulld,              // Multiply Packed Signed Dword Integers and Store Low Result
	NN_S_vpmullw,              // Packed Multiply Low
	NN_S_vpmuludq,             // Multiply Packed Unsigned Doubleword Integers
	NN_S_vpor,                 // Bitwise Logical Or
	NN_S_vpsadbw,              // Packed Sum of Absolute Differences
	NN_S_vpshufb,              // Packed Shuffle Bytes
	NN_S_vpshufd,              // Shuffle Packed Doublewords
	NN_S_vpshufhw,             // Shuffle Packed High Words
	NN_S_vpshuflw,             // Shuffle Packed Low Words
	NN_S_vpsignb,              // Packed SIGN Byte
	NN_S_vpsignd,              // Packed SIGN Doubleword
	NN_S_vpsignw,              // Packed SIGN Word
	NN_S_vpslld,               // Packed Shift Left Logical (Dword)
	NN_S_vpslldq,              // Shift Double Quadword Left Logical
	NN_S_vpsllq,               // Packed Shift Left Logical (Qword)
	NN_S_vpsllvd,              // Variable Bit Shift Left Logical (Dword)
	NN_S_vpsllvq,              // Variable Bit Shift Left Logical (Qword)
	NN_S_vpsllw,               // Packed Shift Left Logical (Word)
	NN_S_vpsrad,               // Packed Shift Right Arithmetic (Dword)
	NN_S_vpsravd,              // Variable Bit Shift Right Arithmetic
	NN_S_vpsraw,               // Packed Shift Right Arithmetic (Word)
	NN_S_vpsrld,               // Packed Shift Right Logical (Dword)
	NN_S_vpsrldq,              // Shift Double Quadword Right Logical (Qword)
	NN_S_vpsrlq,               // Packed Shift Right Logical (Qword)
	NN_S_vpsrlvd,              // Variable Bit Shift Right Logical (Dword)
	NN_S_vpsrlvq,              // Variable Bit Shift Right Logical (Qword)
	NN_S_vpsrlw,               // Packed Shift Right Logical (Word)
	NN_S_vpsubb,               // Packed Subtract Byte
	NN_S_vpsubd,               // Packed Subtract Dword
	NN_S_vpsubq,               // Subtract Packed Quadword Integers
	NN_S_vpsubsb,              // Packed Subtract with Saturation (Byte)
	NN_S_vpsubsw,              // Packed Subtract with Saturation (Word)
	NN_S_vpsubusb,             // Packed Subtract Unsigned with Saturation (Byte)
	NN_S_vpsubusw,             // Packed Subtract Unsigned with Saturation (Word)
	NN_S_vpsubw,               // Packed Subtract Word
	NN_S_vptest,               // Logical Compare
	NN_S_vpunpckhbw,           // Unpack High Packed Data (Byte->Word)
	NN_S_vpunpckhdq,           // Unpack High Packed Data (Dword->Qword)
	NN_S_vpunpckhqdq,          // Unpack High Packed Data (Qword->Xmmword)
	NN_S_vpunpckhwd,           // Unpack High Packed Data (Word->Dword)
	NN_S_vpunpcklbw,           // Unpack Low Packed Data (Byte->Word)
	NN_S_vpunpckldq,           // Unpack Low Packed Data (Dword->Qword)
	NN_S_vpunpcklqdq,          // Unpack Low Packed Data (Qword->Xmmword)
	NN_S_vpunpcklwd,           // Unpack Low Packed Data (Word->Dword)
	NN_S_vpxor,                // Bitwise Logical Exclusive Or
	NN_S_vrcpps,               // Packed Single-FP Reciprocal
	NN_S_vrcpss,               // Scalar Single-FP Reciprocal
	NN_S_vroundpd,             // Round Packed Double Precision Floating-Point Values
	NN_S_vroundps,             // Round Packed Single Precision Floating-Point Values
	NN_S_vroundsd,             // Round Scalar Double Precision Floating-Point Values
	NN_S_vroundss,             // Round Scalar Single Precision Floating-Point Values
	NN_S_vrsqrtps,             // Packed Single-FP Square Root Reciprocal
	NN_S_vrsqrtss,             // Scalar Single-FP Square Root Reciprocal
	NN_S_vshufpd,              // Shuffle Packed Double-Precision Floating-Point Values
	NN_S_vshufps,              // Shuffle Single-FP
	NN_S_vsqrtpd,              // Compute Square Roots of Packed Double-Precision Floating-Point Values
	NN_S_vsqrtps,              // Packed Single-FP Square Root
	NN_S_vsqrtsd,              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
	NN_S_vsqrtss,              // Scalar Single-FP Square Root
	NN_S_vstmxcsr,             // Store Streaming SIMD Extensions Technology Control/Status Register
	NN_S_vsubpd,               // Subtract Packed Double-Precision Floating-Point Values
	NN_S_vsubps,               // Packed Single-FP Subtract
	NN_S_vsubsd,               // Subtract Scalar Double-Precision Floating-Point Values
	NN_S_vsubss,               // Scalar Single-FP Subtract
	NN_S_vtestpd,              // Packed Double-Precision Floating-Point Bit Test
	NN_S_vtestps,              // Packed Single-Precision Floating-Point Bit Test
	NN_S_vucomisd,             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	NN_S_vucomiss,             // Scalar Unordered Single-FP Compare and Set EFLAGS
	NN_S_vunpckhpd,            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
	NN_S_vunpckhps,            // Unpack High Packed Single-FP Data
	NN_S_vunpcklpd,            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
	NN_S_vunpcklps,            // Unpack Low Packed Single-FP Data
	NN_S_vxorpd,               // Bitwise Logical OR of Double-Precision Floating-Point Values
	NN_S_vxorps,               // Bitwise Logical XOR for Single-FP Data
	NN_S_vzeroall,             // Zero All YMM Registers
	NN_S_vzeroupper,           // Zero Upper Bits of YMM Registers

	// Transactional Synchronization Extensions

	NN_S_xabort,               // Transaction Abort
	NN_S_xbegin,               // Transaction Begin
	NN_S_xend,                 // Transaction End
	NN_S_xtest,                // Test If In Transactional Execution

	// Virtual PC synthetic instructions
/*
	NN_S_vmgetinfo,            // Virtual PC - Get VM Information
	NN_S_vmsetinfo,            // Virtual PC - Set VM Information
	NN_S_vmdxdsbl,             // Virtual PC - Disable Direct Execution
	NN_S_vmdxenbl,             // Virtual PC - Enable Direct Execution
	NN_S_vmcpuid,              // Virtual PC - Virtualized CPU Information
	NN_S_vmhlt,                // Virtual PC - Halt
	NN_S_vmsplaf,              // Virtual PC - Spin Lock Acquisition Failed
	NN_S_vmpushfd,             // Virtual PC - Push virtualized flags register
	NN_S_vmpopfd,              // Virtual PC - Pop virtualized flags register
	NN_S_vmcli,                // Virtual PC - Clear Interrupt Flag
	NN_S_vmsti,                // Virtual PC - Set Interrupt Flag
	NN_S_vmiretd,              // Virtual PC - Return From Interrupt
	NN_S_vmsgdt,               // Virtual PC - Store Global Descriptor Table
	NN_S_vmsidt,               // Virtual PC - Store Interrupt Descriptor Table
	NN_S_vmsldt,               // Virtual PC - Store Local Descriptor Table
	NN_S_vmstr,                // Virtual PC - Store Task Register
	NN_S_vmsdte,               // Virtual PC - Store to Descriptor Table Entry
	NN_S_vpcext,               // Virtual PC - ISA extension
*/
	// AMD FMA4
/*
	NN_S_vfmaddsubps,          // Multiply with Alternating Add/Subtract of Packed Single-Precision Floating-Point
	NN_S_vfmaddsubpd,          // Multiply with Alternating Add/Subtract of Packed Double-Precision Floating-Point
	NN_S_vfmsubaddps,          // Multiply with Alternating Subtract/Add of Packed Single-Precision Floating-Point
	NN_S_vfmsubaddpd,          // Multiply with Alternating Subtract/Add of Packed Double-Precision Floating-Point
	NN_S_vfmaddps,             // Multiply and Add Packed Single-Precision Floating-Point
	NN_S_vfmaddpd,             // Multiply and Add Packed Double-Precision Floating-Point
	NN_S_vfmaddss,             // Multiply and Add Scalar Single-Precision Floating-Point
	NN_S_vfmaddsd,             // Multiply and Add Scalar Double-Precision Floating-Point
	NN_S_vfmsubps,             // Multiply and Subtract Packed Single-Precision Floating-Point
	NN_S_vfmsubpd,             // Multiply and Subtract Packed Double-Precision Floating-Point
	NN_S_vfmsubss,             // Multiply and Subtract Scalar Single-Precision Floating-Point
	NN_S_vfmsubsd,             // Multiply and Subtract Scalar Double-Precision Floating-Point
	NN_S_vfnmaddps,            // Negative Multiply and Add Packed Single-Precision Floating-Point
	NN_S_vfnmaddpd,            // Negative Multiply and Add Packed Double-Precision Floating-Point
	NN_S_vfnmaddss,            // Negative Multiply and Add Scalar Single-Precision Floating-Point
	NN_S_vfnmaddsd,            // Negative Multiply and Add Double Single-Precision Floating-Point
	NN_S_vfnmsubps,            // Negative Multiply and Subtract Packed Single-Precision Floating-Point
	NN_S_vfnmsubpd,            // Negative Multiply and Subtract Packed Double-Precision Floating-Point
	NN_S_vfnmsubss,            // Negative Multiply and Subtract Scalar Single-Precision Floating-Point
	NN_S_vfnmsubsd,            // Negative Multiply and Subtract Double Single-Precision Floating-Point
*/
	// Intel Memory Protection Extensions (MPX)
/*
	NN_S_bndmk,                // Make Bounds
	NN_S_bndcl,                // Check Lower Bound
	NN_S_bndcu,                // Check Upper Bound
	NN_S_bndcn,                // Check Upper Bound
	NN_S_bndmov,               // Move Bounds
	NN_S_bndldx,               // Load Extended Bounds Using Address Translation
	NN_S_bndstx,               // Store Extended Bounds Using Address Translation
*/
	// New xstate instructions

/*	NN_S_xrstors,              // Restore Processor Extended States Supervisor
	NN_S_xsavec,               // Save Processor Extended States with Compaction
	NN_S_xsaves,               // Save Processor Extended States Supervisor
*/
	// PREFETCHWT1 support

	NN_S_prefetchwt1,          // Prefetch Vector Data Into Caches with Intent to Write and T1 Hint

	// Memory instructions

	/*NN_S_clflushopt,           // Flush a Cache Line Optimized
	NN_S_clwb,                 // Cache Line Write Back
	NN_S_pcommit,              // Persistent Commit (deprecated by Intel)
*/
	// Protection Key Rights for User Pages

//	NN_S_rdpkru,               // Read Protection Key Rights for User Pages
//	NN_S_wrpkru,               // Write Data to User Page Key Register

	// AVX comparison pseudo-ops

	NN_S_vcmpeqpd,             // Compare Packed Double-Precision Floating-Point Values - Equal (ordered, non-signaling)
	NN_S_vcmpltpd,             // Compare Packed Double-Precision Floating-Point Values - Less-than (ordered, signaling)
	NN_S_vcmplepd,             // Compare Packed Double-Precision Floating-Point Values - Less-than-or-equal (ordered, signaling)
	NN_S_vcmpunordpd,          // Compare Packed Double-Precision Floating-Point Values - Unordered (non-signaling)
	NN_S_vcmpneqpd,            // Compare Packed Double-Precision Floating-Point Values - Not-equal (unordered, non-signaling)
	NN_S_vcmpnltpd,            // Compare Packed Double-Precision Floating-Point Values - Not-less-than (unordered, signaling)
	NN_S_vcmpnlepd,            // Compare Packed Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, signaling)
	NN_S_vcmpordpd,            // Compare Packed Double-Precision Floating-Point Values - Ordered (non-signaling)
	NN_S_vcmpeq_uqpd,          // Compare Packed Double-Precision Floating-Point Values - Equal (unordered, non-signaling)
	NN_S_vcmpngepd,            // Compare Packed Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, signaling)
	NN_S_vcmpngtpd,            // Compare Packed Double-Precision Floating-Point Values - Not-greater-than (unordered, signaling)
	NN_S_vcmpfalsepd,          // Compare Packed Double-Precision Floating-Point Values - False (ordered, non-signaling)
	NN_S_vcmpneq_oqpd,         // Compare Packed Double-Precision Floating-Point Values - Not-equal (ordered, non-signaling)
	NN_S_vcmpgepd,             // Compare Packed Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, signaling)
	NN_S_vcmpgtpd,             // Compare Packed Double-Precision Floating-Point Values - Greater-than (ordered, signaling)
	NN_S_vcmptruepd,           // Compare Packed Double-Precision Floating-Point Values - True (unordered, non-signaling)
	NN_S_vcmpeq_ospd,          // Compare Packed Double-Precision Floating-Point Values - Equal (ordered, signaling)
	NN_S_vcmplt_oqpd,          // Compare Packed Double-Precision Floating-Point Values - Less-than (ordered, non-signaling)
	NN_S_vcmple_oqpd,          // Compare Packed Double-Precision Floating-Point Values - Less-than-or-equal (ordered, non-signaling)
	NN_S_vcmpunord_spd,        // Compare Packed Double-Precision Floating-Point Values - Unordered (signaling)
	NN_S_vcmpneq_uspd,         // Compare Packed Double-Precision Floating-Point Values - Not-equal (unordered, signaling)
	NN_S_vcmpnlt_uqpd,         // Compare Packed Double-Precision Floating-Point Values - Not-less-than (unordered, non-signaling)
	NN_S_vcmpnle_uqpd,         // Compare Packed Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, non-signaling)
	NN_S_vcmpord_spd,          // Compare Packed Double-Precision Floating-Point Values - Ordered (signaling)
	NN_S_vcmpeq_uspd,          // Compare Packed Double-Precision Floating-Point Values - Equal (unordered, signaling)
	NN_S_vcmpnge_uqpd,         // Compare Packed Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, non-signaling)
	NN_S_vcmpngt_uqpd,         // Compare Packed Double-Precision Floating-Point Values - Not-greater-than (unordered, non-signaling)
	NN_S_vcmpfalse_ospd,       // Compare Packed Double-Precision Floating-Point Values - False (ordered, signaling)
	NN_S_vcmpneq_ospd,         // Compare Packed Double-Precision Floating-Point Values - Not-equal (ordered, signaling)
	NN_S_vcmpge_oqpd,          // Compare Packed Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, non-signaling)
	NN_S_vcmpgt_oqpd,          // Compare Packed Double-Precision Floating-Point Values - Greater-than (ordered, non-signaling)
	NN_S_vcmptrue_uspd,        // Compare Packed Double-Precision Floating-Point Values - True (unordered, signaling)

	NN_S_vcmpeqps,             // Packed Single-FP Compare - Equal (ordered, non-signaling)
	NN_S_vcmpltps,             // Packed Single-FP Compare - Less-than (ordered, signaling)
	NN_S_vcmpleps,             // Packed Single-FP Compare - Less-than-or-equal (ordered, signaling)
	NN_S_vcmpunordps,          // Packed Single-FP Compare - Unordered (non-signaling)
	NN_S_vcmpneqps,            // Packed Single-FP Compare - Not-equal (unordered, non-signaling)
	NN_S_vcmpnltps,            // Packed Single-FP Compare - Not-less-than (unordered, signaling)
	NN_S_vcmpnleps,            // Packed Single-FP Compare - Not-less-than-or-equal (unordered, signaling)
	NN_S_vcmpordps,            // Packed Single-FP Compare - Ordered (non-signaling)
	NN_S_vcmpeq_uqps,          // Packed Single-FP Compare - Equal (unordered, non-signaling)
	NN_S_vcmpngeps,            // Packed Single-FP Compare - Not-greater-than-or-equal (unordered, signaling)
	NN_S_vcmpngtps,            // Packed Single-FP Compare - Not-greater-than (unordered, signaling)
	NN_S_vcmpfalseps,          // Packed Single-FP Compare - False (ordered, non-signaling)
	NN_S_vcmpneq_oqps,         // Packed Single-FP Compare - Not-equal (ordered, non-signaling)
	NN_S_vcmpgeps,             // Packed Single-FP Compare - Greater-than-or-equal (ordered, signaling)
	NN_S_vcmpgtps,             // Packed Single-FP Compare - Greater-than (ordered, signaling)
	NN_S_vcmptrueps,           // Packed Single-FP Compare - True (unordered, non-signaling)
	NN_S_vcmpeq_osps,          // Packed Single-FP Compare - Equal (ordered, signaling)
	NN_S_vcmplt_oqps,          // Packed Single-FP Compare - Less-than (ordered, non-signaling)
	NN_S_vcmple_oqps,          // Packed Single-FP Compare - Less-than-or-equal (ordered, non-signaling)
	NN_S_vcmpunord_sps,        // Packed Single-FP Compare - Unordered (signaling)
	NN_S_vcmpneq_usps,         // Packed Single-FP Compare - Not-equal (unordered, signaling)
	NN_S_vcmpnlt_uqps,         // Packed Single-FP Compare - Not-less-than (unordered, non-signaling)
	NN_S_vcmpnle_uqps,         // Packed Single-FP Compare - Not-less-than-or-equal (unordered, non-signaling)
	NN_S_vcmpord_sps,          // Packed Single-FP Compare - Ordered (signaling)
	NN_S_vcmpeq_usps,          // Packed Single-FP Compare - Equal (unordered, signaling)
	NN_S_vcmpnge_uqps,         // Packed Single-FP Compare - Not-greater-than-or-equal (unordered, non-signaling)
	NN_S_vcmpngt_uqps,         // Packed Single-FP Compare - Not-greater-than (unordered, non-signaling)
	NN_S_vcmpfalse_osps,       // Packed Single-FP Compare - False (ordered, signaling)
	NN_S_vcmpneq_osps,         // Packed Single-FP Compare - Not-equal (ordered, signaling)
	NN_S_vcmpge_oqps,          // Packed Single-FP Compare - Greater-than-or-equal (ordered, non-signaling)
	NN_S_vcmpgt_oqps,          // Packed Single-FP Compare - Greater-than (ordered, non-signaling)
	NN_S_vcmptrue_usps,        // Packed Single-FP Compare - True (unordered, signaling)

	NN_S_vcmpeqsd,             // Compare Scalar Double-Precision Floating-Point Values - Equal (ordered, non-signaling)
	NN_S_vcmpltsd,             // Compare Scalar Double-Precision Floating-Point Values - Less-than (ordered, signaling)
	NN_S_vcmplesd,             // Compare Scalar Double-Precision Floating-Point Values - Less-than-or-equal (ordered, signaling)
	NN_S_vcmpunordsd,          // Compare Scalar Double-Precision Floating-Point Values - Unordered (non-signaling)
	NN_S_vcmpneqsd,            // Compare Scalar Double-Precision Floating-Point Values - Not-equal (unordered, non-signaling)
	NN_S_vcmpnltsd,            // Compare Scalar Double-Precision Floating-Point Values - Not-less-than (unordered, signaling)
	NN_S_vcmpnlesd,            // Compare Scalar Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, signaling)
	NN_S_vcmpordsd,            // Compare Scalar Double-Precision Floating-Point Values - Ordered (non-signaling)
	NN_S_vcmpeq_uqsd,          // Compare Scalar Double-Precision Floating-Point Values - Equal (unordered, non-signaling)
	NN_S_vcmpngesd,            // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, signaling)
	NN_S_vcmpngtsd,            // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than (unordered, signaling)
	NN_S_vcmpfalsesd,          // Compare Scalar Double-Precision Floating-Point Values - False (ordered, non-signaling)
	NN_S_vcmpneq_oqsd,         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (ordered, non-signaling)
	NN_S_vcmpgesd,             // Compare Scalar Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, signaling)
	NN_S_vcmpgtsd,             // Compare Scalar Double-Precision Floating-Point Values - Greater-than (ordered, signaling)
	NN_S_vcmptruesd,           // Compare Scalar Double-Precision Floating-Point Values - True (unordered, non-signaling)
	NN_S_vcmpeq_ossd,          // Compare Scalar Double-Precision Floating-Point Values - Equal (ordered, signaling)
	NN_S_vcmplt_oqsd,          // Compare Scalar Double-Precision Floating-Point Values - Less-than (ordered, non-signaling)
	NN_S_vcmple_oqsd,          // Compare Scalar Double-Precision Floating-Point Values - Less-than-or-equal (ordered, non-signaling)
	NN_S_vcmpunord_ssd,        // Compare Scalar Double-Precision Floating-Point Values - Unordered (signaling)
	NN_S_vcmpneq_ussd,         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (unordered, signaling)
	NN_S_vcmpnlt_uqsd,         // Compare Scalar Double-Precision Floating-Point Values - Not-less-than (unordered, non-signaling)
	NN_S_vcmpnle_uqsd,         // Compare Scalar Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, non-signaling)
	NN_S_vcmpord_ssd,          // Compare Scalar Double-Precision Floating-Point Values - Ordered (signaling)
	NN_S_vcmpeq_ussd,          // Compare Scalar Double-Precision Floating-Point Values - Equal (unordered, signaling)
	NN_S_vcmpnge_uqsd,         // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, non-signaling)
	NN_S_vcmpngt_uqsd,         // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than (unordered, non-signaling)
	NN_S_vcmpfalse_ossd,       // Compare Scalar Double-Precision Floating-Point Values - False (ordered, signaling)
	NN_S_vcmpneq_ossd,         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (ordered, signaling)
	NN_S_vcmpge_oqsd,          // Compare Scalar Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, non-signaling)
	NN_S_vcmpgt_oqsd,          // Compare Scalar Double-Precision Floating-Point Values - Greater-than (ordered, non-signaling)
	NN_S_vcmptrue_ussd,        // Compare Scalar Double-Precision Floating-Point Values - True (unordered, signaling)

	NN_S_vcmpeqss,             // Scalar Single-FP Compare - Equal (ordered, non-signaling)
	NN_S_vcmpltss,             // Scalar Single-FP Compare - Less-than (ordered, signaling)
	NN_S_vcmpless,             // Scalar Single-FP Compare - Less-than-or-equal (ordered, signaling)
	NN_S_vcmpunordss,          // Scalar Single-FP Compare - Unordered (non-signaling)
	NN_S_vcmpneqss,            // Scalar Single-FP Compare - Not-equal (unordered, non-signaling)
	NN_S_vcmpnltss,            // Scalar Single-FP Compare - Not-less-than (unordered, signaling)
	NN_S_vcmpnless,            // Scalar Single-FP Compare - Not-less-than-or-equal (unordered, signaling)
	NN_S_vcmpordss,            // Scalar Single-FP Compare - Ordered (non-signaling)
	NN_S_vcmpeq_uqss,          // Scalar Single-FP Compare - Equal (unordered, non-signaling)
	NN_S_vcmpngess,            // Scalar Single-FP Compare - Not-greater-than-or-equal (unordered, signaling)
	NN_S_vcmpngtss,            // Scalar Single-FP Compare - Not-greater-than (unordered, signaling)
	NN_S_vcmpfalsess,          // Scalar Single-FP Compare - False (ordered, non-signaling)
	NN_S_vcmpneq_oqss,         // Scalar Single-FP Compare - Not-equal (ordered, non-signaling)
	NN_S_vcmpgess,             // Scalar Single-FP Compare - Greater-than-or-equal (ordered, signaling)
	NN_S_vcmpgtss,             // Scalar Single-FP Compare - Greater-than (ordered, signaling)
	NN_S_vcmptruess,           // Scalar Single-FP Compare - True (unordered, non-signaling)
	NN_S_vcmpeq_osss,          // Scalar Single-FP Compare - Equal (ordered, signaling)
	NN_S_vcmplt_oqss,          // Scalar Single-FP Compare - Less-than (ordered, non-signaling)
	NN_S_vcmple_oqss,          // Scalar Single-FP Compare - Less-than-or-equal (ordered, non-signaling)
	NN_S_vcmpunord_sss,        // Scalar Single-FP Compare - Unordered (signaling)
	NN_S_vcmpneq_usss,         // Scalar Single-FP Compare - Not-equal (unordered, signaling)
	NN_S_vcmpnlt_uqss,         // Scalar Single-FP Compare - Not-less-than (unordered, non-signaling)
	NN_S_vcmpnle_uqss,         // Scalar Single-FP Compare - Not-less-than-or-equal (unordered, non-signaling)
	NN_S_vcmpord_sss,          // Scalar Single-FP Compare - Ordered (signaling)
	NN_S_vcmpeq_usss,          // Scalar Single-FP Compare - Equal (unordered, signaling)
	NN_S_vcmpnge_uqss,         // Scalar Single-FP Compare - Not-greater-than-or-equal (unordered, non-signaling)
	NN_S_vcmpngt_uqss,         // Scalar Single-FP Compare - Not-greater-than (unordered, non-signaling)
	NN_S_vcmpfalse_osss,       // Scalar Single-FP Compare - False (ordered, signaling)
	NN_S_vcmpneq_osss,         // Scalar Single-FP Compare - Not-equal (ordered, signaling)
	NN_S_vcmpge_oqss,          // Scalar Single-FP Compare - Greater-than-or-equal (ordered, non-signaling)
	NN_S_vcmpgt_oqss,          // Scalar Single-FP Compare - Greater-than (ordered, non-signaling)
	NN_S_vcmptrue_usss,        // Scalar Single-FP Compare - True (unordered, signaling)

	// AVX-512 instructions

	NN_S_valignd,              // Align Doubleword Vectors
	NN_S_valignq,              // Align Quadword Vectors
	NN_S_vblendmpd,            // Blend Float64 Vectors Using an OpMask Control
	NN_S_vblendmps,            // Blend Float32 Vectors Using an OpMask Control
	NN_S_vpblendmb,            // Blend Byte Vectors Using an Opmask Control
	NN_S_vpblendmw,            // Blend Word Vectors Using an Opmask Control
	NN_S_vpblendmd,            // Blend Int32 Vectors Using an OpMask Control
	NN_S_vpblendmq,            // Blend Int64 Vectors Using an OpMask Control
	NN_S_vbroadcastf32x2,      // Load with Broadcast Floating-Point Data
	NN_S_vbroadcastf32x4,      // Load with Broadcast Floating-Point Data
	NN_S_vbroadcastf64x2,      // Load with Broadcast Floating-Point Data
	NN_S_vbroadcastf32x8,      // Load with Broadcast Floating-Point Data
	NN_S_vbroadcastf64x4,      // Load with Broadcast Floating-Point Data
	NN_S_vbroadcasti32x2,      // Load Integer and Broadcast
	NN_S_vbroadcasti32x4,      // Load Integer and Broadcast
	NN_S_vbroadcasti64x2,      // Load Integer and Broadcast
	NN_S_vbroadcasti32x8,      // Load Integer and Broadcast
	NN_S_vbroadcasti64x4,      // Load Integer and Broadcast
	NN_S_vcompresspd,          // Store Sparse Packed Double-Precision Floating-Point Values into Dense Memory
	NN_S_vcompressps,          // Store Sparse Packed Single-Precision Floating-Point Values into Dense Memory
	NN_S_vcvtpd2qq,            // Convert Packed Double-Precision Floating-Point Values to Packed Quadword Integers
	NN_S_vcvtpd2udq,           // Convert Packed Double-Precision Floating-Point Values to Packed Unsigned Doubleword Integers
	NN_S_vcvtpd2uqq,           // Convert Packed Double-Precision Floating-Point Values to Packed Unsigned Quadword Integers
	NN_S_vcvtps2udq,           // Convert Packed Single-Precision Floating-Point Values to Packed Unsigned Doubleword Integer Values
	NN_S_vcvtps2qq,            // Convert Packed Single Precision Floating-Point Values to Packed Singed Quadword Integer Values
	NN_S_vcvtps2uqq,           // Convert Packed Single Precision Floating-Point Values to Packed Unsigned Quadword Integer Values
	NN_S_vcvtqq2pd,            // Convert Packed Quadword Integers to Packed Double-Precision Floating-Point Values
	NN_S_vcvtqq2ps,            // Convert Packed Quadword Integers to Packed Single-Precision Floating-Point Values
	NN_S_vcvtsd2usi,           // Convert Scalar Double-Precision Floating-Point Value to Unsigned Doubleword Integer
	NN_S_vcvtss2usi,           // Convert Scalar Single-Precision Floating-Point Value to Unsigned Doubleword Integer
	NN_S_vcvttpd2qq,           // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Quadword Integers
	NN_S_vcvttpd2udq,          // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Unsigned Doubleword Integers
	NN_S_vcvttpd2uqq,          // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Unsigned Quadword Integers
	NN_S_vcvttps2udq,          // Convert with Truncation Packed Single-Precision Floating-Point Values to Packed Unsigned Doubleword Integer Values
	NN_S_vcvttps2qq,           // Convert with Truncation Packed Single Precision Floating-Point Values to Packed Singed Quadword Integer Values
	NN_S_vcvttps2uqq,          // Convert with Truncation Packed Single Precision Floating-Point Values to Packed Unsigned Quadword Integer Values
	NN_S_vcvttsd2usi,          // Convert with Truncation Scalar Double-Precision Floating-Point Value to Unsigned Integer
	NN_S_vcvttss2usi,          // Convert with Truncation Scalar Single-Precision Floating-Point Value to Unsigned Integer
	NN_S_vcvtudq2pd,           // Convert Packed Unsigned Doubleword Integers to Packed Double-Precision Floating-Point Values
	NN_S_vcvtudq2ps,           // Convert Packed Unsigned Doubleword Integers to Packed Single-Precision Floating-Point Values
	NN_S_vcvtuqq2pd,           // Convert Packed Unsigned Quadword Integers to Packed Double-Precision Floating-Point Values
	NN_S_vcvtuqq2ps,           // Convert Packed Unsigned Quadword Integers to Packed Single-Precision Floating-Point Values
	NN_S_vcvtusi2sd,           // Convert Unsigned Integer to Scalar Double-Precision Floating-Point Value
	NN_S_vcvtusi2ss,           // Convert Unsigned Integer to Scalar Single-Precision Floating-Point Value
	NN_S_vdbpsadbw,            // Double Block Packed Sum-Absolute-Differences (SAD) on Unsigned Bytes
	NN_S_vexpandpd,            // Load Sparse Packed Double-Precision Floating-Point Values from Dense Memory
	NN_S_vexpandps,            // Load Sparse Packed Single-Precision Floating-Point Values from Dense Memory
	NN_S_vextractf32x4,        // Extract Packed Floating-Point Values
	NN_S_vextractf64x2,        // Extract Packed Floating-Point Values
	NN_S_vextractf32x8,        // Extract Packed Floating-Point Values
	NN_S_vextractf64x4,        // Extract Packed Floating-Point Values
	NN_S_vextracti32x4,        // Extract packed Integer Values
	NN_S_vextracti64x2,        // Extract packed Integer Values
	NN_S_vextracti32x8,        // Extract packed Integer Values
	NN_S_vextracti64x4,        // Extract packed Integer Values
	NN_S_vfixupimmpd,          // Fix Up Special Packed Float64 Values
	NN_S_vfixupimmps,          // Fix Up Special Packed Float32 Values
	NN_S_vfixupimmsd,          // Fix Up Special Scalar Float64 Value
	NN_S_vfixupimmss,          // Fix Up Special Scalar Float32 Value
	NN_S_vfpclasspd,           // Tests Types Of a Packed Float64 Values
	NN_S_vfpclassps,           // Tests Types Of a Packed Float32 Values
	NN_S_vfpclasssd,           // Tests Types Of a Scalar Float64 Values
	NN_S_vfpclassss,           // Tests Types Of a Scalar Float32 Values
	NN_S_vgetexppd,            // Convert Exponents of Packed DP FP Values to DP FP Values
	NN_S_vgetexpps,            // Convert Exponents of Packed SP FP Values to SP FP Values
	NN_S_vgetexpsd,            // Convert Exponents of Scalar DP FP Values to DP FP Value
	NN_S_vgetexpss,            // Convert Exponents of Scalar SP FP Values to SP FP Value
	NN_S_vgetmantpd,           // Extract Float64 Vector of Normalized Mantissas from Float64 Vector
	NN_S_vgetmantps,           // Extract Float32 Vector of Normalized Mantissas from Float32 Vector
	NN_S_vgetmantsd,           // Extract Float64 of Normalized Mantissas from Float64 Scalar
	NN_S_vgetmantss,           // Extract Float32 Vector of Normalized Mantissa from Float32 Vector
	NN_S_vinsertf32x4,         // Insert Packed Floating-Point Values
	NN_S_vinsertf64x2,         // Insert Packed Floating-Point Values
	NN_S_vinsertf32x8,         // Insert Packed Floating-Point Values
	NN_S_vinsertf64x4,         // Insert Packed Floating-Point Values
	NN_S_vinserti32x4,         // Insert Packed Integer Values
	NN_S_vinserti64x2,         // Insert Packed Integer Values
	NN_S_vinserti32x8,         // Insert Packed Integer Values
	NN_S_vinserti64x4,         // Insert Packed Integer Values
	NN_S_vmovdqa32,            // Move Aligned Packed Integer Values
	NN_S_vmovdqa64,            // Move Aligned Packed Integer Values
	NN_S_vmovdqu8,             // Move Unaligned Packed Integer Values
	NN_S_vmovdqu16,            // Move Unaligned Packed Integer Values
	NN_S_vmovdqu32,            // Move Unaligned Packed Integer Values
	NN_S_vmovdqu64,            // Move Unaligned Packed Integer Values
	NN_S_vpabsq,               // Packed Absolute Value
	NN_S_vpandd,               // Logical AND
	NN_S_vpandq,               // Logical AND
	NN_S_vpandnd,              // Logical AND NOT
	NN_S_vpandnq,              // Logical AND NOT
	NN_S_vpbroadcastmb2q,      // Broadcast Mask to Vector Register
	NN_S_vpbroadcastmw2d,      // Broadcast Mask to Vector Register
	NN_S_vpcmpb,               // Compare Packed Byte Values Into Mask
	NN_S_vpcmpub,              // Compare Packed Byte Values Into Mask
	NN_S_vpcmpd,               // Compare Packed Integer Values into Mask
	NN_S_vpcmpud,              // Compare Packed Integer Values into Mask
	NN_S_vpcmpq,               // Compare Packed Integer Values into Mask
	NN_S_vpcmpuq,              // Compare Packed Integer Values into Mask
	NN_S_vpcmpw,               // Compare Packed Word Values Into Mask
	NN_S_vpcmpuw,              // Compare Packed Word Values Into Mask
	NN_S_vpcompressd,          // Store Sparse Packed Doubleword Integer Values into Dense Memory/Register
	NN_S_vpcompressq,          // Store Sparse Packed Quadword Integer Values into Dense Memory/Register
	NN_S_vpconflictd,          // Detect Conflicts Within a Vector of Packed Dword Values into Dense Memory/Register
	NN_S_vpconflictq,          // Detect Conflicts Within a Vector of Packed Qword Values into Dense Memory/Register
	NN_S_vpermb,               // Permute Packed Bytes Elements
	NN_S_vpermw,               // Permute Packed Words Elements
	NN_S_vpermi2b,             // Full Permute of Bytes From Two Tables Overwriting the Index
	NN_S_vpermi2w,             // Full Permute From Two Tables Overwriting the Index
	NN_S_vpermi2d,             // Full Permute From Two Tables Overwriting the Index
	NN_S_vpermi2q,             // Full Permute From Two Tables Overwriting the Index
	NN_S_vpermi2ps,            // Full Permute From Two Tables Overwriting the Index
	NN_S_vpermi2pd,            // Full Permute From Two Tables Overwriting the Index
	NN_S_vpermt2b,             // Full Permute of Bytes From Two Tables Overwriting a Table
	NN_S_vpermt2w,             // Full Permute from Two Tables Overwriting one Table
	NN_S_vpermt2d,             // Full Permute from Two Tables Overwriting one Table
	NN_S_vpermt2q,             // Full Permute from Two Tables Overwriting one Table
	NN_S_vpermt2ps,            // Full Permute from Two Tables Overwriting one Table
	NN_S_vpermt2pd,            // Full Permute from Two Tables Overwriting one Table
	NN_S_vpexpandd,            // Load Sparse Packed Doubleword Integer Values from Dense Memory/Register
	NN_S_vpexpandq,            // Load Sparse Packed Quadword Integer Values from Dense Memory/Register
	NN_S_vplzcntd,             // Count the Number of Leading Zero Bits for Packed Dword Values
	NN_S_vplzcntq,             // Count the Number of Leading Zero Bits for Packed Qword Values
	NN_S_vpmadd52luq,          // Packed Multiply of Unsigned 52-bit Integers and Add the Low 52-bit Products to Qword Accumulators
	NN_S_vpmadd52huq,          // Packed Multiply of Unsigned 52-bit Unsigned Integers and Add High 52-bit Products to 64-bit Accumulators
	NN_S_vpmaxsq,              // Maximum of Packed Signed Integers
	NN_S_vpmaxuq,              // Maximum of Packed Unsigned Integers
	NN_S_vpminsq,              // Minimum of Packed Signed Integers
	NN_S_vpminuq,              // Minimum of Packed Unsigned Integers
	NN_S_vpmovm2b,             // Convert a Mask Register to a Vector Register
	NN_S_vpmovm2w,             // Convert a Mask Register to a Vector Register
	NN_S_vpmovm2d,             // Convert a Mask Register to a Vector Register
	NN_S_vpmovm2q,             // Convert a Mask Register to a Vector Register
	NN_S_vpmovb2m,             // Convert a Vector Register to a Mask
	NN_S_vpmovw2m,             // Convert a Vector Register to a Mask
	NN_S_vpmovd2m,             // Convert a Vector Register to a Mask
	NN_S_vpmovq2m,             // Convert a Vector Register to a Mask
	NN_S_vpmovqb,              // Down Convert QWord to Byte
	NN_S_vpmovsqb,             // Down Convert QWord to Byte
	NN_S_vpmovusqb,            // Down Convert QWord to Byte
	NN_S_vpmovqw,              // Down Convert QWord to Word
	NN_S_vpmovsqw,             // Down Convert QWord to Word
	NN_S_vpmovusqw,            // Down Convert QWord to Word
	NN_S_vpmovqd,              // Down Convert QWord to DWord
	NN_S_vpmovsqd,             // Down Convert QWord to DWord
	NN_S_vpmovusqd,            // Down Convert QWord to DWord
	NN_S_vpmovdb,              // Down Convert DWord to Byte
	NN_S_vpmovsdb,             // Down Convert DWord to Byte
	NN_S_vpmovusdb,            // Down Convert DWord to Byte
	NN_S_vpmovdw,              // Down Convert DWord to Word
	NN_S_vpmovsdw,             // Down Convert DWord to Word
	NN_S_vpmovusdw,            // Down Convert DWord to Word
	NN_S_vpmovwb,              // Down Convert Word to Byte
	NN_S_vpmovswb,             // Down Convert Word to Byte
	NN_S_vpmovuswb,            // Down Convert Word to Byte
	NN_S_vpmullq,              // Multiply Packed Integers and Store Low Result
	NN_S_vpmultishiftqb,       // Select Packed Unaligned Bytes from Quadword Sources
	NN_S_vpord,                // Bitwise Logical Or
	NN_S_vporq,                // Bitwise Logical Or
	NN_S_vprold,               // Bit Rotate Left
	NN_S_vprolvd,              // Bit Rotate Left
	NN_S_vprolq,               // Bit Rotate Left
	NN_S_vprolvq,              // Bit Rotate Left
	NN_S_vprord,               // Bit Rotate Right
	NN_S_vprorvd,              // Bit Rotate Right
	NN_S_vprorq,               // Bit Rotate Right
	NN_S_vprorvq,              // Bit Rotate Right
	NN_S_vpscatterdd,          // Scatter Packed Dword with Signed Dword
	NN_S_vpscatterdq,          // Scatter Packed Qword with Signed Qword Indices
	NN_S_vpscatterqd,          // Scatter Packed Dword with Signed Dword
	NN_S_vpscatterqq,          // Scatter Packed Qword with Signed Qword Indices
	NN_S_vpsraq,               // Bit Shift Arithmetic Right
	NN_S_vpsllvw,              // Variable Bit Shift Left Logical
	NN_S_vpsrlvw,              // Variable Bit Shift Right Logical
	NN_S_vptestnmb,            // Logical NAND and Set
	NN_S_vptestnmw,            // Logical NAND and Set
	NN_S_vptestnmd,            // Logical NAND and Set
	NN_S_vptestnmq,            // Logical NAND and Set
	NN_S_vshuff32x4,           // Shuffle Packed Values at 128-bit Granularity
	NN_S_vshuff64x2,           // Shuffle Packed Values at 128-bit Granularity
	NN_S_vshufi32x4,           // Shuffle Packed Values at 128-bit Granularity
	NN_S_vshufi64x2,           // Shuffle Packed Values at 128-bit Granularity
	NN_S_vpternlogd,           // Bitwise Ternary Logic
	NN_S_vpternlogq,           // Bitwise Ternary Logic
	NN_S_vptestmb,             // Logical AND and Set Mask
	NN_S_vptestmw,             // Logical AND and Set Mask
	NN_S_vptestmd,             // Logical AND and Set Mask
	NN_S_vptestmq,             // Logical AND and Set Mask
	NN_S_vpsravw,              // Variable Bit Shift Right Arithmetic
	NN_S_vpsravq,              // Variable Bit Shift Right Arithmetic
	NN_S_vpxord,               // Exclusive Or
	NN_S_vpxorq,               // Exclusive Or
	NN_S_vrangepd,             // Range Restriction Calculation For Packed Pairs of Float64 Values
	NN_S_vrangeps,             // Range Restriction Calculation For Packed Pairs of Float32 Values
	NN_S_vrangesd,             // Range Restriction Calculation From a pair of Scalar Float64 Values
	NN_S_vrangess,             // Range Restriction Calculation From a Pair of Scalar Float32 Values
	NN_S_vrcp14pd,             // Compute Approximate Reciprocals of Packed Float64 Values
	NN_S_vrcp14sd,             // Compute Approximate Reciprocal of Scalar Float64 Value
	NN_S_vrcp14ps,             // Compute Approximate Reciprocals of Packed Float32 Values
	NN_S_vrcp14ss,             // Compute Approximate Reciprocal of Scalar Float32 Value
	NN_S_vreducepd,            // Perform Reduction Transformation on Packed Float64 Values
	NN_S_vreducesd,            // Perform a Reduction Transformation on a Scalar Float64 Value
	NN_S_vreduceps,            // Perform Reduction Transformation on Packed Float32 Values
	NN_S_vreducess,            // Perform a Reduction Transformation on a Scalar Float32 Value
	NN_S_vrndscalepd,          // Round Packed Float64 Values To Include A Given Number Of Fraction Bits
	NN_S_vrndscalesd,          // Round Scalar Float64 Value To Include A Given Number Of Fraction Bits
	NN_S_vrndscaleps,          // Round Packed Float32 Values To Include A Given Number Of Fraction Bits
	NN_S_vrndscaless,          // Round Scalar Float32 Value To Include A Given Number Of Fraction Bits
	NN_S_vrsqrt14pd,           // Compute Approximate Reciprocals of Square Roots of Packed Float64 Values
	NN_S_vrsqrt14sd,           // Compute Approximate Reciprocal of Square Root of Scalar Float64 Value
	NN_S_vrsqrt14ps,           // Compute Approximate Reciprocals of Square Roots of Packed Float32 Values
	NN_S_vrsqrt14ss,           // Compute Approximate Reciprocal of Square Root of Scalar Float32 Value
	NN_S_vscalefpd,            // Scale Packed Float64 Values With Float64 Values
	NN_S_vscalefsd,            // Scale Scalar Float64 Values With Float64 Values
	NN_S_vscalefps,            // Scale Packed Float32 Values With Float32 Values
	NN_S_vscalefss,            // Scale Scalar Float32 Value With Float32 Value
	NN_S_vscatterdps,          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	NN_S_vscatterdpd,          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	NN_S_vscatterqps,          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	NN_S_vscatterqpd,          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices

	NN_S_vexp2pd,              // Approximation to the Exponential 2^x of Packed Double-Precision Floating-Point Values with Less Than 2^-23 Relative Error
	NN_S_vexp2ps,              // Approximation to the Exponential 2^x of Packed Single-Precision Floating-Point Values with Less Than 2^-23 Relative Error
	NN_S_vrcp28pd,             // Approximation to the Reciprocal of Packed Double-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	NN_S_vrcp28sd,             // Approximation to the Reciprocal of Scalar Double-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	NN_S_vrcp28ps,             // Approximation to the Reciprocal of Packed Single-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	NN_S_vrcp28ss,             // Approximation to the Reciprocal of Scalar Single-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	NN_S_vrsqrt28pd,           // Approximation to the Reciprocal Square Root of Packed Double-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	NN_S_vrsqrt28sd,           // Approximation to the Reciprocal Square Root of Scalar Double-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	NN_S_vrsqrt28ps,           // Approximation to the Reciprocal Square Root of Packed Single-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	NN_S_vrsqrt28ss,           // Approximation to the Reciprocal Square Root of Scalar Single-Precision Floating-Point Value with Less Than 2^-28 Relative Error

	NN_S_vgatherpf0dps,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	NN_S_vgatherpf0qps,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	NN_S_vgatherpf0dpd,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	NN_S_vgatherpf0qpd,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	NN_S_vgatherpf1dps,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	NN_S_vgatherpf1qps,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	NN_S_vgatherpf1dpd,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	NN_S_vgatherpf1qpd,        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	NN_S_vscatterpf0dps,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	NN_S_vscatterpf0qps,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	NN_S_vscatterpf0dpd,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	NN_S_vscatterpf0qpd,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	NN_S_vscatterpf1dps,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	NN_S_vscatterpf1qps,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	NN_S_vscatterpf1dpd,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	NN_S_vscatterpf1qpd,       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write

	// AVX-512 comparison pseudo-ops

	NN_S_vpcmpltd,             // Compare Packed Integer Values into Mask
	NN_S_vpcmpled,             // Compare Packed Integer Values into Mask
	NN_S_vpcmpneqd,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnltd,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnled,            // Compare Packed Integer Values into Mask

	NN_S_vpcmpequd,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpltud,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpleud,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnequd,           // Compare Packed Integer Values into Mask
	NN_S_vpcmpnltud,           // Compare Packed Integer Values into Mask
	NN_S_vpcmpnleud,           // Compare Packed Integer Values into Mask

	NN_S_vpcmpltq,             // Compare Packed Integer Values into Mask
	NN_S_vpcmpleq,             // Compare Packed Integer Values into Mask
	NN_S_vpcmpneqq,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnltq,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnleq,            // Compare Packed Integer Values into Mask

	NN_S_vpcmpequq,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpltuq,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpleuq,            // Compare Packed Integer Values into Mask
	NN_S_vpcmpnequq,           // Compare Packed Integer Values into Mask
	NN_S_vpcmpnltuq,           // Compare Packed Integer Values into Mask
	NN_S_vpcmpnleuq,           // Compare Packed Integer Values into Mask

	// Opmask instructions

	NN_S_kaddw,                // ADD Two Masks
	NN_S_kaddb,                // ADD Two Masks
	NN_S_kaddq,                // ADD Two Masks
	NN_S_kaddd,                // ADD Two Masks
	NN_S_kandw,                // Bitwise Logical AND Masks
	NN_S_kandb,                // Bitwise Logical AND Masks
	NN_S_kandq,                // Bitwise Logical AND Masks
	NN_S_kandd,                // Bitwise Logical AND Masks
	NN_S_kandnw,               // Bitwise Logical AND NOT Masks
	NN_S_kandnb,               // Bitwise Logical AND NOT Masks
	NN_S_kandnq,               // Bitwise Logical AND NOT Masks
	NN_S_kandnd,               // Bitwise Logical AND NOT Masks
	NN_S_kmovw,                // Move from and to Mask Registers
	NN_S_kmovb,                // Move from and to Mask Registers
	NN_S_kmovq,                // Move from and to Mask Registers
	NN_S_kmovd,                // Move from and to Mask Registers
	NN_S_kunpckbw,             // Unpack for Mask Registers
	NN_S_kunpckwd,             // Unpack for Mask Registers
	NN_S_kunpckdq,             // Unpack for Mask Registers
	NN_S_knotw,                // NOT Mask Register
	NN_S_knotb,                // NOT Mask Register
	NN_S_knotq,                // NOT Mask Register
	NN_S_knotd,                // NOT Mask Register
	NN_S_korw,                 // Bitwise Logical OR Masks
	NN_S_korb,                 // Bitwise Logical OR Masks
	NN_S_korq,                 // Bitwise Logical OR Masks
	NN_S_kord,                 // Bitwise Logical OR Masks
	NN_S_kortestw,             // OR Masks And Set Flags
	NN_S_kortestb,             // OR Masks And Set Flags
	NN_S_kortestq,             // OR Masks And Set Flags
	NN_S_kortestd,             // OR Masks And Set Flags
	NN_S_kshiftlw,             // Shift Left Mask Registers
	NN_S_kshiftlb,             // Shift Left Mask Registers
	NN_S_kshiftlq,             // Shift Left Mask Registers
	NN_S_kshiftld,             // Shift Left Mask Registers
	NN_S_kshiftrw,             // Shift Right Mask Registers
	NN_S_kshiftrb,             // Shift Right Mask Registers
	NN_S_kshiftrq,             // Shift Right Mask Registers
	NN_S_kshiftrd,             // Shift Right Mask Registers
	NN_S_kxnorw,               // Bitwise Logical XNOR Masks
	NN_S_kxnorb,               // Bitwise Logical XNOR Masks
	NN_S_kxnorq,               // Bitwise Logical XNOR Masks
	NN_S_kxnord,               // Bitwise Logical XNOR Masks
	NN_S_ktestw,               // Packed Bit Test Masks and Set Flags
	NN_S_ktestb,               // Packed Bit Test Masks and Set Flags
	NN_S_ktestq,               // Packed Bit Test Masks and Set Flags
	NN_S_ktestd,               // Packed Bit Test Masks and Set Flags
	NN_S_kxorw,                // Bitwise Logical XOR Masks
	NN_S_kxorb,                // Bitwise Logical XOR Masks
	NN_S_kxorq,                // Bitwise Logical XOR Masks
	NN_S_kxord,                // Bitwise Logical XOR Masks

	// SHA Extensions

	NN_S_sha1rnds4,            // Perform Four Rounds of SHA1 Operation
	NN_S_sha1nexte,            // Calculate SHA1 State Variable E after Four Rounds
	NN_S_sha1msg1,             // Perform an Intermediate Calculation for the Next Four SHA1 Message Dwords
	NN_S_sha1msg2,             // Perform a Final Calculation for the Next Four SHA1 Message Dwords
	NN_S_sha256rnds2,          // Perform Two Rounds of SHA256 Operation
	NN_S_sha256msg1,           // Perform an Intermediate Calculation for the Next Four SHA256 Message Dwords
	NN_S_sha256msg2,           // Perform a Final Calculation for the Next Four SHA256 Message Dwords

	// Intel Software Guard Extensions
/*
	NN_S_encls,                // Execute an Enclave System Function of Specified Leaf Number
	NN_S_enclu,                // Execute an Enclave User Function of Specified Leaf Number
*/
	// AMD XOP
#if 0
	NN_S_vfrczpd,              // Extract Fraction Packed Double-Precision Floating-Point
	NN_S_vfrczps,              // Extract Fraction Packed Single-Precision Floating-Point
	NN_S_vfrczsd,              // Extract Fraction Scalar Double-Precision Floating-Point
	NN_S_vfrczss,              // Extract Fraction Scalar Single-Precision Floating Point
	NN_S_vpcmov,               // Vector Conditional Moves
	NN_S_vpcomb,               // Compare Vector Signed Bytes
	NN_S_vpcomd,               // Compare Vector Signed Doublewords
	NN_S_vpcomq,               // Compare Vector Signed Quadwords
	NN_S_vpcomub,              // Compare Vector Unsigned Bytes
	NN_S_vpcomud,              // Compare Vector Unsigned Doublewords
	NN_S_vpcomuq,              // Compare Vector Unsigned Quadwords
	NN_S_vpcomuw,              // Compare Vector Unsigned Words
	NN_S_vpcomw,               // Compare Vector Signed Words
	NN_S_vpermil2pd,           // Permute Two-Source Double-Precision Floating-Point Values
	NN_S_vpermil2ps,           // Permute Two-Source Single-Precision Floating-Point Values
	NN_S_vphaddbd,             // Packed Horizontal Add Signed Byte to Signed Doubleword
	NN_S_vphaddbq,             // Packed Horizontal Add Signed Byte to Signed Quadword
	NN_S_vphaddbw,             // Packed Horizontal Add Signed Byte to Signed Word
	NN_S_vphadddq,             // Packed Horizontal Add Signed Doubleword to Signed Quadword
	NN_S_vphaddubd,            // Packed Horizontal Add Unsigned Byte to Doubleword
	NN_S_vphaddubq,            // Packed Horizontal Add Unsigned Byte to Quadword
	NN_S_vphaddubw,            // Packed Horizontal Add Unsigned Byte to Word
	NN_S_vphaddudq,            // Packed Horizontal Add Unsigned Doubleword to Quadword
	NN_S_vphadduwd,            // Packed Horizontal Add Unsigned Word to Doubleword
	NN_S_vphadduwq,            // Packed Horizontal Add Unsigned Word to Quadword
	NN_S_vphaddwd,             // Packed Horizontal Add Signed Word to Signed Doubleword
	NN_S_vphaddwq,             // Packed Horizontal Add Signed Word to Signed Quadword
	NN_S_vphsubbw,             // Packed Horizontal Subtract Signed Byte to Signed Word
	NN_S_vphsubdq,             // Packed Horizontal Subtract Signed Doubleword to Signed Quadword
	NN_S_vphsubwd,             // Packed Horizontal Subtract Signed Word to Signed Doubleword
	NN_S_vpmacsdd,             // Packed Multiply Accumulate Signed Doubleword to Signed Doubleword
	NN_S_vpmacsdqh,            // Packed Multiply Accumulate Signed High Doubleword to Signed Quadword
	NN_S_vpmacsdql,            // Packed Multiply Accumulate Signed Low Doubleword to Signed Quadword
	NN_S_vpmacssdd,            // Packed Multiply Accumulate Signed Doubleword to Signed Doubleword with Saturation
	NN_S_vpmacssdqh,           // Packed Multiply Accumulate Signed High Doubleword to Signed Quadword with Saturation
	NN_S_vpmacssdql,           // Packed Multiply Accumulate Signed Low Doubleword to Signed Quadword with Saturation
	NN_S_vpmacsswd,            // Packed Multiply Accumulate Signed Word to Signed Doubleword with Saturation
	NN_S_vpmacssww,            // Packed Multiply Accumulate Signed Word to Signed Word with Saturation
	NN_S_vpmacswd,             // Packed Multiply Accumulate Signed Word to Signed Doubleword
	NN_S_vpmacsww,             // Packed Multiply Accumulate Signed Word to Signed Word
	NN_S_vpmadcsswd,           // Packed Multiply, Add and Accumulate Signed Word to Signed Doubleword with Saturation
	NN_S_vpmadcswd,            // Packed Multiply Add and Accumulate Signed Word to Signed Doubleword
	NN_S_vpperm,               // Packed Permute Bytes
	NN_S_vprotb,               // Packed Rotate Bytes
	NN_S_vprotd,               // Packed Rotate Doublewords
	NN_S_vprotq,               // Packed Rotate Quadwords
	NN_S_vprotw,               // Packed Rotate Words
	NN_S_vpshab,               // Packed Shift Arithmetic Bytes
	NN_S_vpshad,               // Packed Shift Arithmetic Doublewords
	NN_S_vpshaq,               // Packed Shift Arithmetic Quadwords
	NN_S_vpshaw,               // Packed Shift Arithmetic Words
	NN_S_vpshlb,               // Packed Shift Logical Bytes
	NN_S_vpshld,               // Packed Shift Logical Doublewords
	NN_S_vpshlq,               // Packed Shift Logical Quadwords
	NN_S_vpshlw,               // Packed Shift Logical Words

	// AMD XOP comparison pseudo-ops

	NN_S_vpcomltb,             // Compare Vector Signed Bytes
	NN_S_vpcomleb,             // Compare Vector Signed Bytes
	NN_S_vpcomgtb,             // Compare Vector Signed Bytes
	NN_S_vpcomgeb,             // Compare Vector Signed Bytes
	NN_S_vpcomeqb,             // Compare Vector Signed Bytes
	NN_S_vpcomneqb,            // Compare Vector Signed Bytes
	NN_S_vpcomfalseb,          // Compare Vector Signed Bytes
	NN_S_vpcomtrueb,           // Compare Vector Signed Bytes

	NN_S_vpcomltw,             // Compare Vector Signed Words
	NN_S_vpcomlew,             // Compare Vector Signed Words
	NN_S_vpcomgtw,             // Compare Vector Signed Words
	NN_S_vpcomgew,             // Compare Vector Signed Words
	NN_S_vpcomeqw,             // Compare Vector Signed Words
	NN_S_vpcomneqw,            // Compare Vector Signed Words
	NN_S_vpcomfalsew,          // Compare Vector Signed Words
	NN_S_vpcomtruew,           // Compare Vector Signed Words

	NN_S_vpcomltd,             // Compare Vector Signed Doublewords
	NN_S_vpcomled,             // Compare Vector Signed Doublewords
	NN_S_vpcomgtd,             // Compare Vector Signed Doublewords
	NN_S_vpcomged,             // Compare Vector Signed Doublewords
	NN_S_vpcomeqd,             // Compare Vector Signed Doublewords
	NN_S_vpcomneqd,            // Compare Vector Signed Doublewords
	NN_S_vpcomfalsed,          // Compare Vector Signed Doublewords
	NN_S_vpcomtrued,           // Compare Vector Signed Doublewords

	NN_S_vpcomltq,             // Compare Vector Signed Quadwords
	NN_S_vpcomleq,             // Compare Vector Signed Quadwords
	NN_S_vpcomgtq,             // Compare Vector Signed Quadwords
	NN_S_vpcomgeq,             // Compare Vector Signed Quadwords
	NN_S_vpcomeqq,             // Compare Vector Signed Quadwords
	NN_S_vpcomneqq,            // Compare Vector Signed Quadwords
	NN_S_vpcomfalseq,          // Compare Vector Signed Quadwords
	NN_S_vpcomtrueq,           // Compare Vector Signed Quadwords

	NN_S_vpcomltub,            // Compare Vector Unsigned Bytes
	NN_S_vpcomleub,            // Compare Vector Unsigned Bytes
	NN_S_vpcomgtub,            // Compare Vector Unsigned Bytes
	NN_S_vpcomgeub,            // Compare Vector Unsigned Bytes
	NN_S_vpcomequb,            // Compare Vector Unsigned Bytes
	NN_S_vpcomnequb,           // Compare Vector Unsigned Bytes
	NN_S_vpcomfalseub,         // Compare Vector Unsigned Bytes
	NN_S_vpcomtrueub,          // Compare Vector Unsigned Bytes

	NN_S_vpcomltuw,            // Compare Vector Unsigned Words
	NN_S_vpcomleuw,            // Compare Vector Unsigned Words
	NN_S_vpcomgtuw,            // Compare Vector Unsigned Words
	NN_S_vpcomgeuw,            // Compare Vector Unsigned Words
	NN_S_vpcomequw,            // Compare Vector Unsigned Words
	NN_S_vpcomnequw,           // Compare Vector Unsigned Words
	NN_S_vpcomfalseuw,         // Compare Vector Unsigned Words
	NN_S_vpcomtrueuw,          // Compare Vector Unsigned Words

	NN_S_vpcomltud,            // Compare Vector Unsigned Doublewords
	NN_S_vpcomleud,            // Compare Vector Unsigned Doublewords
	NN_S_vpcomgtud,            // Compare Vector Unsigned Doublewords
	NN_S_vpcomgeud,            // Compare Vector Unsigned Doublewords
	NN_S_vpcomequd,            // Compare Vector Unsigned Doublewords
	NN_S_vpcomnequd,           // Compare Vector Unsigned Doublewords
	NN_S_vpcomfalseud,         // Compare Vector Unsigned Doublewords
	NN_S_vpcomtrueud,          // Compare Vector Unsigned Doublewords

	NN_S_vpcomltuq,            // Compare Vector Unsigned Quadwords
	NN_S_vpcomleuq,            // Compare Vector Unsigned Quadwords
	NN_S_vpcomgtuq,            // Compare Vector Unsigned Quadwords
	NN_S_vpcomgeuq,            // Compare Vector Unsigned Quadwords
	NN_S_vpcomequq,            // Compare Vector Unsigned Quadwords
	NN_S_vpcomnequq,           // Compare Vector Unsigned Quadwords
	NN_S_vpcomfalseuq,         // Compare Vector Unsigned Quadwords
	NN_S_vpcomtrueuq,          // Compare Vector Unsigned Quadwords
#endif
/*
	// AMD Excavator

	NN_S_monitorx,             // Setup Monitor Address
	NN_S_mwaitx,               // Monitor Wait with Timeout

	// AMD Zen

	NN_S_clzero,               // Zero out 64 byte cache

	// Intel Processor Trace

	NN_S_ptwrite,              // Write Data to a Processor Trace Packet
*/
	// new Intel AVX-512 instructions (December 2016)

	NN_S_v4fmaddps,            // Packed Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	NN_S_v4fnmaddps,           // Packed Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	NN_S_v4fmaddss,            // Scalar Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	NN_S_v4fnmaddss,           // Scalar Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	NN_S_vp4dpwssd,            // Dot Product of Signed Words with Dword Accumulation (4-iterations)
	NN_S_vp4dpwssds,           // Dot Product of Signed Words with Dword Accumulation and Saturation (4-iterations)
	NN_S_vpopcntd,             // Return the Count of Number of Bits Set to 1 in DWORD
	NN_S_vpopcntq,             // Return the Count of Number of Bits Set to 1 in QWORD

	// Read Processor ID

	NN_S_rdpid,                // Read Processor ID

	// Invoke VM function
/*
	NN_S_vmfunc,               // Invoke VM function

	// Control-flow Enforcement

	NN_S_incsspd,              // Increment Shadow Stack Pointer (by 4)
	NN_S_incsspq,              // Increment Shadow Stack Pointer (by 8)
	NN_S_rdsspd,               // Read (low 32 bits of) Shadow Stack Pointer
	NN_S_rdsspq,               // Read Shadow Stack Pointer
	NN_S_saveprevssp,          // Save Previous Shadow Stack Pointer
	NN_S_rstorssp,             // Restore saved Shadow Stack Pointer
	NN_S_wrssd,                // Write (4 bytes) to shadow stack
	NN_S_wrssq,                // Write (8 bytes) to shadow stack
	NN_S_wrussd,               // Write (4 bytes) to User Shadow Stack
	NN_S_wrussq,               // Write (8 bytes) to User Shadow Stack
	NN_S_setssbsy,             // Mark Shadow Stack Busy
	NN_S_clrssbsy,             // Clear Shadow Stack Busy Flag
	NN_S_endbr64,              // Terminate an Indirect Branch in 64-bit Mode
	NN_S_endbr32,              // Terminate an Indirect Branch in 32-bit and Compatibility Mode
*/
	// Undefined Instruction
/*
	NN_S_ud0,                 // Undefined Instruction
	NN_S_ud1,                 // Undefined Instruction
*/
	NN_S_last,
	NN_S_BADOP = 0xFFFF

};
using remapx64_t = std::array<cs_x86_op_e, NN_last>;

cs_x86_op_e remap_x86(unsigned op);