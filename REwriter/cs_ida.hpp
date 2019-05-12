#pragma once

#include "idaheaders.hpp"
#include "cs_type_metaops.hpp"

static constexpr auto typelist_charlike = cs::meta_ops::make_typelist<char, unsigned char, std::byte>();
namespace cs {

	static inline bool is_addr_code(ea_t ea) {
		return is_code(get_flags(ea));
	}

	namespace x86regs {

		enum x86regs_t : uint8_t {
			RAX = 0,
			RCX=1,
			RDX=2,
			RBX=3,
			RSP=4,
			R8=8,
			R9=9,
			R10=10,
			R11=11,
			R12=12,
			AH=20,

			CF = 35,
			ZF = 36,
			SF = 37,
			OF = 38,
			PF = 39,
			DF = 43,
		};
	}


	namespace idb {

		template<typename T>
		static inline T at(ea_t address) {
			if constexpr (cs::meta_ops::typelist_has<T>(typelist_charlike)) {
				return get_byte(address);
			}
		}

	}
	class allrefs_to_iterator_t;
	template<ea_t (*func_start)(ea_t), ea_t(*func_next)(ea_t, ea_t)>
	struct ref_iterator_base_t {
		class ref_to_iterator_t {
			ea_t m_addr;
			ea_t m_current;
			friend class allrefs_to_iterator_t;
		public:
			ref_to_iterator_t(ea_t addr, ea_t current) : m_addr(addr), m_current(current) {}
			inline ea_t operator *() const &&{
				return m_current;
			}
			inline ea_t operator *() const & {
				return m_current;
			}
			inline ref_to_iterator_t& operator ++()  && {
				ea_t a = func_next(m_addr, m_current);
				m_current = a;
				return *this;
				//return ref_to_iterator_t{ m_addr, a };
			}
			inline ref_to_iterator_t& operator ++() & {
				ea_t a = func_next(m_addr, m_current);
				m_current = a;
				return *this;
				//return ref_to_iterator_t{ m_addr, a };
			}
			inline bool operator != (const ref_to_iterator_t& other) const &&{
				return m_current != other.m_current;
			}
			inline bool operator != (const ref_to_iterator_t& other) const & {
				return m_current != other.m_current;
			}
			inline ea_t target() const &&{
				return m_addr;
			}
			inline ea_t target() const & {
				return m_addr;
			}
		};

		class refs_to_t {
			const ea_t m_addr;
		public:
			inline refs_to_t(ea_t addr) : m_addr(addr) {}

			inline ref_to_iterator_t begin() const  {
				return ref_to_iterator_t{ m_addr, func_start(m_addr) };
			}

			inline ref_to_iterator_t end() const   {
				return ref_to_iterator_t{ m_addr, BADADDR };
			}
		};
	};

	using ref_iterator_base_code_t = ref_iterator_base_t<get_first_cref_to, get_next_cref_to>;

	using ref_iterator_base_data_t = ref_iterator_base_t<get_first_dref_to, get_next_dref_to>;

	static inline auto crefs_to(ea_t addr) {
		return ref_iterator_base_code_t::refs_to_t(addr);
			//crefs_to_t{ addr };
	}

	static inline auto drefs_to(ea_t addr) {
		return ref_iterator_base_data_t::refs_to_t(addr);
		//crefs_to_t{ addr };
	}

	class allrefs_to_iterator_t {
		mutable bool m_drefs;
		union {
			ref_iterator_base_data_t::ref_to_iterator_t m_dref_iterator;
			ref_iterator_base_code_t::ref_to_iterator_t m_cref_iterator;
		};
	public:
		inline allrefs_to_iterator_t(bool drefs, ea_t addr, ea_t current) : m_drefs(drefs) {
			if (drefs) {
				new(&m_dref_iterator)  ref_iterator_base_data_t::ref_to_iterator_t{ addr, current };
			}
			else {
				new(&m_cref_iterator)  ref_iterator_base_code_t::ref_to_iterator_t{ addr, current };
			}
		}
		inline allrefs_to_iterator_t(ref_iterator_base_data_t::ref_to_iterator_t iter) : allrefs_to_iterator_t(true, iter.target(), iter.operator*()) {}
		inline allrefs_to_iterator_t(ref_iterator_base_code_t::ref_to_iterator_t iter) : allrefs_to_iterator_t(false, iter.target(), iter.operator*()) {}
		inline ea_t operator *()  const  {
			return m_drefs ? m_dref_iterator.operator*() : m_cref_iterator.operator*();
		}
		inline allrefs_to_iterator_t operator ++()  {

			if (m_drefs) {
				m_dref_iterator.operator++();

				return *this;
			}
			else {
				m_cref_iterator.operator++();

				if (m_cref_iterator.operator*() == BADADDR) {


					auto bleh = drefs_to(m_cref_iterator.target()).begin();
					m_drefs = true;
					m_dref_iterator.m_addr = bleh.m_addr;
					m_dref_iterator.m_current = bleh.m_current;

					return *this;
				}
				else {
					return *this;
					//return allrefs_to_iterator_t{ nxt };
				}
			}
		}
		template<typename T>
		inline bool operator != (const T other) const {

			if (m_drefs == other.m_drefs) {

				if (!m_drefs) {
					return m_cref_iterator != other.m_cref_iterator;
				}
				else {
					return m_dref_iterator != other.m_dref_iterator;
				}

			}
			else {
				return true;
			}

			//return m_current != other.m_current;
		}
	
	};

	class allrefs_to_t {
		const ea_t m_addr;
	public:
		inline allrefs_to_t(ea_t addr) : m_addr(addr) {}
		inline allrefs_to_iterator_t&& begin() const  {
			auto beg = crefs_to(m_addr).begin();

			if (beg.operator*() == BADADDR) {
				return allrefs_to_iterator_t{  drefs_to(m_addr).begin()};
			}
			else {
				return allrefs_to_iterator_t{ beg };
			}
			
		}
		
		inline allrefs_to_iterator_t&& end() const  {
			return allrefs_to_iterator_t{true, m_addr, BADADDR};
		}

	};


	static inline auto&& allrefs_to(ea_t addr) {
		return allrefs_to_t{ addr };
	}


	struct func_iterator_t {
		ea_t m_addr;

		inline func_iterator_t(ea_t address) : m_addr(address) {}


		inline func_iterator_t& operator ++() {
			func_t* fn = get_next_func(m_addr);

			m_addr = fn ? fn->start_ea : BADADDR;
			return *this;
		}

		inline bool operator != (func_iterator_t other) const {
			return m_addr != other.m_addr;
		}

		inline ea_t operator *() const {
			return m_addr;
		}

	};

	struct func_iterator_master_t {

		inline func_iterator_t begin() {
			return func_iterator_t{ get_next_func(0)->start_ea };
		}
		inline func_iterator_t end() {
			return func_iterator_t{ BADADDR};
		}
	};

	static inline func_iterator_master_t forall_funcs() {
		return func_iterator_master_t();
	}
}

/*
#define CF_USE1 0x00100   ///< The instruction uses value of the first operand
#define CF_USE2 0x00200   ///< The instruction uses value of the second operand
#define CF_USE3 0x00400   ///< The instruction uses value of the third operand
#define CF_USE4 0x00800   ///< The instruction uses value of the 4 operand
#define CF_USE5 0x01000   ///< The instruction uses value of the 5 operand
#define CF_USE6 0x02000   ///< The instruction uses value of the 6 operand
*/

static inline bool has_cf_use(unsigned itype, unsigned n) {
	return has_insn_feature(itype, CF_USE1 << n);
}

static inline bool has_cf_chg(unsigned itype, unsigned n) {
	return has_insn_feature(itype, CF_CHG1 << n);
}

/*

#define dt_byte         0     ///< 8 bit
#define dt_word         1     ///< 16 bit
#define dt_dword        2     ///< 32 bit
#define dt_float        3     ///< 4 byte
#define dt_double       4     ///< 8 byte
#define dt_tbyte        5     ///< variable size (\ph{tbyte_size})
#define dt_packreal     6     ///< packed real format for mc68040
// ...to here the order should not be changed, see mc68000
#define dt_qword        7     ///< 64 bit
#define dt_byte16       8     ///< 128 bit
#define dt_code         9     ///< ptr to code (not used?)
#define dt_void         10    ///< none
#define dt_fword        11    ///< 48 bit
#define dt_bitfild      12    ///< bit field (mc680x0)
#define dt_string       13    ///< pointer to asciiz string
#define dt_unicode      14    ///< pointer to unicode string
#define dt_ldbl         15    ///< long double (which may be different from tbyte)
#define dt_byte32       16    ///< 256 bit
#define dt_byte64       17    ///< 512 bit
*/

constexpr uint8_t sizelut[] = {
	1,
	2,
	4,
	4,
	8,
	0xFF,
	0xFF,
	8,
	16,
	8,
	0xFF,
	6,
	0xff,
	8,
	8,
	10,
	32,
	64
};

static inline unsigned dtype_to_size(unsigned dtype) {
	return sizelut[dtype];
}

