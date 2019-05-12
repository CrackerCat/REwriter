#pragma once
#include "cs_core.hpp"
#include "cs_ida.hpp"
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
	template<typename... Ts>
	class cs_funcset_t {
		const std::array<cs_funcdescr_t, sizeof...(Ts)> m_arr;
	public:
		constexpr cs_funcset_t(Ts&& ... args) : m_arr({ args... })
		{}

		bool is_member(const char* name, unsigned length, unsigned hashcode) const {
			bool result = false;

			for (unsigned i = 0; i < sizeof...(Ts); ++i) {
				auto&& descr = m_arr[i];

				if (descr.length() == length && descr.hashcode() == hashcode) {
					if (!strcmp(descr.name(), name)) {
						result = true;
						break;
					}
				}
			}
			return result;
		}

	};
	template<typename... Ts>
	static constexpr auto make_funcset(Ts&& ...args) {
		return cs_funcset_t{ cs_funcdescr_t{args} ... };
	}

	enum class cs_funcclass_t : uint8_t{
#define FCLASS(name)		name,
#include "function_class_xmacro.hpp"

#undef FCLASS
		none
	};

	/*
	bool is_memory_allocation(const char* s, unsigned length, unsigned hashcode);

	bool is_byteswap64(const char* s, unsigned length, unsigned hashcode);*/

	cs_funcclass_t classify_call(insn_t* ins);

	const char* function_class_to_string(cs_funcclass_t claz);
}