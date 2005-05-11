#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-codes.h"
#include "insn-modes.h"
#include "insn-attr.h"
#include "flags.h"
#include "except.h"
#include "function.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "toplev.h"
#include "basic-block.h"
#include "ggc.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "cgraph.h"
#include "tree-gimple.h"

enum reg_class const regclass_map[FIRST_PSEUDO_REGISTER] =
    {
        /* IMR      IFR      ST0      ST1 */
           IMR_REG, IFR_REG, ST0_REG, ST1_REG,
           
        /* A      B      T      TRN */
           A_REG, B_REG, T_REG, TRN_REG,
           
        /* AR0      AR1 */
           AR0_REG, AUX_REGS,
           
        /* AR2          AR3          AR4          AR5  */
           DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS,
           
        /* AR6       AR7       SP      BK      BRC */
           AUX_REGS, AUX_REGS, SP_REG, BK_REG, BRC_REG,
           
        /* RSA      REA      PMST      XPC      DP      ARG */
           RSA_REG, REA_REG, PMST_REG, XPC_REG, DP_REG, NO_REGS
    };

struct gcc_target targetm = TARGET_INITIALIZER;

int
c54x_hard_regno_mode_ok (unsigned int regno, enum machine_mode mode)
{
	switch(mode) {
	case QImode:
		return (AUX_REGNO_P(regno) || ACC_REGNO_P(regno)
				|| SP_REGNO_P(regno) || T_REGNO_P(regno));
	case HImode:
		return (ACC_REGNO_P(regno));
	case CCmode:
		return (ST_REGNO_P(regno));
	default:
		return 0;
	}
}

void
init_cumulative_args (CUMULATIVE_ARGS *cum, tree fntype, rtx libname, tree fndecl)
{
	cum->numarg = 0;
	cum->has_varargs = false;

	tree param, next_param;

	for (param = (fntype) ? TYPE_ARG_TYPES (fntype) : 0; param != 0; param = next_param) {
		next_param = TREE_CHAIN (param);
		if (next_param == 0 && TREE_VALUE (param) != void_type_node) {
			/* If the last parameter is not of type void_type_node, we have a variadic function */
			cum->has_varargs = true;
		}
	}
}

rtx
function_arg (CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int named)
{
	rtx ret = NULL_RTX;

	if(cum->numarg == 0 && mode != VOIDmode && cum->has_varargs == false) {
		ret = gen_rtx_REG(mode, A_REGNO);
	}

	return ret;
}

void
function_arg_advance (CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type, int named)
{
	cum->numarg++;
}

int
legitimate_address_p (enum machine_mode mode, rtx addr, int strict)
{
	int valid=0;
	
	switch(GET_CODE(addr)) {
	case REG:
		valid = (AUX_REGNO_P(REGNO(addr)) || (!strict && PSEUDO_REGNO_P(REGNO(addr))));
		break;
	case CONST:
	case CONST_INT:
	case SYMBOL_REF:
	case LABEL_REF:
	case POST_DEC:
	case POST_INC:
	case PRE_INC:
		valid = 1;
		break;
	default:
		break;
	}
	
	print_rtl(stderr, addr);
	fprintf(stderr, " valid: %s, strict: %s\n", (valid ? "yes" : "no"), (strict ? "yes" : "no" ));
	
	return valid;
}

int
c54x_legitimize_move(enum machine_mode mode, rtx op0, rtx op1)
{
	/* TODO: WriteMe */
	return 0;
}

void
c54x_initialize_trampoline(rtx tramp, rtx fnaddr, rtx cxt)
{
	/* TODO: WriteMe */
}

void
c54x_globalize_label (FILE *stream, const char *name)
{
/*   default_globalize_label (stream, name); */
  /* Will need to be investigated further */
}

void
c54x_override_options(void)
{
	/* May be useful in the far future */
}

int
c54x_xmem_p(rtx value, char letter)
{
	int valid = 0;
	rtx addr;

	if(GET_CODE(value) != MEM)
		return valid;

	addr = XEXP(value, 0);

	switch(GET_CODE(addr)) {
	case POST_INC:
	case POST_DEC:
		valid = XMEM_REGNO_P(REGNO(XEXP(addr, 0)));
		break;
	case REG:
		valid = XMEM_REGNO_P(REGNO(addr));
		break;
	default:
		break;
	}

	if(valid) {
		print_rtl(stderr, addr);
		fprintf(stderr, ": Xmem\n");
	}

	return valid;
}

int
c54x_smem_p(rtx value, char letter)
{
	int valid = 0;
	rtx addr;

	if(GET_CODE(value) != MEM)
		return valid;

	addr = XEXP(value, 0);

	switch(GET_CODE(addr)) {
	case POST_INC:
	case POST_DEC:
		valid = AUX_REGNO_P(REGNO(XEXP(addr, 0)));
		break;
	case PRE_INC:
		/* Allowed only on a write operand */
		valid = AUX_REGNO_P(REGNO(XEXP(addr, 0))) && letter == 'T';
		break;
	case REG:
		valid = AUX_REGNO_P(REGNO(addr));
		break;
	case CONST:
	case CONST_INT:
	case SYMBOL_REF:
		valid = 1;
	default:
		break;
	}

	if(!valid) {
		print_rtl(stderr, addr);
		fprintf(stderr, ": Smem\n");
	}

	return valid;
}

int
c54x_dmad_p(rtx value, char letter)
{
	int valid = 0;
	rtx addr;

	if(GET_CODE(value) != MEM)
		return valid;

	addr = XEXP(value, 0);

	switch(GET_CODE(addr)) {
	case CONST:
	case CONST_INT:
	case SYMBOL_REF:
		valid = 1;
		break;
	default:
		break;
	}
	
	if(valid) {
		print_rtl(stderr, addr);
		fprintf(stderr, ": dmad\n");
	}

	return valid;
}

void
c54x_print_operand(FILE *stream, rtx op, char letter)
{
	rtx mem;
	
	switch(GET_CODE(op)) {
	case REG:
		fprintf(stream, "%s", reg_names[REGNO(op)]);
		break;
	case CONST_INT:
		fprintf(stream, "#%xh", XINT(op, 0));
		break;
	case MEM:
		mem = XEXP(op, 0);
		switch(GET_CODE(mem)) {
		case LABEL_REF:
		case SYMBOL_REF:
			fprintf(stream, "%s", XSTR(mem, 0));
			break;
		case REG:
			fprintf(stream, "*%s", reg_names[REGNO(mem)]);
			break;
		default:
			fprintf(stream, "mem:");
			print_rtl(stream, mem);
			break;
		}
		break;
	default:
		fprintf(stream, "op:");
		print_rtl(stream, op);
		break;
	}
}

void
c54x_print_operand_address(FILE *stream, rtx addr)
{
	rtx value = XEXP(addr, 0);

	switch(GET_CODE(value)) {
	case SYMBOL_REF:
	case LABEL_REF:
		fprintf(stream, "%s", XSTR(value, 0));
		break;
	default:
		fprintf(stream, "addr:");
		print_rtl(stream, value);
		break;
	}
}
