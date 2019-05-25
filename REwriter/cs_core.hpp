#pragma once
#include <type_traits>
#include <cstdint>
#include <intrin.h>

#define		CS_RESTRICT		__restrict
#define		CS_NOINLINE		__declspec(noinline) 
#define		CS_NORETURN		__declspec(noreturn)

#define		CS_DATA_SEG(name)		__declspec(allocate(name))
#define		CS_CODE_SEG(name)		__declspec(code_seg(name)) 


#define		CS_COLD_CODE			CS_CODE_SEG(".cold")
namespace cs::const_ops {
	constexpr unsigned hash_string(const char* x) {
		unsigned long long int sumx = 0x811c9dc5;
		int carry = 0;
		unsigned ix = 0;
		for (; x[ix]; ++ix) {
			unsigned long long int oldsum = sumx;
			sumx += x[ix];
			if (oldsum > sumx)
				++carry;
			oldsum = sumx;
			sumx += (sumx << 1) + (sumx << 4) + (sumx << 7) + (sumx << 8) + (sumx << 24);
			if (oldsum > sumx)
				carry += 1;

		}
		return (unsigned)((sumx ^ (sumx >> 32)) + carry);
	}

	constexpr unsigned string_length(const char* s) {
		unsigned i = 0;

		for (; s[i]; ++i)
			;
		return i;
	}

	constexpr bool cstreq(const char* s1, const char* s2) {
		
		unsigned i = 0;
		for (; s1[i] == s2[i] && s1[i] != 0; ++i) {

		}

		return s1[i] == 0 && s2[i] == 0;
	}

	constexpr char _constexpr_assert(bool expr, const char* str, unsigned sz) {
		if (!expr) {
			throw str;
			
		}
		else {
			return 0;
		}
	}

#define cs_constexpr_assert(...)	cs::const_ops::_constexpr_assert((__VA_ARGS__), #__VA_ARGS__, sizeof(#__VA_ARGS__));
}
namespace cs {
	struct cs_assert_fail_descr_t {
		const char* _err_msg;
		const char* _file_name;
		unsigned line_no;
	};
	CS_COLD_CODE
	CS_NOINLINE
	CS_NORETURN
	void _cs_assert_fail_raise(const cs_assert_fail_descr_t* descr);

}
namespace cs::_cs_assert_impl {

	template<typename T, unsigned line_no>
	struct _cs_assert_fail {
		static constexpr cs_assert_fail_descr_t _descr{ T::__msg(), T::__file(), line_no };

		CS_NOINLINE
		CS_NORETURN
		CS_CODE_SEG(".assert_failures")
		static void raise_failure() {
			_cs_assert_fail_raise(&_descr);
		}

	};
}
#if 0
#define cs_assert(...)		\
	do {\
		static  const char* __assert_msg = #__VA_ARGS__;\
		static  const char* __assert_failure_file = __FILE__;\
		using __failure_handler = cs::_cs_assert_impl::_cs_assert_fail<&__assert_msg,&__assert_failure_file, __LINE__>;\
		if(!(__VA_ARGS__)) \
			__failure_handler::raise_failure();\
	}while(false)
#else
#define cs_assert(...)		\
	do {\
		class _arg_passer {\
		public:\
			static constexpr const char* __msg() {\
				return #__VA_ARGS__; \
			}\
			static constexpr const char* __file() {\
				return __FILE__;\
			}\
		};\
		using __failure_handler = cs::_cs_assert_impl::_cs_assert_fail<_arg_passer, __LINE__>;\
		if(!(__VA_ARGS__)) \
			__failure_handler::raise_failure();\
	}while(false)
#endif