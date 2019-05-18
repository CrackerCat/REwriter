#pragma once
#include "cs_core.hpp"
#include <array>
namespace cs::register_file {
	namespace _build_register_file {
		class register_file_builder_t;

	}
	//name, bitoffset, bitlength
	class contigreg_t {
		const char* m_name;

		struct {
			uint64_t m_bitoffs : 20;

			uint64_t m_bitlength : 10;
			uint64_t m_ida_register : 8;
			uint64_t m_index : 10;

		};


		friend class _build_register_file::register_file_builder_t;


	public:
		constexpr contigreg_t(const char* name, unsigned bit_offset, unsigned bit_length, unsigned ida_register) : m_name(name),
			m_bitoffs(bit_offset), m_bitlength(bit_length), m_ida_register(ida_register), m_index(0) {

		}
		constexpr contigreg_t() : m_name(""), m_bitoffs(0), m_bitlength(0), m_ida_register(0), m_index(0) {}


		constexpr contigreg_t make_subentity(const char* subent_name, unsigned displ, unsigned bit_length, unsigned ida_register) const {
			return contigreg_t{ subent_name, (unsigned)m_bitoffs + displ, bit_length, ida_register };
		}

		constexpr void set_index(unsigned v) {
			m_index = v;
		}

		constexpr unsigned index() const {
			return m_index;
		}

		constexpr const char* name() const {
			return m_name;
		}

		constexpr unsigned bitoffset() const {
			return m_bitoffs;
		}

		constexpr unsigned bitlength() const {
			return m_bitlength;
		}

		constexpr unsigned idareg() const {
			return m_ida_register;
		}


	};

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
			NIDAREGS
		};
	}
}

namespace cs::register_file::_build_register_file {

	


	
	
#if 0
	class register_file_builder_t {
		//more alias registers exist than whole registers

		static constexpr unsigned NAVAIL_ALIASES = TOTAL_CONTIGREGS*2;

		unsigned m_current_bit_offset;
		unsigned m_curr_contigreg;
		std::array< contigreg_t, TOTAL_CONTIGREGS> m_contigregs;

		unsigned m_curr_alias;
		std::array< contigreg_t, NAVAIL_ALIASES> m_contigreg_subents;

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
			//cs_constexpr_assert(m_curr_alias < NAVAIL_ALIASES);

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
				out_regs[i].set_index(i);
			}

			for (unsigned i = 0; i < m_curr_alias; ++i) {
				out_regs[i + m_curr_contigreg] = m_contigreg_subents[i];
				out_regs[i+m_curr_contigreg].set_index(i+m_curr_contigreg);
			}
			return out_regs;
		}
	};
#else 
	class register_file_builder_t {
		//more alias registers exist than whole registers

		static constexpr unsigned NAVAIL_ALIASES = TOTAL_CONTIGREGS * 2;
		/*
		unsigned m_current_bit_offset;
		unsigned m_curr_contigreg;
		std::array< contigreg_t, TOTAL_CONTIGREGS> m_contigregs;

		unsigned m_curr_alias;
		std::array< contigreg_t, NAVAIL_ALIASES> m_contigreg_subents;*/

		unsigned m_current_bit_offset;
		unsigned m_curr_contigreg;
		std::array< contigreg_t, TOTAL_CONTIGREGS*4> m_contigregs;


	public:
		constexpr register_file_builder_t() : m_current_bit_offset(0), m_curr_contigreg(0), m_contigregs({}) {}


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
			//cs_constexpr_assert(m_curr_alias < NAVAIL_ALIASES);

			m_contigregs[m_curr_contigreg++] = find_contigreg_by_name(master_name)->make_subentity(subname, displ, bitlength, idareg);
		}


		constexpr unsigned finalized_size() const {
			return m_curr_contigreg;
		}
		template<size_t n>
		constexpr std::array<contigreg_t, n> finalize() const {
			std::array<contigreg_t, n> out_regs{};

			for (unsigned i = 0; i < m_curr_contigreg; ++i) {
				out_regs[i] = m_contigregs[i];
				out_regs[i].set_index(i);
			}
			/*
			for (unsigned i = 0; i < m_curr_alias; ++i) {
				out_regs[i + m_curr_contigreg] = m_contigreg_subents[i];
				out_regs[i + m_curr_contigreg].set_index(i + m_curr_contigreg);
			}*/
			return out_regs;
		}
	};
#endif
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

	template<unsigned digits>
	constexpr void encode_zmmaliases(register_file_builder_t& f) {
		const char* zmmstr = nullptr, *ymmstr = nullptr, *xmmstr = nullptr;

		if constexpr (digits > 9) {

			constexpr char l = (char)((digits % 10) + '0');
			constexpr char h = (char)((digits / 10) + '0');

			constexpr char _zmm[] = { 'z', 'm', 'm', h, l, 0 };
			constexpr char _ymm[] = { 'y', 'm', 'm',  h, l, 0 };
			constexpr char _xmm[] = { 'x', 'm', 'm',  h, l, 0 };
			zmmstr = _zmm;
			ymmstr = _ymm;
			xmmstr = _xmm;
		}
		else {
			constexpr char l = (char)((digits % 10) + '0');
			constexpr char _zmm[] = { 'z', 'm', 'm', l, 0 };
			constexpr char _ymm[] = { 'y', 'm', 'm', l, 0 };
			constexpr char _xmm[] = { 'x', 'm', 'm', l, 0 };
			zmmstr = _zmm;
			ymmstr = _ymm;
			xmmstr = _xmm;
		}

		f.add_register(zmmstr, 512, ida_registers::zmm0 + digits);

		unsigned ymmt = 0, xmmt = 0;

		if (digits < 16) {
			ymmt = ida_registers::ymm0 + digits;
			xmmt = ida_registers::xmm0 + digits;

		}
		else {
			ymmt = ida_registers::ymm16 + (digits-16);
			xmmt = ida_registers::xmm16 + (digits-16);
		}
	
		f.add_subreg(zmmstr, ymmstr, 0, 256, ymmt);
		f.add_subreg(zmmstr, xmmstr, 0, 128, xmmt);
		
		

	}
	constexpr void build_x86_register_file(register_file_builder_t& f) {
		namespace idr = ida_registers;
		f.add_register("RAX", 64, idr::rax);

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

		auto add_rflag = [&f](const char* name, unsigned offs, unsigned idareg) {
			f.add_subreg("RFLAGS", name, offs, 1, idareg);
		};
		

		add_rflag("CF", 0, idr::cf);
		add_rflag("PF", 2, idr::pf);

		add_rflag("AF", 4, idr::af);
		add_rflag("ZF", 6, idr::zf);
		add_rflag("SF", 7, idr::sf);

		add_rflag("TF", 8, idr::tf);
		add_rflag("IF", 9, idr::if_);
		add_rflag("DF", 10, idr::df);
		add_rflag("OF", 11, idr::of);

		auto add_segreg = [&f](const char* name, unsigned idareg) {
			f.add_register(name, 64, idareg);
		};
		
		add_segreg("ES", idr::res);
		add_segreg("CS", idr::rcs);
		add_segreg("SS", idr::rss);
		add_segreg("DS", idr::rds);
		add_segreg("FS", idr::rfs);
		add_segreg("GS", idr::rgs);



		auto add_x87reg = [&f](const char* name, unsigned idareg) {
			f.add_register(name, 80, idareg);
		};
#define X87R(n)		add_x87reg("ST" #n, idr::st##n)
		X87R(0);
		X87R(1);
		X87R(2);
		X87R(3);
		X87R(4);
		X87R(5);
		X87R(6);
		X87R(7);
#undef X87R

		f.add_register("FPTAGS", 8, idr::fptags);
		f.add_register("FPCTRL", 16, idr::fpctrl);
		f.add_register("FPSTATUS", 16, idr::fpstat);
		//causes an internal error in msvc so disabled for now
#if 0
		encode_zmmaliases<0>(f);
		encode_zmmaliases<1>(f);
		encode_zmmaliases<2>(f);
		encode_zmmaliases<3>(f);
		encode_zmmaliases<4>(f);
		encode_zmmaliases<5>(f);
		encode_zmmaliases<6>(f);
		encode_zmmaliases<7>(f);
		encode_zmmaliases<8>(f);
		encode_zmmaliases<9>(f);
		encode_zmmaliases<10>(f);
		encode_zmmaliases<11>(f);
		encode_zmmaliases<12>(f);
		encode_zmmaliases<13>(f);
		encode_zmmaliases<14>(f);
		encode_zmmaliases<15>(f);
		encode_zmmaliases<16>(f);
		encode_zmmaliases<17>(f);
		encode_zmmaliases<18>(f);
		encode_zmmaliases<19>(f);
		encode_zmmaliases<20>(f);
		encode_zmmaliases<21>(f);
		encode_zmmaliases<22>(f);
		encode_zmmaliases<23>(f);
		encode_zmmaliases<24>(f);
		encode_zmmaliases<25>(f);
		encode_zmmaliases<26>(f);
		encode_zmmaliases<27>(f);
		encode_zmmaliases<28>(f);
		encode_zmmaliases<29>(f);
		encode_zmmaliases<30>(f);
		encode_zmmaliases<31>(f);


	
#else
		/*
		pre-generated in chrome dev console with 
		lel=""
		for(let i = 0; i < 32; ++i) lel+=`f.add_register("ZMM${i}", 512, idr::zmm${i});\nf.add_subreg("ZMM${i}","YMM${i}", 0, 256, idr::ymm${i});\nf.add_subreg("ZMM${i}","XMM${i}", 0, 128, idr::xmm${i});\n`
		rip fancy template magix
		*/
		f.add_register("ZMM0", 512, idr::zmm0);
		f.add_subreg("ZMM0", "YMM0", 0, 256, idr::ymm0);
		f.add_subreg("ZMM0", "XMM0", 0, 128, idr::xmm0);
		f.add_register("ZMM1", 512, idr::zmm1);
		f.add_subreg("ZMM1", "YMM1", 0, 256, idr::ymm1);
		f.add_subreg("ZMM1", "XMM1", 0, 128, idr::xmm1);
		f.add_register("ZMM2", 512, idr::zmm2);
		f.add_subreg("ZMM2", "YMM2", 0, 256, idr::ymm2);
		f.add_subreg("ZMM2", "XMM2", 0, 128, idr::xmm2);
		f.add_register("ZMM3", 512, idr::zmm3);
		f.add_subreg("ZMM3", "YMM3", 0, 256, idr::ymm3);
		f.add_subreg("ZMM3", "XMM3", 0, 128, idr::xmm3);
		f.add_register("ZMM4", 512, idr::zmm4);
		f.add_subreg("ZMM4", "YMM4", 0, 256, idr::ymm4);
		f.add_subreg("ZMM4", "XMM4", 0, 128, idr::xmm4);
		f.add_register("ZMM5", 512, idr::zmm5);
		f.add_subreg("ZMM5", "YMM5", 0, 256, idr::ymm5);
		f.add_subreg("ZMM5", "XMM5", 0, 128, idr::xmm5);
		f.add_register("ZMM6", 512, idr::zmm6);
		f.add_subreg("ZMM6", "YMM6", 0, 256, idr::ymm6);
		f.add_subreg("ZMM6", "XMM6", 0, 128, idr::xmm6);
		f.add_register("ZMM7", 512, idr::zmm7);
		f.add_subreg("ZMM7", "YMM7", 0, 256, idr::ymm7);
		f.add_subreg("ZMM7", "XMM7", 0, 128, idr::xmm7);
		f.add_register("ZMM8", 512, idr::zmm8);
		f.add_subreg("ZMM8", "YMM8", 0, 256, idr::ymm8);
		f.add_subreg("ZMM8", "XMM8", 0, 128, idr::xmm8);
		f.add_register("ZMM9", 512, idr::zmm9);
		f.add_subreg("ZMM9", "YMM9", 0, 256, idr::ymm9);
		f.add_subreg("ZMM9", "XMM9", 0, 128, idr::xmm9);
		f.add_register("ZMM10", 512, idr::zmm10);
		f.add_subreg("ZMM10", "YMM10", 0, 256, idr::ymm10);
		f.add_subreg("ZMM10", "XMM10", 0, 128, idr::xmm10);
		f.add_register("ZMM11", 512, idr::zmm11);
		f.add_subreg("ZMM11", "YMM11", 0, 256, idr::ymm11);
		f.add_subreg("ZMM11", "XMM11", 0, 128, idr::xmm11);
		f.add_register("ZMM12", 512, idr::zmm12);
		f.add_subreg("ZMM12", "YMM12", 0, 256, idr::ymm12);
		f.add_subreg("ZMM12", "XMM12", 0, 128, idr::xmm12);
		f.add_register("ZMM13", 512, idr::zmm13);
		f.add_subreg("ZMM13", "YMM13", 0, 256, idr::ymm13);
		f.add_subreg("ZMM13", "XMM13", 0, 128, idr::xmm13);
		f.add_register("ZMM14", 512, idr::zmm14);
		f.add_subreg("ZMM14", "YMM14", 0, 256, idr::ymm14);
		f.add_subreg("ZMM14", "XMM14", 0, 128, idr::xmm14);
		f.add_register("ZMM15", 512, idr::zmm15);
		f.add_subreg("ZMM15", "YMM15", 0, 256, idr::ymm15);
		f.add_subreg("ZMM15", "XMM15", 0, 128, idr::xmm15);
		f.add_register("ZMM16", 512, idr::zmm16);
		f.add_subreg("ZMM16", "YMM16", 0, 256, idr::ymm16);
		f.add_subreg("ZMM16", "XMM16", 0, 128, idr::xmm16);
		f.add_register("ZMM17", 512, idr::zmm17);
		f.add_subreg("ZMM17", "YMM17", 0, 256, idr::ymm17);
		f.add_subreg("ZMM17", "XMM17", 0, 128, idr::xmm17);
		f.add_register("ZMM18", 512, idr::zmm18);
		f.add_subreg("ZMM18", "YMM18", 0, 256, idr::ymm18);
		f.add_subreg("ZMM18", "XMM18", 0, 128, idr::xmm18);
		f.add_register("ZMM19", 512, idr::zmm19);
		f.add_subreg("ZMM19", "YMM19", 0, 256, idr::ymm19);
		f.add_subreg("ZMM19", "XMM19", 0, 128, idr::xmm19);
		f.add_register("ZMM20", 512, idr::zmm20);
		f.add_subreg("ZMM20", "YMM20", 0, 256, idr::ymm20);
		f.add_subreg("ZMM20", "XMM20", 0, 128, idr::xmm20);
		f.add_register("ZMM21", 512, idr::zmm21);
		f.add_subreg("ZMM21", "YMM21", 0, 256, idr::ymm21);
		f.add_subreg("ZMM21", "XMM21", 0, 128, idr::xmm21);
		f.add_register("ZMM22", 512, idr::zmm22);
		f.add_subreg("ZMM22", "YMM22", 0, 256, idr::ymm22);
		f.add_subreg("ZMM22", "XMM22", 0, 128, idr::xmm22);
		f.add_register("ZMM23", 512, idr::zmm23);
		f.add_subreg("ZMM23", "YMM23", 0, 256, idr::ymm23);
		f.add_subreg("ZMM23", "XMM23", 0, 128, idr::xmm23);
		f.add_register("ZMM24", 512, idr::zmm24);
		f.add_subreg("ZMM24", "YMM24", 0, 256, idr::ymm24);
		f.add_subreg("ZMM24", "XMM24", 0, 128, idr::xmm24);
		f.add_register("ZMM25", 512, idr::zmm25);
		f.add_subreg("ZMM25", "YMM25", 0, 256, idr::ymm25);
		f.add_subreg("ZMM25", "XMM25", 0, 128, idr::xmm25);
		f.add_register("ZMM26", 512, idr::zmm26);
		f.add_subreg("ZMM26", "YMM26", 0, 256, idr::ymm26);
		f.add_subreg("ZMM26", "XMM26", 0, 128, idr::xmm26);
		f.add_register("ZMM27", 512, idr::zmm27);
		f.add_subreg("ZMM27", "YMM27", 0, 256, idr::ymm27);
		f.add_subreg("ZMM27", "XMM27", 0, 128, idr::xmm27);
		f.add_register("ZMM28", 512, idr::zmm28);
		f.add_subreg("ZMM28", "YMM28", 0, 256, idr::ymm28);
		f.add_subreg("ZMM28", "XMM28", 0, 128, idr::xmm28);
		f.add_register("ZMM29", 512, idr::zmm29);
		f.add_subreg("ZMM29", "YMM29", 0, 256, idr::ymm29);
		f.add_subreg("ZMM29", "XMM29", 0, 128, idr::xmm29);
		f.add_register("ZMM30", 512, idr::zmm30);
		f.add_subreg("ZMM30", "YMM30", 0, 256, idr::ymm30);
		f.add_subreg("ZMM30", "XMM30", 0, 128, idr::xmm30);
		f.add_register("ZMM31", 512, idr::zmm31);
		f.add_subreg("ZMM31", "YMM31", 0, 256, idr::ymm31);
		f.add_subreg("ZMM31", "XMM31", 0, 128, idr::xmm31);
		auto add_kmaskreg = [&f](const char* name, unsigned idareg) {
			f.add_register(name, 64, idareg);
		};

		add_kmaskreg("K0", idr::k0);
		add_kmaskreg("K1", idr::k1);
		add_kmaskreg("K2", idr::k2);
		add_kmaskreg("K3", idr::k3);
		add_kmaskreg("K4", idr::k4);
		add_kmaskreg("K5", idr::k5);
		add_kmaskreg("K6", idr::k6);
		add_kmaskreg("K7", idr::k7);


		f.add_register("MXCSR", 32, idr::mxcsr);
#endif
	}
	/*
	template<void (*build_file)(register_file_builder_t&)>
	static constexpr auto register_file_result = file_generator_t<build_file>::register_file;

	*/

	constexpr unsigned register_file_x86_64_length = file_generator_t<build_x86_register_file>::_finalized_sz;


	static constexpr std::array<contigreg_t, register_file_x86_64_length> register_file_x86_64 = file_generator_t<build_x86_register_file>::register_file;


}

namespace cs::register_file {

	
	/*
		we put all runtime ops involving the register file in the cpp file to avoid multiple tables being created
		and only use the constexpr register file model in constant expressions everywhere else
	*/

	const contigreg_t* idareg_to_contigreg(unsigned idr);




}