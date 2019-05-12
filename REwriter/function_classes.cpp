#include "function_classes.hpp"

using cs::function_classes::make_funcset;
using cs::function_classes::cs_funcclass_t;

static constexpr auto g_funcset_memalloc = make_funcset("malloc", 
	"??2@YAPEAX_K@Z", 
	"callnewh", 
	"realloc", 
	"_callnewh",
	"qalloc",
	"qrealloc_or_throw",
	"HeapAlloc",
	"GlobalAlloc",
	"LocalAlloc",
	"VirtualAlloc",
	"HeapReAlloc",
	"calloc",
	"VirtualAllocEx"
	);
static constexpr auto g_funcset_memfree = make_funcset("free",
	"qfree",
	"LocalFree",
	"HeapFree",
	"GlobalFree",
	"VirtualFreeEx"
);
static constexpr auto g_funcset_byteswap64 = make_funcset("swap64");

static constexpr auto g_funcset_unhandeable_error = make_funcset("_invalid_parameter_noinfo_noreturn",
	"?_Xbad_alloc@std@@YAXXZ",
	"?_Xlength_error@std@@YAXPEBD@Z",
	"interr",

	"abort");
/*
bool cs::function_classes::is_memory_allocation(const char* s, unsigned length, unsigned hashcode) {
	return g_funcset_memalloc.is_member(s, length, hashcode);
}
bool cs::function_classes::is_byteswap64(const char* s, unsigned length, unsigned hashcode) {
	return g_funcset_byteswap64.is_member(s, length, hashcode);
}*/

cs_funcclass_t cs::function_classes::classify_call(insn_t* ins) {
	ea_t ad = ins->ops[0].addr;


	qstring name = get_name(ad);
	

	
	const char* _name = name.c_str();
	if (strstr(_name, "__imp_") == _name) {

		_name = &_name[sizeof("__imp_")-1];
	}
	//msg("%s\n", _name);
	unsigned length = (unsigned)name.length();

	unsigned hashcode = const_ops::hash_string(_name);

	auto tst = [_name, length, hashcode](const auto & iset) {
		return iset.is_member(_name, length, hashcode);
	};

	if (tst(g_funcset_memalloc)) {
		return cs_funcclass_t::memory_allocation;
	}
	if (tst(g_funcset_memfree)) {
		return cs_funcclass_t::memory_deallocation;
	}

	 if (tst(g_funcset_byteswap64)) {
		return cs_funcclass_t::byteswap64;
	}
	 if (tst(g_funcset_unhandeable_error)) {
		return cs_funcclass_t::unhandleable_error;
	}
	 insn_t thunkcheck{};
	 //resolve thunks
	 if (decode_insn(&thunkcheck, ad) != 0) {
		 while (thunkcheck.itype == NN_jmp && thunkcheck.ops[0].type == o_near) {
			 if (decode_insn(&thunkcheck, thunkcheck.ops[0].addr) == 0) {
				 return cs_funcclass_t::none;
			 }



		 }
		// msg("Recursing to resolve call.\n");
		 return classify_call(&thunkcheck);
	 }

	 return cs_funcclass_t::none;
}



const char* cs::function_classes::function_class_to_string(cs_funcclass_t claz) {

	switch (claz) {
#define		FCLASS(name)		case cs_funcclass_t::name: return #name;
#include "function_class_xmacro.hpp"

#undef FCLASS
	case cs_funcclass_t::none:
		return "unknown";
	};

}