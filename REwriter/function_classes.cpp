#include "function_classes.hpp"

using cs::function_classes::make_funcset;
using cs::function_classes::cs_funcclass_t;


static cs::function_classes::cs_funcclass_tree_t* g_functree = nullptr;

static constexpr auto g_funcset_memalloc = make_funcset(cs_funcclass_t::memory_allocation, "malloc", 
	"??2@YAPEAX_K@Z", 
	"??_U@YAPEAX_K@Z",
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
	"VirtualAllocEx",
	"xmalloc",
	"xcalloc",
	"xrealloc"
	);

static constexpr auto g_funcset_memfree = make_funcset(cs_funcclass_t::memory_deallocation, "free",
	"qfree",
	"LocalFree",
	"HeapFree",
	"GlobalFree",
	"VirtualFreeEx",
	"??3@YAXPEAX_K@Z",
	"??3@YAXPEAX@Z"
);

static constexpr auto g_funcset_byteswap64 = make_funcset(cs_funcclass_t::byteswap64, "swap64");

static constexpr auto g_funcset_unhandeable_error = make_funcset(cs_funcclass_t::unhandleable_error, "_invalid_parameter_noinfo_noreturn",
	"?_Xbad_alloc@std@@YAXXZ",
	"?_Xlength_error@std@@YAXPEBD@Z",
	"interr",

	"abort");

static constexpr auto g_funcset_cxx_dyncast = make_funcset(cs_funcclass_t::cxx_dynamic_cast,
	"__RTDynamicCast");

static constexpr auto g_funcset_cxx_throw = make_funcset(cs_funcclass_t::cxx_throw_exception,
	"_CxxThrowException");


template<auto&& ... whatevs> struct build_classtree {

	template<auto&& x, auto&&... ys>
	static void build() {

		x.add_to_tree(*g_functree);
		if constexpr (sizeof...(ys)) {
			build<ys...>();
		}
	}

	static void build_full() {
		build<whatevs...>();
	}
};
using classtree_builder = build_classtree< g_funcset_memalloc,
	g_funcset_memfree,
	g_funcset_byteswap64,
	g_funcset_unhandeable_error,
	g_funcset_cxx_dyncast,
	g_funcset_cxx_throw>;

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
	/*
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
	}*/
	cs_funcset_submit_t key;
	key.m_hashcode = hashcode;
	key.m_len = length;
	key.m_name = _name;
	auto iter = g_functree->find(key);

	if (iter != g_functree->end()) {
		return iter->second;
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
void cs::function_classes::init_class_tree() {
	g_functree = new cs_funcclass_tree_t();
	classtree_builder::build_full();
}
void cs::function_classes::deinit_class_tree() {
	delete g_functree;
}