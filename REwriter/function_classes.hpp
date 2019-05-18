#pragma once
#include "cs_core.hpp"
#include "cs_ida.hpp"
#include "mem_manager.hpp"
#include <array>

namespace cs::function_classes {

	class cs_funcdescr_t {
		const char* const m_name;
		const unsigned m_length;
		const unsigned m_hashcode;
	public:
		constexpr cs_funcdescr_t() : m_name(nullptr), m_length(0), m_hashcode(0)
		{}
		constexpr cs_funcdescr_t(const char* name) : m_name(name), m_length(const_ops::string_length(name)), m_hashcode(const_ops::hash_string(name))
		{}


		constexpr const char* name() const {
			return m_name;
		}

		constexpr unsigned length() const {
			return m_length;
		}

		constexpr unsigned hashcode() const {
			return m_hashcode;
		}


	};




	enum class cs_funcclass_t : uint8_t {
#define FCLASS(name)		name,
#include "function_class_xmacro.hpp"

#undef FCLASS
		none
	};

	struct cs_funcset_submit_t {
		unsigned m_len;
		unsigned m_hashcode;
		const char* m_name;
		inline bool operator <(cs_funcset_submit_t other)const {
			return m_len < other.m_len || m_hashcode < other.m_hashcode || strcmp(m_name, other.m_name) < 0;

		}
		//cs_funcclass_t m_cls;
	};

	using cs_funcclass_tree_t = std::map< cs_funcset_submit_t, cs_funcclass_t, std::less<cs_funcset_submit_t>>;//, 
		//cs::mem::low32_allocator<std::pair<const cs_funcset_submit_t, cs_funcclass_t>>>;

	template<typename... Ts>
	class cs_funcset_t {
		const std::array<cs_funcdescr_t, sizeof...(Ts)> m_arr;
		const cs_funcclass_t m_cls;
	public:
		constexpr cs_funcset_t(cs_funcclass_t cls, Ts&& ... args) : m_arr({ args... }), m_cls(cls)
		{}


		void add_to_tree(cs_funcclass_tree_t& tr) const {
			for (unsigned i = 0; i < sizeof...(Ts); ++i) {
				auto&& descr = m_arr[i];
				cs_funcset_submit_t sub{  descr.length(),descr.hashcode(),descr.name() };
				tr[sub] = m_cls;
			}
		}

	};
	template<typename... Ts>
	static constexpr auto make_funcset(cs_funcclass_t cls, Ts&& ...args) {
		return cs_funcset_t{ cls, cs_funcdescr_t{args} ... };
	}



	/*
	bool is_memory_allocation(const char* s, unsigned length, unsigned hashcode);

	bool is_byteswap64(const char* s, unsigned length, unsigned hashcode);*/

	cs_funcclass_t classify_call(insn_t* ins);

	const char* function_class_to_string(cs_funcclass_t claz);

	void init_class_tree();
	void deinit_class_tree();
}