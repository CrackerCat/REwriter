#include "cs_core.hpp"
#include <Windows.h>

CS_COLD_CODE
CS_NOINLINE
CS_NORETURN
void cs::_cs_assert_fail_raise(const cs_assert_fail_descr_t* descr) {


	MessageBoxA(nullptr, (descr->_err_msg), "Assertion failed", MB_ICONERROR);
	if (IsDebuggerPresent()) {
		__debugbreak();
	}

	exit(1);

}