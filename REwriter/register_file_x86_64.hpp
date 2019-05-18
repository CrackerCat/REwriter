#pragma once
#include "cs_core.hpp"
#include <array>
namespace cs::register_file {

	//name, bitoffset, bitlength


	namespace gpregister_defs {
		//all 64 bits
		enum gpreg {
			RAX = 0,
			RBX = 1,
			RCX = 2,
			RDX = 3,
			RBP = 4,
			RSP = 5,
			RSI = 6,
			RDI = 7,
			R8 = 8,
			R9 = 9,
			R10 = 10,
			R11 = 11,
			R12 = 12,
			R13 = 13,
			R14 = 14,
			R15 = 15,
			RIP = 16,
			RFLAGS = 17,
			NGPREGS
		};
	}

	namespace segreg_defs {
		enum segreg {
			ES = 0,
			CS = 1,
			SS = 2,
			DS = 3,
			FS = 4,
			GS = 5,
			NSEGREGS
		};
	}

	namespace x87reg_defs {
		//all 80 bits
		enum x87reg {
			ST0 = 0,
			ST1 = 1,
			ST2 = 2,
			ST3 = 3,
			ST4 = 4,
			ST5 = 5,
			ST6 = 6,
			ST7 = 7,
			NX87REGS
		};
		//8 bits
		enum x87tag {
			FPTAGS = 0,
			NFPTAGREGS
		};
		//16 bits each
		enum x87conf {
			FPCTRL = 0,
			FPSTATUS=1,
			NX87CONFREGS
		};
	}

	namespace vector_defs {
		//each 512 bit
		enum zmmreg {
			ZMM0 = 0,
			ZMM1 = 1,
			ZMM2 = 2,
			ZMM3 = 3,
			ZMM4 = 4,
			ZMM5 = 5,
			ZMM6 = 6,
			ZMM7 = 7,
			ZMM8 = 8,
			ZMM9 = 9,
			ZMM10 = 10,
			ZMM11 = 11,
			ZMM12 = 12,
			ZMM13 = 13,
			ZMM14 = 14,
			ZMM15 = 15,
			ZMM16 = 16,
			ZMM17 = 17,
			ZMM18 = 18,
			ZMM19 = 19,
			ZMM20 = 20,
			ZMM21 = 21,
			ZMM22 = 22,
			ZMM23 = 23,
			ZMM24 = 24,
			ZMM25 = 25,
			ZMM26 = 26,
			ZMM27 = 27,
			ZMM28 = 28,
			ZMM29 = 29,
			ZMM30 = 30,
			ZMM31 = 31,
			NZMMREGS
		};
		//each 64 bit 
		enum maskreg {
			K0=0,
			K1=1,
			K2=2,
			K3=3,
			K4=4,
			K5=5,
			K6=6,
			K7=7,
			NMASKREGS
		};
		//32 bits
		enum vecconf {
			MXCSR = 0,
			NVECCONF
		};
	}

	static constexpr unsigned TOTAL_CONTIGREGS =
		gpregister_defs::NGPREGS
		+ segreg_defs::NSEGREGS
		+ x87reg_defs::NFPTAGREGS
		+ x87reg_defs::NX87CONFREGS
		+ x87reg_defs::NX87REGS
		+ vector_defs::NMASKREGS
		+ vector_defs::NZMMREGS
		+ vector_defs::NVECCONF;
	

}

namespace cs::register_file::_build_register_file {

	namespace ida_registers {
		enum {
			rax = 0,
			rcx = 1,
			rdx = 2,
			rbx = 3,
			rsp = 4,
			rbp = 5,
			rsi = 6,
			rdi = 7,
			r8 = 8,
			r9 = 9,
			r10 = 10,
			r11 = 11,
			r12 = 12,
			r13 = 13,
			r14 = 14,
			r15 = 15,
			al = 16,
			cl = 17,
			dl = 18,
			bl = 19,
			ah = 20,
			ch = 21,
			dh = 22,
			bh = 23,
			spl = 24,
			bpl = 25,
			sil = 26,
			dil = 27,
			rip = 28,
			res = 29,
			rcs = 30,
			rss = 31,
			rds = 32,
			rfs = 33,
			rgs = 34,
			cf = 35,
			zf = 36,
			sf = 37,
			of = 38,
			pf = 39,
			af = 40,
			tf = 41,
			if_ = 42,
			df = 43,
			rfl = 44,
			st0 = 45,
			st1 = 46,
			st2 = 47,
			st3 = 48,
			st4 = 49,
			st5 = 50,
			st6 = 51,
			st7 = 52,
			fpctrl = 53,
			fpstat = 54,
			fptags = 55,
			mm0 = 56,
			mm1 = 57,
			mm2 = 58,
			mm3 = 59,
			mm4 = 60,
			mm5 = 61,
			mm6 = 62,
			mm7 = 63,
			xmm0 = 64,
			xmm1 = 65,
			xmm2 = 66,
			xmm3 = 67,
			xmm4 = 68,
			xmm5 = 69,
			xmm6 = 70,
			xmm7 = 71,
			xmm8 = 72,
			xmm9 = 73,
			xmm10 = 74,
			xmm11 = 75,
			xmm12 = 76,
			xmm13 = 77,
			xmm14 = 78,
			xmm15 = 79,
			mxcsr = 80,
			ymm0 = 81,
			ymm1 = 82,
			ymm2 = 83,
			ymm3 = 84,
			ymm4 = 85,
			ymm5 = 86,
			ymm6 = 87,
			ymm7 = 88,
			ymm8 = 89,
			ymm9 = 90,
			ymm10 = 91,
			ymm11 = 92,
			ymm12 = 93,
			ymm13 = 94,
			ymm14 = 95,
			ymm15 = 96,
			bnd0 = 97,
			bnd1 = 98,
			bnd2 = 99,
			bnd3 = 100,
			xmm16 = 101,
			xmm17 = 102,
			xmm18 = 103,
			xmm19 = 104,
			xmm20 = 105,
			xmm21 = 106,
			xmm22 = 107,
			xmm23 = 108,
			xmm24 = 109,
			xmm25 = 110,
			xmm26 = 111,
			xmm27 = 112,
			xmm28 = 113,
			xmm29 = 114,
			xmm30 = 115,
			xmm31 = 116,
			ymm16 = 117,
			ymm17 = 118,
			ymm18 = 119,
			ymm19 = 120,
			ymm20 = 121,
			ymm21 = 122,
			ymm22 = 123,
			ymm23 = 124,
			ymm24 = 125,
			ymm25 = 126,
			ymm26 = 127,
			ymm27 = 128,
			ymm28 = 129,
			ymm29 = 130,
			ymm30 = 131,
			ymm31 = 132,
			zmm0 = 133,
			zmm1 = 134,
			zmm2 = 135,
			zmm3 = 136,
			zmm4 = 137,
			zmm5 = 138,
			zmm6 = 139,
			zmm7 = 140,
			zmm8 = 141,
			zmm9 = 142,
			zmm10 = 143,
			zmm11 = 144,
			zmm12 = 145,
			zmm13 = 146,
			zmm14 = 147,
			zmm15 = 148,
			zmm16 = 149,
			zmm17 = 150,
			zmm18 = 151,
			zmm19 = 152,
			zmm20 = 153,
			zmm21 = 154,
			zmm22 = 155,
			zmm23 = 156,
			zmm24 = 157,
			zmm25 = 158,
			zmm26 = 159,
			zmm27 = 160,
			zmm28 = 161,
			zmm29 = 162,
			zmm30 = 163,
			zmm31 = 164,
			k0 = 165,
			k1 = 166,
			k2 = 167,
			k3 = 168,
			k4 = 169,
			k5 = 170,
			k6 = 171,
			k7 = 172,
		};
	}


	/*static constexpr uint64_t encode_regdescriptor(unsigned bit_offset, unsigned bit_length, unsigned ida_register) {
		return static_cast<uint64_t>(bit_offset) | (static_cast<uint64_t>(bit_length) << 32) | (static_cast<uint64_t>(ida_register) << 48);
	}*/
	class register_file_builder_t;
	class contigreg_t {
		const char* m_name;
		
		struct {
			uint64_t m_bitoffs : 20;

			uint64_t m_bitlength : 10;
			uint64_t m_ida_register : 8;

		};
		//unsigned short m_bitlength;
		

		friend class register_file_builder_t;


	public:
		constexpr contigreg_t(const char* name, unsigned bit_offset, unsigned bit_length, unsigned ida_register) : m_name(name),
			m_bitoffs(bit_offset), m_bitlength(bit_length), m_ida_register(ida_register) {
		
		}
		constexpr contigreg_t() : m_name(""), m_bitoffs(0), m_bitlength(0), m_ida_register(0) {}
		

		constexpr contigreg_t make_subentity(const char* subent_name, unsigned displ, unsigned bit_length, unsigned ida_register) {
			return contigreg_t{ subent_name, (unsigned)m_bitoffs + displ, bit_length, ida_register };

		}

	};

	class register_file_builder_t {

		unsigned m_current_bit_offset;
		unsigned m_curr_contigreg;
		std::array< contigreg_t, TOTAL_CONTIGREGS> m_contigregs;

		unsigned m_curr_alias;
		std::array< contigreg_t, TOTAL_CONTIGREGS> m_contigreg_subents;

	public:
		constexpr register_file_builder_t() : m_current_bit_offset(0), m_curr_contigreg(0), m_contigregs({}), m_curr_alias(0), m_contigreg_subents({}) {}


		constexpr contigreg_t* find_contigreg_by_name(const char* name) {

			for (unsigned i = 0; i < m_curr_contigreg; ++i) {
				if (const_ops::cstreq(name, m_contigregs[i].m_name)) {
					return &m_contigregs[i];
				}
			}
			return nullptr;
		}

		
		constexpr void add_register(const char* name, unsigned bit_length, unsigned ida_reg) {
			m_contigregs[m_curr_contigreg++] = contigreg_t{ name, m_current_bit_offset, bit_length, ida_reg };

			m_current_bit_offset += bit_length;

		}

		constexpr void add_subreg(const char* master_name, const char* subname, unsigned displ, unsigned bitlength, unsigned idareg) {
			m_contigreg_subents[m_curr_alias++] = find_contigreg_by_name(master_name)->make_subentity(subname, displ, bitlength, idareg);
		}


		constexpr unsigned finalized_size() const {
			return m_curr_contigreg + m_curr_alias;
		}
		template<size_t n> 
		constexpr std::array<contigreg_t, n> finalize() const {
			std::array<contigreg_t, n> out_regs{};

			for (unsigned i = 0; i < m_curr_contigreg; ++i) {
				out_regs[i] = m_contigregs[i];
			}

			for (unsigned i = 0; i < m_curr_alias; ++i) {
				out_regs[i + m_curr_contigreg] = m_contigreg_subents[i];
			}
			return out_regs;
		}
	};

	template<void (*build_file)(register_file_builder_t&)>
	struct file_generator_t {
		static constexpr register_file_builder_t _initial() {
			register_file_builder_t res{};
			build_file(res);
			return res;
		}

		static constexpr register_file_builder_t _initial_build = _initial();

		static constexpr unsigned _finalized_sz = _initial_build.finalized_size();

		static constexpr auto register_file = _initial_build.finalize<_finalized_sz>();
	};

	
	constexpr void build_x86_register_file(register_file_builder_t& f) {
		namespace idr = ida_registers;
		f.add_register("RAX", 64, idr::rax);
		/*
			RAX = 0,
			RBX = 1,
			RCX = 2,
			RDX = 3,
			RBP = 4,
			RSP = 5,
			RSI = 6,
			RDI = 7,
			R8 = 8,
			R9 = 9,
			R10 = 10,
			R11 = 11,
			R12 = 12,
			R13 = 13,
			R14 = 14,
			R15 = 15,
			RIP = 16,
			RFLAGS = 17,
		
		*/

		f.add_subreg("RAX", "AH", 8, 8, idr::ah);

		f.add_register("RBX", 64, idr::rbx);
		f.add_subreg("RBX", "BH", 8, 8, idr::bh);

		f.add_register("RCX", 64, idr::rcx);
		f.add_subreg("RCX", "CH", 8, 8, idr::ch);
		
		f.add_register("RDX", 64, idr::rdx);
		f.add_subreg("RDX", "DH", 8, 8, idr::dh);

		f.add_register("RBP", 64, idr::rbp);

		f.add_register("RSP", 64, idr::rsp);
		f.add_register("RSI", 64, idr::rsi);
		f.add_register("RDI", 64, idr::rdi);
		f.add_register("R8", 64, idr::r8);
		f.add_register("R9", 64, idr::r9);
		f.add_register("R10", 64, idr::r10);
		f.add_register("R11", 64, idr::r11);
		f.add_register("R12", 64, idr::r12);
		f.add_register("R13", 64, idr::r13);
		f.add_register("R14", 64, idr::r14);
		f.add_register("R15", 64, idr::r15);
		f.add_register("RIP", 64, idr::rip);
		f.add_register("RFLAGS", 64, idr::rfl);


	}

	template<void (*build_file)(register_file_builder_t&)>
	static constexpr auto register_file_result = file_generator_t<build_file>::register_file;


	constexpr auto file = register_file_result< build_x86_register_file>;

}