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
		/* IMR		IFR		 ST0	  ST1 */
		   IMR_REG, IFR_REG, ST0_REG, ST1_REG,
		   
		/* A	  B		 T		TRN */
		   A_REG, B_REG, T_REG, TRN_REG,
		   
		/* AR0		AR1 */
		   AR0_REG, AUX_REGS,
		   
		/* AR2			AR3			 AR4		  AR5  */
		   DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS,
		   
		/* AR6 AR7	 SP		 BK		 BRC */
		   AUX_REGS, SP_REG, BK_REG, BRC_REG,
		   
		/* RSA REA	PMST	  XPC	   DP	   ARG */
		   RSA_REG, PMST_REG, XPC_REG, DP_REG, NO_REGS
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
	/* TODO: Fixme */
	return 1;
}
