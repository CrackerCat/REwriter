#include "saner_instruction_range.hpp"

/*
	By remapping the opcode range to a denser range that omits some of the very rare or useless opcodes
	we can use small bit luts to check properties of instructions quickly
*/
static constexpr remapx64_t create_remap_table() {
	remapx64_t TABLE{};


	TABLE[NN_null] = NN_S_null;            // Unknown Operation
	TABLE[NN_aaa] = NN_S_BADOP;                 // ASCII Adjust after Addition
	TABLE[NN_aad] = NN_S_BADOP;                 // ASCII Adjust AX before Division
	TABLE[NN_aam] = NN_S_BADOP;                 // ASCII Adjust AX after Multiply
	TABLE[NN_aas] = NN_S_BADOP;                 // ASCII Adjust AL after Subtraction
	TABLE[NN_adc] = NN_S_adc;                 // Add with Carry
	TABLE[NN_add] = NN_S_add;                 // Add
	TABLE[NN_and] = NN_S_and;                 // Logical AND
	TABLE[NN_arpl] = NN_S_BADOP;                // Adjust RPL Field of Selector
	TABLE[NN_bound] = NN_S_BADOP;               // Check Array Index Against Bounds
	TABLE[NN_bsf] = NN_S_bsf;                 // Bit Scan Forward
	TABLE[NN_bsr] = NN_S_bsr;                 // Bit Scan Reverse
	TABLE[NN_bt] = NN_S_bt;                  // Bit Test
	TABLE[NN_btc] = NN_S_btc;                 // Bit Test and Complement
	TABLE[NN_btr] = NN_S_btr;                 // Bit Test and Reset
	TABLE[NN_bts] = NN_S_bts;                 // Bit Test and Set
	TABLE[NN_call] = NN_S_call;                // Call Procedure
	TABLE[NN_callfi] = NN_S_callfi;              // Indirect Call Far Procedure
	TABLE[NN_callni] = NN_S_callni;              // Indirect Call Near Procedure
	TABLE[NN_cbw] = NN_S_cbw;                 // AL -> AX (with sign)
	TABLE[NN_cwde] = NN_S_cwde;                // AX -> EAX (with sign)
	TABLE[NN_cdqe] = NN_S_cdqe;                // EAX -> RAX (with sign)
	TABLE[NN_clc] = NN_S_clc;                 // Clear Carry Flag
	TABLE[NN_cld] = NN_S_cld;                 // Clear Direction Flag
	TABLE[NN_cli] = NN_S_BADOP;                 // Clear Interrupt Flag
	TABLE[NN_clts] = NN_S_BADOP;                // Clear Task-Switched Flag in CR0
	TABLE[NN_cmc] = NN_S_cmc;                 // Complement Carry Flag
	TABLE[NN_cmp] = NN_S_cmp;                 // Compare Two Operands
	TABLE[NN_cmps] = NN_S_cmps;                // Compare Strings
	TABLE[NN_cwd] = NN_S_cwd;                 // AX -> DX:AX (with sign)
	TABLE[NN_cdq] = NN_S_cdq;                 // EAX -> EDX:EAX (with sign)
	TABLE[NN_cqo] = NN_S_cqo;                 // RAX -> RDX:RAX (with sign)
	TABLE[NN_daa] = NN_S_daa;                 // Decimal Adjust AL after Addition
	TABLE[NN_das] = NN_S_das;                 // Decimal Adjust AL after Subtraction
	TABLE[NN_dec] = NN_S_dec;                 // Decrement by 1
	TABLE[NN_div] = NN_S_div;                 // Unsigned Divide
	TABLE[NN_enterw] = NN_S_enterw;              // Make Stack Frame for Procedure Parameters
	TABLE[NN_enter] = NN_S_enter;               // Make Stack Frame for Procedure Parameters
	TABLE[NN_enterd] = NN_S_enterd;              // Make Stack Frame for Procedure Parameters
	TABLE[NN_enterq] = NN_S_enterq;              // Make Stack Frame for Procedure Parameters
	TABLE[NN_hlt] = NN_S_BADOP;                 // Halt
	TABLE[NN_idiv] = NN_S_idiv;                // Signed Divide
	TABLE[NN_imul] = NN_S_imul;                // Signed Multiply
	TABLE[NN_in] = NN_S_BADOP;                  // Input from Port
	TABLE[NN_inc] = NN_S_inc;                 // Increment by 1
	TABLE[NN_ins] = NN_S_ins;                 // Input Byte(s) from Port to String
	TABLE[NN_int] = NN_S_int;                 // Call to Interrupt Procedure
	TABLE[NN_into] = NN_S_into;                // Call to Interrupt Procedure if Overflow Flag = 1
	TABLE[NN_int3] = NN_S_int3;                // Trap to Debugger
	TABLE[NN_iretw] = NN_S_BADOP;               // Interrupt Return
	TABLE[NN_iret] = NN_S_BADOP;                // Interrupt Return
	TABLE[NN_iretd] = NN_S_BADOP;               // Interrupt Return (use32)
	TABLE[NN_iretq] = NN_S_BADOP;               // Interrupt Return (use64)
	TABLE[NN_ja] = NN_S_ja;                  // Jump if Above (CF=0 & ZF=0)
	TABLE[NN_jae] = NN_S_jae;                 // Jump if Above or Equal (CF=0)
	TABLE[NN_jb] = NN_S_jb;                  // Jump if Below (CF=1)
	TABLE[NN_jbe] = NN_S_jbe;                 // Jump if Below or Equal (CF=1 | ZF=1)
	TABLE[NN_jc] = NN_S_jc;                  // Jump if Carry (CF=1)
	TABLE[NN_jcxz] = NN_S_jcxz;                // Jump if CX is 0
	TABLE[NN_jecxz] = NN_S_jecxz;               // Jump if ECX is 0
	TABLE[NN_jrcxz] = NN_S_jrcxz;               // Jump if RCX is 0
	TABLE[NN_je] = NN_S_je;                  // Jump if Equal (ZF=1)
	TABLE[NN_jg] = NN_S_jg;                  // Jump if Greater (ZF=0 & SF=OF)
	TABLE[NN_jge] = NN_S_jge;                 // Jump if Greater or Equal (SF=OF)
	TABLE[NN_jl] = NN_S_jl;                  // Jump if Less (SF!=OF)
	TABLE[NN_jle] = NN_S_jle;                 // Jump if Less or Equal (ZF=1 | SF!=OF)
	TABLE[NN_jna] = NN_S_jna;                 // Jump if Not Above (CF=1 | ZF=1)
	TABLE[NN_jnae] = NN_S_jnae;                // Jump if Not Above or Equal (CF=1)
	TABLE[NN_jnb] = NN_S_jnb;                 // Jump if Not Below (CF=0)
	TABLE[NN_jnbe] = NN_S_jnbe;                // Jump if Not Below or Equal (CF=0 & ZF=0)
	TABLE[NN_jnc] = NN_S_jnc;                 // Jump if Not Carry (CF=0)
	TABLE[NN_jne] = NN_S_jne;                 // Jump if Not Equal (ZF=0)
	TABLE[NN_jng] = NN_S_jng;                 // Jump if Not Greater (ZF=1 | SF!=OF)
	TABLE[NN_jnge] = NN_S_jnge;                // Jump if Not Greater or Equal (ZF=1)
	TABLE[NN_jnl] = NN_S_jnl;                 // Jump if Not Less (SF=OF)
	TABLE[NN_jnle] = NN_S_jnle;                // Jump if Not Less or Equal (ZF=0 & SF=OF)
	TABLE[NN_jno] = NN_S_jno;                 // Jump if Not Overflow (OF=0)
	TABLE[NN_jnp] = NN_S_jnp;                 // Jump if Not Parity (PF=0)
	TABLE[NN_jns] = NN_S_jns;                 // Jump if Not Sign (SF=0)
	TABLE[NN_jnz] = NN_S_jnz;                 // Jump if Not Zero (ZF=0)
	TABLE[NN_jo] = NN_S_jo;                  // Jump if Overflow (OF=1)
	TABLE[NN_jp] = NN_S_jp;                  // Jump if Parity (PF=1)
	TABLE[NN_jpe] = NN_S_jpe;                 // Jump if Parity Even (PF=1)
	TABLE[NN_jpo] = NN_S_jpo;                 // Jump if Parity Odd  (PF=0)
	TABLE[NN_js] = NN_S_js;                  // Jump if Sign (SF=1)
	TABLE[NN_jz] = NN_S_jz;                  // Jump if Zero (ZF=1)
	TABLE[NN_jmp] = NN_S_jmp;                 // Jump
	TABLE[NN_jmpfi] = NN_S_jmpfi;               // Indirect Far Jump
	TABLE[NN_jmpni] = NN_S_jmpni;               // Indirect Near Jump
	TABLE[NN_jmpshort] = NN_S_jmpshort;            // Jump Short (not used)
	TABLE[NN_lahf] = NN_S_lahf;                // Load Flags into AH Register
	TABLE[NN_lar] = NN_S_BADOP;                 // Load Access Right Byte
	TABLE[NN_lea] = NN_S_lea;                 // Load Effective Address
	TABLE[NN_leavew] = NN_S_leavew;              // High Level Procedure Exit
	TABLE[NN_leave] = NN_S_leave;               // High Level Procedure Exit
	TABLE[NN_leaved] = NN_S_leaved;              // High Level Procedure Exit
	TABLE[NN_leaveq] = NN_S_leaveq;              // High Level Procedure Exit
	TABLE[NN_lgdt] = NN_S_BADOP;                // Load Global Descriptor Table Register
	TABLE[NN_lidt] = NN_S_BADOP;                // Load Interrupt Descriptor Table Register
	TABLE[NN_lgs] = NN_S_lgs;                 // Load Full Pointer to GS:xx
	TABLE[NN_lss] = NN_S_lss;                 // Load Full Pointer to SS:xx
	TABLE[NN_lds] = NN_S_lds;                 // Load Full Pointer to DS:xx
	TABLE[NN_les] = NN_S_les;                 // Load Full Pointer to ES:xx
	TABLE[NN_lfs] = NN_S_lfs;                 // Load Full Pointer to FS:xx
	TABLE[NN_lldt] = NN_S_BADOP;                // Load Local Descriptor Table Register
	TABLE[NN_lmsw] = NN_S_BADOP;                // Load Machine Status Word
	TABLE[NN_lock] = NN_S_lock;                // Assert LOCK# Signal Prefix
	TABLE[NN_lods] = NN_S_lods;                // Load String
	TABLE[NN_loopw] = NN_S_BADOP;               // Loop while ECX != 0
	TABLE[NN_loop] = NN_S_BADOP;                // Loop while CX != 0
	TABLE[NN_loopd] = NN_S_BADOP;               // Loop while ECX != 0
	TABLE[NN_loopq] = NN_S_BADOP;               // Loop while RCX != 0
	TABLE[NN_loopwe] = NN_S_BADOP;              // Loop while CX != 0 and ZF=1
	TABLE[NN_loope] = NN_S_BADOP;               // Loop while rCX != 0 and ZF=1
	TABLE[NN_loopde] = NN_S_BADOP;              // Loop while ECX != 0 and ZF=1
	TABLE[NN_loopqe] = NN_S_BADOP;              // Loop while RCX != 0 and ZF=1
	TABLE[NN_loopwne] = NN_S_BADOP;             // Loop while CX != 0 and ZF=0
	TABLE[NN_loopne] = NN_S_BADOP;              // Loop while rCX != 0 and ZF=0
	TABLE[NN_loopdne] = NN_S_BADOP;             // Loop while ECX != 0 and ZF=0
	TABLE[NN_loopqne] = NN_S_BADOP;             // Loop while RCX != 0 and ZF=0
	TABLE[NN_lsl] = NN_S_BADOP;                 // Load Segment Limit
	TABLE[NN_ltr] = NN_S_BADOP;                 // Load Task Register
	TABLE[NN_mov] = NN_S_mov;                 // Move Data
	TABLE[NN_movsp] = NN_S_movsp;               // Move to/from Special Registers
	TABLE[NN_movs] = NN_S_movs;                // Move Byte(s) from String to String
	TABLE[NN_movsx] = NN_S_movsx;               // Move with Sign-Extend
	TABLE[NN_movzx] = NN_S_movzx;               // Move with Zero-Extend
	TABLE[NN_mul] = NN_S_mul;                 // Unsigned Multiplication of AL or AX
	TABLE[NN_neg] = NN_S_neg;                 // Two's Complement Negation
	TABLE[NN_nop] = NN_S_nop;                 // No Operation
	TABLE[NN_not] = NN_S_not;                 // One's Complement Negation
	TABLE[NN_or] = NN_S_or;                  // Logical Inclusive OR
	TABLE[NN_out] = NN_S_BADOP;                 // Output to Port
	TABLE[NN_outs] = NN_S_BADOP;                // Output Byte(s) to Port
	TABLE[NN_pop] = NN_S_pop;                 // Pop a word from the Stack
	TABLE[NN_popaw] = NN_S_BADOP;               // Pop all General Registers
	TABLE[NN_popa] = NN_S_BADOP;                // Pop all General Registers
	TABLE[NN_popad] = NN_S_BADOP;               // Pop all General Registers (use32)
	TABLE[NN_popaq] = NN_S_popaq;               // Pop all General Registers (use64)
	TABLE[NN_popfw] = NN_S_BADOP;               // Pop Stack into Flags Register
	TABLE[NN_popf] = NN_S_BADOP;                // Pop Stack into Flags Register
	TABLE[NN_popfd] = NN_S_BADOP;               // Pop Stack into Eflags Register
	TABLE[NN_popfq] = NN_S_popfq;               // Pop Stack into Rflags Register
	TABLE[NN_push] = NN_S_push;                // Push Operand onto the Stack
	TABLE[NN_pushaw] = NN_S_BADOP;              // Push all General Registers
	TABLE[NN_pusha] = NN_S_BADOP;               // Push all General Registers
	TABLE[NN_pushad] = NN_S_BADOP;              // Push all General Registers (use32)
	TABLE[NN_pushaq] = NN_S_pushaq;              // Push all General Registers (use64)
	TABLE[NN_pushfw] = NN_S_BADOP;              // Push Flags Register onto the Stack
	TABLE[NN_pushf] = NN_S_BADOP;               // Push Flags Register onto the Stack
	TABLE[NN_pushfd] = NN_S_BADOP;              // Push Flags Register onto the Stack (use32)
	TABLE[NN_pushfq] = NN_S_pushfq;              // Push Flags Register onto the Stack (use64)
	TABLE[NN_rcl] = NN_S_rcl;                 // Rotate Through Carry Left
	TABLE[NN_rcr] = NN_S_rcr;                 // Rotate Through Carry Right
	TABLE[NN_rol] = NN_S_rol;                 // Rotate Left
	TABLE[NN_ror] = NN_S_ror;                 // Rotate Right
	TABLE[NN_rep] = NN_S_rep;                 // Repeat String Operation
	TABLE[NN_repe] = NN_S_repe;                // Repeat String Operation while ZF=1
	TABLE[NN_repne] = NN_S_repne;               // Repeat String Operation while ZF=0
	TABLE[NN_retn] = NN_S_retn;                // Return Near from Procedure
	TABLE[NN_retf] = NN_S_retf;                // Return Far from Procedure
	TABLE[NN_sahf] = NN_S_sahf;                // Store AH into Flags Register
	TABLE[NN_sal] = NN_S_sal;                 // Shift Arithmetic Left
	TABLE[NN_sar] = NN_S_sar;                 // Shift Arithmetic Right
	TABLE[NN_shl] = NN_S_shl;                 // Shift Logical Left
	TABLE[NN_shr] = NN_S_shr;                 // Shift Logical Right
	TABLE[NN_sbb] = NN_S_sbb;                 // Integer Subtraction with Borrow
	TABLE[NN_scas] = NN_S_scas;                // Compare String
	TABLE[NN_seta] = NN_S_seta;                // Set Byte if Above (CF=0 & ZF=0)
	TABLE[NN_setae] = NN_S_setae;               // Set Byte if Above or Equal (CF=0)
	TABLE[NN_setb] = NN_S_setb;                // Set Byte if Below (CF=1)
	TABLE[NN_setbe] = NN_S_setbe;               // Set Byte if Below or Equal (CF=1 | ZF=1)
	TABLE[NN_setc] = NN_S_setc;                // Set Byte if Carry (CF=1)
	TABLE[NN_sete] = NN_S_sete;                // Set Byte if Equal (ZF=1)
	TABLE[NN_setg] = NN_S_setg;                // Set Byte if Greater (ZF=0 & SF=OF)
	TABLE[NN_setge] = NN_S_setge;               // Set Byte if Greater or Equal (SF=OF)
	TABLE[NN_setl] = NN_S_setl;                // Set Byte if Less (SF!=OF)
	TABLE[NN_setle] = NN_S_setle;               // Set Byte if Less or Equal (ZF=1 | SF!=OF)
	TABLE[NN_setna] = NN_S_setna;               // Set Byte if Not Above (CF=1 | ZF=1)
	TABLE[NN_setnae] = NN_S_setnae;              // Set Byte if Not Above or Equal (CF=1)
	TABLE[NN_setnb] = NN_S_setnb;               // Set Byte if Not Below (CF=0)
	TABLE[NN_setnbe] = NN_S_setnbe;              // Set Byte if Not Below or Equal (CF=0 & ZF=0)
	TABLE[NN_setnc] = NN_S_setnc;               // Set Byte if Not Carry (CF=0)
	TABLE[NN_setne] = NN_S_setne;               // Set Byte if Not Equal (ZF=0)
	TABLE[NN_setng] = NN_S_setng;               // Set Byte if Not Greater (ZF=1 | SF!=OF)
	TABLE[NN_setnge] = NN_S_setnge;              // Set Byte if Not Greater or Equal (ZF=1)
	TABLE[NN_setnl] = NN_S_setnl;               // Set Byte if Not Less (SF=OF)
	TABLE[NN_setnle] = NN_S_setnle;              // Set Byte if Not Less or Equal (ZF=0 & SF=OF)
	TABLE[NN_setno] = NN_S_setno;               // Set Byte if Not Overflow (OF=0)
	TABLE[NN_setnp] = NN_S_setnp;               // Set Byte if Not Parity (PF=0)
	TABLE[NN_setns] = NN_S_setns;               // Set Byte if Not Sign (SF=0)
	TABLE[NN_setnz] = NN_S_setnz;               // Set Byte if Not Zero (ZF=0)
	TABLE[NN_seto] = NN_S_seto;                // Set Byte if Overflow (OF=1)
	TABLE[NN_setp] = NN_S_setp;                // Set Byte if Parity (PF=1)
	TABLE[NN_setpe] = NN_S_setpe;               // Set Byte if Parity Even (PF=1)
	TABLE[NN_setpo] = NN_S_setpo;               // Set Byte if Parity Odd  (PF=0)
	TABLE[NN_sets] = NN_S_sets;                // Set Byte if Sign (SF=1)
	TABLE[NN_setz] = NN_S_setz;                // Set Byte if Zero (ZF=1)
	TABLE[NN_sgdt] = NN_S_BADOP;                // Store Global Descriptor Table Register
	TABLE[NN_sidt] = NN_S_BADOP;                // Store Interrupt Descriptor Table Register
	TABLE[NN_shld] = NN_S_shld;                // Double Precision Shift Left
	TABLE[NN_shrd] = NN_S_shrd;                // Double Precision Shift Right
	TABLE[NN_sldt] = NN_S_BADOP;                // Store Local Descriptor Table Register
	TABLE[NN_smsw] = NN_S_BADOP;                // Store Machine Status Word
	TABLE[NN_stc] = NN_S_stc;                 // Set Carry Flag
	TABLE[NN_std] = NN_S_std;                 // Set Direction Flag
	TABLE[NN_sti] = NN_S_BADOP;                 // Set Interrupt Flag
	TABLE[NN_stos] = NN_S_stos;                // Store String
	TABLE[NN_str] = NN_S_BADOP;                 // Store Task Register
	TABLE[NN_sub] = NN_S_sub;                 // Integer Subtraction
	TABLE[NN_test] = NN_S_test;                // Logical Compare
	TABLE[NN_verr] = NN_S_BADOP;                // Verify a Segment for Reading
	TABLE[NN_verw] = NN_S_BADOP;                // Verify a Segment for Writing
	TABLE[NN_wait] = NN_S_BADOP;                // Wait until BUSY# Pin is Inactive (HIGH)
	TABLE[NN_xchg] = NN_S_xchg;                // Exchange Register/Memory with Register
	TABLE[NN_xlat] = NN_S_BADOP;                // Table Lookup Translation
	TABLE[NN_xor] = NN_S_xor;                 // Logical Exclusive OR

	//
	//      486 instructions
	//

	TABLE[NN_cmpxchg] = NN_S_cmpxchg;             // Compare and Exchange
	TABLE[NN_bswap] = NN_S_bswap;               // Swap bits in EAX
	TABLE[NN_xadd] = NN_S_xadd;                // t<-dest; dest<-src+dest; src<-t
	TABLE[NN_invd] = NN_S_BADOP;                // Invalidate Data Cache
	TABLE[NN_wbinvd] = NN_S_BADOP;              // Invalidate Data Cache (write changes)
	TABLE[NN_invlpg] = NN_S_BADOP;              // Invalidate TLB entry

	//
	//      Pentium instructions
	//

	TABLE[NN_rdmsr] = NN_S_BADOP;               // Read Machine Status Register
	TABLE[NN_wrmsr] = NN_S_BADOP;               // Write Machine Status Register
	TABLE[NN_cpuid] = NN_S_cpuid;               // Get CPU ID
	TABLE[NN_cmpxchg8b] = NN_S_cmpxchg8b;           // Compare and Exchange Eight Bytes
	TABLE[NN_rdtsc] = NN_S_rdtsc;               // Read Time Stamp Counter
	TABLE[NN_rsm] = NN_S_BADOP;                 // Resume from System Management Mode

	//
	//      Pentium Pro instructions
	//

	TABLE[NN_cmova] = NN_S_cmova;               // Move if Above (CF=0 & ZF=0)
	TABLE[NN_cmovb] = NN_S_cmovb;               // Move if Below (CF=1)
	TABLE[NN_cmovbe] = NN_S_cmovbe;              // Move if Below or Equal (CF=1 | ZF=1)
	TABLE[NN_cmovg] = NN_S_cmovg;               // Move if Greater (ZF=0 & SF=OF)
	TABLE[NN_cmovge] = NN_S_cmovge;              // Move if Greater or Equal (SF=OF)
	TABLE[NN_cmovl] = NN_S_cmovl;               // Move if Less (SF!=OF)
	TABLE[NN_cmovle] = NN_S_cmovle;              // Move if Less or Equal (ZF=1 | SF!=OF)
	TABLE[NN_cmovnb] = NN_S_cmovnb;              // Move if Not Below (CF=0)
	TABLE[NN_cmovno] = NN_S_cmovno;              // Move if Not Overflow (OF=0)
	TABLE[NN_cmovnp] = NN_S_cmovnp;              // Move if Not Parity (PF=0)
	TABLE[NN_cmovns] = NN_S_cmovns;              // Move if Not Sign (SF=0)
	TABLE[NN_cmovnz] = NN_S_cmovnz;              // Move if Not Zero (ZF=0)
	TABLE[NN_cmovo] = NN_S_cmovo;               // Move if Overflow (OF=1)
	TABLE[NN_cmovp] = NN_S_cmovp;               // Move if Parity (PF=1)
	TABLE[NN_cmovs] = NN_S_cmovs;               // Move if Sign (SF=1)
	TABLE[NN_cmovz] = NN_S_cmovz;               // Move if Zero (ZF=1)
	TABLE[NN_fcmovb] = NN_S_fcmovb;              // Floating Move if Below
	TABLE[NN_fcmove] = NN_S_fcmove;              // Floating Move if Equal
	TABLE[NN_fcmovbe] = NN_S_fcmovbe;             // Floating Move if Below or Equal
	TABLE[NN_fcmovu] = NN_S_fcmovu;              // Floating Move if Unordered
	TABLE[NN_fcmovnb] = NN_S_fcmovnb;             // Floating Move if Not Below
	TABLE[NN_fcmovne] = NN_S_fcmovne;             // Floating Move if Not Equal
	TABLE[NN_fcmovnbe] = NN_S_fcmovnbe;            // Floating Move if Not Below or Equal
	TABLE[NN_fcmovnu] = NN_S_fcmovnu;             // Floating Move if Not Unordered
	TABLE[NN_fcomi] = NN_S_fcomi;               // FP Compare, result in EFLAGS
	TABLE[NN_fucomi] = NN_S_fucomi;              // FP Unordered Compare, result in EFLAGS
	TABLE[NN_fcomip] = NN_S_fcomip;              // FP Compare, result in EFLAGS, pop stack
	TABLE[NN_fucomip] = NN_S_fucomip;             // FP Unordered Compare, result in EFLAGS, pop stack
	TABLE[NN_rdpmc] = NN_S_rdpmc;               // Read Performance Monitor Counter

	//
	//      FPP instructuions
	//

	TABLE[NN_fld] = NN_S_fld;                 // Load Real
	TABLE[NN_fst] = NN_S_fst;                 // Store Real
	TABLE[NN_fstp] = NN_S_fstp;                // Store Real and Pop
	TABLE[NN_fxch] = NN_S_fxch;                // Exchange Registers
	TABLE[NN_fild] = NN_S_fild;                // Load Integer
	TABLE[NN_fist] = NN_S_fist;                // Store Integer
	TABLE[NN_fistp] = NN_S_fistp;               // Store Integer and Pop
	TABLE[NN_fbld] = NN_S_fbld;                // Load BCD
	TABLE[NN_fbstp] = NN_S_fbstp;               // Store BCD and Pop
	TABLE[NN_fadd] = NN_S_fadd;                // Add Real
	TABLE[NN_faddp] = NN_S_faddp;               // Add Real and Pop
	TABLE[NN_fiadd] = NN_S_fiadd;               // Add Integer
	TABLE[NN_fsub] = NN_S_fsub;                // Subtract Real
	TABLE[NN_fsubp] = NN_S_fsubp;               // Subtract Real and Pop
	TABLE[NN_fisub] = NN_S_fisub;               // Subtract Integer
	TABLE[NN_fsubr] = NN_S_fsubr;               // Subtract Real Reversed
	TABLE[NN_fsubrp] = NN_S_fsubrp;              // Subtract Real Reversed and Pop
	TABLE[NN_fisubr] = NN_S_fisubr;              // Subtract Integer Reversed
	TABLE[NN_fmul] = NN_S_fmul;                // Multiply Real
	TABLE[NN_fmulp] = NN_S_fmulp;               // Multiply Real and Pop
	TABLE[NN_fimul] = NN_S_fimul;               // Multiply Integer
	TABLE[NN_fdiv] = NN_S_fdiv;                // Divide Real
	TABLE[NN_fdivp] = NN_S_fdivp;               // Divide Real and Pop
	TABLE[NN_fidiv] = NN_S_fidiv;               // Divide Integer
	TABLE[NN_fdivr] = NN_S_fdivr;               // Divide Real Reversed
	TABLE[NN_fdivrp] = NN_S_fdivrp;              // Divide Real Reversed and Pop
	TABLE[NN_fidivr] = NN_S_fidivr;              // Divide Integer Reversed
	TABLE[NN_fsqrt] = NN_S_fsqrt;               // Square Root
	TABLE[NN_fscale] = NN_S_fscale;              // Scale:  st(0) <- st(0) * 2^st(1)
	TABLE[NN_fprem] = NN_S_fprem;               // Partial Remainder
	TABLE[NN_frndint] = NN_S_frndint;             // Round to Integer
	TABLE[NN_fxtract] = NN_S_fxtract;             // Extract exponent and significand
	TABLE[NN_fabs] = NN_S_fabs;                // Absolute value
	TABLE[NN_fchs] = NN_S_fchs;                // Change Sign
	TABLE[NN_fcom] = NN_S_fcom;                // Compare Real
	TABLE[NN_fcomp] = NN_S_fcomp;               // Compare Real and Pop
	TABLE[NN_fcompp] = NN_S_fcompp;              // Compare Real and Pop Twice
	TABLE[NN_ficom] = NN_S_ficom;               // Compare Integer
	TABLE[NN_ficomp] = NN_S_ficomp;              // Compare Integer and Pop
	TABLE[NN_ftst] = NN_S_ftst;                // Test
	TABLE[NN_fxam] = NN_S_fxam;                // Examine
	TABLE[NN_fptan] = NN_S_fptan;               // Partial tangent
	TABLE[NN_fpatan] = NN_S_fpatan;              // Partial arctangent
	TABLE[NN_f2xm1] = NN_S_f2xm1;               // 2^x - 1
	TABLE[NN_fyl2x] = NN_S_fyl2x;               // Y * lg2(X)
	TABLE[NN_fyl2xp1] = NN_S_fyl2xp1;             // Y * lg2(X+1)
	TABLE[NN_fldz] = NN_S_fldz;                // Load +0.0
	TABLE[NN_fld1] = NN_S_fld1;                // Load +1.0
	TABLE[NN_fldpi] = NN_S_fldpi;               // Load PI=3.14...
	TABLE[NN_fldl2t] = NN_S_fldl2t;              // Load lg2(10)
	TABLE[NN_fldl2e] = NN_S_fldl2e;              // Load lg2(e)
	TABLE[NN_fldlg2] = NN_S_fldlg2;              // Load lg10(2)
	TABLE[NN_fldln2] = NN_S_fldln2;              // Load ln(2)
	TABLE[NN_finit] = NN_S_finit;               // Initialize Processor
	TABLE[NN_fninit] = NN_S_fninit;              // Initialize Processor (no wait)
	TABLE[NN_fsetpm] = NN_S_fsetpm;              // Set Protected Mode
	TABLE[NN_fldcw] = NN_S_fldcw;               // Load Control Word
	TABLE[NN_fstcw] = NN_S_fstcw;               // Store Control Word
	TABLE[NN_fnstcw] = NN_S_fnstcw;              // Store Control Word (no wait)
	TABLE[NN_fstsw] = NN_S_fstsw;               // Store Status Word
	TABLE[NN_fnstsw] = NN_S_fnstsw;              // Store Status Word (no wait)
	TABLE[NN_fclex] = NN_S_fclex;               // Clear Exceptions
	TABLE[NN_fnclex] = NN_S_fnclex;              // Clear Exceptions (no wait)
	TABLE[NN_fstenv] = NN_S_fstenv;              // Store Environment
	TABLE[NN_fnstenv] = NN_S_fnstenv;             // Store Environment (no wait)
	TABLE[NN_fldenv] = NN_S_fldenv;              // Load Environment
	TABLE[NN_fsave] = NN_S_fsave;               // Save State
	TABLE[NN_fnsave] = NN_S_fnsave;              // Save State (no wait)
	TABLE[NN_frstor] = NN_S_frstor;              // Restore State
	TABLE[NN_fincstp] = NN_S_fincstp;             // Increment Stack Pointer
	TABLE[NN_fdecstp] = NN_S_fdecstp;             // Decrement Stack Pointer
	TABLE[NN_ffree] = NN_S_ffree;               // Free Register
	TABLE[NN_fnop] = NN_S_fnop;                // No Operation
	TABLE[NN_feni] = NN_S_feni;                // (8087 only)
	TABLE[NN_fneni] = NN_S_fneni;               // (no wait) (8087 only)
	TABLE[NN_fdisi] = NN_S_fdisi;               // (8087 only)
	TABLE[NN_fndisi] = NN_S_fndisi;              // (no wait) (8087 only)

	//
	//      80387 instructions
	//

	TABLE[NN_fprem1] = NN_S_fprem1;              // Partial Remainder ( < half )
	TABLE[NN_fsincos] = NN_S_fsincos;             // t<-cos(st); st<-sin(st); push t
	TABLE[NN_fsin] = NN_S_fsin;                // Sine
	TABLE[NN_fcos] = NN_S_fcos;                // Cosine
	TABLE[NN_fucom] = NN_S_fucom;               // Compare Unordered Real
	TABLE[NN_fucomp] = NN_S_fucomp;              // Compare Unordered Real and Pop
	TABLE[NN_fucompp] = NN_S_fucompp;             // Compare Unordered Real and Pop Twice

	//
	//      Instructions added 28.02.96
	//

	TABLE[NN_setalc] = NN_S_BADOP;              // Set AL to Carry Flag
	TABLE[NN_svdc] = NN_S_BADOP;                // Save Register and Descriptor
	TABLE[NN_rsdc] = NN_S_BADOP;                // Restore Register and Descriptor
	TABLE[NN_svldt] = NN_S_BADOP;               // Save LDTR and Descriptor
	TABLE[NN_rsldt] = NN_S_BADOP;               // Restore LDTR and Descriptor
	TABLE[NN_svts] = NN_S_BADOP;                // Save TR and Descriptor
	TABLE[NN_rsts] = NN_S_BADOP;                // Restore TR and Descriptor
	TABLE[NN_icebp] = NN_S_BADOP;               // ICE Break Point
	TABLE[NN_loadall] = NN_S_BADOP;             // Load the entire CPU state from ES:EDI

	//
	//      MMX instructions
	//

	TABLE[NN_emms] = NN_S_emms;                // Empty MMX state
	TABLE[NN_movd] = NN_S_movd;                // Move 32 bits
	TABLE[NN_movq] = NN_S_movq;                // Move 64 bits
	TABLE[NN_packsswb] = NN_S_packsswb;            // Pack with Signed Saturation (Word->Byte)
	TABLE[NN_packssdw] = NN_S_packssdw;            // Pack with Signed Saturation (Dword->Word)
	TABLE[NN_packuswb] = NN_S_packuswb;            // Pack with Unsigned Saturation (Word->Byte)
	TABLE[NN_paddb] = NN_S_paddb;               // Packed Add Byte
	TABLE[NN_paddw] = NN_S_paddw;               // Packed Add Word
	TABLE[NN_paddd] = NN_S_paddd;               // Packed Add Dword
	TABLE[NN_paddsb] = NN_S_paddsb;              // Packed Add with Saturation (Byte)
	TABLE[NN_paddsw] = NN_S_paddsw;              // Packed Add with Saturation (Word)
	TABLE[NN_paddusb] = NN_S_paddusb;             // Packed Add Unsigned with Saturation (Byte)
	TABLE[NN_paddusw] = NN_S_paddusw;             // Packed Add Unsigned with Saturation (Word)
	TABLE[NN_pand] = NN_S_pand;                // Bitwise Logical And
	TABLE[NN_pandn] = NN_S_pandn;               // Bitwise Logical And Not
	TABLE[NN_pcmpeqb] = NN_S_pcmpeqb;             // Packed Compare for Equal (Byte)
	TABLE[NN_pcmpeqw] = NN_S_pcmpeqw;             // Packed Compare for Equal (Word)
	TABLE[NN_pcmpeqd] = NN_S_pcmpeqd;             // Packed Compare for Equal (Dword)
	TABLE[NN_pcmpgtb] = NN_S_pcmpgtb;             // Packed Compare for Greater Than (Byte)
	TABLE[NN_pcmpgtw] = NN_S_pcmpgtw;             // Packed Compare for Greater Than (Word)
	TABLE[NN_pcmpgtd] = NN_S_pcmpgtd;             // Packed Compare for Greater Than (Dword)
	TABLE[NN_pmaddwd] = NN_S_pmaddwd;             // Packed Multiply and Add
	TABLE[NN_pmulhw] = NN_S_pmulhw;              // Packed Multiply High
	TABLE[NN_pmullw] = NN_S_pmullw;              // Packed Multiply Low
	TABLE[NN_por] = NN_S_por;                 // Bitwise Logical Or
	TABLE[NN_psllw] = NN_S_psllw;               // Packed Shift Left Logical (Word)
	TABLE[NN_pslld] = NN_S_pslld;               // Packed Shift Left Logical (Dword)
	TABLE[NN_psllq] = NN_S_psllq;               // Packed Shift Left Logical (Qword)
	TABLE[NN_psraw] = NN_S_psraw;               // Packed Shift Right Arithmetic (Word)
	TABLE[NN_psrad] = NN_S_psrad;               // Packed Shift Right Arithmetic (Dword)
	TABLE[NN_psrlw] = NN_S_psrlw;               // Packed Shift Right Logical (Word)
	TABLE[NN_psrld] = NN_S_psrld;               // Packed Shift Right Logical (Dword)
	TABLE[NN_psrlq] = NN_S_psrlq;               // Packed Shift Right Logical (Qword)
	TABLE[NN_psubb] = NN_S_psubb;               // Packed Subtract Byte
	TABLE[NN_psubw] = NN_S_psubw;               // Packed Subtract Word
	TABLE[NN_psubd] = NN_S_psubd;               // Packed Subtract Dword
	TABLE[NN_psubsb] = NN_S_psubsb;              // Packed Subtract with Saturation (Byte)
	TABLE[NN_psubsw] = NN_S_psubsw;              // Packed Subtract with Saturation (Word)
	TABLE[NN_psubusb] = NN_S_psubusb;             // Packed Subtract Unsigned with Saturation (Byte)
	TABLE[NN_psubusw] = NN_S_psubusw;             // Packed Subtract Unsigned with Saturation (Word)
	TABLE[NN_punpckhbw] = NN_S_punpckhbw;           // Unpack High Packed Data (Byte->Word)
	TABLE[NN_punpckhwd] = NN_S_punpckhwd;           // Unpack High Packed Data (Word->Dword)
	TABLE[NN_punpckhdq] = NN_S_punpckhdq;           // Unpack High Packed Data (Dword->Qword)
	TABLE[NN_punpcklbw] = NN_S_punpcklbw;           // Unpack Low Packed Data (Byte->Word)
	TABLE[NN_punpcklwd] = NN_S_punpcklwd;           // Unpack Low Packed Data (Word->Dword)
	TABLE[NN_punpckldq] = NN_S_punpckldq;           // Unpack Low Packed Data (Dword->Qword)
	TABLE[NN_pxor] = NN_S_pxor;                // Bitwise Logical Exclusive Or

	//
	//      Undocumented Deschutes processor instructions
	//

	TABLE[NN_fxsave] = NN_S_BADOP;              // Fast save FP context
	TABLE[NN_fxrstor] = NN_S_BADOP;             // Fast restore FP context

	//      Pentium II instructions

	TABLE[NN_sysenter] = NN_S_sysenter;            // Fast Transition to System Call Entry Point
	TABLE[NN_sysexit] = NN_S_BADOP;             // Fast Transition from System Call Entry Point

	//      3DNow! instructions

	TABLE[NN_pavgusb] = NN_S_BADOP;             // Packed 8-bit Unsigned Integer Averaging
	TABLE[NN_pfadd] = NN_S_BADOP;               // Packed Floating-Point Addition
	TABLE[NN_pfsub] = NN_S_BADOP;               // Packed Floating-Point Subtraction
	TABLE[NN_pfsubr] = NN_S_BADOP;              // Packed Floating-Point Reverse Subtraction
	TABLE[NN_pfacc] = NN_S_BADOP;               // Packed Floating-Point Accumulate
	TABLE[NN_pfcmpge] = NN_S_BADOP;             // Packed Floating-Point Comparison, Greater or Equal
	TABLE[NN_pfcmpgt] = NN_S_BADOP;             // Packed Floating-Point Comparison, Greater
	TABLE[NN_pfcmpeq] = NN_S_BADOP;             // Packed Floating-Point Comparison, Equal
	TABLE[NN_pfmin] = NN_S_BADOP;               // Packed Floating-Point Minimum
	TABLE[NN_pfmax] = NN_S_BADOP;               // Packed Floating-Point Maximum
	TABLE[NN_pi2fd] = NN_S_BADOP;               // Packed 32-bit Integer to Floating-Point
	TABLE[NN_pf2id] = NN_S_BADOP;               // Packed Floating-Point to 32-bit Integer
	TABLE[NN_pfrcp] = NN_S_BADOP;               // Packed Floating-Point Reciprocal Approximation
	TABLE[NN_pfrsqrt] = NN_S_BADOP;             // Packed Floating-Point Reciprocal Square Root Approximation
	TABLE[NN_pfmul] = NN_S_BADOP;               // Packed Floating-Point Multiplication
	TABLE[NN_pfrcpit1] = NN_S_BADOP;            // Packed Floating-Point Reciprocal First Iteration Step
	TABLE[NN_pfrsqit1] = NN_S_BADOP;            // Packed Floating-Point Reciprocal Square Root First Iteration Step
	TABLE[NN_pfrcpit2] = NN_S_BADOP;            // Packed Floating-Point Reciprocal Second Iteration Step
	TABLE[NN_pmulhrw] = NN_S_BADOP;             // Packed Floating-Point 16-bit Integer Multiply with rounding
	TABLE[NN_femms] = NN_S_BADOP;               // Faster entry/exit of the MMX or floating-point state
	TABLE[NN_prefetch] = NN_S_BADOP;            // Prefetch at least a 32-byte line into L1 data cache
	TABLE[NN_prefetchw] = NN_S_BADOP;           // Prefetch processor cache line into L1 data cache (mark as modified)


	//      Pentium III instructions

	TABLE[NN_addps] = NN_S_addps;               // Packed Single-FP Add
	TABLE[NN_addss] = NN_S_addss;               // Scalar Single-FP Add
	TABLE[NN_andnps] = NN_S_andnps;              // Bitwise Logical And Not for Single-FP
	TABLE[NN_andps] = NN_S_andps;               // Bitwise Logical And for Single-FP
	TABLE[NN_cmpps] = NN_S_cmpps;               // Packed Single-FP Compare
	TABLE[NN_cmpss] = NN_S_cmpss;               // Scalar Single-FP Compare
	TABLE[NN_comiss] = NN_S_comiss;              // Scalar Ordered Single-FP Compare and Set EFLAGS
	TABLE[NN_cvtpi2ps] = NN_S_cvtpi2ps;            // Packed signed INT32 to Packed Single-FP conversion
	TABLE[NN_cvtps2pi] = NN_S_cvtps2pi;            // Packed Single-FP to Packed INT32 conversion
	TABLE[NN_cvtsi2ss] = NN_S_cvtsi2ss;            // Scalar signed INT32 to Single-FP conversion
	TABLE[NN_cvtss2si] = NN_S_cvtss2si;            // Scalar Single-FP to signed INT32 conversion
	TABLE[NN_cvttps2pi] = NN_S_cvttps2pi;           // Packed Single-FP to Packed INT32 conversion (truncate)
	TABLE[NN_cvttss2si] = NN_S_cvttss2si;           // Scalar Single-FP to signed INT32 conversion (truncate)
	TABLE[NN_divps] = NN_S_divps;               // Packed Single-FP Divide
	TABLE[NN_divss] = NN_S_divss;               // Scalar Single-FP Divide
	TABLE[NN_ldmxcsr] = NN_S_ldmxcsr;             // Load Streaming SIMD Extensions Technology Control/Status Register
	TABLE[NN_maxps] = NN_S_maxps;               // Packed Single-FP Maximum
	TABLE[NN_maxss] = NN_S_maxss;               // Scalar Single-FP Maximum
	TABLE[NN_minps] = NN_S_minps;               // Packed Single-FP Minimum
	TABLE[NN_minss] = NN_S_minss;               // Scalar Single-FP Minimum
	TABLE[NN_movaps] = NN_S_movaps;              // Move Aligned Four Packed Single-FP
	TABLE[NN_movhlps] = NN_S_movhlps;             // Move High to Low Packed Single-FP
	TABLE[NN_movhps] = NN_S_movhps;              // Move High Packed Single-FP
	TABLE[NN_movlhps] = NN_S_movlhps;             // Move Low to High Packed Single-FP
	TABLE[NN_movlps] = NN_S_movlps;              // Move Low Packed Single-FP
	TABLE[NN_movmskps] = NN_S_movmskps;            // Move Mask to Register
	TABLE[NN_movss] = NN_S_movss;               // Move Scalar Single-FP
	TABLE[NN_movups] = NN_S_movups;              // Move Unaligned Four Packed Single-FP
	TABLE[NN_mulps] = NN_S_mulps;               // Packed Single-FP Multiply
	TABLE[NN_mulss] = NN_S_mulss;               // Scalar Single-FP Multiply
	TABLE[NN_orps] = NN_S_orps;                // Bitwise Logical OR for Single-FP Data
	TABLE[NN_rcpps] = NN_S_rcpps;               // Packed Single-FP Reciprocal
	TABLE[NN_rcpss] = NN_S_rcpss;               // Scalar Single-FP Reciprocal
	TABLE[NN_rsqrtps] = NN_S_rsqrtps;             // Packed Single-FP Square Root Reciprocal
	TABLE[NN_rsqrtss] = NN_S_rsqrtss;             // Scalar Single-FP Square Root Reciprocal
	TABLE[NN_shufps] = NN_S_shufps;              // Shuffle Single-FP
	TABLE[NN_sqrtps] = NN_S_sqrtps;              // Packed Single-FP Square Root
	TABLE[NN_sqrtss] = NN_S_sqrtss;              // Scalar Single-FP Square Root
	TABLE[NN_stmxcsr] = NN_S_stmxcsr;             // Store Streaming SIMD Extensions Technology Control/Status Register
	TABLE[NN_subps] = NN_S_subps;               // Packed Single-FP Subtract
	TABLE[NN_subss] = NN_S_subss;               // Scalar Single-FP Subtract
	TABLE[NN_ucomiss] = NN_S_ucomiss;             // Scalar Unordered Single-FP Compare and Set EFLAGS
	TABLE[NN_unpckhps] = NN_S_unpckhps;            // Unpack High Packed Single-FP Data
	TABLE[NN_unpcklps] = NN_S_unpcklps;            // Unpack Low Packed Single-FP Data
	TABLE[NN_xorps] = NN_S_xorps;               // Bitwise Logical XOR for Single-FP Data
	TABLE[NN_pavgb] = NN_S_pavgb;               // Packed Average (Byte)
	TABLE[NN_pavgw] = NN_S_pavgw;               // Packed Average (Word)
	TABLE[NN_pextrw] = NN_S_pextrw;              // Extract Word
	TABLE[NN_pinsrw] = NN_S_pinsrw;              // Insert Word
	TABLE[NN_pmaxsw] = NN_S_pmaxsw;              // Packed Signed Integer Word Maximum
	TABLE[NN_pmaxub] = NN_S_pmaxub;              // Packed Unsigned Integer Byte Maximum
	TABLE[NN_pminsw] = NN_S_pminsw;              // Packed Signed Integer Word Minimum
	TABLE[NN_pminub] = NN_S_pminub;              // Packed Unsigned Integer Byte Minimum
	TABLE[NN_pmovmskb] = NN_S_pmovmskb;            // Move Byte Mask to Integer
	TABLE[NN_pmulhuw] = NN_S_pmulhuw;             // Packed Multiply High Unsigned
	TABLE[NN_psadbw] = NN_S_psadbw;              // Packed Sum of Absolute Differences
	TABLE[NN_pshufw] = NN_S_pshufw;              // Packed Shuffle Word
	TABLE[NN_maskmovq] = NN_S_maskmovq;            // Byte Mask write
	TABLE[NN_movntps] = NN_S_movntps;             // Move Aligned Four Packed Single-FP Non Temporal
	TABLE[NN_movntq] = NN_S_movntq;              // Move 64 Bits Non Temporal
	TABLE[NN_prefetcht0] = NN_S_prefetcht0;          // Prefetch to all cache levels
	TABLE[NN_prefetcht1] = NN_S_prefetcht1;          // Prefetch to all cache levels
	TABLE[NN_prefetcht2] = NN_S_prefetcht2;          // Prefetch to L2 cache
	TABLE[NN_prefetchnta] = NN_S_prefetchnta;         // Prefetch to L1 cache
	TABLE[NN_sfence] = NN_S_sfence;              // Store Fence

	// Pentium III Pseudo instructions

	TABLE[NN_cmpeqps] = NN_S_cmpeqps;             // Packed Single-FP Compare EQ
	TABLE[NN_cmpltps] = NN_S_cmpltps;             // Packed Single-FP Compare LT
	TABLE[NN_cmpleps] = NN_S_cmpleps;             // Packed Single-FP Compare LE
	TABLE[NN_cmpunordps] = NN_S_cmpunordps;          // Packed Single-FP Compare UNORD
	TABLE[NN_cmpneqps] = NN_S_cmpneqps;            // Packed Single-FP Compare NOT EQ
	TABLE[NN_cmpnltps] = NN_S_cmpnltps;            // Packed Single-FP Compare NOT LT
	TABLE[NN_cmpnleps] = NN_S_cmpnleps;            // Packed Single-FP Compare NOT LE
	TABLE[NN_cmpordps] = NN_S_cmpordps;            // Packed Single-FP Compare ORDERED
	TABLE[NN_cmpeqss] = NN_S_cmpeqss;             // Scalar Single-FP Compare EQ
	TABLE[NN_cmpltss] = NN_S_cmpltss;             // Scalar Single-FP Compare LT
	TABLE[NN_cmpless] = NN_S_cmpless;             // Scalar Single-FP Compare LE
	TABLE[NN_cmpunordss] = NN_S_cmpunordss;          // Scalar Single-FP Compare UNORD
	TABLE[NN_cmpneqss] = NN_S_cmpneqss;            // Scalar Single-FP Compare NOT EQ
	TABLE[NN_cmpnltss] = NN_S_cmpnltss;            // Scalar Single-FP Compare NOT LT
	TABLE[NN_cmpnless] = NN_S_cmpnless;            // Scalar Single-FP Compare NOT LE
	TABLE[NN_cmpordss] = NN_S_cmpordss;            // Scalar Single-FP Compare ORDERED

	// AMD K7 instructions

	TABLE[NN_pf2iw] = NN_S_BADOP;               // Packed Floating-Point to Integer with Sign Extend
	TABLE[NN_pfnacc] = NN_S_BADOP;              // Packed Floating-Point Negative Accumulate
	TABLE[NN_pfpnacc] = NN_S_BADOP;             // Packed Floating-Point Mixed Positive-Negative Accumulate
	TABLE[NN_pi2fw] = NN_S_BADOP;               // Packed 16-bit Integer to Floating-Point
	TABLE[NN_pswapd] = NN_S_BADOP;              // Packed Swap Double Word

	// Undocumented FP instructions (thanks to norbert.juffa@amd.com)

	TABLE[NN_fstp1] = NN_S_fstp1;               // Alias of Store Real and Pop
	TABLE[NN_fcom2] = NN_S_fcom2;               // Alias of Compare Real
	TABLE[NN_fcomp3] = NN_S_fcomp3;              // Alias of Compare Real and Pop
	TABLE[NN_fxch4] = NN_S_fxch4;               // Alias of Exchange Registers
	TABLE[NN_fcomp5] = NN_S_fcomp5;              // Alias of Compare Real and Pop
	TABLE[NN_ffreep] = NN_S_ffreep;              // Free Register and Pop
	TABLE[NN_fxch7] = NN_S_fxch7;               // Alias of Exchange Registers
	TABLE[NN_fstp8] = NN_S_fstp8;               // Alias of Store Real and Pop
	TABLE[NN_fstp9] = NN_S_fstp9;               // Alias of Store Real and Pop

	// Pentium 4 instructions

	TABLE[NN_addpd] = NN_S_addpd;               // Add Packed Double-Precision Floating-Point Values
	TABLE[NN_addsd] = NN_S_addsd;               // Add Scalar Double-Precision Floating-Point Values
	TABLE[NN_andnpd] = NN_S_andnpd;              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
	TABLE[NN_andpd] = NN_S_andpd;               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
	TABLE[NN_clflush] = NN_S_BADOP;             // Flush Cache Line
	TABLE[NN_cmppd] = NN_S_cmppd;               // Compare Packed Double-Precision Floating-Point Values
	TABLE[NN_cmpsd] = NN_S_cmpsd;               // Compare Scalar Double-Precision Floating-Point Values
	TABLE[NN_comisd] = NN_S_comisd;              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	TABLE[NN_cvtdq2pd] = NN_S_cvtdq2pd;            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
	TABLE[NN_cvtdq2ps] = NN_S_cvtdq2ps;            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_cvtpd2dq] = NN_S_cvtpd2dq;            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvtpd2pi] = NN_S_cvtpd2pi;            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvtpd2ps] = NN_S_cvtpd2ps;            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
	TABLE[NN_cvtpi2pd] = NN_S_cvtpi2pd;            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_cvtps2dq] = NN_S_cvtps2dq;            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvtps2pd] = NN_S_cvtps2pd;            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
	TABLE[NN_cvtsd2si] = NN_S_cvtsd2si;            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
	TABLE[NN_cvtsd2ss] = NN_S_cvtsd2ss;            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
	TABLE[NN_cvtsi2sd] = NN_S_cvtsi2sd;            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
	TABLE[NN_cvtss2sd] = NN_S_cvtss2sd;            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
	TABLE[NN_cvttpd2dq] = NN_S_cvttpd2dq;           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvttpd2pi] = NN_S_cvttpd2pi;           // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvttps2dq] = NN_S_cvttps2dq;           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_cvttsd2si] = NN_S_cvttsd2si;           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
	TABLE[NN_divpd] = NN_S_divpd;               // Divide Packed Double-Precision Floating-Point Values
	TABLE[NN_divsd] = NN_S_divsd;               // Divide Scalar Double-Precision Floating-Point Values
	TABLE[NN_lfence] = NN_S_lfence;              // Load Fence
	TABLE[NN_maskmovdqu] = NN_S_maskmovdqu;          // Store Selected Bytes of Double Quadword
	TABLE[NN_maxpd] = NN_S_maxpd;               // Return Maximum Packed Double-Precision Floating-Point Values
	TABLE[NN_maxsd] = NN_S_maxsd;               // Return Maximum Scalar Double-Precision Floating-Point Value
	TABLE[NN_mfence] = NN_S_mfence;              // Memory Fence
	TABLE[NN_minpd] = NN_S_minpd;               // Return Minimum Packed Double-Precision Floating-Point Values
	TABLE[NN_minsd] = NN_S_minsd;               // Return Minimum Scalar Double-Precision Floating-Point Value
	TABLE[NN_movapd] = NN_S_movapd;              // Move Aligned Packed Double-Precision Floating-Point Values
	TABLE[NN_movdq2q] = NN_S_movdq2q;             // Move Quadword from XMM to MMX Register
	TABLE[NN_movdqa] = NN_S_movdqa;              // Move Aligned Double Quadword
	TABLE[NN_movdqu] = NN_S_movdqu;              // Move Unaligned Double Quadword
	TABLE[NN_movhpd] = NN_S_movhpd;              // Move High Packed Double-Precision Floating-Point Values
	TABLE[NN_movlpd] = NN_S_movlpd;              // Move Low Packed Double-Precision Floating-Point Values
	TABLE[NN_movmskpd] = NN_S_movmskpd;            // Extract Packed Double-Precision Floating-Point Sign Mask
	TABLE[NN_movntdq] = NN_S_movntdq;             // Store Double Quadword Using Non-Temporal Hint
	TABLE[NN_movnti] = NN_S_movnti;              // Store Doubleword Using Non-Temporal Hint
	TABLE[NN_movntpd] = NN_S_movntpd;             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
	TABLE[NN_movq2dq] = NN_S_movq2dq;             // Move Quadword from MMX to XMM Register
	TABLE[NN_movsd] = NN_S_movsd;               // Move Scalar Double-Precision Floating-Point Values
	TABLE[NN_movupd] = NN_S_movupd;              // Move Unaligned Packed Double-Precision Floating-Point Values
	TABLE[NN_mulpd] = NN_S_mulpd;               // Multiply Packed Double-Precision Floating-Point Values
	TABLE[NN_mulsd] = NN_S_mulsd;               // Multiply Scalar Double-Precision Floating-Point Values
	TABLE[NN_orpd] = NN_S_orpd;                // Bitwise Logical OR of Double-Precision Floating-Point Values
	TABLE[NN_paddq] = NN_S_paddq;               // Add Packed Quadword Integers
	TABLE[NN_pause] = NN_S_pause;               // Spin Loop Hint
	TABLE[NN_pmuludq] = NN_S_pmuludq;             // Multiply Packed Unsigned Doubleword Integers
	TABLE[NN_pshufd] = NN_S_pshufd;              // Shuffle Packed Doublewords
	TABLE[NN_pshufhw] = NN_S_pshufhw;             // Shuffle Packed High Words
	TABLE[NN_pshuflw] = NN_S_pshuflw;             // Shuffle Packed Low Words
	TABLE[NN_pslldq] = NN_S_pslldq;              // Shift Double Quadword Left Logical
	TABLE[NN_psrldq] = NN_S_psrldq;              // Shift Double Quadword Right Logical
	TABLE[NN_psubq] = NN_S_psubq;               // Subtract Packed Quadword Integers
	TABLE[NN_punpckhqdq] = NN_S_punpckhqdq;          // Unpack High Data
	TABLE[NN_punpcklqdq] = NN_S_punpcklqdq;          // Unpack Low Data
	TABLE[NN_shufpd] = NN_S_shufpd;              // Shuffle Packed Double-Precision Floating-Point Values
	TABLE[NN_sqrtpd] = NN_S_sqrtpd;              // Compute Square Roots of Packed Double-Precision Floating-Point Values
	TABLE[NN_sqrtsd] = NN_S_sqrtsd;              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
	TABLE[NN_subpd] = NN_S_subpd;               // Subtract Packed Double-Precision Floating-Point Values
	TABLE[NN_subsd] = NN_S_subsd;               // Subtract Scalar Double-Precision Floating-Point Values
	TABLE[NN_ucomisd] = NN_S_ucomisd;             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	TABLE[NN_unpckhpd] = NN_S_unpckhpd;            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
	TABLE[NN_unpcklpd] = NN_S_unpcklpd;            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
	TABLE[NN_xorpd] = NN_S_xorpd;               // Bitwise Logical OR of Double-Precision Floating-Point Values

	// AMD syscall/sysret instructions

	TABLE[NN_syscall] = NN_S_syscall;             // Low latency system call
	TABLE[NN_sysret] = NN_S_BADOP;              // Return from system call

	// AMD64 instructions

	TABLE[NN_swapgs] = NN_S_BADOP;              // Exchange GS base with KernelGSBase MSR

	// New Pentium instructions (SSE3)

	TABLE[NN_movddup] = NN_S_movddup;             // Move One Double-FP and Duplicate
	TABLE[NN_movshdup] = NN_S_movshdup;            // Move Packed Single-FP High and Duplicate
	TABLE[NN_movsldup] = NN_S_movsldup;            // Move Packed Single-FP Low and Duplicate

	// Missing AMD64 instructions

	TABLE[NN_movsxd] = NN_S_movsxd;              // Move with Sign-Extend Doubleword
	TABLE[NN_cmpxchg16b] = NN_S_cmpxchg16b;          // Compare and Exchange 16 Bytes

	// SSE3 instructions

	TABLE[NN_addsubpd] = NN_S_addsubpd;            // Add /Sub packed DP FP numbers
	TABLE[NN_addsubps] = NN_S_addsubps;            // Add /Sub packed SP FP numbers
	TABLE[NN_haddpd] = NN_S_haddpd;              // Add horizontally packed DP FP numbers
	TABLE[NN_haddps] = NN_S_haddps;              // Add horizontally packed SP FP numbers
	TABLE[NN_hsubpd] = NN_S_hsubpd;              // Sub horizontally packed DP FP numbers
	TABLE[NN_hsubps] = NN_S_hsubps;              // Sub horizontally packed SP FP numbers
	TABLE[NN_monitor] = NN_S_BADOP;             // Set up a linear address range to be monitored by hardware
	TABLE[NN_mwait] = NN_S_BADOP;               // Wait until write-back store performed within the range specified by the MONITOR instruction
	TABLE[NN_fisttp] = NN_S_fisttp;              // Store ST in intXX (chop) and pop
	TABLE[NN_lddqu] = NN_S_lddqu;               // Load unaligned integer 128-bit

	// SSSE3 instructions

	TABLE[NN_psignb] = NN_S_psignb;              // Packed SIGN Byte
	TABLE[NN_psignw] = NN_S_psignw;              // Packed SIGN Word
	TABLE[NN_psignd] = NN_S_psignd;              // Packed SIGN Doubleword
	TABLE[NN_pshufb] = NN_S_pshufb;              // Packed Shuffle Bytes
	TABLE[NN_pmulhrsw] = NN_S_pmulhrsw;            // Packed Multiply High with Round and Scale
	TABLE[NN_pmaddubsw] = NN_S_pmaddubsw;           // Multiply and Add Packed Signed and Unsigned Bytes
	TABLE[NN_phsubsw] = NN_S_phsubsw;             // Packed Horizontal Subtract and Saturate
	TABLE[NN_phaddsw] = NN_S_phaddsw;             // Packed Horizontal Add and Saturate
	TABLE[NN_phaddw] = NN_S_phaddw;              // Packed Horizontal Add Word
	TABLE[NN_phaddd] = NN_S_phaddd;              // Packed Horizontal Add Doubleword
	TABLE[NN_phsubw] = NN_S_phsubw;              // Packed Horizontal Subtract Word
	TABLE[NN_phsubd] = NN_S_phsubd;              // Packed Horizontal Subtract Doubleword
	TABLE[NN_palignr] = NN_S_palignr;             // Packed Align Right
	TABLE[NN_pabsb] = NN_S_pabsb;               // Packed Absolute Value Byte
	TABLE[NN_pabsw] = NN_S_pabsw;               // Packed Absolute Value Word
	TABLE[NN_pabsd] = NN_S_pabsd;               // Packed Absolute Value Doubleword

	// VMX instructions

	TABLE[NN_vmcall] = NN_S_BADOP;              // Call to VM Monitor
	TABLE[NN_vmclear] = NN_S_BADOP;             // Clear Virtual Machine Control Structure
	TABLE[NN_vmlaunch] = NN_S_BADOP;            // Launch Virtual Machine
	TABLE[NN_vmresume] = NN_S_BADOP;            // Resume Virtual Machine
	TABLE[NN_vmptrld] = NN_S_BADOP;             // Load Pointer to Virtual Machine Control Structure
	TABLE[NN_vmptrst] = NN_S_BADOP;             // Store Pointer to Virtual Machine Control Structure
	TABLE[NN_vmread] = NN_S_BADOP;              // Read Field from Virtual Machine Control Structure
	TABLE[NN_vmwrite] = NN_S_BADOP;             // Write Field from Virtual Machine Control Structure
	TABLE[NN_vmxoff] = NN_S_BADOP;              // Leave VMX Operation
	TABLE[NN_vmxon] = NN_S_BADOP;               // Enter VMX Operation

	// Undefined Instruction

	TABLE[NN_ud2] = NN_S_BADOP;                 // Undefined Instruction

	// Added with x86-64

	TABLE[NN_rdtscp] = NN_S_BADOP;              // Read Time-Stamp Counter and Processor ID

	// Geode LX 3DNow! extensions

	TABLE[NN_pfrcpv] = NN_S_BADOP;              // Reciprocal Approximation for a Pair of 32-bit Floats
	TABLE[NN_pfrsqrtv] = NN_S_BADOP;            // Reciprocal Square Root Approximation for a Pair of 32-bit Floats

	// SSE2 pseudoinstructions

	TABLE[NN_cmpeqpd] = NN_S_cmpeqpd;             // Packed Double-FP Compare EQ
	TABLE[NN_cmpltpd] = NN_S_cmpltpd;             // Packed Double-FP Compare LT
	TABLE[NN_cmplepd] = NN_S_cmplepd;             // Packed Double-FP Compare LE
	TABLE[NN_cmpunordpd] = NN_S_cmpunordpd;          // Packed Double-FP Compare UNORD
	TABLE[NN_cmpneqpd] = NN_S_cmpneqpd;            // Packed Double-FP Compare NOT EQ
	TABLE[NN_cmpnltpd] = NN_S_cmpnltpd;            // Packed Double-FP Compare NOT LT
	TABLE[NN_cmpnlepd] = NN_S_cmpnlepd;            // Packed Double-FP Compare NOT LE
	TABLE[NN_cmpordpd] = NN_S_cmpordpd;            // Packed Double-FP Compare ORDERED
	TABLE[NN_cmpeqsd] = NN_S_cmpeqsd;             // Scalar Double-FP Compare EQ
	TABLE[NN_cmpltsd] = NN_S_cmpltsd;             // Scalar Double-FP Compare LT
	TABLE[NN_cmplesd] = NN_S_cmplesd;             // Scalar Double-FP Compare LE
	TABLE[NN_cmpunordsd] = NN_S_cmpunordsd;          // Scalar Double-FP Compare UNORD
	TABLE[NN_cmpneqsd] = NN_S_cmpneqsd;            // Scalar Double-FP Compare NOT EQ
	TABLE[NN_cmpnltsd] = NN_S_cmpnltsd;            // Scalar Double-FP Compare NOT LT
	TABLE[NN_cmpnlesd] = NN_S_cmpnlesd;            // Scalar Double-FP Compare NOT LE
	TABLE[NN_cmpordsd] = NN_S_cmpordsd;            // Scalar Double-FP Compare ORDERED

	// SSSE4.1 instructions

	TABLE[NN_blendpd] = NN_S_blendpd;              // Blend Packed Double Precision Floating-Point Values
	TABLE[NN_blendps] = NN_S_blendps;              // Blend Packed Single Precision Floating-Point Values
	TABLE[NN_blendvpd] = NN_S_blendvpd;             // Variable Blend Packed Double Precision Floating-Point Values
	TABLE[NN_blendvps] = NN_S_blendvps;             // Variable Blend Packed Single Precision Floating-Point Values
	TABLE[NN_dppd] = NN_S_dppd;                 // Dot Product of Packed Double Precision Floating-Point Values
	TABLE[NN_dpps] = NN_S_dpps;                 // Dot Product of Packed Single Precision Floating-Point Values
	TABLE[NN_extractps] = NN_S_extractps;            // Extract Packed Single Precision Floating-Point Value
	TABLE[NN_insertps] = NN_S_insertps;             // Insert Packed Single Precision Floating-Point Value
	TABLE[NN_movntdqa] = NN_S_movntdqa;             // Load Double Quadword Non-Temporal Aligned Hint
	TABLE[NN_mpsadbw] = NN_S_mpsadbw;              // Compute Multiple Packed Sums of Absolute Difference
	TABLE[NN_packusdw] = NN_S_packusdw;             // Pack with Unsigned Saturation
	TABLE[NN_pblendvb] = NN_S_pblendvb;             // Variable Blend Packed Bytes
	TABLE[NN_pblendw] = NN_S_pblendw;              // Blend Packed Words
	TABLE[NN_pcmpeqq] = NN_S_pcmpeqq;              // Compare Packed Qword Data for Equal
	TABLE[NN_pextrb] = NN_S_pextrb;               // Extract Byte
	TABLE[NN_pextrd] = NN_S_pextrd;               // Extract Dword
	TABLE[NN_pextrq] = NN_S_pextrq;               // Extract Qword
	TABLE[NN_phminposuw] = NN_S_phminposuw;           // Packed Horizontal Word Minimum
	TABLE[NN_pinsrb] = NN_S_pinsrb;               // Insert Byte
	TABLE[NN_pinsrd] = NN_S_pinsrd;               // Insert Dword
	TABLE[NN_pinsrq] = NN_S_pinsrq;               // Insert Qword
	TABLE[NN_pmaxsb] = NN_S_pmaxsb;               // Maximum of Packed Signed Byte Integers
	TABLE[NN_pmaxsd] = NN_S_pmaxsd;               // Maximum of Packed Signed Dword Integers
	TABLE[NN_pmaxud] = NN_S_pmaxud;               // Maximum of Packed Unsigned Dword Integers
	TABLE[NN_pmaxuw] = NN_S_pmaxuw;               // Maximum of Packed Word Integers
	TABLE[NN_pminsb] = NN_S_pminsb;               // Minimum of Packed Signed Byte Integers
	TABLE[NN_pminsd] = NN_S_pminsd;               // Minimum of Packed Signed Dword Integers
	TABLE[NN_pminud] = NN_S_pminud;               // Minimum of Packed Unsigned Dword Integers
	TABLE[NN_pminuw] = NN_S_pminuw;               // Minimum of Packed Word Integers
	TABLE[NN_pmovsxbw] = NN_S_pmovsxbw;             // Packed Move with Sign Extend
	TABLE[NN_pmovsxbd] = NN_S_pmovsxbd;             // Packed Move with Sign Extend
	TABLE[NN_pmovsxbq] = NN_S_pmovsxbq;             // Packed Move with Sign Extend
	TABLE[NN_pmovsxwd] = NN_S_pmovsxwd;             // Packed Move with Sign Extend
	TABLE[NN_pmovsxwq] = NN_S_pmovsxwq;             // Packed Move with Sign Extend
	TABLE[NN_pmovsxdq] = NN_S_pmovsxdq;             // Packed Move with Sign Extend
	TABLE[NN_pmovzxbw] = NN_S_pmovzxbw;             // Packed Move with Zero Extend
	TABLE[NN_pmovzxbd] = NN_S_pmovzxbd;             // Packed Move with Zero Extend
	TABLE[NN_pmovzxbq] = NN_S_pmovzxbq;             // Packed Move with Zero Extend
	TABLE[NN_pmovzxwd] = NN_S_pmovzxwd;             // Packed Move with Zero Extend
	TABLE[NN_pmovzxwq] = NN_S_pmovzxwq;             // Packed Move with Zero Extend
	TABLE[NN_pmovzxdq] = NN_S_pmovzxdq;             // Packed Move with Zero Extend
	TABLE[NN_pmuldq] = NN_S_pmuldq;               // Multiply Packed Signed Dword Integers
	TABLE[NN_pmulld] = NN_S_pmulld;               // Multiply Packed Signed Dword Integers and Store Low Result
	TABLE[NN_ptest] = NN_S_ptest;                // Logical Compare
	TABLE[NN_roundpd] = NN_S_roundpd;              // Round Packed Double Precision Floating-Point Values
	TABLE[NN_roundps] = NN_S_roundps;              // Round Packed Single Precision Floating-Point Values
	TABLE[NN_roundsd] = NN_S_roundsd;              // Round Scalar Double Precision Floating-Point Values
	TABLE[NN_roundss] = NN_S_roundss;              // Round Scalar Single Precision Floating-Point Values

	// SSSE4.2 instructions

	TABLE[NN_crc32] = NN_S_crc32;                // Accumulate CRC32 Value
	TABLE[NN_pcmpestri] = NN_S_pcmpestri;            // Packed Compare Explicit Length Strings, Return Index
	TABLE[NN_pcmpestrm] = NN_S_pcmpestrm;            // Packed Compare Explicit Length Strings, Return Mask
	TABLE[NN_pcmpistri] = NN_S_pcmpistri;            // Packed Compare Implicit Length Strings, Return Index
	TABLE[NN_pcmpistrm] = NN_S_pcmpistrm;            // Packed Compare Implicit Length Strings, Return Mask
	TABLE[NN_pcmpgtq] = NN_S_pcmpgtq;              // Compare Packed Data for Greater Than
	TABLE[NN_popcnt] = NN_S_popcnt;               // Return the Count of Number of Bits Set to 1

	// AMD SSE4a instructions

	TABLE[NN_extrq] = NN_S_BADOP;                // Extract Field From Register
	TABLE[NN_insertq] = NN_S_BADOP;              // Insert Field
	TABLE[NN_movntsd] = NN_S_BADOP;              // Move Non-Temporal Scalar Double-Precision Floating-Point
	TABLE[NN_movntss] = NN_S_BADOP;              // Move Non-Temporal Scalar Single-Precision Floating-Point
	TABLE[NN_lzcnt] = NN_S_lzcnt;                // Leading Zero Count

	// xsave/xrstor instructions

	TABLE[NN_xgetbv] = NN_S_BADOP;               // Get Value of Extended Control Register
	TABLE[NN_xrstor] = NN_S_BADOP;               // Restore Processor Extended States
	TABLE[NN_xsave] = NN_S_BADOP;                // Save Processor Extended States
	TABLE[NN_xsetbv] = NN_S_BADOP;               // Set Value of Extended Control Register

	// Intel Safer Mode Extensions (SMX)

	TABLE[NN_getsec] = NN_S_BADOP;               // Safer Mode Extensions (SMX) Instruction

	// AMD-V Virtualization ISA Extension

	TABLE[NN_clgi] = NN_S_BADOP;                 // Clear Global Interrupt Flag
	TABLE[NN_invlpga] = NN_S_BADOP;              // Invalidate TLB Entry in a Specified ASID
	TABLE[NN_skinit] = NN_S_BADOP;               // Secure Init and Jump with Attestation
	TABLE[NN_stgi] = NN_S_BADOP;                 // Set Global Interrupt Flag
	TABLE[NN_vmexit] = NN_S_BADOP;               // Stop Executing Guest, Begin Executing Host
	TABLE[NN_vmload] = NN_S_BADOP;               // Load State from VMCB
	TABLE[NN_vmmcall] = NN_S_BADOP;              // Call VMM
	TABLE[NN_vmrun] = NN_S_BADOP;                // Run Virtual Machine
	TABLE[NN_vmsave] = NN_S_BADOP;               // Save State to VMCB

	// VMX+ instructions

	TABLE[NN_invept] = NN_S_BADOP;               // Invalidate Translations Derived from EPT
	TABLE[NN_invvpid] = NN_S_BADOP;              // Invalidate Translations Based on VPID

	// Intel Atom instructions

	TABLE[NN_movbe] = NN_S_movbe;                // Move Data After Swapping Bytes

	// Intel AES instructions

	TABLE[NN_aesenc] = NN_S_aesenc;                // Perform One Round of an AES Encryption Flow
	TABLE[NN_aesenclast] = NN_S_aesenclast;            // Perform the Last Round of an AES Encryption Flow
	TABLE[NN_aesdec] = NN_S_aesdec;                // Perform One Round of an AES Decryption Flow
	TABLE[NN_aesdeclast] = NN_S_aesdeclast;            // Perform the Last Round of an AES Decryption Flow
	TABLE[NN_aesimc] = NN_S_aesimc;                // Perform the AES InvMixColumn Transformation
	TABLE[NN_aeskeygenassist] = NN_S_aeskeygenassist;       // AES Round Key Generation Assist

	// Carryless multiplication

	TABLE[NN_pclmulqdq] = NN_S_pclmulqdq;            // Carry-Less Multiplication Quadword

	// Returns modifies by operand size prefixes

	TABLE[NN_retnw] = NN_S_BADOP;               // Return Near from Procedure (use16)
	TABLE[NN_retnd] = NN_S_BADOP;               // Return Near from Procedure (use32)
	TABLE[NN_retnq] = NN_S_retnq;               // Return Near from Procedure (use64)
	TABLE[NN_retfw] = NN_S_BADOP;               // Return Far from Procedure (use16)
	TABLE[NN_retfd] = NN_S_BADOP;               // Return Far from Procedure (use32)
	TABLE[NN_retfq] = NN_S_retfq;               // Return Far from Procedure (use64)

	// RDRAND support

	TABLE[NN_rdrand] = NN_S_rdrand;              // Read Random Number

	// new GPR instructions

	TABLE[NN_adcx] = NN_S_adcx;                 // Unsigned Integer Addition of Two Operands with Carry Flag
	TABLE[NN_adox] = NN_S_adox;                 // Unsigned Integer Addition of Two Operands with Overflow Flag
	TABLE[NN_andn] = NN_S_andn;                 // Logical AND NOT
	TABLE[NN_bextr] = NN_S_bextr;                // Bit Field Extract
	TABLE[NN_blsi] = NN_S_blsi;                 // Extract Lowest Set Isolated Bit
	TABLE[NN_blsmsk] = NN_S_blsmsk;               // Get Mask Up to Lowest Set Bit
	TABLE[NN_blsr] = NN_S_blsr;                 // Reset Lowest Set Bit
	TABLE[NN_bzhi] = NN_S_bzhi;                 // Zero High Bits Starting with Specified Bit Position
	TABLE[NN_clac] = NN_S_clac;                 // Clear AC Flag in EFLAGS Register
	TABLE[NN_mulx] = NN_S_mulx;                 // Unsigned Multiply Without Affecting Flags
	TABLE[NN_pdep] = NN_S_pdep;                 // Parallel Bits Deposit
	TABLE[NN_pext] = NN_S_pext;                 // Parallel Bits Extract
	TABLE[NN_rorx] = NN_S_rorx;                 // Rotate Right Logical Without Affecting Flags
	TABLE[NN_sarx] = NN_S_sarx;                 // Shift Arithmetically Right Without Affecting Flags
	TABLE[NN_shlx] = NN_S_shlx;                 // Shift Logically Left Without Affecting Flags
	TABLE[NN_shrx] = NN_S_shrx;                 // Shift Logically Right Without Affecting Flags
	TABLE[NN_stac] = NN_S_stac;                 // Set AC Flag in EFLAGS Register
	TABLE[NN_tzcnt] = NN_S_tzcnt;                // Count the Number of Trailing Zero Bits
	TABLE[NN_xsaveopt] = NN_S_BADOP;             // Save Processor Extended States Optimized
	TABLE[NN_invpcid] = NN_S_BADOP;              // Invalidate Processor Context ID
	TABLE[NN_rdseed] = NN_S_rdseed;               // Read Random Seed
	TABLE[NN_rdfsbase] = NN_S_rdfsbase;             // Read FS Segment Base
	TABLE[NN_rdgsbase] = NN_S_rdgsbase;             // Read GS Segment Base
	TABLE[NN_wrfsbase] = NN_S_wrfsbase;             // Write FS Segment Base
	TABLE[NN_wrgsbase] = NN_S_wrgsbase;             // Write GS Segment Base

	// new AVX instructions

	TABLE[NN_vaddpd] = NN_S_vaddpd;               // Add Packed Double-Precision Floating-Point Values
	TABLE[NN_vaddps] = NN_S_vaddps;               // Packed Single-FP Add
	TABLE[NN_vaddsd] = NN_S_vaddsd;               // Add Scalar Double-Precision Floating-Point Values
	TABLE[NN_vaddss] = NN_S_vaddss;               // Scalar Single-FP Add
	TABLE[NN_vaddsubpd] = NN_S_vaddsubpd;            // Add /Sub packed DP FP numbers
	TABLE[NN_vaddsubps] = NN_S_vaddsubps;            // Add /Sub packed SP FP numbers
	TABLE[NN_vaesdec] = NN_S_vaesdec;              // Perform One Round of an AES Decryption Flow
	TABLE[NN_vaesdeclast] = NN_S_vaesdeclast;          // Perform the Last Round of an AES Decryption Flow
	TABLE[NN_vaesenc] = NN_S_vaesenc;              // Perform One Round of an AES Encryption Flow
	TABLE[NN_vaesenclast] = NN_S_vaesenclast;          // Perform the Last Round of an AES Encryption Flow
	TABLE[NN_vaesimc] = NN_S_vaesimc;              // Perform the AES InvMixColumn Transformation
	TABLE[NN_vaeskeygenassist] = NN_S_vaeskeygenassist;     // AES Round Key Generation Assist
	TABLE[NN_vandnpd] = NN_S_vandnpd;              // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
	TABLE[NN_vandnps] = NN_S_vandnps;              // Bitwise Logical And Not for Single-FP
	TABLE[NN_vandpd] = NN_S_vandpd;               // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
	TABLE[NN_vandps] = NN_S_vandps;               // Bitwise Logical And for Single-FP
	TABLE[NN_vblendpd] = NN_S_vblendpd;             // Blend Packed Double Precision Floating-Point Values
	TABLE[NN_vblendps] = NN_S_vblendps;             // Blend Packed Single Precision Floating-Point Values
	TABLE[NN_vblendvpd] = NN_S_vblendvpd;            // Variable Blend Packed Double Precision Floating-Point Values
	TABLE[NN_vblendvps] = NN_S_vblendvps;            // Variable Blend Packed Single Precision Floating-Point Values
	TABLE[NN_vbroadcastf128] = NN_S_vbroadcastf128;       // Broadcast 128 Bits of Floating-Point Data
	TABLE[NN_vbroadcasti128] = NN_S_vbroadcasti128;       // Broadcast 128 Bits of Integer Data
	TABLE[NN_vbroadcastsd] = NN_S_vbroadcastsd;         // Broadcast Double-Precision Floating-Point Element
	TABLE[NN_vbroadcastss] = NN_S_vbroadcastss;         // Broadcast Single-Precision Floating-Point Element
	TABLE[NN_vcmppd] = NN_S_vcmppd;               // Compare Packed Double-Precision Floating-Point Values
	TABLE[NN_vcmpps] = NN_S_vcmpps;               // Packed Single-FP Compare
	TABLE[NN_vcmpsd] = NN_S_vcmpsd;               // Compare Scalar Double-Precision Floating-Point Values
	TABLE[NN_vcmpss] = NN_S_vcmpss;               // Scalar Single-FP Compare
	TABLE[NN_vcomisd] = NN_S_vcomisd;              // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	TABLE[NN_vcomiss] = NN_S_vcomiss;              // Scalar Ordered Single-FP Compare and Set EFLAGS
	TABLE[NN_vcvtdq2pd] = NN_S_vcvtdq2pd;            // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
	TABLE[NN_vcvtdq2ps] = NN_S_vcvtdq2ps;            // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_vcvtpd2dq] = NN_S_vcvtpd2dq;            // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_vcvtpd2ps] = NN_S_vcvtpd2ps;            // Convert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
	TABLE[NN_vcvtph2ps] = NN_S_vcvtph2ps;            // Convert 16-bit FP Values to Single-Precision FP Values
	TABLE[NN_vcvtps2dq] = NN_S_vcvtps2dq;            // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_vcvtps2pd] = NN_S_vcvtps2pd;            // Convert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
	TABLE[NN_vcvtps2ph] = NN_S_vcvtps2ph;            // Convert Single-Precision FP value to 16-bit FP value
	TABLE[NN_vcvtsd2si] = NN_S_vcvtsd2si;            // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
	TABLE[NN_vcvtsd2ss] = NN_S_vcvtsd2ss;            // Convert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
	TABLE[NN_vcvtsi2sd] = NN_S_vcvtsi2sd;            // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
	TABLE[NN_vcvtsi2ss] = NN_S_vcvtsi2ss;            // Scalar signed INT32 to Single-FP conversion
	TABLE[NN_vcvtss2sd] = NN_S_vcvtss2sd;            // Convert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
	TABLE[NN_vcvtss2si] = NN_S_vcvtss2si;            // Scalar Single-FP to signed INT32 conversion
	TABLE[NN_vcvttpd2dq] = NN_S_vcvttpd2dq;           // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_vcvttps2dq] = NN_S_vcvttps2dq;           // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
	TABLE[NN_vcvttsd2si] = NN_S_vcvttsd2si;           // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
	TABLE[NN_vcvttss2si] = NN_S_vcvttss2si;           // Scalar Single-FP to signed INT32 conversion (truncate)
	TABLE[NN_vdivpd] = NN_S_vdivpd;               // Divide Packed Double-Precision Floating-Point Values
	TABLE[NN_vdivps] = NN_S_vdivps;               // Packed Single-FP Divide
	TABLE[NN_vdivsd] = NN_S_vdivsd;               // Divide Scalar Double-Precision Floating-Point Values
	TABLE[NN_vdivss] = NN_S_vdivss;               // Scalar Single-FP Divide
	TABLE[NN_vdppd] = NN_S_vdppd;                // Dot Product of Packed Double Precision Floating-Point Values
	TABLE[NN_vdpps] = NN_S_vdpps;                // Dot Product of Packed Single Precision Floating-Point Values
	TABLE[NN_vextractf128] = NN_S_vextractf128;         // Extract Packed Floating-Point Values
	TABLE[NN_vextracti128] = NN_S_vextracti128;         // Extract Packed Integer Values
	TABLE[NN_vextractps] = NN_S_vextractps;           // Extract Packed Floating-Point Values
	TABLE[NN_vfmadd132pd] = NN_S_vfmadd132pd;          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmadd132ps] = NN_S_vfmadd132ps;          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmadd132sd] = NN_S_vfmadd132sd;          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmadd132ss] = NN_S_vfmadd132ss;          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmadd213pd] = NN_S_vfmadd213pd;          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmadd213ps] = NN_S_vfmadd213ps;          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmadd213sd] = NN_S_vfmadd213sd;          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmadd213ss] = NN_S_vfmadd213ss;          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmadd231pd] = NN_S_vfmadd231pd;          // Fused Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmadd231ps] = NN_S_vfmadd231ps;          // Fused Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmadd231sd] = NN_S_vfmadd231sd;          // Fused Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmadd231ss] = NN_S_vfmadd231ss;          // Fused Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmaddsub132pd] = NN_S_vfmaddsub132pd;       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmaddsub132ps] = NN_S_vfmaddsub132ps;       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmaddsub213pd] = NN_S_vfmaddsub213pd;       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmaddsub213ps] = NN_S_vfmaddsub213ps;       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmaddsub231pd] = NN_S_vfmaddsub231pd;       // Fused Multiply-Alternating Add/Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmaddsub231ps] = NN_S_vfmaddsub231ps;       // Fused Multiply-Alternating Add/Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsub132pd] = NN_S_vfmsub132pd;          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsub132ps] = NN_S_vfmsub132ps;          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsub132sd] = NN_S_vfmsub132sd;          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmsub132ss] = NN_S_vfmsub132ss;          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmsub213pd] = NN_S_vfmsub213pd;          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsub213ps] = NN_S_vfmsub213ps;          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsub213sd] = NN_S_vfmsub213sd;          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmsub213ss] = NN_S_vfmsub213ss;          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmsub231pd] = NN_S_vfmsub231pd;          // Fused Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsub231ps] = NN_S_vfmsub231ps;          // Fused Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsub231sd] = NN_S_vfmsub231sd;          // Fused Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfmsub231ss] = NN_S_vfmsub231ss;          // Fused Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfmsubadd132pd] = NN_S_vfmsubadd132pd;       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsubadd132ps] = NN_S_vfmsubadd132ps;       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsubadd213pd] = NN_S_vfmsubadd213pd;       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsubadd213ps] = NN_S_vfmsubadd213ps;       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfmsubadd231pd] = NN_S_vfmsubadd231pd;       // Fused Multiply-Alternating Subtract/Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfmsubadd231ps] = NN_S_vfmsubadd231ps;       // Fused Multiply-Alternating Subtract/Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd132pd] = NN_S_vfnmadd132pd;         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd132ps] = NN_S_vfnmadd132ps;         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd132sd] = NN_S_vfnmadd132sd;         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd132ss] = NN_S_vfnmadd132ss;         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd213pd] = NN_S_vfnmadd213pd;         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd213ps] = NN_S_vfnmadd213ps;         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd213sd] = NN_S_vfnmadd213sd;         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd213ss] = NN_S_vfnmadd213ss;         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd231pd] = NN_S_vfnmadd231pd;         // Fused Negative Multiply-Add of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd231ps] = NN_S_vfnmadd231ps;         // Fused Negative Multiply-Add of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmadd231sd] = NN_S_vfnmadd231sd;         // Fused Negative Multiply-Add of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmadd231ss] = NN_S_vfnmadd231ss;         // Fused Negative Multiply-Add of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub132pd] = NN_S_vfnmsub132pd;         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub132ps] = NN_S_vfnmsub132ps;         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub132sd] = NN_S_vfnmsub132sd;         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub132ss] = NN_S_vfnmsub132ss;         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub213pd] = NN_S_vfnmsub213pd;         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub213ps] = NN_S_vfnmsub213ps;         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub213sd] = NN_S_vfnmsub213sd;         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub213ss] = NN_S_vfnmsub213ss;         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub231pd] = NN_S_vfnmsub231pd;         // Fused Negative Multiply-Subtract of Packed Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub231ps] = NN_S_vfnmsub231ps;         // Fused Negative Multiply-Subtract of Packed Single-Precision Floating-Point Values
	TABLE[NN_vfnmsub231sd] = NN_S_vfnmsub231sd;         // Fused Negative Multiply-Subtract of Scalar Double-Precision Floating-Point Values
	TABLE[NN_vfnmsub231ss] = NN_S_vfnmsub231ss;         // Fused Negative Multiply-Subtract of Scalar Single-Precision Floating-Point Values
	TABLE[NN_vgatherdps] = NN_S_vgatherdps;           // Gather Packed SP FP Values Using Signed Dword Indices
	TABLE[NN_vgatherdpd] = NN_S_vgatherdpd;           // Gather Packed DP FP Values Using Signed Dword Indices
	TABLE[NN_vgatherqps] = NN_S_vgatherqps;           // Gather Packed SP FP Values Using Signed Qword Indices
	TABLE[NN_vgatherqpd] = NN_S_vgatherqpd;           // Gather Packed DP FP Values Using Signed Qword Indices
	TABLE[NN_vhaddpd] = NN_S_vhaddpd;              // Add horizontally packed DP FP numbers
	TABLE[NN_vhaddps] = NN_S_vhaddps;              // Add horizontally packed SP FP numbers
	TABLE[NN_vhsubpd] = NN_S_vhsubpd;              // Sub horizontally packed DP FP numbers
	TABLE[NN_vhsubps] = NN_S_vhsubps;              // Sub horizontally packed SP FP numbers
	TABLE[NN_vinsertf128] = NN_S_vinsertf128;          // Insert Packed Floating-Point Values
	TABLE[NN_vinserti128] = NN_S_vinserti128;          // Insert Packed Integer Values
	TABLE[NN_vinsertps] = NN_S_vinsertps;            // Insert Packed Single Precision Floating-Point Value
	TABLE[NN_vlddqu] = NN_S_vlddqu;               // Load Unaligned Packed Integer Values
	TABLE[NN_vldmxcsr] = NN_S_vldmxcsr;             // Load Streaming SIMD Extensions Technology Control/Status Register
	TABLE[NN_vmaskmovdqu] = NN_S_vmaskmovdqu;          // Store Selected Bytes of Double Quadword with NT Hint
	TABLE[NN_vmaskmovpd] = NN_S_vmaskmovpd;           // Conditionally Load Packed Double-Precision Floating-Point Values
	TABLE[NN_vmaskmovps] = NN_S_vmaskmovps;           // Conditionally Load Packed Single-Precision Floating-Point Values
	TABLE[NN_vmaxpd] = NN_S_vmaxpd;               // Return Maximum Packed Double-Precision Floating-Point Values
	TABLE[NN_vmaxps] = NN_S_vmaxps;               // Packed Single-FP Maximum
	TABLE[NN_vmaxsd] = NN_S_vmaxsd;               // Return Maximum Scalar Double-Precision Floating-Point Value
	TABLE[NN_vmaxss] = NN_S_vmaxss;               // Scalar Single-FP Maximum
	TABLE[NN_vminpd] = NN_S_vminpd;               // Return Minimum Packed Double-Precision Floating-Point Values
	TABLE[NN_vminps] = NN_S_vminps;               // Packed Single-FP Minimum
	TABLE[NN_vminsd] = NN_S_vminsd;               // Return Minimum Scalar Double-Precision Floating-Point Value
	TABLE[NN_vminss] = NN_S_vminss;               // Scalar Single-FP Minimum
	TABLE[NN_vmovapd] = NN_S_vmovapd;              // Move Aligned Packed Double-Precision Floating-Point Values
	TABLE[NN_vmovaps] = NN_S_vmovaps;              // Move Aligned Four Packed Single-FP
	TABLE[NN_vmovd] = NN_S_vmovd;                // Move 32 bits
	TABLE[NN_vmovddup] = NN_S_vmovddup;             // Move One Double-FP and Duplicate
	TABLE[NN_vmovdqa] = NN_S_vmovdqa;              // Move Aligned Double Quadword
	TABLE[NN_vmovdqu] = NN_S_vmovdqu;              // Move Unaligned Double Quadword
	TABLE[NN_vmovhlps] = NN_S_vmovhlps;             // Move High to Low Packed Single-FP
	TABLE[NN_vmovhpd] = NN_S_vmovhpd;              // Move High Packed Double-Precision Floating-Point Values
	TABLE[NN_vmovhps] = NN_S_vmovhps;              // Move High Packed Single-FP
	TABLE[NN_vmovlhps] = NN_S_vmovlhps;             // Move Low to High Packed Single-FP
	TABLE[NN_vmovlpd] = NN_S_vmovlpd;              // Move Low Packed Double-Precision Floating-Point Values
	TABLE[NN_vmovlps] = NN_S_vmovlps;              // Move Low Packed Single-FP
	TABLE[NN_vmovmskpd] = NN_S_vmovmskpd;            // Extract Packed Double-Precision Floating-Point Sign Mask
	TABLE[NN_vmovmskps] = NN_S_vmovmskps;            // Move Mask to Register
	TABLE[NN_vmovntdq] = NN_S_vmovntdq;             // Store Double Quadword Using Non-Temporal Hint
	TABLE[NN_vmovntdqa] = NN_S_vmovntdqa;            // Load Double Quadword Non-Temporal Aligned Hint
	TABLE[NN_vmovntpd] = NN_S_vmovntpd;             // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
	TABLE[NN_vmovntps] = NN_S_vmovntps;             // Move Aligned Four Packed Single-FP Non Temporal
	TABLE[NN_vmovq] = NN_S_vmovq;                // Move 64 bits
	TABLE[NN_vmovsd] = NN_S_vmovsd;               // Move Scalar Double-Precision Floating-Point Values
	TABLE[NN_vmovshdup] = NN_S_vmovshdup;            // Move Packed Single-FP High and Duplicate
	TABLE[NN_vmovsldup] = NN_S_vmovsldup;            // Move Packed Single-FP Low and Duplicate
	TABLE[NN_vmovss] = NN_S_vmovss;               // Move Scalar Single-FP
	TABLE[NN_vmovupd] = NN_S_vmovupd;              // Move Unaligned Packed Double-Precision Floating-Point Values
	TABLE[NN_vmovups] = NN_S_vmovups;              // Move Unaligned Four Packed Single-FP
	TABLE[NN_vmpsadbw] = NN_S_vmpsadbw;             // Compute Multiple Packed Sums of Absolute Difference
	TABLE[NN_vmulpd] = NN_S_vmulpd;               // Multiply Packed Double-Precision Floating-Point Values
	TABLE[NN_vmulps] = NN_S_vmulps;               // Packed Single-FP Multiply
	TABLE[NN_vmulsd] = NN_S_vmulsd;               // Multiply Scalar Double-Precision Floating-Point Values
	TABLE[NN_vmulss] = NN_S_vmulss;               // Scalar Single-FP Multiply
	TABLE[NN_vorpd] = NN_S_vorpd;                // Bitwise Logical OR of Double-Precision Floating-Point Values
	TABLE[NN_vorps] = NN_S_vorps;                // Bitwise Logical OR for Single-FP Data
	TABLE[NN_vpabsb] = NN_S_vpabsb;               // Packed Absolute Value Byte
	TABLE[NN_vpabsd] = NN_S_vpabsd;               // Packed Absolute Value Doubleword
	TABLE[NN_vpabsw] = NN_S_vpabsw;               // Packed Absolute Value Word
	TABLE[NN_vpackssdw] = NN_S_vpackssdw;            // Pack with Signed Saturation (Dword->Word)
	TABLE[NN_vpacksswb] = NN_S_vpacksswb;            // Pack with Signed Saturation (Word->Byte)
	TABLE[NN_vpackusdw] = NN_S_vpackusdw;            // Pack with Unsigned Saturation
	TABLE[NN_vpackuswb] = NN_S_vpackuswb;            // Pack with Unsigned Saturation (Word->Byte)
	TABLE[NN_vpaddb] = NN_S_vpaddb;               // Packed Add Byte
	TABLE[NN_vpaddd] = NN_S_vpaddd;               // Packed Add Dword
	TABLE[NN_vpaddq] = NN_S_vpaddq;               // Add Packed Quadword Integers
	TABLE[NN_vpaddsb] = NN_S_vpaddsb;              // Packed Add with Saturation (Byte)
	TABLE[NN_vpaddsw] = NN_S_vpaddsw;              // Packed Add with Saturation (Word)
	TABLE[NN_vpaddusb] = NN_S_vpaddusb;             // Packed Add Unsigned with Saturation (Byte)
	TABLE[NN_vpaddusw] = NN_S_vpaddusw;             // Packed Add Unsigned with Saturation (Word)
	TABLE[NN_vpaddw] = NN_S_vpaddw;               // Packed Add Word
	TABLE[NN_vpalignr] = NN_S_vpalignr;             // Packed Align Right
	TABLE[NN_vpand] = NN_S_vpand;                // Bitwise Logical And
	TABLE[NN_vpandn] = NN_S_vpandn;               // Bitwise Logical And Not
	TABLE[NN_vpavgb] = NN_S_vpavgb;               // Packed Average (Byte)
	TABLE[NN_vpavgw] = NN_S_vpavgw;               // Packed Average (Word)
	TABLE[NN_vpblendd] = NN_S_vpblendd;             // Blend Packed Dwords
	TABLE[NN_vpblendvb] = NN_S_vpblendvb;            // Variable Blend Packed Bytes
	TABLE[NN_vpblendw] = NN_S_vpblendw;             // Blend Packed Words
	TABLE[NN_vpbroadcastb] = NN_S_vpbroadcastb;         // Broadcast a Byte Integer
	TABLE[NN_vpbroadcastd] = NN_S_vpbroadcastd;         // Broadcast a Dword Integer
	TABLE[NN_vpbroadcastq] = NN_S_vpbroadcastq;         // Broadcast a Qword Integer
	TABLE[NN_vpbroadcastw] = NN_S_vpbroadcastw;         // Broadcast a Word Integer
	TABLE[NN_vpclmulqdq] = NN_S_vpclmulqdq;           // Carry-Less Multiplication Quadword
	TABLE[NN_vpcmpeqb] = NN_S_vpcmpeqb;             // Packed Compare for Equal (Byte)
	TABLE[NN_vpcmpeqd] = NN_S_vpcmpeqd;             // Packed Compare for Equal (Dword)
	TABLE[NN_vpcmpeqq] = NN_S_vpcmpeqq;             // Compare Packed Qword Data for Equal
	TABLE[NN_vpcmpeqw] = NN_S_vpcmpeqw;             // Packed Compare for Equal (Word)
	TABLE[NN_vpcmpestri] = NN_S_vpcmpestri;           // Packed Compare Explicit Length Strings, Return Index
	TABLE[NN_vpcmpestrm] = NN_S_vpcmpestrm;           // Packed Compare Explicit Length Strings, Return Mask
	TABLE[NN_vpcmpgtb] = NN_S_vpcmpgtb;             // Packed Compare for Greater Than (Byte)
	TABLE[NN_vpcmpgtd] = NN_S_vpcmpgtd;             // Packed Compare for Greater Than (Dword)
	TABLE[NN_vpcmpgtq] = NN_S_vpcmpgtq;             // Compare Packed Data for Greater Than
	TABLE[NN_vpcmpgtw] = NN_S_vpcmpgtw;             // Packed Compare for Greater Than (Word)
	TABLE[NN_vpcmpistri] = NN_S_vpcmpistri;           // Packed Compare Implicit Length Strings, Return Index
	TABLE[NN_vpcmpistrm] = NN_S_vpcmpistrm;           // Packed Compare Implicit Length Strings, Return Mask
	TABLE[NN_vperm2f128] = NN_S_vperm2f128;           // Permute Floating-Point Values
	TABLE[NN_vperm2i128] = NN_S_vperm2i128;           // Permute Integer Values
	TABLE[NN_vpermd] = NN_S_vpermd;               // Full Doublewords Element Permutation
	TABLE[NN_vpermilpd] = NN_S_vpermilpd;            // Permute Double-Precision Floating-Point Values
	TABLE[NN_vpermilps] = NN_S_vpermilps;            // Permute Single-Precision Floating-Point Values
	TABLE[NN_vpermpd] = NN_S_vpermpd;              // Permute Double-Precision Floating-Point Elements
	TABLE[NN_vpermps] = NN_S_vpermps;              // Permute Single-Precision Floating-Point Elements
	TABLE[NN_vpermq] = NN_S_vpermq;               // Qwords Element Permutation
	TABLE[NN_vpextrb] = NN_S_vpextrb;              // Extract Byte
	TABLE[NN_vpextrd] = NN_S_vpextrd;              // Extract Dword
	TABLE[NN_vpextrq] = NN_S_vpextrq;              // Extract Qword
	TABLE[NN_vpextrw] = NN_S_vpextrw;              // Extract Word
	TABLE[NN_vpgatherdd] = NN_S_vpgatherdd;           // Gather Packed Dword Values Using Signed Dword Indices
	TABLE[NN_vpgatherdq] = NN_S_vpgatherdq;           // Gather Packed Qword Values Using Signed Dword Indices
	TABLE[NN_vpgatherqd] = NN_S_vpgatherqd;           // Gather Packed Dword Values Using Signed Qword Indices
	TABLE[NN_vpgatherqq] = NN_S_vpgatherqq;           // Gather Packed Qword Values Using Signed Qword Indices
	TABLE[NN_vphaddd] = NN_S_vphaddd;              // Packed Horizontal Add Doubleword
	TABLE[NN_vphaddsw] = NN_S_vphaddsw;             // Packed Horizontal Add and Saturate
	TABLE[NN_vphaddw] = NN_S_vphaddw;              // Packed Horizontal Add Word
	TABLE[NN_vphminposuw] = NN_S_vphminposuw;          // Packed Horizontal Word Minimum
	TABLE[NN_vphsubd] = NN_S_vphsubd;              // Packed Horizontal Subtract Doubleword
	TABLE[NN_vphsubsw] = NN_S_vphsubsw;             // Packed Horizontal Subtract and Saturate
	TABLE[NN_vphsubw] = NN_S_vphsubw;              // Packed Horizontal Subtract Word
	TABLE[NN_vpinsrb] = NN_S_vpinsrb;              // Insert Byte
	TABLE[NN_vpinsrd] = NN_S_vpinsrd;              // Insert Dword
	TABLE[NN_vpinsrq] = NN_S_vpinsrq;              // Insert Qword
	TABLE[NN_vpinsrw] = NN_S_vpinsrw;              // Insert Word
	TABLE[NN_vpmaddubsw] = NN_S_vpmaddubsw;           // Multiply and Add Packed Signed and Unsigned Bytes
	TABLE[NN_vpmaddwd] = NN_S_vpmaddwd;             // Packed Multiply and Add
	TABLE[NN_vpmaskmovd] = NN_S_vpmaskmovd;           // Conditionally Store Dword Values Using Mask
	TABLE[NN_vpmaskmovq] = NN_S_vpmaskmovq;           // Conditionally Store Qword Values Using Mask
	TABLE[NN_vpmaxsb] = NN_S_vpmaxsb;              // Maximum of Packed Signed Byte Integers
	TABLE[NN_vpmaxsd] = NN_S_vpmaxsd;              // Maximum of Packed Signed Dword Integers
	TABLE[NN_vpmaxsw] = NN_S_vpmaxsw;              // Packed Signed Integer Word Maximum
	TABLE[NN_vpmaxub] = NN_S_vpmaxub;              // Packed Unsigned Integer Byte Maximum
	TABLE[NN_vpmaxud] = NN_S_vpmaxud;              // Maximum of Packed Unsigned Dword Integers
	TABLE[NN_vpmaxuw] = NN_S_vpmaxuw;              // Maximum of Packed Word Integers
	TABLE[NN_vpminsb] = NN_S_vpminsb;              // Minimum of Packed Signed Byte Integers
	TABLE[NN_vpminsd] = NN_S_vpminsd;              // Minimum of Packed Signed Dword Integers
	TABLE[NN_vpminsw] = NN_S_vpminsw;              // Packed Signed Integer Word Minimum
	TABLE[NN_vpminub] = NN_S_vpminub;              // Packed Unsigned Integer Byte Minimum
	TABLE[NN_vpminud] = NN_S_vpminud;              // Minimum of Packed Unsigned Dword Integers
	TABLE[NN_vpminuw] = NN_S_vpminuw;              // Minimum of Packed Word Integers
	TABLE[NN_vpmovmskb] = NN_S_vpmovmskb;            // Move Byte Mask to Integer
	TABLE[NN_vpmovsxbd] = NN_S_vpmovsxbd;            // Packed Move with Sign Extend
	TABLE[NN_vpmovsxbq] = NN_S_vpmovsxbq;            // Packed Move with Sign Extend
	TABLE[NN_vpmovsxbw] = NN_S_vpmovsxbw;            // Packed Move with Sign Extend
	TABLE[NN_vpmovsxdq] = NN_S_vpmovsxdq;            // Packed Move with Sign Extend
	TABLE[NN_vpmovsxwd] = NN_S_vpmovsxwd;            // Packed Move with Sign Extend
	TABLE[NN_vpmovsxwq] = NN_S_vpmovsxwq;            // Packed Move with Sign Extend
	TABLE[NN_vpmovzxbd] = NN_S_vpmovzxbd;            // Packed Move with Zero Extend
	TABLE[NN_vpmovzxbq] = NN_S_vpmovzxbq;            // Packed Move with Zero Extend
	TABLE[NN_vpmovzxbw] = NN_S_vpmovzxbw;            // Packed Move with Zero Extend
	TABLE[NN_vpmovzxdq] = NN_S_vpmovzxdq;            // Packed Move with Zero Extend
	TABLE[NN_vpmovzxwd] = NN_S_vpmovzxwd;            // Packed Move with Zero Extend
	TABLE[NN_vpmovzxwq] = NN_S_vpmovzxwq;            // Packed Move with Zero Extend
	TABLE[NN_vpmuldq] = NN_S_vpmuldq;              // Multiply Packed Signed Dword Integers
	TABLE[NN_vpmulhrsw] = NN_S_vpmulhrsw;            // Packed Multiply High with Round and Scale
	TABLE[NN_vpmulhuw] = NN_S_vpmulhuw;             // Packed Multiply High Unsigned
	TABLE[NN_vpmulhw] = NN_S_vpmulhw;              // Packed Multiply High
	TABLE[NN_vpmulld] = NN_S_vpmulld;              // Multiply Packed Signed Dword Integers and Store Low Result
	TABLE[NN_vpmullw] = NN_S_vpmullw;              // Packed Multiply Low
	TABLE[NN_vpmuludq] = NN_S_vpmuludq;             // Multiply Packed Unsigned Doubleword Integers
	TABLE[NN_vpor] = NN_S_vpor;                 // Bitwise Logical Or
	TABLE[NN_vpsadbw] = NN_S_vpsadbw;              // Packed Sum of Absolute Differences
	TABLE[NN_vpshufb] = NN_S_vpshufb;              // Packed Shuffle Bytes
	TABLE[NN_vpshufd] = NN_S_vpshufd;              // Shuffle Packed Doublewords
	TABLE[NN_vpshufhw] = NN_S_vpshufhw;             // Shuffle Packed High Words
	TABLE[NN_vpshuflw] = NN_S_vpshuflw;             // Shuffle Packed Low Words
	TABLE[NN_vpsignb] = NN_S_vpsignb;              // Packed SIGN Byte
	TABLE[NN_vpsignd] = NN_S_vpsignd;              // Packed SIGN Doubleword
	TABLE[NN_vpsignw] = NN_S_vpsignw;              // Packed SIGN Word
	TABLE[NN_vpslld] = NN_S_vpslld;               // Packed Shift Left Logical (Dword)
	TABLE[NN_vpslldq] = NN_S_vpslldq;              // Shift Double Quadword Left Logical
	TABLE[NN_vpsllq] = NN_S_vpsllq;               // Packed Shift Left Logical (Qword)
	TABLE[NN_vpsllvd] = NN_S_vpsllvd;              // Variable Bit Shift Left Logical (Dword)
	TABLE[NN_vpsllvq] = NN_S_vpsllvq;              // Variable Bit Shift Left Logical (Qword)
	TABLE[NN_vpsllw] = NN_S_vpsllw;               // Packed Shift Left Logical (Word)
	TABLE[NN_vpsrad] = NN_S_vpsrad;               // Packed Shift Right Arithmetic (Dword)
	TABLE[NN_vpsravd] = NN_S_vpsravd;              // Variable Bit Shift Right Arithmetic
	TABLE[NN_vpsraw] = NN_S_vpsraw;               // Packed Shift Right Arithmetic (Word)
	TABLE[NN_vpsrld] = NN_S_vpsrld;               // Packed Shift Right Logical (Dword)
	TABLE[NN_vpsrldq] = NN_S_vpsrldq;              // Shift Double Quadword Right Logical (Qword)
	TABLE[NN_vpsrlq] = NN_S_vpsrlq;               // Packed Shift Right Logical (Qword)
	TABLE[NN_vpsrlvd] = NN_S_vpsrlvd;              // Variable Bit Shift Right Logical (Dword)
	TABLE[NN_vpsrlvq] = NN_S_vpsrlvq;              // Variable Bit Shift Right Logical (Qword)
	TABLE[NN_vpsrlw] = NN_S_vpsrlw;               // Packed Shift Right Logical (Word)
	TABLE[NN_vpsubb] = NN_S_vpsubb;               // Packed Subtract Byte
	TABLE[NN_vpsubd] = NN_S_vpsubd;               // Packed Subtract Dword
	TABLE[NN_vpsubq] = NN_S_vpsubq;               // Subtract Packed Quadword Integers
	TABLE[NN_vpsubsb] = NN_S_vpsubsb;              // Packed Subtract with Saturation (Byte)
	TABLE[NN_vpsubsw] = NN_S_vpsubsw;              // Packed Subtract with Saturation (Word)
	TABLE[NN_vpsubusb] = NN_S_vpsubusb;             // Packed Subtract Unsigned with Saturation (Byte)
	TABLE[NN_vpsubusw] = NN_S_vpsubusw;             // Packed Subtract Unsigned with Saturation (Word)
	TABLE[NN_vpsubw] = NN_S_vpsubw;               // Packed Subtract Word
	TABLE[NN_vptest] = NN_S_vptest;               // Logical Compare
	TABLE[NN_vpunpckhbw] = NN_S_vpunpckhbw;           // Unpack High Packed Data (Byte->Word)
	TABLE[NN_vpunpckhdq] = NN_S_vpunpckhdq;           // Unpack High Packed Data (Dword->Qword)
	TABLE[NN_vpunpckhqdq] = NN_S_vpunpckhqdq;          // Unpack High Packed Data (Qword->Xmmword)
	TABLE[NN_vpunpckhwd] = NN_S_vpunpckhwd;           // Unpack High Packed Data (Word->Dword)
	TABLE[NN_vpunpcklbw] = NN_S_vpunpcklbw;           // Unpack Low Packed Data (Byte->Word)
	TABLE[NN_vpunpckldq] = NN_S_vpunpckldq;           // Unpack Low Packed Data (Dword->Qword)
	TABLE[NN_vpunpcklqdq] = NN_S_vpunpcklqdq;          // Unpack Low Packed Data (Qword->Xmmword)
	TABLE[NN_vpunpcklwd] = NN_S_vpunpcklwd;           // Unpack Low Packed Data (Word->Dword)
	TABLE[NN_vpxor] = NN_S_vpxor;                // Bitwise Logical Exclusive Or
	TABLE[NN_vrcpps] = NN_S_vrcpps;               // Packed Single-FP Reciprocal
	TABLE[NN_vrcpss] = NN_S_vrcpss;               // Scalar Single-FP Reciprocal
	TABLE[NN_vroundpd] = NN_S_vroundpd;             // Round Packed Double Precision Floating-Point Values
	TABLE[NN_vroundps] = NN_S_vroundps;             // Round Packed Single Precision Floating-Point Values
	TABLE[NN_vroundsd] = NN_S_vroundsd;             // Round Scalar Double Precision Floating-Point Values
	TABLE[NN_vroundss] = NN_S_vroundss;             // Round Scalar Single Precision Floating-Point Values
	TABLE[NN_vrsqrtps] = NN_S_vrsqrtps;             // Packed Single-FP Square Root Reciprocal
	TABLE[NN_vrsqrtss] = NN_S_vrsqrtss;             // Scalar Single-FP Square Root Reciprocal
	TABLE[NN_vshufpd] = NN_S_vshufpd;              // Shuffle Packed Double-Precision Floating-Point Values
	TABLE[NN_vshufps] = NN_S_vshufps;              // Shuffle Single-FP
	TABLE[NN_vsqrtpd] = NN_S_vsqrtpd;              // Compute Square Roots of Packed Double-Precision Floating-Point Values
	TABLE[NN_vsqrtps] = NN_S_vsqrtps;              // Packed Single-FP Square Root
	TABLE[NN_vsqrtsd] = NN_S_vsqrtsd;              // Compute Square Rootof Scalar Double-Precision Floating-Point Value
	TABLE[NN_vsqrtss] = NN_S_vsqrtss;              // Scalar Single-FP Square Root
	TABLE[NN_vstmxcsr] = NN_S_vstmxcsr;             // Store Streaming SIMD Extensions Technology Control/Status Register
	TABLE[NN_vsubpd] = NN_S_vsubpd;               // Subtract Packed Double-Precision Floating-Point Values
	TABLE[NN_vsubps] = NN_S_vsubps;               // Packed Single-FP Subtract
	TABLE[NN_vsubsd] = NN_S_vsubsd;               // Subtract Scalar Double-Precision Floating-Point Values
	TABLE[NN_vsubss] = NN_S_vsubss;               // Scalar Single-FP Subtract
	TABLE[NN_vtestpd] = NN_S_vtestpd;              // Packed Double-Precision Floating-Point Bit Test
	TABLE[NN_vtestps] = NN_S_vtestps;              // Packed Single-Precision Floating-Point Bit Test
	TABLE[NN_vucomisd] = NN_S_vucomisd;             // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
	TABLE[NN_vucomiss] = NN_S_vucomiss;             // Scalar Unordered Single-FP Compare and Set EFLAGS
	TABLE[NN_vunpckhpd] = NN_S_vunpckhpd;            // Unpack and Interleave High Packed Double-Precision Floating-Point Values
	TABLE[NN_vunpckhps] = NN_S_vunpckhps;            // Unpack High Packed Single-FP Data
	TABLE[NN_vunpcklpd] = NN_S_vunpcklpd;            // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
	TABLE[NN_vunpcklps] = NN_S_vunpcklps;            // Unpack Low Packed Single-FP Data
	TABLE[NN_vxorpd] = NN_S_vxorpd;               // Bitwise Logical OR of Double-Precision Floating-Point Values
	TABLE[NN_vxorps] = NN_S_vxorps;               // Bitwise Logical XOR for Single-FP Data
	TABLE[NN_vzeroall] = NN_S_vzeroall;             // Zero All YMM Registers
	TABLE[NN_vzeroupper] = NN_S_vzeroupper;           // Zero Upper Bits of YMM Registers

	// Transactional Synchronization Extensions

	TABLE[NN_xabort] = NN_S_xabort;               // Transaction Abort
	TABLE[NN_xbegin] = NN_S_xbegin;               // Transaction Begin
	TABLE[NN_xend] = NN_S_xend;                 // Transaction End
	TABLE[NN_xtest] = NN_S_xtest;                // Test If In Transactional Execution

	// Virtual PC synthetic instructions

	TABLE[NN_vmgetinfo] = NN_S_BADOP;            // Virtual PC - Get VM Information
	TABLE[NN_vmsetinfo] = NN_S_BADOP;            // Virtual PC - Set VM Information
	TABLE[NN_vmdxdsbl] = NN_S_BADOP;             // Virtual PC - Disable Direct Execution
	TABLE[NN_vmdxenbl] = NN_S_BADOP;             // Virtual PC - Enable Direct Execution
	TABLE[NN_vmcpuid] = NN_S_BADOP;              // Virtual PC - Virtualized CPU Information
	TABLE[NN_vmhlt] = NN_S_BADOP;                // Virtual PC - Halt
	TABLE[NN_vmsplaf] = NN_S_BADOP;              // Virtual PC - Spin Lock Acquisition Failed
	TABLE[NN_vmpushfd] = NN_S_BADOP;             // Virtual PC - Push virtualized flags register
	TABLE[NN_vmpopfd] = NN_S_BADOP;              // Virtual PC - Pop virtualized flags register
	TABLE[NN_vmcli] = NN_S_BADOP;                // Virtual PC - Clear Interrupt Flag
	TABLE[NN_vmsti] = NN_S_BADOP;                // Virtual PC - Set Interrupt Flag
	TABLE[NN_vmiretd] = NN_S_BADOP;              // Virtual PC - Return From Interrupt
	TABLE[NN_vmsgdt] = NN_S_BADOP;               // Virtual PC - Store Global Descriptor Table
	TABLE[NN_vmsidt] = NN_S_BADOP;               // Virtual PC - Store Interrupt Descriptor Table
	TABLE[NN_vmsldt] = NN_S_BADOP;               // Virtual PC - Store Local Descriptor Table
	TABLE[NN_vmstr] = NN_S_BADOP;                // Virtual PC - Store Task Register
	TABLE[NN_vmsdte] = NN_S_BADOP;               // Virtual PC - Store to Descriptor Table Entry
	TABLE[NN_vpcext] = NN_S_BADOP;               // Virtual PC - ISA extension

	// AMD FMA4

	TABLE[NN_vfmaddsubps] = NN_S_BADOP;          // Multiply with Alternating Add/Subtract of Packed Single-Precision Floating-Point
	TABLE[NN_vfmaddsubpd] = NN_S_BADOP;          // Multiply with Alternating Add/Subtract of Packed Double-Precision Floating-Point
	TABLE[NN_vfmsubaddps] = NN_S_BADOP;          // Multiply with Alternating Subtract/Add of Packed Single-Precision Floating-Point
	TABLE[NN_vfmsubaddpd] = NN_S_BADOP;          // Multiply with Alternating Subtract/Add of Packed Double-Precision Floating-Point
	TABLE[NN_vfmaddps] = NN_S_BADOP;             // Multiply and Add Packed Single-Precision Floating-Point
	TABLE[NN_vfmaddpd] = NN_S_BADOP;             // Multiply and Add Packed Double-Precision Floating-Point
	TABLE[NN_vfmaddss] = NN_S_BADOP;             // Multiply and Add Scalar Single-Precision Floating-Point
	TABLE[NN_vfmaddsd] = NN_S_BADOP;             // Multiply and Add Scalar Double-Precision Floating-Point
	TABLE[NN_vfmsubps] = NN_S_BADOP;             // Multiply and Subtract Packed Single-Precision Floating-Point
	TABLE[NN_vfmsubpd] = NN_S_BADOP;             // Multiply and Subtract Packed Double-Precision Floating-Point
	TABLE[NN_vfmsubss] = NN_S_BADOP;             // Multiply and Subtract Scalar Single-Precision Floating-Point
	TABLE[NN_vfmsubsd] = NN_S_BADOP;             // Multiply and Subtract Scalar Double-Precision Floating-Point
	TABLE[NN_vfnmaddps] = NN_S_BADOP;            // Negative Multiply and Add Packed Single-Precision Floating-Point
	TABLE[NN_vfnmaddpd] = NN_S_BADOP;            // Negative Multiply and Add Packed Double-Precision Floating-Point
	TABLE[NN_vfnmaddss] = NN_S_BADOP;            // Negative Multiply and Add Scalar Single-Precision Floating-Point
	TABLE[NN_vfnmaddsd] = NN_S_BADOP;            // Negative Multiply and Add Double Single-Precision Floating-Point
	TABLE[NN_vfnmsubps] = NN_S_BADOP;            // Negative Multiply and Subtract Packed Single-Precision Floating-Point
	TABLE[NN_vfnmsubpd] = NN_S_BADOP;            // Negative Multiply and Subtract Packed Double-Precision Floating-Point
	TABLE[NN_vfnmsubss] = NN_S_BADOP;            // Negative Multiply and Subtract Scalar Single-Precision Floating-Point
	TABLE[NN_vfnmsubsd] = NN_S_BADOP;            // Negative Multiply and Subtract Double Single-Precision Floating-Point

	// Intel Memory Protection Extensions (MPX)

	TABLE[NN_bndmk] = NN_S_BADOP;                // Make Bounds
	TABLE[NN_bndcl] = NN_S_BADOP;                // Check Lower Bound
	TABLE[NN_bndcu] = NN_S_BADOP;                // Check Upper Bound
	TABLE[NN_bndcn] = NN_S_BADOP;                // Check Upper Bound
	TABLE[NN_bndmov] = NN_S_BADOP;               // Move Bounds
	TABLE[NN_bndldx] = NN_S_BADOP;               // Load Extended Bounds Using Address Translation
	TABLE[NN_bndstx] = NN_S_BADOP;               // Store Extended Bounds Using Address Translation

	// New xstate instructions

	TABLE[NN_xrstors] = NN_S_BADOP;              // Restore Processor Extended States Supervisor
	TABLE[NN_xsavec] = NN_S_BADOP;               // Save Processor Extended States with Compaction
	TABLE[NN_xsaves] = NN_S_BADOP;               // Save Processor Extended States Supervisor

	// PREFETCHWT1 support

	TABLE[NN_prefetchwt1] = NN_S_prefetchwt1;          // Prefetch Vector Data Into Caches with Intent to Write and T1 Hint

	// Memory instructions

	TABLE[NN_clflushopt] = NN_S_BADOP;           // Flush a Cache Line Optimized
	TABLE[NN_clwb] = NN_S_BADOP;                 // Cache Line Write Back
	TABLE[NN_pcommit] = NN_S_BADOP;              // Persistent Commit (deprecated by Intel)

	// Protection Key Rights for User Pages

	TABLE[NN_rdpkru] = NN_S_BADOP;               // Read Protection Key Rights for User Pages
	TABLE[NN_wrpkru] = NN_S_BADOP;               // Write Data to User Page Key Register

	// AVX comparison pseudo-ops

	TABLE[NN_vcmpeqpd] = NN_S_vcmpeqpd;             // Compare Packed Double-Precision Floating-Point Values - Equal (ordered, non-signaling)
	TABLE[NN_vcmpltpd] = NN_S_vcmpltpd;             // Compare Packed Double-Precision Floating-Point Values - Less-than (ordered, signaling)
	TABLE[NN_vcmplepd] = NN_S_vcmplepd;             // Compare Packed Double-Precision Floating-Point Values - Less-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpunordpd] = NN_S_vcmpunordpd;          // Compare Packed Double-Precision Floating-Point Values - Unordered (non-signaling)
	TABLE[NN_vcmpneqpd] = NN_S_vcmpneqpd;            // Compare Packed Double-Precision Floating-Point Values - Not-equal (unordered, non-signaling)
	TABLE[NN_vcmpnltpd] = NN_S_vcmpnltpd;            // Compare Packed Double-Precision Floating-Point Values - Not-less-than (unordered, signaling)
	TABLE[NN_vcmpnlepd] = NN_S_vcmpnlepd;            // Compare Packed Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpordpd] = NN_S_vcmpordpd;            // Compare Packed Double-Precision Floating-Point Values - Ordered (non-signaling)
	TABLE[NN_vcmpeq_uqpd] = NN_S_vcmpeq_uqpd;          // Compare Packed Double-Precision Floating-Point Values - Equal (unordered, non-signaling)
	TABLE[NN_vcmpngepd] = NN_S_vcmpngepd;            // Compare Packed Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpngtpd] = NN_S_vcmpngtpd;            // Compare Packed Double-Precision Floating-Point Values - Not-greater-than (unordered, signaling)
	TABLE[NN_vcmpfalsepd] = NN_S_vcmpfalsepd;          // Compare Packed Double-Precision Floating-Point Values - False (ordered, non-signaling)
	TABLE[NN_vcmpneq_oqpd] = NN_S_vcmpneq_oqpd;         // Compare Packed Double-Precision Floating-Point Values - Not-equal (ordered, non-signaling)
	TABLE[NN_vcmpgepd] = NN_S_vcmpgepd;             // Compare Packed Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpgtpd] = NN_S_vcmpgtpd;             // Compare Packed Double-Precision Floating-Point Values - Greater-than (ordered, signaling)
	TABLE[NN_vcmptruepd] = NN_S_vcmptruepd;           // Compare Packed Double-Precision Floating-Point Values - True (unordered, non-signaling)
	TABLE[NN_vcmpeq_ospd] = NN_S_vcmpeq_ospd;          // Compare Packed Double-Precision Floating-Point Values - Equal (ordered, signaling)
	TABLE[NN_vcmplt_oqpd] = NN_S_vcmplt_oqpd;          // Compare Packed Double-Precision Floating-Point Values - Less-than (ordered, non-signaling)
	TABLE[NN_vcmple_oqpd] = NN_S_vcmple_oqpd;          // Compare Packed Double-Precision Floating-Point Values - Less-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpunord_spd] = NN_S_vcmpunord_spd;        // Compare Packed Double-Precision Floating-Point Values - Unordered (signaling)
	TABLE[NN_vcmpneq_uspd] = NN_S_vcmpneq_uspd;         // Compare Packed Double-Precision Floating-Point Values - Not-equal (unordered, signaling)
	TABLE[NN_vcmpnlt_uqpd] = NN_S_vcmpnlt_uqpd;         // Compare Packed Double-Precision Floating-Point Values - Not-less-than (unordered, non-signaling)
	TABLE[NN_vcmpnle_uqpd] = NN_S_vcmpnle_uqpd;         // Compare Packed Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpord_spd] = NN_S_vcmpord_spd;          // Compare Packed Double-Precision Floating-Point Values - Ordered (signaling)
	TABLE[NN_vcmpeq_uspd] = NN_S_vcmpeq_uspd;          // Compare Packed Double-Precision Floating-Point Values - Equal (unordered, signaling)
	TABLE[NN_vcmpnge_uqpd] = NN_S_vcmpnge_uqpd;         // Compare Packed Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpngt_uqpd] = NN_S_vcmpngt_uqpd;         // Compare Packed Double-Precision Floating-Point Values - Not-greater-than (unordered, non-signaling)
	TABLE[NN_vcmpfalse_ospd] = NN_S_vcmpfalse_ospd;       // Compare Packed Double-Precision Floating-Point Values - False (ordered, signaling)
	TABLE[NN_vcmpneq_ospd] = NN_S_vcmpneq_ospd;         // Compare Packed Double-Precision Floating-Point Values - Not-equal (ordered, signaling)
	TABLE[NN_vcmpge_oqpd] = NN_S_vcmpge_oqpd;          // Compare Packed Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpgt_oqpd] = NN_S_vcmpgt_oqpd;          // Compare Packed Double-Precision Floating-Point Values - Greater-than (ordered, non-signaling)
	TABLE[NN_vcmptrue_uspd] = NN_S_vcmptrue_uspd;        // Compare Packed Double-Precision Floating-Point Values - True (unordered, signaling)

	TABLE[NN_vcmpeqps] = NN_S_vcmpeqps;             // Packed Single-FP Compare - Equal (ordered, non-signaling)
	TABLE[NN_vcmpltps] = NN_S_vcmpltps;             // Packed Single-FP Compare - Less-than (ordered, signaling)
	TABLE[NN_vcmpleps] = NN_S_vcmpleps;             // Packed Single-FP Compare - Less-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpunordps] = NN_S_vcmpunordps;          // Packed Single-FP Compare - Unordered (non-signaling)
	TABLE[NN_vcmpneqps] = NN_S_vcmpneqps;            // Packed Single-FP Compare - Not-equal (unordered, non-signaling)
	TABLE[NN_vcmpnltps] = NN_S_vcmpnltps;            // Packed Single-FP Compare - Not-less-than (unordered, signaling)
	TABLE[NN_vcmpnleps] = NN_S_vcmpnleps;            // Packed Single-FP Compare - Not-less-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpordps] = NN_S_vcmpordps;            // Packed Single-FP Compare - Ordered (non-signaling)
	TABLE[NN_vcmpeq_uqps] = NN_S_vcmpeq_uqps;          // Packed Single-FP Compare - Equal (unordered, non-signaling)
	TABLE[NN_vcmpngeps] = NN_S_vcmpngeps;            // Packed Single-FP Compare - Not-greater-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpngtps] = NN_S_vcmpngtps;            // Packed Single-FP Compare - Not-greater-than (unordered, signaling)
	TABLE[NN_vcmpfalseps] = NN_S_vcmpfalseps;          // Packed Single-FP Compare - False (ordered, non-signaling)
	TABLE[NN_vcmpneq_oqps] = NN_S_vcmpneq_oqps;         // Packed Single-FP Compare - Not-equal (ordered, non-signaling)
	TABLE[NN_vcmpgeps] = NN_S_vcmpgeps;             // Packed Single-FP Compare - Greater-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpgtps] = NN_S_vcmpgtps;             // Packed Single-FP Compare - Greater-than (ordered, signaling)
	TABLE[NN_vcmptrueps] = NN_S_vcmptrueps;           // Packed Single-FP Compare - True (unordered, non-signaling)
	TABLE[NN_vcmpeq_osps] = NN_S_vcmpeq_osps;          // Packed Single-FP Compare - Equal (ordered, signaling)
	TABLE[NN_vcmplt_oqps] = NN_S_vcmplt_oqps;          // Packed Single-FP Compare - Less-than (ordered, non-signaling)
	TABLE[NN_vcmple_oqps] = NN_S_vcmple_oqps;          // Packed Single-FP Compare - Less-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpunord_sps] = NN_S_vcmpunord_sps;        // Packed Single-FP Compare - Unordered (signaling)
	TABLE[NN_vcmpneq_usps] = NN_S_vcmpneq_usps;         // Packed Single-FP Compare - Not-equal (unordered, signaling)
	TABLE[NN_vcmpnlt_uqps] = NN_S_vcmpnlt_uqps;         // Packed Single-FP Compare - Not-less-than (unordered, non-signaling)
	TABLE[NN_vcmpnle_uqps] = NN_S_vcmpnle_uqps;         // Packed Single-FP Compare - Not-less-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpord_sps] = NN_S_vcmpord_sps;          // Packed Single-FP Compare - Ordered (signaling)
	TABLE[NN_vcmpeq_usps] = NN_S_vcmpeq_usps;          // Packed Single-FP Compare - Equal (unordered, signaling)
	TABLE[NN_vcmpnge_uqps] = NN_S_vcmpnge_uqps;         // Packed Single-FP Compare - Not-greater-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpngt_uqps] = NN_S_vcmpngt_uqps;         // Packed Single-FP Compare - Not-greater-than (unordered, non-signaling)
	TABLE[NN_vcmpfalse_osps] = NN_S_vcmpfalse_osps;       // Packed Single-FP Compare - False (ordered, signaling)
	TABLE[NN_vcmpneq_osps] = NN_S_vcmpneq_osps;         // Packed Single-FP Compare - Not-equal (ordered, signaling)
	TABLE[NN_vcmpge_oqps] = NN_S_vcmpge_oqps;          // Packed Single-FP Compare - Greater-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpgt_oqps] = NN_S_vcmpgt_oqps;          // Packed Single-FP Compare - Greater-than (ordered, non-signaling)
	TABLE[NN_vcmptrue_usps] = NN_S_vcmptrue_usps;        // Packed Single-FP Compare - True (unordered, signaling)

	TABLE[NN_vcmpeqsd] = NN_S_vcmpeqsd;             // Compare Scalar Double-Precision Floating-Point Values - Equal (ordered, non-signaling)
	TABLE[NN_vcmpltsd] = NN_S_vcmpltsd;             // Compare Scalar Double-Precision Floating-Point Values - Less-than (ordered, signaling)
	TABLE[NN_vcmplesd] = NN_S_vcmplesd;             // Compare Scalar Double-Precision Floating-Point Values - Less-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpunordsd] = NN_S_vcmpunordsd;          // Compare Scalar Double-Precision Floating-Point Values - Unordered (non-signaling)
	TABLE[NN_vcmpneqsd] = NN_S_vcmpneqsd;            // Compare Scalar Double-Precision Floating-Point Values - Not-equal (unordered, non-signaling)
	TABLE[NN_vcmpnltsd] = NN_S_vcmpnltsd;            // Compare Scalar Double-Precision Floating-Point Values - Not-less-than (unordered, signaling)
	TABLE[NN_vcmpnlesd] = NN_S_vcmpnlesd;            // Compare Scalar Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpordsd] = NN_S_vcmpordsd;            // Compare Scalar Double-Precision Floating-Point Values - Ordered (non-signaling)
	TABLE[NN_vcmpeq_uqsd] = NN_S_vcmpeq_uqsd;          // Compare Scalar Double-Precision Floating-Point Values - Equal (unordered, non-signaling)
	TABLE[NN_vcmpngesd] = NN_S_vcmpngesd;            // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpngtsd] = NN_S_vcmpngtsd;            // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than (unordered, signaling)
	TABLE[NN_vcmpfalsesd] = NN_S_vcmpfalsesd;          // Compare Scalar Double-Precision Floating-Point Values - False (ordered, non-signaling)
	TABLE[NN_vcmpneq_oqsd] = NN_S_vcmpneq_oqsd;         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (ordered, non-signaling)
	TABLE[NN_vcmpgesd] = NN_S_vcmpgesd;             // Compare Scalar Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpgtsd] = NN_S_vcmpgtsd;             // Compare Scalar Double-Precision Floating-Point Values - Greater-than (ordered, signaling)
	TABLE[NN_vcmptruesd] = NN_S_vcmptruesd;           // Compare Scalar Double-Precision Floating-Point Values - True (unordered, non-signaling)
	TABLE[NN_vcmpeq_ossd] = NN_S_vcmpeq_ossd;          // Compare Scalar Double-Precision Floating-Point Values - Equal (ordered, signaling)
	TABLE[NN_vcmplt_oqsd] = NN_S_vcmplt_oqsd;          // Compare Scalar Double-Precision Floating-Point Values - Less-than (ordered, non-signaling)
	TABLE[NN_vcmple_oqsd] = NN_S_vcmple_oqsd;          // Compare Scalar Double-Precision Floating-Point Values - Less-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpunord_ssd] = NN_S_vcmpunord_ssd;        // Compare Scalar Double-Precision Floating-Point Values - Unordered (signaling)
	TABLE[NN_vcmpneq_ussd] = NN_S_vcmpneq_ussd;         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (unordered, signaling)
	TABLE[NN_vcmpnlt_uqsd] = NN_S_vcmpnlt_uqsd;         // Compare Scalar Double-Precision Floating-Point Values - Not-less-than (unordered, non-signaling)
	TABLE[NN_vcmpnle_uqsd] = NN_S_vcmpnle_uqsd;         // Compare Scalar Double-Precision Floating-Point Values - Not-less-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpord_ssd] = NN_S_vcmpord_ssd;          // Compare Scalar Double-Precision Floating-Point Values - Ordered (signaling)
	TABLE[NN_vcmpeq_ussd] = NN_S_vcmpeq_ussd;          // Compare Scalar Double-Precision Floating-Point Values - Equal (unordered, signaling)
	TABLE[NN_vcmpnge_uqsd] = NN_S_vcmpnge_uqsd;         // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpngt_uqsd] = NN_S_vcmpngt_uqsd;         // Compare Scalar Double-Precision Floating-Point Values - Not-greater-than (unordered, non-signaling)
	TABLE[NN_vcmpfalse_ossd] = NN_S_vcmpfalse_ossd;       // Compare Scalar Double-Precision Floating-Point Values - False (ordered, signaling)
	TABLE[NN_vcmpneq_ossd] = NN_S_vcmpneq_ossd;         // Compare Scalar Double-Precision Floating-Point Values - Not-equal (ordered, signaling)
	TABLE[NN_vcmpge_oqsd] = NN_S_vcmpge_oqsd;          // Compare Scalar Double-Precision Floating-Point Values - Greater-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpgt_oqsd] = NN_S_vcmpgt_oqsd;          // Compare Scalar Double-Precision Floating-Point Values - Greater-than (ordered, non-signaling)
	TABLE[NN_vcmptrue_ussd] = NN_S_vcmptrue_ussd;        // Compare Scalar Double-Precision Floating-Point Values - True (unordered, signaling)

	TABLE[NN_vcmpeqss] = NN_S_vcmpeqss;             // Scalar Single-FP Compare - Equal (ordered, non-signaling)
	TABLE[NN_vcmpltss] = NN_S_vcmpltss;             // Scalar Single-FP Compare - Less-than (ordered, signaling)
	TABLE[NN_vcmpless] = NN_S_vcmpless;             // Scalar Single-FP Compare - Less-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpunordss] = NN_S_vcmpunordss;          // Scalar Single-FP Compare - Unordered (non-signaling)
	TABLE[NN_vcmpneqss] = NN_S_vcmpneqss;            // Scalar Single-FP Compare - Not-equal (unordered, non-signaling)
	TABLE[NN_vcmpnltss] = NN_S_vcmpnltss;            // Scalar Single-FP Compare - Not-less-than (unordered, signaling)
	TABLE[NN_vcmpnless] = NN_S_vcmpnless;            // Scalar Single-FP Compare - Not-less-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpordss] = NN_S_vcmpordss;            // Scalar Single-FP Compare - Ordered (non-signaling)
	TABLE[NN_vcmpeq_uqss] = NN_S_vcmpeq_uqss;          // Scalar Single-FP Compare - Equal (unordered, non-signaling)
	TABLE[NN_vcmpngess] = NN_S_vcmpngess;            // Scalar Single-FP Compare - Not-greater-than-or-equal (unordered, signaling)
	TABLE[NN_vcmpngtss] = NN_S_vcmpngtss;            // Scalar Single-FP Compare - Not-greater-than (unordered, signaling)
	TABLE[NN_vcmpfalsess] = NN_S_vcmpfalsess;          // Scalar Single-FP Compare - False (ordered, non-signaling)
	TABLE[NN_vcmpneq_oqss] = NN_S_vcmpneq_oqss;         // Scalar Single-FP Compare - Not-equal (ordered, non-signaling)
	TABLE[NN_vcmpgess] = NN_S_vcmpgess;             // Scalar Single-FP Compare - Greater-than-or-equal (ordered, signaling)
	TABLE[NN_vcmpgtss] = NN_S_vcmpgtss;             // Scalar Single-FP Compare - Greater-than (ordered, signaling)
	TABLE[NN_vcmptruess] = NN_S_vcmptruess;           // Scalar Single-FP Compare - True (unordered, non-signaling)
	TABLE[NN_vcmpeq_osss] = NN_S_vcmpeq_osss;          // Scalar Single-FP Compare - Equal (ordered, signaling)
	TABLE[NN_vcmplt_oqss] = NN_S_vcmplt_oqss;          // Scalar Single-FP Compare - Less-than (ordered, non-signaling)
	TABLE[NN_vcmple_oqss] = NN_S_vcmple_oqss;          // Scalar Single-FP Compare - Less-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpunord_sss] = NN_S_vcmpunord_sss;        // Scalar Single-FP Compare - Unordered (signaling)
	TABLE[NN_vcmpneq_usss] = NN_S_vcmpneq_usss;         // Scalar Single-FP Compare - Not-equal (unordered, signaling)
	TABLE[NN_vcmpnlt_uqss] = NN_S_vcmpnlt_uqss;         // Scalar Single-FP Compare - Not-less-than (unordered, non-signaling)
	TABLE[NN_vcmpnle_uqss] = NN_S_vcmpnle_uqss;         // Scalar Single-FP Compare - Not-less-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpord_sss] = NN_S_vcmpord_sss;          // Scalar Single-FP Compare - Ordered (signaling)
	TABLE[NN_vcmpeq_usss] = NN_S_vcmpeq_usss;          // Scalar Single-FP Compare - Equal (unordered, signaling)
	TABLE[NN_vcmpnge_uqss] = NN_S_vcmpnge_uqss;         // Scalar Single-FP Compare - Not-greater-than-or-equal (unordered, non-signaling)
	TABLE[NN_vcmpngt_uqss] = NN_S_vcmpngt_uqss;         // Scalar Single-FP Compare - Not-greater-than (unordered, non-signaling)
	TABLE[NN_vcmpfalse_osss] = NN_S_vcmpfalse_osss;       // Scalar Single-FP Compare - False (ordered, signaling)
	TABLE[NN_vcmpneq_osss] = NN_S_vcmpneq_osss;         // Scalar Single-FP Compare - Not-equal (ordered, signaling)
	TABLE[NN_vcmpge_oqss] = NN_S_vcmpge_oqss;          // Scalar Single-FP Compare - Greater-than-or-equal (ordered, non-signaling)
	TABLE[NN_vcmpgt_oqss] = NN_S_vcmpgt_oqss;          // Scalar Single-FP Compare - Greater-than (ordered, non-signaling)
	TABLE[NN_vcmptrue_usss] = NN_S_vcmptrue_usss;        // Scalar Single-FP Compare - True (unordered, signaling)

	// AVX-512 instructions

	TABLE[NN_valignd] = NN_S_valignd;              // Align Doubleword Vectors
	TABLE[NN_valignq] = NN_S_valignq;              // Align Quadword Vectors
	TABLE[NN_vblendmpd] = NN_S_vblendmpd;            // Blend Float64 Vectors Using an OpMask Control
	TABLE[NN_vblendmps] = NN_S_vblendmps;            // Blend Float32 Vectors Using an OpMask Control
	TABLE[NN_vpblendmb] = NN_S_vpblendmb;            // Blend Byte Vectors Using an Opmask Control
	TABLE[NN_vpblendmw] = NN_S_vpblendmw;            // Blend Word Vectors Using an Opmask Control
	TABLE[NN_vpblendmd] = NN_S_vpblendmd;            // Blend Int32 Vectors Using an OpMask Control
	TABLE[NN_vpblendmq] = NN_S_vpblendmq;            // Blend Int64 Vectors Using an OpMask Control
	TABLE[NN_vbroadcastf32x2] = NN_S_vbroadcastf32x2;      // Load with Broadcast Floating-Point Data
	TABLE[NN_vbroadcastf32x4] = NN_S_vbroadcastf32x4;      // Load with Broadcast Floating-Point Data
	TABLE[NN_vbroadcastf64x2] = NN_S_vbroadcastf64x2;      // Load with Broadcast Floating-Point Data
	TABLE[NN_vbroadcastf32x8] = NN_S_vbroadcastf32x8;      // Load with Broadcast Floating-Point Data
	TABLE[NN_vbroadcastf64x4] = NN_S_vbroadcastf64x4;      // Load with Broadcast Floating-Point Data
	TABLE[NN_vbroadcasti32x2] = NN_S_vbroadcasti32x2;      // Load Integer and Broadcast
	TABLE[NN_vbroadcasti32x4] = NN_S_vbroadcasti32x4;      // Load Integer and Broadcast
	TABLE[NN_vbroadcasti64x2] = NN_S_vbroadcasti64x2;      // Load Integer and Broadcast
	TABLE[NN_vbroadcasti32x8] = NN_S_vbroadcasti32x8;      // Load Integer and Broadcast
	TABLE[NN_vbroadcasti64x4] = NN_S_vbroadcasti64x4;      // Load Integer and Broadcast
	TABLE[NN_vcompresspd] = NN_S_vcompresspd;          // Store Sparse Packed Double-Precision Floating-Point Values into Dense Memory
	TABLE[NN_vcompressps] = NN_S_vcompressps;          // Store Sparse Packed Single-Precision Floating-Point Values into Dense Memory
	TABLE[NN_vcvtpd2qq] = NN_S_vcvtpd2qq;            // Convert Packed Double-Precision Floating-Point Values to Packed Quadword Integers
	TABLE[NN_vcvtpd2udq] = NN_S_vcvtpd2udq;           // Convert Packed Double-Precision Floating-Point Values to Packed Unsigned Doubleword Integers
	TABLE[NN_vcvtpd2uqq] = NN_S_vcvtpd2uqq;           // Convert Packed Double-Precision Floating-Point Values to Packed Unsigned Quadword Integers
	TABLE[NN_vcvtps2udq] = NN_S_vcvtps2udq;           // Convert Packed Single-Precision Floating-Point Values to Packed Unsigned Doubleword Integer Values
	TABLE[NN_vcvtps2qq] = NN_S_vcvtps2qq;            // Convert Packed Single Precision Floating-Point Values to Packed Singed Quadword Integer Values
	TABLE[NN_vcvtps2uqq] = NN_S_vcvtps2uqq;           // Convert Packed Single Precision Floating-Point Values to Packed Unsigned Quadword Integer Values
	TABLE[NN_vcvtqq2pd] = NN_S_vcvtqq2pd;            // Convert Packed Quadword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_vcvtqq2ps] = NN_S_vcvtqq2ps;            // Convert Packed Quadword Integers to Packed Single-Precision Floating-Point Values
	TABLE[NN_vcvtsd2usi] = NN_S_vcvtsd2usi;           // Convert Scalar Double-Precision Floating-Point Value to Unsigned Doubleword Integer
	TABLE[NN_vcvtss2usi] = NN_S_vcvtss2usi;           // Convert Scalar Single-Precision Floating-Point Value to Unsigned Doubleword Integer
	TABLE[NN_vcvttpd2qq] = NN_S_vcvttpd2qq;           // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Quadword Integers
	TABLE[NN_vcvttpd2udq] = NN_S_vcvttpd2udq;          // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Unsigned Doubleword Integers
	TABLE[NN_vcvttpd2uqq] = NN_S_vcvttpd2uqq;          // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Unsigned Quadword Integers
	TABLE[NN_vcvttps2udq] = NN_S_vcvttps2udq;          // Convert with Truncation Packed Single-Precision Floating-Point Values to Packed Unsigned Doubleword Integer Values
	TABLE[NN_vcvttps2qq] = NN_S_vcvttps2qq;           // Convert with Truncation Packed Single Precision Floating-Point Values to Packed Singed Quadword Integer Values
	TABLE[NN_vcvttps2uqq] = NN_S_vcvttps2uqq;          // Convert with Truncation Packed Single Precision Floating-Point Values to Packed Unsigned Quadword Integer Values
	TABLE[NN_vcvttsd2usi] = NN_S_vcvttsd2usi;          // Convert with Truncation Scalar Double-Precision Floating-Point Value to Unsigned Integer
	TABLE[NN_vcvttss2usi] = NN_S_vcvttss2usi;          // Convert with Truncation Scalar Single-Precision Floating-Point Value to Unsigned Integer
	TABLE[NN_vcvtudq2pd] = NN_S_vcvtudq2pd;           // Convert Packed Unsigned Doubleword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_vcvtudq2ps] = NN_S_vcvtudq2ps;           // Convert Packed Unsigned Doubleword Integers to Packed Single-Precision Floating-Point Values
	TABLE[NN_vcvtuqq2pd] = NN_S_vcvtuqq2pd;           // Convert Packed Unsigned Quadword Integers to Packed Double-Precision Floating-Point Values
	TABLE[NN_vcvtuqq2ps] = NN_S_vcvtuqq2ps;           // Convert Packed Unsigned Quadword Integers to Packed Single-Precision Floating-Point Values
	TABLE[NN_vcvtusi2sd] = NN_S_vcvtusi2sd;           // Convert Unsigned Integer to Scalar Double-Precision Floating-Point Value
	TABLE[NN_vcvtusi2ss] = NN_S_vcvtusi2ss;           // Convert Unsigned Integer to Scalar Single-Precision Floating-Point Value
	TABLE[NN_vdbpsadbw] = NN_S_vdbpsadbw;            // Double Block Packed Sum-Absolute-Differences (SAD) on Unsigned Bytes
	TABLE[NN_vexpandpd] = NN_S_vexpandpd;            // Load Sparse Packed Double-Precision Floating-Point Values from Dense Memory
	TABLE[NN_vexpandps] = NN_S_vexpandps;            // Load Sparse Packed Single-Precision Floating-Point Values from Dense Memory
	TABLE[NN_vextractf32x4] = NN_S_vextractf32x4;        // Extract Packed Floating-Point Values
	TABLE[NN_vextractf64x2] = NN_S_vextractf64x2;        // Extract Packed Floating-Point Values
	TABLE[NN_vextractf32x8] = NN_S_vextractf32x8;        // Extract Packed Floating-Point Values
	TABLE[NN_vextractf64x4] = NN_S_vextractf64x4;        // Extract Packed Floating-Point Values
	TABLE[NN_vextracti32x4] = NN_S_vextracti32x4;        // Extract packed Integer Values
	TABLE[NN_vextracti64x2] = NN_S_vextracti64x2;        // Extract packed Integer Values
	TABLE[NN_vextracti32x8] = NN_S_vextracti32x8;        // Extract packed Integer Values
	TABLE[NN_vextracti64x4] = NN_S_vextracti64x4;        // Extract packed Integer Values
	TABLE[NN_vfixupimmpd] = NN_S_vfixupimmpd;          // Fix Up Special Packed Float64 Values
	TABLE[NN_vfixupimmps] = NN_S_vfixupimmps;          // Fix Up Special Packed Float32 Values
	TABLE[NN_vfixupimmsd] = NN_S_vfixupimmsd;          // Fix Up Special Scalar Float64 Value
	TABLE[NN_vfixupimmss] = NN_S_vfixupimmss;          // Fix Up Special Scalar Float32 Value
	TABLE[NN_vfpclasspd] = NN_S_vfpclasspd;           // Tests Types Of a Packed Float64 Values
	TABLE[NN_vfpclassps] = NN_S_vfpclassps;           // Tests Types Of a Packed Float32 Values
	TABLE[NN_vfpclasssd] = NN_S_vfpclasssd;           // Tests Types Of a Scalar Float64 Values
	TABLE[NN_vfpclassss] = NN_S_vfpclassss;           // Tests Types Of a Scalar Float32 Values
	TABLE[NN_vgetexppd] = NN_S_vgetexppd;            // Convert Exponents of Packed DP FP Values to DP FP Values
	TABLE[NN_vgetexpps] = NN_S_vgetexpps;            // Convert Exponents of Packed SP FP Values to SP FP Values
	TABLE[NN_vgetexpsd] = NN_S_vgetexpsd;            // Convert Exponents of Scalar DP FP Values to DP FP Value
	TABLE[NN_vgetexpss] = NN_S_vgetexpss;            // Convert Exponents of Scalar SP FP Values to SP FP Value
	TABLE[NN_vgetmantpd] = NN_S_vgetmantpd;           // Extract Float64 Vector of Normalized Mantissas from Float64 Vector
	TABLE[NN_vgetmantps] = NN_S_vgetmantps;           // Extract Float32 Vector of Normalized Mantissas from Float32 Vector
	TABLE[NN_vgetmantsd] = NN_S_vgetmantsd;           // Extract Float64 of Normalized Mantissas from Float64 Scalar
	TABLE[NN_vgetmantss] = NN_S_vgetmantss;           // Extract Float32 Vector of Normalized Mantissa from Float32 Vector
	TABLE[NN_vinsertf32x4] = NN_S_vinsertf32x4;         // Insert Packed Floating-Point Values
	TABLE[NN_vinsertf64x2] = NN_S_vinsertf64x2;         // Insert Packed Floating-Point Values
	TABLE[NN_vinsertf32x8] = NN_S_vinsertf32x8;         // Insert Packed Floating-Point Values
	TABLE[NN_vinsertf64x4] = NN_S_vinsertf64x4;         // Insert Packed Floating-Point Values
	TABLE[NN_vinserti32x4] = NN_S_vinserti32x4;         // Insert Packed Integer Values
	TABLE[NN_vinserti64x2] = NN_S_vinserti64x2;         // Insert Packed Integer Values
	TABLE[NN_vinserti32x8] = NN_S_vinserti32x8;         // Insert Packed Integer Values
	TABLE[NN_vinserti64x4] = NN_S_vinserti64x4;         // Insert Packed Integer Values
	TABLE[NN_vmovdqa32] = NN_S_vmovdqa32;            // Move Aligned Packed Integer Values
	TABLE[NN_vmovdqa64] = NN_S_vmovdqa64;            // Move Aligned Packed Integer Values
	TABLE[NN_vmovdqu8] = NN_S_vmovdqu8;             // Move Unaligned Packed Integer Values
	TABLE[NN_vmovdqu16] = NN_S_vmovdqu16;            // Move Unaligned Packed Integer Values
	TABLE[NN_vmovdqu32] = NN_S_vmovdqu32;            // Move Unaligned Packed Integer Values
	TABLE[NN_vmovdqu64] = NN_S_vmovdqu64;            // Move Unaligned Packed Integer Values
	TABLE[NN_vpabsq] = NN_S_vpabsq;               // Packed Absolute Value
	TABLE[NN_vpandd] = NN_S_vpandd;               // Logical AND
	TABLE[NN_vpandq] = NN_S_vpandq;               // Logical AND
	TABLE[NN_vpandnd] = NN_S_vpandnd;              // Logical AND NOT
	TABLE[NN_vpandnq] = NN_S_vpandnq;              // Logical AND NOT
	TABLE[NN_vpbroadcastmb2q] = NN_S_vpbroadcastmb2q;      // Broadcast Mask to Vector Register
	TABLE[NN_vpbroadcastmw2d] = NN_S_vpbroadcastmw2d;      // Broadcast Mask to Vector Register
	TABLE[NN_vpcmpb] = NN_S_vpcmpb;               // Compare Packed Byte Values Into Mask
	TABLE[NN_vpcmpub] = NN_S_vpcmpub;              // Compare Packed Byte Values Into Mask
	TABLE[NN_vpcmpd] = NN_S_vpcmpd;               // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpud] = NN_S_vpcmpud;              // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpq] = NN_S_vpcmpq;               // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpuq] = NN_S_vpcmpuq;              // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpw] = NN_S_vpcmpw;               // Compare Packed Word Values Into Mask
	TABLE[NN_vpcmpuw] = NN_S_vpcmpuw;              // Compare Packed Word Values Into Mask
	TABLE[NN_vpcompressd] = NN_S_vpcompressd;          // Store Sparse Packed Doubleword Integer Values into Dense Memory/Register
	TABLE[NN_vpcompressq] = NN_S_vpcompressq;          // Store Sparse Packed Quadword Integer Values into Dense Memory/Register
	TABLE[NN_vpconflictd] = NN_S_vpconflictd;          // Detect Conflicts Within a Vector of Packed Dword Values into Dense Memory/Register
	TABLE[NN_vpconflictq] = NN_S_vpconflictq;          // Detect Conflicts Within a Vector of Packed Qword Values into Dense Memory/Register
	TABLE[NN_vpermb] = NN_S_vpermb;               // Permute Packed Bytes Elements
	TABLE[NN_vpermw] = NN_S_vpermw;               // Permute Packed Words Elements
	TABLE[NN_vpermi2b] = NN_S_vpermi2b;             // Full Permute of Bytes From Two Tables Overwriting the Index
	TABLE[NN_vpermi2w] = NN_S_vpermi2w;             // Full Permute From Two Tables Overwriting the Index
	TABLE[NN_vpermi2d] = NN_S_vpermi2d;             // Full Permute From Two Tables Overwriting the Index
	TABLE[NN_vpermi2q] = NN_S_vpermi2q;             // Full Permute From Two Tables Overwriting the Index
	TABLE[NN_vpermi2ps] = NN_S_vpermi2ps;            // Full Permute From Two Tables Overwriting the Index
	TABLE[NN_vpermi2pd] = NN_S_vpermi2pd;            // Full Permute From Two Tables Overwriting the Index
	TABLE[NN_vpermt2b] = NN_S_vpermt2b;             // Full Permute of Bytes From Two Tables Overwriting a Table
	TABLE[NN_vpermt2w] = NN_S_vpermt2w;             // Full Permute from Two Tables Overwriting one Table
	TABLE[NN_vpermt2d] = NN_S_vpermt2d;             // Full Permute from Two Tables Overwriting one Table
	TABLE[NN_vpermt2q] = NN_S_vpermt2q;             // Full Permute from Two Tables Overwriting one Table
	TABLE[NN_vpermt2ps] = NN_S_vpermt2ps;            // Full Permute from Two Tables Overwriting one Table
	TABLE[NN_vpermt2pd] = NN_S_vpermt2pd;            // Full Permute from Two Tables Overwriting one Table
	TABLE[NN_vpexpandd] = NN_S_vpexpandd;            // Load Sparse Packed Doubleword Integer Values from Dense Memory/Register
	TABLE[NN_vpexpandq] = NN_S_vpexpandq;            // Load Sparse Packed Quadword Integer Values from Dense Memory/Register
	TABLE[NN_vplzcntd] = NN_S_vplzcntd;             // Count the Number of Leading Zero Bits for Packed Dword Values
	TABLE[NN_vplzcntq] = NN_S_vplzcntq;             // Count the Number of Leading Zero Bits for Packed Qword Values
	TABLE[NN_vpmadd52luq] = NN_S_vpmadd52luq;          // Packed Multiply of Unsigned 52-bit Integers and Add the Low 52-bit Products to Qword Accumulators
	TABLE[NN_vpmadd52huq] = NN_S_vpmadd52huq;          // Packed Multiply of Unsigned 52-bit Unsigned Integers and Add High 52-bit Products to 64-bit Accumulators
	TABLE[NN_vpmaxsq] = NN_S_vpmaxsq;              // Maximum of Packed Signed Integers
	TABLE[NN_vpmaxuq] = NN_S_vpmaxuq;              // Maximum of Packed Unsigned Integers
	TABLE[NN_vpminsq] = NN_S_vpminsq;              // Minimum of Packed Signed Integers
	TABLE[NN_vpminuq] = NN_S_vpminuq;              // Minimum of Packed Unsigned Integers
	TABLE[NN_vpmovm2b] = NN_S_vpmovm2b;             // Convert a Mask Register to a Vector Register
	TABLE[NN_vpmovm2w] = NN_S_vpmovm2w;             // Convert a Mask Register to a Vector Register
	TABLE[NN_vpmovm2d] = NN_S_vpmovm2d;             // Convert a Mask Register to a Vector Register
	TABLE[NN_vpmovm2q] = NN_S_vpmovm2q;             // Convert a Mask Register to a Vector Register
	TABLE[NN_vpmovb2m] = NN_S_vpmovb2m;             // Convert a Vector Register to a Mask
	TABLE[NN_vpmovw2m] = NN_S_vpmovw2m;             // Convert a Vector Register to a Mask
	TABLE[NN_vpmovd2m] = NN_S_vpmovd2m;             // Convert a Vector Register to a Mask
	TABLE[NN_vpmovq2m] = NN_S_vpmovq2m;             // Convert a Vector Register to a Mask
	TABLE[NN_vpmovqb] = NN_S_vpmovqb;              // Down Convert QWord to Byte
	TABLE[NN_vpmovsqb] = NN_S_vpmovsqb;             // Down Convert QWord to Byte
	TABLE[NN_vpmovusqb] = NN_S_vpmovusqb;            // Down Convert QWord to Byte
	TABLE[NN_vpmovqw] = NN_S_vpmovqw;              // Down Convert QWord to Word
	TABLE[NN_vpmovsqw] = NN_S_vpmovsqw;             // Down Convert QWord to Word
	TABLE[NN_vpmovusqw] = NN_S_vpmovusqw;            // Down Convert QWord to Word
	TABLE[NN_vpmovqd] = NN_S_vpmovqd;              // Down Convert QWord to DWord
	TABLE[NN_vpmovsqd] = NN_S_vpmovsqd;             // Down Convert QWord to DWord
	TABLE[NN_vpmovusqd] = NN_S_vpmovusqd;            // Down Convert QWord to DWord
	TABLE[NN_vpmovdb] = NN_S_vpmovdb;              // Down Convert DWord to Byte
	TABLE[NN_vpmovsdb] = NN_S_vpmovsdb;             // Down Convert DWord to Byte
	TABLE[NN_vpmovusdb] = NN_S_vpmovusdb;            // Down Convert DWord to Byte
	TABLE[NN_vpmovdw] = NN_S_vpmovdw;              // Down Convert DWord to Word
	TABLE[NN_vpmovsdw] = NN_S_vpmovsdw;             // Down Convert DWord to Word
	TABLE[NN_vpmovusdw] = NN_S_vpmovusdw;            // Down Convert DWord to Word
	TABLE[NN_vpmovwb] = NN_S_vpmovwb;              // Down Convert Word to Byte
	TABLE[NN_vpmovswb] = NN_S_vpmovswb;             // Down Convert Word to Byte
	TABLE[NN_vpmovuswb] = NN_S_vpmovuswb;            // Down Convert Word to Byte
	TABLE[NN_vpmullq] = NN_S_vpmullq;              // Multiply Packed Integers and Store Low Result
	TABLE[NN_vpmultishiftqb] = NN_S_vpmultishiftqb;       // Select Packed Unaligned Bytes from Quadword Sources
	TABLE[NN_vpord] = NN_S_vpord;                // Bitwise Logical Or
	TABLE[NN_vporq] = NN_S_vporq;                // Bitwise Logical Or
	TABLE[NN_vprold] = NN_S_vprold;               // Bit Rotate Left
	TABLE[NN_vprolvd] = NN_S_vprolvd;              // Bit Rotate Left
	TABLE[NN_vprolq] = NN_S_vprolq;               // Bit Rotate Left
	TABLE[NN_vprolvq] = NN_S_vprolvq;              // Bit Rotate Left
	TABLE[NN_vprord] = NN_S_vprord;               // Bit Rotate Right
	TABLE[NN_vprorvd] = NN_S_vprorvd;              // Bit Rotate Right
	TABLE[NN_vprorq] = NN_S_vprorq;               // Bit Rotate Right
	TABLE[NN_vprorvq] = NN_S_vprorvq;              // Bit Rotate Right
	TABLE[NN_vpscatterdd] = NN_S_vpscatterdd;          // Scatter Packed Dword with Signed Dword
	TABLE[NN_vpscatterdq] = NN_S_vpscatterdq;          // Scatter Packed Qword with Signed Qword Indices
	TABLE[NN_vpscatterqd] = NN_S_vpscatterqd;          // Scatter Packed Dword with Signed Dword
	TABLE[NN_vpscatterqq] = NN_S_vpscatterqq;          // Scatter Packed Qword with Signed Qword Indices
	TABLE[NN_vpsraq] = NN_S_vpsraq;               // Bit Shift Arithmetic Right
	TABLE[NN_vpsllvw] = NN_S_vpsllvw;              // Variable Bit Shift Left Logical
	TABLE[NN_vpsrlvw] = NN_S_vpsrlvw;              // Variable Bit Shift Right Logical
	TABLE[NN_vptestnmb] = NN_S_vptestnmb;            // Logical NAND and Set
	TABLE[NN_vptestnmw] = NN_S_vptestnmw;            // Logical NAND and Set
	TABLE[NN_vptestnmd] = NN_S_vptestnmd;            // Logical NAND and Set
	TABLE[NN_vptestnmq] = NN_S_vptestnmq;            // Logical NAND and Set
	TABLE[NN_vshuff32x4] = NN_S_vshuff32x4;           // Shuffle Packed Values at 128-bit Granularity
	TABLE[NN_vshuff64x2] = NN_S_vshuff64x2;           // Shuffle Packed Values at 128-bit Granularity
	TABLE[NN_vshufi32x4] = NN_S_vshufi32x4;           // Shuffle Packed Values at 128-bit Granularity
	TABLE[NN_vshufi64x2] = NN_S_vshufi64x2;           // Shuffle Packed Values at 128-bit Granularity
	TABLE[NN_vpternlogd] = NN_S_vpternlogd;           // Bitwise Ternary Logic
	TABLE[NN_vpternlogq] = NN_S_vpternlogq;           // Bitwise Ternary Logic
	TABLE[NN_vptestmb] = NN_S_vptestmb;             // Logical AND and Set Mask
	TABLE[NN_vptestmw] = NN_S_vptestmw;             // Logical AND and Set Mask
	TABLE[NN_vptestmd] = NN_S_vptestmd;             // Logical AND and Set Mask
	TABLE[NN_vptestmq] = NN_S_vptestmq;             // Logical AND and Set Mask
	TABLE[NN_vpsravw] = NN_S_vpsravw;              // Variable Bit Shift Right Arithmetic
	TABLE[NN_vpsravq] = NN_S_vpsravq;              // Variable Bit Shift Right Arithmetic
	TABLE[NN_vpxord] = NN_S_vpxord;               // Exclusive Or
	TABLE[NN_vpxorq] = NN_S_vpxorq;               // Exclusive Or
	TABLE[NN_vrangepd] = NN_S_vrangepd;             // Range Restriction Calculation For Packed Pairs of Float64 Values
	TABLE[NN_vrangeps] = NN_S_vrangeps;             // Range Restriction Calculation For Packed Pairs of Float32 Values
	TABLE[NN_vrangesd] = NN_S_vrangesd;             // Range Restriction Calculation From a pair of Scalar Float64 Values
	TABLE[NN_vrangess] = NN_S_vrangess;             // Range Restriction Calculation From a Pair of Scalar Float32 Values
	TABLE[NN_vrcp14pd] = NN_S_vrcp14pd;             // Compute Approximate Reciprocals of Packed Float64 Values
	TABLE[NN_vrcp14sd] = NN_S_vrcp14sd;             // Compute Approximate Reciprocal of Scalar Float64 Value
	TABLE[NN_vrcp14ps] = NN_S_vrcp14ps;             // Compute Approximate Reciprocals of Packed Float32 Values
	TABLE[NN_vrcp14ss] = NN_S_vrcp14ss;             // Compute Approximate Reciprocal of Scalar Float32 Value
	TABLE[NN_vreducepd] = NN_S_vreducepd;            // Perform Reduction Transformation on Packed Float64 Values
	TABLE[NN_vreducesd] = NN_S_vreducesd;            // Perform a Reduction Transformation on a Scalar Float64 Value
	TABLE[NN_vreduceps] = NN_S_vreduceps;            // Perform Reduction Transformation on Packed Float32 Values
	TABLE[NN_vreducess] = NN_S_vreducess;            // Perform a Reduction Transformation on a Scalar Float32 Value
	TABLE[NN_vrndscalepd] = NN_S_vrndscalepd;          // Round Packed Float64 Values To Include A Given Number Of Fraction Bits
	TABLE[NN_vrndscalesd] = NN_S_vrndscalesd;          // Round Scalar Float64 Value To Include A Given Number Of Fraction Bits
	TABLE[NN_vrndscaleps] = NN_S_vrndscaleps;          // Round Packed Float32 Values To Include A Given Number Of Fraction Bits
	TABLE[NN_vrndscaless] = NN_S_vrndscaless;          // Round Scalar Float32 Value To Include A Given Number Of Fraction Bits
	TABLE[NN_vrsqrt14pd] = NN_S_vrsqrt14pd;           // Compute Approximate Reciprocals of Square Roots of Packed Float64 Values
	TABLE[NN_vrsqrt14sd] = NN_S_vrsqrt14sd;           // Compute Approximate Reciprocal of Square Root of Scalar Float64 Value
	TABLE[NN_vrsqrt14ps] = NN_S_vrsqrt14ps;           // Compute Approximate Reciprocals of Square Roots of Packed Float32 Values
	TABLE[NN_vrsqrt14ss] = NN_S_vrsqrt14ss;           // Compute Approximate Reciprocal of Square Root of Scalar Float32 Value
	TABLE[NN_vscalefpd] = NN_S_vscalefpd;            // Scale Packed Float64 Values With Float64 Values
	TABLE[NN_vscalefsd] = NN_S_vscalefsd;            // Scale Scalar Float64 Values With Float64 Values
	TABLE[NN_vscalefps] = NN_S_vscalefps;            // Scale Packed Float32 Values With Float32 Values
	TABLE[NN_vscalefss] = NN_S_vscalefss;            // Scale Scalar Float32 Value With Float32 Value
	TABLE[NN_vscatterdps] = NN_S_vscatterdps;          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	TABLE[NN_vscatterdpd] = NN_S_vscatterdpd;          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	TABLE[NN_vscatterqps] = NN_S_vscatterqps;          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices
	TABLE[NN_vscatterqpd] = NN_S_vscatterqpd;          // Scatter Packed Single, Packed Double with Signed Dword and Qword Indices

	TABLE[NN_vexp2pd] = NN_S_vexp2pd;              // Approximation to the Exponential 2^x of Packed Double-Precision Floating-Point Values with Less Than 2^-23 Relative Error
	TABLE[NN_vexp2ps] = NN_S_vexp2ps;              // Approximation to the Exponential 2^x of Packed Single-Precision Floating-Point Values with Less Than 2^-23 Relative Error
	TABLE[NN_vrcp28pd] = NN_S_vrcp28pd;             // Approximation to the Reciprocal of Packed Double-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	TABLE[NN_vrcp28sd] = NN_S_vrcp28sd;             // Approximation to the Reciprocal of Scalar Double-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	TABLE[NN_vrcp28ps] = NN_S_vrcp28ps;             // Approximation to the Reciprocal of Packed Single-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	TABLE[NN_vrcp28ss] = NN_S_vrcp28ss;             // Approximation to the Reciprocal of Scalar Single-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	TABLE[NN_vrsqrt28pd] = NN_S_vrsqrt28pd;           // Approximation to the Reciprocal Square Root of Packed Double-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	TABLE[NN_vrsqrt28sd] = NN_S_vrsqrt28sd;           // Approximation to the Reciprocal Square Root of Scalar Double-Precision Floating-Point Value with Less Than 2^-28 Relative Error
	TABLE[NN_vrsqrt28ps] = NN_S_vrsqrt28ps;           // Approximation to the Reciprocal Square Root of Packed Single-Precision Floating-Point Values with Less Than 2^-28 Relative Error
	TABLE[NN_vrsqrt28ss] = NN_S_vrsqrt28ss;           // Approximation to the Reciprocal Square Root of Scalar Single-Precision Floating-Point Value with Less Than 2^-28 Relative Error

	TABLE[NN_vgatherpf0dps] = NN_S_vgatherpf0dps;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	TABLE[NN_vgatherpf0qps] = NN_S_vgatherpf0qps;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	TABLE[NN_vgatherpf0dpd] = NN_S_vgatherpf0dpd;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	TABLE[NN_vgatherpf0qpd] = NN_S_vgatherpf0qpd;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint
	TABLE[NN_vgatherpf1dps] = NN_S_vgatherpf1dps;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	TABLE[NN_vgatherpf1qps] = NN_S_vgatherpf1qps;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	TABLE[NN_vgatherpf1dpd] = NN_S_vgatherpf1dpd;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	TABLE[NN_vgatherpf1qpd] = NN_S_vgatherpf1qpd;        // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint
	TABLE[NN_vscatterpf0dps] = NN_S_vscatterpf0dps;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	TABLE[NN_vscatterpf0qps] = NN_S_vscatterpf0qps;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	TABLE[NN_vscatterpf0dpd] = NN_S_vscatterpf0dpd;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	TABLE[NN_vscatterpf0qpd] = NN_S_vscatterpf0qpd;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T0 Hint with Intent to Write
	TABLE[NN_vscatterpf1dps] = NN_S_vscatterpf1dps;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	TABLE[NN_vscatterpf1qps] = NN_S_vscatterpf1qps;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	TABLE[NN_vscatterpf1dpd] = NN_S_vscatterpf1dpd;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write
	TABLE[NN_vscatterpf1qpd] = NN_S_vscatterpf1qpd;       // Sparse Prefetch Packed SP/DP Data Values with Signed Dword, Signed Qword Indices Using T1 Hint with Intent to Write

	// AVX-512 comparison pseudo-ops

	TABLE[NN_vpcmpltd] = NN_S_vpcmpltd;             // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpled] = NN_S_vpcmpled;             // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpneqd] = NN_S_vpcmpneqd;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnltd] = NN_S_vpcmpnltd;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnled] = NN_S_vpcmpnled;            // Compare Packed Integer Values into Mask

	TABLE[NN_vpcmpequd] = NN_S_vpcmpequd;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpltud] = NN_S_vpcmpltud;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpleud] = NN_S_vpcmpleud;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnequd] = NN_S_vpcmpnequd;           // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnltud] = NN_S_vpcmpnltud;           // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnleud] = NN_S_vpcmpnleud;           // Compare Packed Integer Values into Mask

	TABLE[NN_vpcmpltq] = NN_S_vpcmpltq;             // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpleq] = NN_S_vpcmpleq;             // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpneqq] = NN_S_vpcmpneqq;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnltq] = NN_S_vpcmpnltq;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnleq] = NN_S_vpcmpnleq;            // Compare Packed Integer Values into Mask

	TABLE[NN_vpcmpequq] = NN_S_vpcmpequq;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpltuq] = NN_S_vpcmpltuq;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpleuq] = NN_S_vpcmpleuq;            // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnequq] = NN_S_vpcmpnequq;           // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnltuq] = NN_S_vpcmpnltuq;           // Compare Packed Integer Values into Mask
	TABLE[NN_vpcmpnleuq] = NN_S_vpcmpnleuq;           // Compare Packed Integer Values into Mask

	// Opmask instructions

	TABLE[NN_kaddw] = NN_S_kaddw;                // ADD Two Masks
	TABLE[NN_kaddb] = NN_S_kaddb;                // ADD Two Masks
	TABLE[NN_kaddq] = NN_S_kaddq;                // ADD Two Masks
	TABLE[NN_kaddd] = NN_S_kaddd;                // ADD Two Masks
	TABLE[NN_kandw] = NN_S_kandw;                // Bitwise Logical AND Masks
	TABLE[NN_kandb] = NN_S_kandb;                // Bitwise Logical AND Masks
	TABLE[NN_kandq] = NN_S_kandq;                // Bitwise Logical AND Masks
	TABLE[NN_kandd] = NN_S_kandd;                // Bitwise Logical AND Masks
	TABLE[NN_kandnw] = NN_S_kandnw;               // Bitwise Logical AND NOT Masks
	TABLE[NN_kandnb] = NN_S_kandnb;               // Bitwise Logical AND NOT Masks
	TABLE[NN_kandnq] = NN_S_kandnq;               // Bitwise Logical AND NOT Masks
	TABLE[NN_kandnd] = NN_S_kandnd;               // Bitwise Logical AND NOT Masks
	TABLE[NN_kmovw] = NN_S_kmovw;                // Move from and to Mask Registers
	TABLE[NN_kmovb] = NN_S_kmovb;                // Move from and to Mask Registers
	TABLE[NN_kmovq] = NN_S_kmovq;                // Move from and to Mask Registers
	TABLE[NN_kmovd] = NN_S_kmovd;                // Move from and to Mask Registers
	TABLE[NN_kunpckbw] = NN_S_kunpckbw;             // Unpack for Mask Registers
	TABLE[NN_kunpckwd] = NN_S_kunpckwd;             // Unpack for Mask Registers
	TABLE[NN_kunpckdq] = NN_S_kunpckdq;             // Unpack for Mask Registers
	TABLE[NN_knotw] = NN_S_knotw;                // NOT Mask Register
	TABLE[NN_knotb] = NN_S_knotb;                // NOT Mask Register
	TABLE[NN_knotq] = NN_S_knotq;                // NOT Mask Register
	TABLE[NN_knotd] = NN_S_knotd;                // NOT Mask Register
	TABLE[NN_korw] = NN_S_korw;                 // Bitwise Logical OR Masks
	TABLE[NN_korb] = NN_S_korb;                 // Bitwise Logical OR Masks
	TABLE[NN_korq] = NN_S_korq;                 // Bitwise Logical OR Masks
	TABLE[NN_kord] = NN_S_kord;                 // Bitwise Logical OR Masks
	TABLE[NN_kortestw] = NN_S_kortestw;             // OR Masks And Set Flags
	TABLE[NN_kortestb] = NN_S_kortestb;             // OR Masks And Set Flags
	TABLE[NN_kortestq] = NN_S_kortestq;             // OR Masks And Set Flags
	TABLE[NN_kortestd] = NN_S_kortestd;             // OR Masks And Set Flags
	TABLE[NN_kshiftlw] = NN_S_kshiftlw;             // Shift Left Mask Registers
	TABLE[NN_kshiftlb] = NN_S_kshiftlb;             // Shift Left Mask Registers
	TABLE[NN_kshiftlq] = NN_S_kshiftlq;             // Shift Left Mask Registers
	TABLE[NN_kshiftld] = NN_S_kshiftld;             // Shift Left Mask Registers
	TABLE[NN_kshiftrw] = NN_S_kshiftrw;             // Shift Right Mask Registers
	TABLE[NN_kshiftrb] = NN_S_kshiftrb;             // Shift Right Mask Registers
	TABLE[NN_kshiftrq] = NN_S_kshiftrq;             // Shift Right Mask Registers
	TABLE[NN_kshiftrd] = NN_S_kshiftrd;             // Shift Right Mask Registers
	TABLE[NN_kxnorw] = NN_S_kxnorw;               // Bitwise Logical XNOR Masks
	TABLE[NN_kxnorb] = NN_S_kxnorb;               // Bitwise Logical XNOR Masks
	TABLE[NN_kxnorq] = NN_S_kxnorq;               // Bitwise Logical XNOR Masks
	TABLE[NN_kxnord] = NN_S_kxnord;               // Bitwise Logical XNOR Masks
	TABLE[NN_ktestw] = NN_S_ktestw;               // Packed Bit Test Masks and Set Flags
	TABLE[NN_ktestb] = NN_S_ktestb;               // Packed Bit Test Masks and Set Flags
	TABLE[NN_ktestq] = NN_S_ktestq;               // Packed Bit Test Masks and Set Flags
	TABLE[NN_ktestd] = NN_S_ktestd;               // Packed Bit Test Masks and Set Flags
	TABLE[NN_kxorw] = NN_S_kxorw;                // Bitwise Logical XOR Masks
	TABLE[NN_kxorb] = NN_S_kxorb;                // Bitwise Logical XOR Masks
	TABLE[NN_kxorq] = NN_S_kxorq;                // Bitwise Logical XOR Masks
	TABLE[NN_kxord] = NN_S_kxord;                // Bitwise Logical XOR Masks

	// SHA Extensions

	TABLE[NN_sha1rnds4] = NN_S_sha1rnds4;            // Perform Four Rounds of SHA1 Operation
	TABLE[NN_sha1nexte] = NN_S_sha1nexte;            // Calculate SHA1 State Variable E after Four Rounds
	TABLE[NN_sha1msg1] = NN_S_sha1msg1;             // Perform an Intermediate Calculation for the Next Four SHA1 Message Dwords
	TABLE[NN_sha1msg2] = NN_S_sha1msg2;             // Perform a Final Calculation for the Next Four SHA1 Message Dwords
	TABLE[NN_sha256rnds2] = NN_S_sha256rnds2;          // Perform Two Rounds of SHA256 Operation
	TABLE[NN_sha256msg1] = NN_S_sha256msg1;           // Perform an Intermediate Calculation for the Next Four SHA256 Message Dwords
	TABLE[NN_sha256msg2] = NN_S_sha256msg2;           // Perform a Final Calculation for the Next Four SHA256 Message Dwords

	// Intel Software Guard Extensions

	TABLE[NN_encls] = NN_S_BADOP;                // Execute an Enclave System Function of Specified Leaf Number
	TABLE[NN_enclu] = NN_S_BADOP;                // Execute an Enclave User Function of Specified Leaf Number


	for (unsigned i = NN_vfrczpd; i < NN_vpcomtrueuq + 1; ++i) {
		TABLE[i] = NN_S_BADOP;
	}

	// AMD Excavator

	TABLE[NN_monitorx] = NN_S_BADOP;             // Setup Monitor Address
	TABLE[NN_mwaitx] = NN_S_BADOP;               // Monitor Wait with Timeout

	// AMD Zen

	TABLE[NN_clzero] = NN_S_BADOP;               // Zero out 64 byte cache

	// Intel Processor Trace

	TABLE[NN_ptwrite] = NN_S_BADOP;              // Write Data to a Processor Trace Packet

	// new Intel AVX-512 instructions (December 2016)

	TABLE[NN_v4fmaddps] = NN_S_v4fmaddps;            // Packed Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	TABLE[NN_v4fnmaddps] = NN_S_v4fnmaddps;           // Packed Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	TABLE[NN_v4fmaddss] = NN_S_v4fmaddss;            // Scalar Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	TABLE[NN_v4fnmaddss] = NN_S_v4fnmaddss;           // Scalar Single-Precision Floating-Point Fused Multiply-Add (4-iterations)
	TABLE[NN_vp4dpwssd] = NN_S_vp4dpwssd;            // Dot Product of Signed Words with Dword Accumulation (4-iterations)
	TABLE[NN_vp4dpwssds] = NN_S_vp4dpwssds;           // Dot Product of Signed Words with Dword Accumulation and Saturation (4-iterations)
	TABLE[NN_vpopcntd] = NN_S_vpopcntd;             // Return the Count of Number of Bits Set to 1 in DWORD
	TABLE[NN_vpopcntq] = NN_S_vpopcntq;             // Return the Count of Number of Bits Set to 1 in QWORD

	// Read Processor ID

	TABLE[NN_rdpid] = NN_S_rdpid;                // Read Processor ID

	// Invoke VM function

	TABLE[NN_vmfunc] = NN_S_BADOP;               // Invoke VM function

	// Control-flow Enforcement

	TABLE[NN_incsspd] = NN_S_BADOP;              // Increment Shadow Stack Pointer (by 4)
	TABLE[NN_incsspq] = NN_S_BADOP;              // Increment Shadow Stack Pointer (by 8)
	TABLE[NN_rdsspd] = NN_S_BADOP;               // Read (low 32 bits of) Shadow Stack Pointer
	TABLE[NN_rdsspq] = NN_S_BADOP;               // Read Shadow Stack Pointer
	TABLE[NN_saveprevssp] = NN_S_BADOP;          // Save Previous Shadow Stack Pointer
	TABLE[NN_rstorssp] = NN_S_BADOP;             // Restore saved Shadow Stack Pointer
	TABLE[NN_wrssd] = NN_S_BADOP;                // Write (4 bytes) to shadow stack
	TABLE[NN_wrssq] = NN_S_BADOP;                // Write (8 bytes) to shadow stack
	TABLE[NN_wrussd] = NN_S_BADOP;               // Write (4 bytes) to User Shadow Stack
	TABLE[NN_wrussq] = NN_S_BADOP;               // Write (8 bytes) to User Shadow Stack
	TABLE[NN_setssbsy] = NN_S_BADOP;             // Mark Shadow Stack Busy
	TABLE[NN_clrssbsy] = NN_S_BADOP;             // Clear Shadow Stack Busy Flag
	TABLE[NN_endbr64] = NN_S_BADOP;              // Terminate an Indirect Branch in 64-bit Mode
	TABLE[NN_endbr32] = NN_S_BADOP;              // Terminate an Indirect Branch in 32-bit and Compatibility Mode

	// Undefined Instruction

	//TABLE[NN_ud0] = NN_S_BADOP;                 // Undefined Instruction
	//TABLE[NN_ud1] = NN_S_BADOP;                 // Undefined Instruction

	//TABLE[NN_last] = NN_S_last;
	return TABLE;
}
static constexpr remapx64_t g_remaptable = create_remap_table();

cs_x86_op_e remap_x86(unsigned op) {
	return g_remaptable[op];
}