/* Definitions of target machine for GCC for the Texas Instruments TMS320C54x
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Bryan Richter and Jonathan Bastien-Filiatrault

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

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
#include "emit-rtl.h"

enum reg_class const regclass_map[FIRST_PSEUDO_REGISTER] =
    {
        /* IMR      IFR      ST0      ST1 */
           IMR_REG, IFR_REG, ST_REGS, ST_REGS,

        /* A      B      T      TRN */
           A_REG, B_REG, T_REG, TRN_REG,

        /* AR0       AR1 */
           AUX_REGS, AUX_REGS,

        /* AR2          AR3          AR4          AR5  */
           DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS, DBL_OP_REGS,

        /* AR6       AR7       SP      BK      BRC */
           AUX_REGS, AUX_REGS, SP_REG, BK_REG, BRC_REG,

        /* RSA      REA      PMST      XPC      DP      ARG */
           RSA_REG, REA_REG, PMST_REG, XPC_REG, DP_REG, GENERAL_REGS
    };

struct gcc_target targetm = TARGET_INITIALIZER;

int
c54x_hard_regno_mode_ok (unsigned int regno, enum machine_mode mode)
{
	int valid;

	switch(mode) {
	case QImode:
		valid =  (AUX_REGNO_P(regno) || ACC_REGNO_P(regno)
				  || SP_REGNO_P(regno) || T_REGNO_P(regno));
		break;
	case ACCmode:
	case HImode:
		valid = ACC_REGNO_P(regno);
		break;
	case CCmode:
		valid = ST_REGNO_P(regno);
		break;
	default:
		valid =  0;
		break;
	}

	return valid;
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

	if(cum->numarg == 0
	   && (mode == HImode || mode == ACCmode || mode == HImode)
	   && cum->has_varargs == false) {
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
	rtx base, index;

	switch(GET_CODE(addr)) {
	case REG:
		valid = (AUX_REG_P(addr) || SP_REG_P(addr) || (!strict && PSEUDO_REG_P(addr)));
		break;
	case PLUS:
		base = XEXP(addr, 0);
		index = XEXP(addr, 1);

		valid =
			/* Indirect + offset Smem addressing */
			((AUX_REG_P(base) || (!strict && PSEUDO_REG_P(base)))
			 && (GET_CODE(index) == CONST_INT) && IN_RANGE_P(XINT(index, 0), -32768, 65535))
			/* Direct, offset from SP (cpl=1) */
			|| ((SP_REG_P(base) || (!strict && PSEUDO_REG_P(base)))
				&& (GET_CODE(index) == CONST_INT) && IN_RANGE_P(XINT(index, 0), 0, 128));
		break;
	case PRE_DEC:
	case POST_INC:
	case PRE_INC:
	case POST_DEC:
		base = XEXP(addr, 0);
		valid = AUX_REG_P(base) || ((!strict && PSEUDO_REG_P(base)));
		break;
	case CONST:
	case CONST_INT:
	case SYMBOL_REF:
/* 	case LABEL_REF: */
		valid = 1;
		break;
	default:
		break;
	}

/* 	print_rtl(stderr, addr); */
/* 	fprintf(stderr, " valid: %s, strict: %s\n", (valid ? "yes" : "no"), (strict ? "yes" : "no" )); */

	return valid;
}

int
c54x_expand_movqi(rtx ops[])
{
	const enum machine_mode mode = QImode;
	int i;
	rtx tmp1, tmp2;

	fprintf(stderr, "-%s--movqi:", no_new_pseudos?"noP":"P");
	for(i=0; i < 2; i++) {
		print_rtl(stderr, ops[i]);
	}
	fprintf(stderr, "--\n");
	
	/* Push */
	if(memory_operand(ops[0], mode) && GET_CODE(XEXP(ops[0], 0)) == PRE_DEC) {
		emit_insn(gen_pushqi(ops[1]));
		fprintf(stderr, "generated \"push\"\n");
		return 1;
	}

	if( (GET_CODE(ops[0]) == SUBREG && GET_MODE(XEXP(ops[0], 0)) == ACCmode)
		&& (GET_CODE(ops[1]) == SUBREG && GET_MODE(XEXP(ops[1], 0)) == ACCmode)) {
		fprintf(stderr, "ZOINK!\n");
		tmp1 = gen_reg_rtx(QImode);
		emit_move_insn(tmp1, ops[1]);
		emit_move_insn(ops[0], tmp1);
		return 1;
	}

	/* ld */
/* 	if(GET_CODE(ops[0]) == SUBREG && GET_MODE(XEXP(ops[0], 0)) == ACCmode) { */
/* 		ops[0] = convert_to_mode(ACCmode, ops[0], 0); */

/* 		if(immediate_operand(ops[1], mode)) { */
/* 			emit_insn(gen_ld_const(ops[0], ops[1])); */
/* 		} else { */
/* 			emit_insn(gen_ldu(ops[0], ops[1])); */
/* 		} */
/* 		fprintf(stderr, "generated \"ld\"\n"); */
/* 		return 1; */
/* 	} */

	return 0;
}

int
c54x_expand_addqi(rtx ops[])
{
	int i;
	rtx tmp, tmp2;

	fprintf(stderr, "-%s--addqi:", no_new_pseudos?"noP":"P");
	for(i=0; i < 3; i++) {
		print_rtl(stderr, ops[i]);
	}
	fprintf(stderr, "--\n");

	if( (SP_REG_P(ops[0]) && !PSEUDO_REG_P(ops[0])) && immediate_operand(ops[2], VOIDmode)) {
		emit_insn(gen_frame(ops[1], ops[2]));
		emit_move_insn(ops[0], ops[1]);
		return 1;
	} else if( (register_operand(ops[1], QImode) || memory_operand(ops[1], QImode))
			   && immediate_operand(ops[2], VOIDmode)) {
		emit_insn(gen_addm(ops[1], ops[2]));
		emit_move_insn(ops[0], ops[1]);
		return 1;
	} else if(register_operand(ops[1], QImode) && general_operand(ops[2], QImode) && !no_new_pseudos) {
		ops[1] = convert_to_mode(ACCmode, ops[1], 0);
		tmp = gen_rtx_SUBREG(QImode, ops[1], 0);
		if(immediate_operand(ops[2], VOIDmode)) {
			emit_insn(gen_add_const(ops[1], ops[2]));
		} else {
			emit_insn(gen_add(ops[1], ops[2]));
		}
		emit_move_insn(ops[0], tmp);
		return 1;
	}
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
	rtx base, index;

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
		valid = (AUX_REG_P(XEXP(addr, 0))||PSEUDO_REG_P(XEXP(addr, 0))) && letter == 'T';
		break;
	case REG:
		valid = AUX_REG_P(addr)||PSEUDO_REG_P(addr);
		break;
	case CONST:
	case CONST_INT:
	case SYMBOL_REF:
		valid = 1;
		break;
	case PLUS:
		base = XEXP(addr, 0);
		index = XEXP(addr, 1);

		valid =
			/* Indirect + offset Smem addressing */
			((AUX_REG_P(base)||PSEUDO_REG_P(base))
				 && (GET_CODE(index) == CONST_INT) && IN_RANGE_P(XINT(index, 0), -32768, 65535))
			/* Direct, offset from SP (cpl=1) */
			|| ((SP_REG_P(base)||PSEUDO_REG_P(base))
				&& (GET_CODE(index) == CONST_INT) && IN_RANGE_P(XINT(index, 0), 0, 128));
		break;
	default:
		break;
	}

	if(!valid) {
		print_rtl(stderr, addr);
		fprintf(stderr, ": mem\n");
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
	rtx base;
	rtx disp;

	switch(GET_CODE(op)) {
	case REG:
		fprintf(stream, "%s", reg_names[REGNO(op)]);
		break;
	case CONST_INT:
		if(letter == 'I') {
			fprintf(stream, "#0%xh", 0xffff & XINT(op, 0));
		} else {
			fprintf(stream, "%d", XINT(op, 0));
		}
		break;
	case MEM:
		mem = XEXP(op, 0);
		switch(GET_CODE(mem)) {
		case LABEL_REF:
		case SYMBOL_REF:
			fprintf(stream, "*(%s)", XSTR(mem, 0));
			break;
		case REG:
			fprintf(stream, "*%s", reg_names[REGNO(mem)]);
			break;
		case PLUS:
			base = XEXP(mem, 0);
			disp = XEXP(mem, 1);
			if( REG_P(base) && CONSTANT_P(disp) ) {
				fprintf(stream, "*%s(%d)", reg_names[REGNO(base)], XINT(disp, 0));
			}
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
		fprintf(stream, "addr:%s", XSTR(value, 0));
		break;
	default:
		fprintf(stream, "addr:");
		print_rtl(stream, value);
		break;
	}
}

void
c54x_expand_prologue()
{
	int r;
	rtx insn;

	for(r = 0; r < FIRST_PSEUDO_REGISTER; r++) {
		if(c54x_save_register_p(r)) {
			insn = emit_insn(gen_pushqi(gen_rtx_REG(QImode, r)));
			RTX_FRAME_RELATED_P(insn) = 1;
		}
	}

	if(get_frame_size() > 0) {
		insn = emit_insn(gen_frame(gen_rtx_REG (QImode, STACK_POINTER_REGNUM),
							gen_rtx_CONST_INT(VOIDmode, get_frame_size())));
		RTX_FRAME_RELATED_P(insn) = 1;
	}

	if(frame_pointer_needed) {
		insn = emit_move_insn(gen_rtx_REG (QImode,	FRAME_POINTER_REGNUM),
					   gen_rtx_REG (QImode, STACK_POINTER_REGNUM));
		RTX_FRAME_RELATED_P(insn) = 1;
	}
}

void
c54x_expand_epilogue()
{
	int r;
	rtx insn;

	if(get_frame_size() > 0) {
		insn = emit_insn(gen_frame(gen_rtx_REG (QImode, STACK_POINTER_REGNUM),
							gen_rtx_CONST_INT(VOIDmode, -get_frame_size())));
		RTX_FRAME_RELATED_P(insn) = 1;
	}

	for(r = FIRST_PSEUDO_REGISTER - 1; r > 0; r--) {
		if(c54x_save_register_p(r)) {
			insn = emit_insn(gen_popqi(gen_rtx_REG(QImode, r)));
			RTX_FRAME_RELATED_P(insn) = 1;
		}
	}

	insn = emit_insn(gen_return());
	RTX_FRAME_RELATED_P(insn) = 1;
}

int
c54x_save_register_p(int regno)
{
	gcc_assert(regno <= FIRST_PSEUDO_REGISTER);

	return regs_ever_live[regno] && !call_used_regs[regno];
}

void
c54x_file_start(void)
{
	default_file_start();
	fprintf(asm_out_file, "\t.mmregs\n");
}

int
c54x_initial_elimination_offset(int from, int to)
{
	/* {ARG_POINTER_REGNUM, STACK_POINTER_REGNUM}   */
	/* {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM}   */
	/* {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM} */
   int aptospfp = get_frame_size() + 1;
   printf("frame size: %d\n", aptospfp);
	int offset = 0xdead;
	int regno;

	for(regno=0; regno < FIRST_PSEUDO_REGISTER; regno++) {
		if(c54x_save_register_p(regno)) {
			aptospfp++;
		}
	}

	if( (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
		|| (from == ARG_POINTER_REGNUM && to == FRAME_POINTER_REGNUM) )	{
		offset = aptospfp;
	} else if(from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM) {
		offset = 0;
	}

	gcc_assert(offset != 0xdead);

	/* Im not sure which direction the offset is in:
	   from/to seems suggestive */
	offset = -offset;

	fprintf(stderr, "\n%s:%s = %d\n\n", reg_names[from], reg_names[to], offset);

	return offset;
}
