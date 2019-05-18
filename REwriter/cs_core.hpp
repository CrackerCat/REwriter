#pragma once
#include <type_traits>
#include <cstdint>
#include <intrin.h>

#define		CS_RESTRICT		__restrict
#define		CS_NOINLINE		__declspec(noinline) 

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