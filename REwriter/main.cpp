#include "cs_core.hpp"
#include "idaheaders.hpp"

#include <Windows.h>
#include "xfunc.hpp"
#include "mem_manager.hpp"

#include "cs_type_metaops.hpp"

void eliminate_security_check_cookie() {
	unsigned nelims_cookie = 0;
	unsigned nelim_guard_disp = 0;
	unsigned ntrivial_inlines = 0;
	unsigned ncondbranch_elims = 0;
	for (auto func : cs::forall_funcs()) {
		csfunc_t fn{};
		if (csfunc_t::decode(&fn, func) == -1) {
			continue;
		}
#if 1
		for (csbb_t* bb = fn.firstb(); bb; bb = bb->nextb()) {
			
			for (csinsn_t* ins = bb->head(); ins; ins = ins->next()) {
				if (ins->is_security_check_cookie()) {
					auto prv = ins->prev();
					if (prv && prv->itype == NN_xor && prv->ops[0].is_reg(cs::x86regs::RCX) && prv->ops[1].is_reg(cs::x86regs::RSP)) {
						cs::patchops::nop_ea(prv->ea, prv->size + ins->size);
					}
					else {
						ins->nop_out();
					}
					nelims_cookie++;

				}
				else if (ins->is_call_to_fn("__guard_dispatch_icall_fptr")) {
					ins->replace_with_byteseq<0xFF, 0xd0>();
					nelim_guard_disp++;
				}
			//	else if (ins->may_trivially_inline_call()) {


					//msg("Located trivially inlinable function call at 0x%llX\n", ins->ea);
				//	ntrivial_inlines++;
				//}

			
			}

			

		}
#else
		for (csbb_t* bb = fn.firstb(); bb; bb = bb->nextb()) {
			auto bbtail = bb->tail();
			if (bb->is_error_endpoint()) {

				for (auto&& pred : bb->predecessors()) {

					auto tail = pred->tail();

					if (tail->is_conditional_branch() && tail->op_get_known_address(0) != bb->startea() && tail->may_make_branch_unconditional()) {
						ncondbranch_elims++;
						tail->make_unconditional_branch();
						//msg("Made branch unconditional at 0x%llX\n", (uint64_t)tail->ea);
					}
					/*
					else if (tail->is_conditional_branch() && tail->op_get_known_address(0) == bb->startea()) {
						tail->nop_out();
						//	msg("Made fallthrough unconditional at 0x%llX\n", (uint64_t)tail->ea);
						ncondbranch_elims++;
					}*/
				}
			}

		}
#endif
	}
	msg("Eliminated %d security cookie checks.\n", nelims_cookie);
	msg("Eliminated %d __guard_dispatch_icall_fptr calls.\n", nelim_guard_disp);
	msg("Eliminated %d conditional branches to unhandleable error functions\n", ncondbranch_elims);
	msg("Located %d potential trivial inlined function calls.\n", ntrivial_inlines);
}

unsigned eliminate_null_pointer_allocation_result_checks(ea_t address) {
	csfunc_t result{};
	csreglist_t templist{};

	using cs::function_classes::cs_funcclass_t;
	if (csfunc_t::decode(&result, address) == -1) {

		return 0;
	}
	unsigned n_elims = 0;
	for (csbb_t* bb = result.firstb(); bb; bb = bb->nextb()) {

		auto tail = bb->tail();

		if (!tail->has_role(cs_funcclass_t::memory_allocation))
			continue;

		csbb_t* fallthrough = result.get_fallthrough(bb);


		auto raxuse = fallthrough->find_use_p(nullptr, cs::x86regs::RAX, &templist, [](csinsn_t * ins) {
			return ins->is_possible_zero_test();
		});
		if (!raxuse) {
			continue;
		}
		//msg("First use is a zero test.\n");

		csinsn_t* hopefully_jzx = fallthrough->find_next_use(raxuse, cs::x86regs::ZF, &templist);

		if (!hopefully_jzx) {
			//msg("Couldn't trace ZF :(\n");
			continue;
		}

		if (hopefully_jzx != fallthrough->tail() || !hopefully_jzx->is_conditional_branch()) {
			continue;
		}

		if (hopefully_jzx->itype != NN_jnz && hopefully_jzx->itype != NN_jz) {
			continue;
		}

		if (hopefully_jzx->itype == NN_jz) {
			hopefully_jzx->nop_out();
		}
		else if (hopefully_jzx->itype == NN_jnz) {
			hopefully_jzx->make_unconditional_branch();
		}

		raxuse->nop_out();
		n_elims++;

		//msg("Got branch to nullboi at 0x%llX!!\n", hopefully_jzx->ea);



		/*for (csinsn_t* ins = bb->head(); ins; ins = ins->next()) {

		}*/
	}
	return n_elims;
}

bool idaapi run(size_t)
{
#if 0
	ea_t ea = get_screen_ea();

	csfunc_t result{};

	if (csfunc_t::decode(&result, ea) == -1) {
		
		return true;
	}
	
	for (csbb_t* bb = result.firstb(); bb; bb = bb->nextb()) {
		
		//bb->regenerate_lists();
		bb->dump_lists();
		for (csinsn_t* ins = bb->head(); ins; ins = ins->next()) {

			qstring istr = ins->to_string(true);

			msg("\t%s\n", istr.c_str());

			if (ins->is_conditional_branch()) {
				csreglist_t cond_uses{};

				ins->gen_use_list(&cond_uses);
				csreglist_t temp{};

				//for (auto&& reg : cond_uses) 
				for(unsigned regidx = 0; regidx < NREGS; ++regidx)
				{
					if (!cond_uses.has(regidx)) {
						continue;
					}
					auto def = bb->find_definition_backwards(ins, regidx, &temp);

					if (def) {
						qstring fk{};

						get_reg_name(&fk, regidx, 1);

						msg("Flag use %s defined at 0x%llX\n", fk.c_str(), def->ea);
					}
				}

			}
		}
	}



	//result.print();

	return true;
#else
	eliminate_security_check_cookie();
	unsigned elimed_checks = 0;
	for (auto func : cs::forall_funcs()) {
		elimed_checks += eliminate_null_pointer_allocation_result_checks(func);
	}

	msg("Eliminated %d null pointer after alloc checks.\n", elimed_checks);
	//eliminate_security_check_cookie();
	return true;
#endif
}
//--------------------------------------------------------------------------
int idaapi init(void)
{
	cs::mem::init_memmanager();
	return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
	cs::mem::shutdown_memmanager();
}

//--------------------------------------------------------------------------
static const char help[] = "REwriter";
static const char comment[] = "REwriter";
static const char wanted_name[] = "REwriter";
static const char wanted_hotkey[] = "";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  0,          // plugin flags
  init,                 // initialize

  term,                 // terminate. this pointer may be NULL.

  run,                  // invoke plugin

  nullptr,              // long comment about the plugin
						// it could appear in the status line
						// or as a hint

  nullptr,                 // multiline help about the plugin

  wanted_name,          // the preferred short name of the plugin
  "Ctrl+1"         // the preferred hotkey to run the plugin
};
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return true;
}