/* to be completed.... */
enum reg_class c54x_regclass_map[FIRST_PSEUDO_REGISTER] =
{
//#define IMR_REGNO   0
//#define IFR_REGNO   1
//#define ST0_REGNO   2
//#define ST1_REGNO   3
//#define A_REGNO     4
//#define B_REGNO     5
//#define T_REGNO     6
//#define TRN_REGNO   7
//#define AR0_REGNO   8
//#define AR1_REGNO   9
//#define AR2_REGNO  10
//#define AR3_REGNO  11
//#define AR4_REGNO  12
//#define AR5_REGNO  13
//#define AR6_REGNO  14
//#define AR7_REGNO  15
//#define SP_REGNO   16
//#define BK_REGNO   17
//#define BRC_REGNO  18
//#define RSA_REGNO  19
//#define REA_REGNO  20
//#define PMST_REGNO 21
//#define XPC_REGNO  22
//#define DP_REGNO   23
//#define ARG_REGNO  24
};

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
			cum->has_vaargs = true;
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
