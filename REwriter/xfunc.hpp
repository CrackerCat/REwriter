#pragma once

#include "x86_64_ops.hpp"
#include "cs_ida.hpp"
#include "reglist.hpp"
#include <set>
#include "patchops.hpp"

#include "function_classes.hpp"
class csinsn_t;
class csbb_t;

enum {
	INSN_FLAG_IS_CALL = 1,
	INSN_FLAG_IS_RET = 2
};




class csinsn_t : public insn_t {
	
	
	csinsn_t* CS_RESTRICT m_next;
	csinsn_t* CS_RESTRICT m_prev;
	uint64_t m_exflags;
	cs_x86_op_e m_norm_op;
	uint8_t m_operand_count;
	cs::function_classes::cs_funcclass_t m_call_class;

	void compute_operand_count() {
		for (unsigned i = 0; i < UA_MAXOP; ++i) {
			if (ops[i].type == o_void) {
				m_operand_count = i;
				return;
			}
		}
		m_operand_count = UA_MAXOP;
	}

public:
	csinsn_t() : 
		insn_t::insn_t(), 
		m_next(nullptr), 
		m_prev(nullptr), 
		m_exflags(0ULL), 
		m_norm_op(NN_S_BADOP), 
		m_operand_count(0xFF), 
		m_call_class(cs::function_classes::cs_funcclass_t::none){}

	bool is_conditional_branch() const {
		return opcode_classes::is_conditional_branch(m_norm_op);
	}


	csinsn_t* next() {
		return m_next;
	}

	csinsn_t* prev() {
		return m_prev;
	}
	bool is_flag_based_conditional_branch() const {
		return opcode_classes::is_flag_based_conditional_branch(m_norm_op);
	}

	bool is_gpreg_cmov() const {
		return opcode_classes::is_gpreg_cmov(m_norm_op);
	}

	void linkin_prev(csinsn_t* ins) {
		m_prev = ins;
		ins->m_next = this;
	}

	void linkin_next(csinsn_t* ins) {
		m_next = ins;
		ins->m_prev = this;
	}

	func_t* callee_func() const {
		if (!is_call()) {
			return nullptr;
		}
		else {
			if (!op_is_known_address(0)) {
				return nullptr;
			}
			else {
				func_t* fn = get_func(op_get_known_address(0));
				return fn;
			}
		}
	}
	/*
		return INT_MAX to indicate error
		meant for inline cost estimation, INT_MAX will ensure it gets skipped safely without any
		changes to the logic
	*/
	int callee_size() const {
		if (!is_call()) {
			return INT_MAX;
		}
		else {
			if (!op_is_known_address(0)) {
				return INT_MAX;
			}
			else {
				func_t* fn = get_func(op_get_known_address(0));

				if (!fn) {
					return INT_MAX;
				}

				return fn->end_ea - fn->start_ea;
			}
		}
	}
	/*
		- 1 for retn 0xC3
	*/
	bool speculative_callee_fits_in_call() const {
		return (callee_size() - 1) <= size;
	}


	bool may_trivially_inline_call() const;

	int decode(ea_t ea) {
		
		if (!cs::is_addr_code(ea)) {

			if (cs::idb::at<uint8_t>(ea) != 0xCC) {
				return -1;
			}

			
		}
		auto decode_res = decode_insn(this, ea);

		if (decode_res  == 0) {
			return -1;
		}
		cs_x86_op_e norm_op = remap_x86(this->itype);
		//unsupported opcode
		if (norm_op == NN_S_BADOP) {
			return -1;
		}
		m_norm_op = norm_op;
		compute_operand_count();
		m_exflags |= is_call_insn(*this)  ? INSN_FLAG_IS_CALL : 0;

		m_exflags |= is_ret_insn(*this, false) ? INSN_FLAG_IS_RET : 0;
		

		if (is_call() && op_is_known_address(0)) {
			m_call_class = cs::function_classes::classify_call(this);
		}

		

		return decode_res;

	}

	bool is_call() const {
		return (m_exflags & INSN_FLAG_IS_CALL) != 0;
	}

	bool is_ret() const {
		return (m_exflags & INSN_FLAG_IS_RET) != 0;
	}

	bool is_int3() const {
		return m_norm_op == NN_S_int3;
	}
	bool is_absolute_end_of_flow() const {
		if (is_int3()) {
			return true;
		}
		else {
			return is_call_insn(*this) && !func_does_return(this->ops[0].addr);
		}
	}


	ea_t altflow() const {
		if (ops[0].type != o_near) {
			return BADADDR;
		}
		return ops[0].addr;
	}


	void gen_use_list(csreglist_t* CS_RESTRICT out) CS_RESTRICT {

		if (opcode_classes::uses_carry_flag(m_norm_op)) {
			out->add(cs::x86regs::CF, 1);
		}

		if (opcode_classes::uses_sign_flag(m_norm_op)) {
			out->add(cs::x86regs::SF, 1);
		}

		if (opcode_classes::uses_zero_flag(m_norm_op)) {
			out->add(cs::x86regs::ZF, 1);
		}

		if (opcode_classes::uses_overflow_flag(m_norm_op)) {
			out->add(cs::x86regs::OF, 1);
		}
		if (opcode_classes::uses_parity_flag(m_norm_op)) {
			out->add(cs::x86regs::PF, 1);
		}

		if (opcode_classes::potentially_has_nonflag_hidden_operands(m_norm_op)) {
			opcode_classes::get_hidden_usedefs(this, out);
		}
		unsigned n_ops = m_operand_count;
		for (unsigned i = 0; i < n_ops; ++i) {



			//if (!)
			//	continue;
			if (ops[i].type == o_reg && has_cf_use(itype, i)) {
				out->add(ops[i].reg, dtype_to_size(ops[i].dtype));
			}
			else if (ops[i].type == o_phrase || ops[i].type == o_displ) {
				out->add(ops[i].phrase, 8);
			}
		}
	}

	void gen_def_list(csreglist_t* CS_RESTRICT out) {
		
		if (opcode_classes::spoils_all_baseflags(m_norm_op)) {
			out->add(cs::x86regs::CF, 1);
			out->add(cs::x86regs::SF, 1);
			out->add(cs::x86regs::ZF, 1);
			out->add(cs::x86regs::OF, 1);
			out->add(cs::x86regs::PF, 1);

		}
		else {
			if (opcode_classes::spoils_carry_flag(m_norm_op)) {
				out->add(cs::x86regs::CF, 1);
			}

			if (opcode_classes::spoils_sign_flag(m_norm_op)) {
				out->add(cs::x86regs::SF, 1);
			}

			if (opcode_classes::spoils_zero_flag(m_norm_op)) {
				out->add(cs::x86regs::ZF, 1);
			}


			if (opcode_classes::spoils_overflow_flag(m_norm_op)) {
				out->add(cs::x86regs::OF, 1);
			}
			if (opcode_classes::spoils_parity_flag(m_norm_op)) {
				out->add(cs::x86regs::PF, 1);
			}
		}

		if (opcode_classes::potentially_has_nonflag_hidden_operands(m_norm_op)) {
			opcode_classes::get_hidden_usedefs(this, nullptr, out);
		}
		unsigned n_ops = m_operand_count;
		for (unsigned i = 0; i < n_ops; ++i) {



			if (!has_cf_chg(itype, i))
				continue;

			if (ops[i].type == o_reg) {
				

				out->add(ops[i].reg, dtype_to_size(ops[i].dtype));
				
			}


			
		}
	}

	bool op_is_known_address(unsigned opidx)const {
		return ops[opidx].type == o_near || ops[opidx].type == o_mem;
	}

	ea_t op_get_known_address(unsigned opidx) const {
		return ops[opidx].addr;
	}
	bool is_call_to_fn(const char* name_) {
		if (is_call()) {
			if (op_is_known_address(0)) {

				auto name = get_name(op_get_known_address(0), 0);
				if (name == name_) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename T, typename... Ts>
	inline bool is_call_to_any_fn(T name, Ts... names) {
		
		if (is_call_to_fn(name)) {
			return true;
		}

		if constexpr (sizeof...(Ts) == 0) {
			return false;
		}
		else {
			return is_call_to_any_fn(names...);
		}
	}
	bool is_security_check_cookie() {
		return is_call_to_fn("__security_check_cookie");
	}


	size_t may_eliminate() {

		if (is_call()) {
			if (op_is_known_address(0)) {

				auto name = get_name(op_get_known_address(0), 0);
				if (name == "__security_check_cookie") {
					return size;
				}
				else if (name == "_alloca_probe") {
					return size;
				}
			}
		}
		return 0;

	}
	void nop_out() {
		/*
			alloca isnt really necessary
		*/
		uint8_t* buffer = (uint8_t*)alloca(this->size);

		cs::patchops::fill_nopbuffer(buffer, this->size);

		patch_bytes(this->ea, (void*)buffer, size);

	}

	template<uint8_t... bytes>
	void replace_with_byteseq() {
		uint8_t bytes_[] = { bytes... };

		cs::patchops::replace_code(bytes_, sizeof...(bytes), ea, size);

	}

	bool may_make_branch_unconditional() const {

		if (!is_conditional_branch()) {
			return false;
		}
		if (size != 2) {
			return false;
		}
		return true;
	}

	void make_unconditional_branch() const {
		cs::patchops::make_short_branch_uncond(ea);

	}


	bool is_any_invalid_param_call() {
		return has_role(cs::function_classes::cs_funcclass_t::unhandleable_error);
	}

	qstring print_ud_list(bool use = true) {
		qstring ulist{ use ? "USE:(" : "DEF:(" };
		qstring tempreg{};
		csreglist_t rlist{};
		char itoa_buf[32];
		if (use)
			gen_use_list(&rlist);
		else
			gen_def_list(&rlist);
		unsigned ngot = 0;

		for (unsigned i = 0; i < NREGS; ++i) {
			if (rlist.has(i)) {
				get_reg_name(&tempreg, i, rlist.at(i));

				//ulist+= tempreg + ":" + 
				itoa(rlist.at(i), itoa_buf, 10);
				if (ngot != 0) {
					ulist += ",";
				}
				ulist += tempreg + ":" + itoa_buf;

				ngot++;
			}
		}
		return ulist + ")";
	}

	qstring to_string(bool ud_lists = false) {
		qstring res{};

		print_insn_mnem(&res, ea);

		for (unsigned i = 0; i < m_operand_count; ++i) {
			qstring operand{};
			print_operand(&operand, ea, i);
			tag_remove(&operand, 0);
			res += qstring(" ") +  operand;
			
			if (i != m_operand_count - 1) {

				res += ",";
			}


		}

		if (m_call_class != cs::function_classes::cs_funcclass_t::none) {
			res += "(CALL ROLE: ";
			res += cs::function_classes::function_class_to_string(m_call_class);

			res += ") ";
		}

		if (ud_lists) {
			
			return (res + "\n\t\t") + (print_ud_list(true) + "\n\t\t") + print_ud_list(false);
		}

		return res;

	}
	bool has_role(cs::function_classes::cs_funcclass_t role) const {
		return m_call_class == role;
	}
	/*
		returns true if we can safely deduce from just the instruction itself that the only purpose
		of the instruction is to test if a register is zero

		only intended for GPRS
	*/
	bool is_possible_zero_test() const {
		if (m_norm_op == NN_S_test && ops[0].is_reg(ops[1].reg) && ops[1].is_reg(ops[0].reg)) {
			return true;
		}
		else if (m_norm_op == NN_S_cmp && ops[0].type == o_reg &&
			ops[1].type == o_imm && ops[1].value == 0) {
			return true;
		}
		return false;
	}
};


static inline bool _is_basic_block_end(insn_t* ins) {
	
	if (is_basic_block_end(*ins, true)) {
		return true;
	}
	else {
		insn_t unused{};
		ea_t nextea = ins->ea + ins->size;//decode_prev_insn(&unused, ins->ea);

		if (nextea == BADADDR) {
			return false;
		}
		else {
			func_t* mefunc = get_func(ins->ea);

			if (nextea >= mefunc->end_ea) {
				return true;
			}
			unsigned nrefs = 0;
			for ( auto ref : cs::allrefs_to(nextea)) {
				if (ref == ins->ea) {
					continue;
				}

				//if (mefunc->contains(ref)) {
					//referenced by non successor instruction in function
					//
					return true;
				//}

			}
			
		}

	}
	return false;
}
class csfunc_t;

enum : uint32_t {
	CSBB_IS_ERROR_ENDPOINT = 1
};
class csbb_t {
	ea_t m_start_addr;
	ea_t m_end_addr;


	csinsn_t* CS_RESTRICT m_head;
	csinsn_t* CS_RESTRICT m_tail;

	csbb_t* CS_RESTRICT m_prevb;
	csbb_t* CS_RESTRICT m_nextb;
	csreglist_t m_use;
	csreglist_t m_def;

	std::set<csbb_t*> m_predecessors;
	std::set<csbb_t*> m_successors;
	unsigned m_serial;
	unsigned m_flags;
	friend class csfunc_t;
public:

	csbb_t() : m_start_addr(BADADDR), m_end_addr(BADADDR), m_head(nullptr), m_tail(nullptr), m_prevb(nullptr), m_nextb(nullptr) , m_use(), m_def(), m_serial(-1), m_flags(0){}

	~csbb_t() {
		
		m_start_addr = BADADDR;
		m_end_addr = BADADDR;
		csinsn_t* ins = m_head;

		csinsn_t* previns = nullptr;

		for (; ins; previns = ins, ins = ins->next()) {
			if (previns) {
				delete previns;
			}
		}

		if (previns) {
			delete previns;
		}



	}

	csinsn_t* find_next_use(csinsn_t* CS_RESTRICT start, unsigned reg, csreglist_t* CS_RESTRICT buffer) CS_RESTRICT  {
		
		buffer->clear();
		for (csinsn_t* CS_RESTRICT ins = start ? start->next() : m_head; ins; ins = ins->next()) {
			ins->gen_use_list(buffer);
			if (buffer->has(reg)) {
				return ins;
			}

		}
		return nullptr;
	}

	csinsn_t* find_first_use(unsigned reg, csreglist_t* CS_RESTRICT buffer) CS_RESTRICT {
		return find_next_use(nullptr, reg, buffer);
	}

	csinsn_t* find_definition_backwards(csinsn_t* CS_RESTRICT start, unsigned reg, csreglist_t* CS_RESTRICT buffer) CS_RESTRICT {
		buffer->clear();
		for (csinsn_t* CS_RESTRICT ins = start->prev(); ins; ins = ins->prev()) {
			ins->gen_def_list(buffer);
			if (buffer->has(reg)) {
				return ins;
			}
		}
		return nullptr;
	}


	csinsn_t* find_redefinition(csinsn_t* CS_RESTRICT start, unsigned reg, csreglist_t* CS_RESTRICT buffer) CS_RESTRICT  {
		buffer->clear();
		for (csinsn_t* CS_RESTRICT ins = start ? start->next() : m_head; ins; ins = ins->next()) {
			ins->gen_def_list(buffer);
			if (buffer->has(reg)) {
				return ins;
			}
		}
		return nullptr;
	}

	csinsn_t* find_next_use_without_redef(csinsn_t* CS_RESTRICT start, unsigned reg, csreglist_t* CS_RESTRICT buffer) CS_RESTRICT {
		auto u = find_next_use(start, reg, buffer);

		if (!u) {
			return nullptr;
		}

		auto d = find_redefinition(start, reg, buffer);

		if (!d) {
			return u;
		}
		if (d->ea >= u->ea) {
			return u;
		}

		return nullptr;
	}

	template<typename TPredicate>
	inline csinsn_t* find_use_p(csinsn_t* CS_RESTRICT start, unsigned reg, csreglist_t* CS_RESTRICT buffer, TPredicate&& pred) CS_RESTRICT {
		for (csinsn_t* CS_RESTRICT ins = find_next_use_without_redef(start, reg, buffer); ins; ins = find_next_use_without_redef(ins, reg, buffer)) {
			if (pred(ins)) {
				return ins;
			}
		}
		return nullptr;
	}


	void add_instruction(csinsn_t* ins) {
		if (m_head == nullptr && m_tail == nullptr) {
			m_head = ins;
			m_tail = ins;
			m_start_addr = ins->ea;
			m_end_addr = ins->ea + ins->size;
		}
		else if (m_tail != nullptr) {
			m_tail->linkin_next(ins);
			m_tail = ins;
			m_end_addr = ins->ea + ins->size;
		}
	}

	ea_t startea() const {
		return m_start_addr;
	}

	ea_t endea() const {
		return m_end_addr;
	}


	csinsn_t* head() {
		return m_head;
	}

	csinsn_t* tail() {
		return m_tail;
	}

	csbb_t* prevb() {
		return m_prevb;
	}

	csbb_t* nextb() {
		return m_nextb;
	}

	ea_t decode(ea_t addr, bool& reflow) {
		int size = 0;
		csinsn_t* ins;
		do {
			addr += size;
			ins = new csinsn_t();

			size = ins->decode(addr);

			

			add_instruction(ins);
			if (ins->is_absolute_end_of_flow()) {
				reflow = true;
				break;
			}


		} while (size != -1 && !_is_basic_block_end(ins));

		if (size == -1) {
			return BADADDR;
		}
		else {
			return addr + size;
		}
	}

	void linkin_nextb(csbb_t* bb) {
		bb->m_prevb = this;
		m_nextb = bb;
	}

	void linkin_prevb(csbb_t* bb) {
		bb->m_nextb = this;
		m_prevb = bb;
	}


	void regenerate_use_list() {
		m_use.clear();

		for (csinsn_t* ins = head(); ins; ins = ins->next()) {
			ins->gen_use_list(&m_use);
		}
	}

	void regenerate_def_list() {
		m_def.clear();

		for (csinsn_t* ins = head(); ins; ins = ins->next()) {
			ins->gen_def_list(&m_def);
		}
	}

	void regenerate_lists() {
		regenerate_def_list();
		regenerate_use_list();
	}

	void dump_lists() {
		m_use.print(true);
		m_def.print(false);
	}

	void print() {
		msg("Basic block %d: range - 0x%llX until 0x%llX\n", (unsigned)m_serial, (uint64_t)m_start_addr, (uint64_t)m_end_addr);
		msg("Uses:\n");
		m_use.print(true);
		msg("Defs:\n");
		m_def.print(false);
	}

	auto predecessors() {
		return m_predecessors;
	}

	auto successors() {
		return m_successors;
	}

	bool is_error_endpoint() const {
		return (m_flags & CSBB_IS_ERROR_ENDPOINT) != 0;
	}
};

class csfunc_t {

	func_t* CS_RESTRICT m_func;
	csbb_t* CS_RESTRICT m_blocks_head;
	csbb_t* CS_RESTRICT m_blocks_tail;
	std::map<ea_t, csbb_t*> m_ea_to_bb;

	csbb_t** m_bbserial_lut;
public:

	csfunc_t() : m_func(nullptr), m_blocks_head(nullptr), m_blocks_tail(nullptr), m_bbserial_lut(nullptr){}


	~csfunc_t() {
		if (m_blocks_head != nullptr) {
			if (m_blocks_head == m_blocks_tail) {
				delete m_blocks_head;
				

			}
			else {

				for (csbb_t* bb = m_blocks_head; bb != m_blocks_tail; ) {

					csbb_t* nextbb = bb->nextb();

					delete bb;
					bb = nextbb;

				}

				delete m_blocks_tail;
	

			}
		}

		m_func = nullptr;
		m_blocks_head = nullptr;
		m_blocks_tail = nullptr;
	}


	ea_t startea() {
		return m_func->start_ea;
	}

	ea_t endea() {
		return m_func->end_ea;
	}


	csbb_t* firstb() {
		return m_blocks_head;
	}

	csbb_t* lastb() {
		return m_blocks_tail;
	}


	void linkin_block(csbb_t* bb) {
		if (!m_blocks_head && !m_blocks_tail) {
			m_blocks_head = bb;
			m_blocks_tail = bb;
		}
		else {
			
			auto tail = m_blocks_tail;
			m_blocks_tail = bb;

			tail->linkin_nextb(bb);
		}
	}

	csbb_t* get_fallthrough(csbb_t* bb) {
		return m_bbserial_lut[bb->m_serial + 1];
	}

	void regen_serial() {
		unsigned currser = 0;

		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {
			bb->m_serial = currser;
			currser++;
		}

		if (m_bbserial_lut) {
			delete[] m_bbserial_lut;

		}

		m_bbserial_lut = new csbb_t * [currser];
		currser = 0;
		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {
			m_bbserial_lut[currser] = bb;
			currser++;
		}
	}

	void regen_usedefs() {

		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {
			bb->regenerate_lists();
		}
	}
	/*
		i really fucked this one up and should rewrite this part from scratch

	*/
	void regen_predecessors_and_successors() {
		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {

			bb->m_predecessors.clear();
			bb->m_successors.clear();
		}
		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {

			
			auto tail = bb->tail();

			

			if (tail->is_conditional_branch()) {
				auto fallthrough = m_bbserial_lut[bb->m_serial + 1];

				fallthrough->m_predecessors.insert(bb);
				bb->m_successors.insert(fallthrough);
				ea_t targ = tail->op_get_known_address(0);

				if (targ == BADADDR) {
					continue;
				}
				else {
					auto iter = m_ea_to_bb.find(targ);

					if (iter == m_ea_to_bb.end()) {
						continue;
					}
					else {
						iter->second->m_predecessors.insert(bb);
						bb->m_successors.insert(iter->second);
					}
				}



			}
			else if (tail->is_call() && !tail->is_absolute_end_of_flow()) {
				m_bbserial_lut[bb->m_serial + 1]->m_predecessors.insert(bb);
				bb->m_successors.insert(m_bbserial_lut[bb->m_serial + 1]);
			}
			else if(!tail->is_ret() && !tail->is_absolute_end_of_flow()  && tail->op_is_known_address(0) ) {
				auto addr = tail->op_get_known_address(0);
				auto addr_iter = m_ea_to_bb.find(addr);
				if (addr_iter != m_ea_to_bb.end()) {
					addr_iter->second->m_predecessors.insert(bb);
					bb->m_successors.insert(addr_iter->second);
				}
			}

		}
	}
	/*
		this is also disabled, i think it might be a little too aggressive
	*/
	void regen_endpoint_blocks() {
		/*
			clear error endpoint flag on all bbs
		*/
		for (csbb_t* CS_RESTRICT bb = m_blocks_head; bb; bb = bb->nextb()) {

			bb->m_flags &= ~CSBB_IS_ERROR_ENDPOINT;
		}
		
		bool should_rerun_endpoint_analysis = false;
		do {
			should_rerun_endpoint_analysis = false;
			for (csbb_t* CS_RESTRICT bb = m_blocks_head; bb; bb = bb->nextb()) {
				unsigned n_endpoints = 0;
				for (csbb_t* CS_RESTRICT succ : bb->successors()) {
					if (succ->is_error_endpoint()) {
						n_endpoints++;
					}
					else {
						csinsn_t* CS_RESTRICT bbtail = succ->tail();


						if (bbtail->is_any_invalid_param_call() /*|| bbtail->is_int3()*/) {

							succ->m_flags |= CSBB_IS_ERROR_ENDPOINT;
							n_endpoints++;
						}
					}

					
				}
				/*
					if all successors of a basic block are error conditions, then the block is an error condition and simply
					dispatches to different handlers so we mark it and trigger another pass to propagate its status
				*/
				if (n_endpoints == bb->successors().size() && !bb->is_error_endpoint()) {
					bb->m_flags |= CSBB_IS_ERROR_ENDPOINT;
					//msg("All successors of bb at 0x%llX are error endpoints, propagating status upwards.\n", (uint64_t)bb->startea());
					should_rerun_endpoint_analysis = false;
				}
			}
		} while (should_rerun_endpoint_analysis);
	}
	static int decode(csfunc_t* out, ea_t ea) {
		func_t* idafunc = get_func(ea);
		if (!idafunc) {
			return -1;
		}

		out->m_func = idafunc;

		std::set<ea_t> bbset{};
	gen_bbs:
		for (ea_t currea = idafunc->start_ea; currea < idafunc->end_ea; ) {
			bool need_reflow = false;

			csbb_t* bb = new csbb_t();
			
			auto next_currea = bb->decode(currea, need_reflow);
			//need_reflow = false;
			bbset.insert(currea);

			out->m_ea_to_bb[currea] = bb;
			currea = next_currea;


			if (currea == BADADDR) {
				delete bb;
				return -1;
			}

			

			out->linkin_block(bb);


			/*
				if we hit an operation that unconditionally terminates control flow we need to rewind a bit and
				trace the branches of already generated bbs and follow their paths for the next bbs
			*/
			if (need_reflow || bbset.find(currea) != bbset.end()) {
				//msg("Doing reflow\n");
				for (csbb_t* reflowbb = out->m_blocks_head; reflowbb; reflowbb = reflowbb->nextb()) {
					auto ending = reflowbb->tail();
					if (ending->is_conditional_branch()) {
						auto alt = ending->altflow();

						if (alt > idafunc->start_ea && alt < idafunc->end_ea) {
							if (bbset.find(alt) == bbset.end()) {
								//msg("Got reflow to 0x%llX\n", alt);
								currea = alt;
								goto gen_bbs_end;
							}
						}
					}
				}
			}
		gen_bbs_end:
			;

		}


		out->regen_serial();

		out->regen_usedefs();
		//out->regen_predecessors_and_successors();
		//out->regen_endpoint_blocks();
		return 0;
	}


	void print() {
		msg("Func range: 0x%llX - 0x%llX.\n", (uint64_t)m_func->start_ea, (uint64_t)m_func->end_ea);

		for (csbb_t* bb = m_blocks_head; bb; bb = bb->nextb()) {
			bb->print();

		}
	}
};

bool csinsn_t::may_trivially_inline_call() const {
	/*
		first check whether the bytes of the callee - 0xC3 byte will fit in the call itself
	*/
	if (!speculative_callee_fits_in_call()) {
		return false;
	}
	else {
		auto fn = callee_func();


		/*
			get dat func_t
		*/
		if (!fn) {
			return false;
		}

		csfunc_t csfn{};


		/*
			its honestly not necessary to translate the callee but the analysis runs pretty quickly 
			so im not too worried about it
		*/
		if (csfunc_t::decode(&csfn, fn->start_ea) == -1) {
			//we couldn't translate to our format, bail out
			return false;
		}
		/*
			function has actual control flow, no bueno

		*/
		if (csfn.firstb() != csfn.lastb()) {
			return false;
		}

		csbb_t* bb = csfn.firstb();

		if (bb->tail()->itype != NN_retn) {
			//doesnt end in a ret, give up
			return false;
		}

		return true;


	}
}