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

/* c54x.h
Definitions and stuff */

/* As "Porting GCC For Dunces" is one of my references, this will draw from the
 * cris port that is highlighted therein. It will also rely on the c4x, for
 * obvious reasons.
 * Edit: Gah, too bad they are both buggy and under-maintained. */

/* Node: Driver */
/* Node: Run-time Target Specification */

#define TARGET_CPU_CPP_BUILTINS()       \
do {                                    \
    builtin_assert ("cpu=c54x");        \
    builtin_assert ("machine=c54x");    \
    builtin_define_std ("c54x");        \
} while (0)

extern int target_flags;

#define TARGET_VERSION fprintf (stderr, " (C54x)");

#define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT

#define OVERRIDE_OPTIONS c54x_override_options()

/* Node: Per-Function Data */

/* Node: Storage Layout */
#define BITS_BIG_ENDIAN         1
#define BYTES_BIG_ENDIAN        1
#define WORDS_BIG_ENDIAN        1
#define FLOAT_WORDS_BIG_ENDIAN  1
#define BITS_PER_UNIT           16
#define UNITS_PER_WORD          1

/* Promotion stuff, which doesn't matter. No mode is narrower than 16. */

#define PARM_BOUNDARY            BITS_PER_WORD
#define STACK_BOUNDARY           BITS_PER_WORD
#define FUNCTION_BOUNDARY        BITS_PER_WORD
#define BIGGEST_ALIGNMENT        BITS_PER_WORD*2
#define MINIMUM_ATOMIC_ALIGNMENT BITS_PER_WORD
#define EMPTY_FIELD_BOUNDARY	 BITS_PER_WORD
#define STRICT_ALIGNMENT	     1  /* Nothing is smaller than alignment.. */
#define MAX_FIXED_MODE_SIZE      32 /* HImode, same as c4x */
/* VECTOR_MODE_SUPPORTED? */

/* Node: Type Layout */

#define SHORT_TYPE_SIZE     BITS_PER_WORD
#define LONG_TYPE_SIZE      (2*BITS_PER_WORD)
#define LONG_LONG_TYPE_SIZE (2*BITS_PER_WORD)
#define FLOAT_TYPE_SIZE     (2*BITS_PER_WORD)

#define SIZE_TYPE "unsigned int" /* default is too big for these */
#define PTRDIFF_TYPE "int"

/* hmmm.. we have 16 bits and 40 bits... what to do? How many of our
   functions are actually 32 bit? */
#define DEFAULT_SIGNED_CHAR     0  /* FIXME (ripped from c4x) */


/* Node: (not a node) */

/* Define register numbers */

/* Interrupt mask and flag regs */

#define IMR_REGNO   0
#define IFR_REGNO   1

/* Status regs 0 and 1 */

#define ST0_REGNO   2
#define ST1_REGNO   3

/* Accumulators A and B */

#define A_REGNO     4
#define B_REGNO     5

/* Tmp and transition regs */

#define T_REGNO     6
#define TRN_REGNO   7

/* Auxiliary regs */

#define AR0_REGNO   8
#define AR1_REGNO   9
#define AR2_REGNO  10
#define AR3_REGNO  11
#define AR4_REGNO  12
#define AR5_REGNO  13
#define AR6_REGNO  14
#define AR7_REGNO  15

/* Stack pointer */

#define SP_REGNO   16

/* Circular buffer size reg */

#define BK_REGNO   17

/* Block-repeat counter, start addr, and end addr regs */

#define BRC_REGNO  18
#define RSA_REGNO  19
#define REA_REGNO  20

/* Proc mode status reg */

#define PMST_REGNO 21

/* PC extension reg */

#define XPC_REGNO  22

/* Data Page register */
#define DP_REGNO   23

/* Fake argument pointer reg */
#define ARG_REGNO  24

/* PC reg? */


#define AUX_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) - AR0_REGNO <= AR7_REGNO - AR0_REGNO)
#define ACC_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) - A_REGNO <= B_REGNO - A_REGNO)
#define ST_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) - ST0_REGNO <= ST1_REGNO - ST0_REGNO)
#define T_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) == T_REGNO)
#define DP_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) == DP_REGNO)
#define SP_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) == SP_REGNO)
#define PSEUDO_REGNO_P(REGNO) \
	((unsigned int)(REGNO) >= FIRST_PSEUDO_REGISTER)
#define XMEM_REGNO_P(REGNO) \
    ((unsigned int)(REGNO) - AR2_REGNO <= AR5_REGNO - AR2_REGNO)

#define AUX_REG_P(X)    (AUX_REGNO_P(REGNO(X)))
#define ACC_REG_P(X)    (ACC_REGNO_P(REGNO(X)))
#define ST_REG_P(X)     (ST_REGNO_P(REGNO(X)))
#define T_REG_P(X)      (T_REGNO_P(REGNO(X)))
#define DP_REG_P(X)     (DP_REGNO_P(REGNO(X)))
#define SP_REG_P(X)     (SP_REGNO_P(REGNO(X)))
#define PSEUDO_REG_P(X) (PSEUDO_REGNO_P(REGNO(X)))
#define XMEM_REG_P(X)   (XMEM_REGNO_P(REGNO(X)))

/* Node: Register Basics */

/* number of registers */
#define FIRST_PSEUDO_REGISTER 25

/* registers that have a fixed purpose and can't be used for general tasks. */
#define FIXED_REGISTERS \
{ \
  /* IMR IFR ST0 ST1 A   B   T   TRN AR0 AR1 AR2 */ \
     1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0, \
  /* AR3 AR4 AR5 AR6 AR7 SP  BK  BRC RSA REA PMST XPC DP ARG*/ \
     0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  1,   1,  1, 1  \
}

/* FIXED */
#define CALL_USED_REGISTERS \
{ \
  /* IMR IFR ST0 ST1 A   B   T   TRN AR0 AR1 AR2 */ \
     1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1, \
  /* AR3 AR4 AR5 AR6 AR7 SP  BK  BRC RSA REA PMST XPC DP ARG */ \
     1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,   1,  1, 1 \
}

/* FIXME, where does this go, and does it exist? */
/* #define CONST_DOUBLE_OK_FOR_CONSTRAINT_P(VALUE, C, STR)  1 */

/* c4x doesn't use CALL_REALLY_USED_REGISTERS */
/* They do use CONDITIONAL_REGISTER_USAGE though; it handles things that change
 * from c.l.a:s (like c4x vs .3x) */

/* Node: Allocation Order */
/* REG_ALLOC_ORDER will be important */

/* Node: Values in Registers */

/* Ripped from c4x.h, should be fine */
#define HARD_REGNO_NREGS(REGNO, MODE) \
    ( (ACC_REGNO_P( REGNO )) ? 1 /* accumulators hold anything */  \
    : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD) ) 

#define HARD_REGNO_MODE_OK(REGNO, MODE) c54x_hard_regno_mode_ok(REGNO, MODE)

/* Node: Leaf Functions */
/* cris and c4x don't use these, but I think I could, what with the
   quick-return insn. */

/* Node: Stack Registers : not to be used */

/* Node: Register Classes */
enum reg_class
  {
    NO_REGS,
    IMR_REG,
    IFR_REG,
    ST0_REG,
    ST1_REG,
    A_REG,
    B_REG,
    T_REG,
    TRN_REG,
    AR0_REG,
    DBL_OP_REGS,
    AUX_REGS,
    SP_REG,
    BK_REG,
    BRC_REG,
    RSA_REG,
    REA_REG,
    PMST_REG,
    XPC_REG,
    DP_REG,
    INT_REGS,
    STAT_REGS,
    ACC_REGS,
    BR_REGS,
    MMR_REGS,
    GENERAL_REGS,
    ALL_REGS,
    LIM_REG_CLASSES
  };

#define N_REG_CLASSES (int) LIM_REG_CLASSES

#define REG_CLASS_NAMES \
{                       \
    "NO_REGS",          \
    "IMR_REG",          \
    "IFR_REG",          \
    "ST0_REG",          \
    "ST1_REG",          \
    "A_REG",            \
    "B_REG",            \
    "T_REG",            \
    "TRN_REG",          \
    "AR0_REG",          \
    "DBL_OP_REGS",      \
    "AUX_REGS",         \
    "SP_REG",           \
    "BK_REG",           \
    "BRC_REG",          \
    "RSA_REG",          \
    "REA_REG",          \
    "PMST_REG",         \
    "XPC_REG",          \
    "DP_REG",           \
    "INT_REGS",         \
    "STAT_REGS",        \
    "ACC_REGS",         \
    "BR_REGS",          \
    "MMR_REGS",         \
    "GENERAL_REGS",     \
    "ALL_REGS",         \
}

/* Defines which registers are in which classes */
#define REG_CLASS_CONTENTS \
{                          \
    {0x00000000}, /* NO_REGS */ \
    {0x00000001}, /* IMR_REG */ \
    {0x00000002}, /* IFR_REG */ \
    {0x00000004}, /* ST0_REG */ \
    {0x00000008}, /* ST1_REG */ \
    {0x00000010}, /* A_REG */ \
    {0x00000020}, /* B_REG */ \
    {0x00000040}, /* T_REG */ \
    {0x00000080}, /* TRN_REG */ \
    {0x00000100}, /* AR0_REG */ \
    {0x00003C00}, /* DBL_OP_REGS */ \
    {0x0000FF00}, /* AUX_REGS */ \
    {0x00010000}, /* SP_REG */ \
    {0x00020000}, /* BK_REG */ \
    {0x00040000}, /* BRC_REG */ \
    {0x00080000}, /* RSA_REG */ \
    {0x00100000}, /* REA_REG */ \
    {0x00200000}, /* PMST_REG */ \
    {0x00400000}, /* XPC_REG */ \
    {0x00800000}, /* DP_REG */ \
    {0x00000003}, /* INT_REGS */ \
    {0x0020000c}, /* STAT_REGS */ \
    {0x00000030}, /* ACC_REGS */ \
    {0x001c0000}, /* BR_REGS */ \
    {0x007fffcf}, /* MMR_REGS */ \
    {0x011efff0}, /* GENERAL_REGS */ \
    {0xFFFFFFFF}  /* ALL_REGS */ \
}

#define BASE_REG_CLASS AUX_REGS

#define INDEX_REG_CLASS AUX_REGS

#define REG_OK_FOR_INDEX_P(X) AUX_REGNO_P(REGNO(X))

#define REG_OK_FOR_BASE_P(X) AUX_REGNO_P(REGNO(X))

/* Register constraint letters
 *
 * a - Accum. A
 * b - Accum. B
 * c - Accums
 * d - AR0
 * e - AUX_REGS
 * f - T reg
 * g - TRN reg
 * h - SP reg
 * i - INT_REGS
 * j - IMR reg
 * k - IFR reg
 * l - BK reg
 * m - BR_REGS
 * n - BRC reg
 * o - RSA reg
 * p - REA reg
 * r - General regs
 * s - STAT_REGS
 * t - ST0 reg
 * u - ST1 reg
 * v - PMST reg
 * w - DP reg
 * x - XPC reg
 * y - DBL_OP_REGS
 * z - MMR_REGS
 *
 * Integer range constraints
 *
 * I - 16 bit int
 * J - 3 bit unsigned int
 * K - 4 bit unsigned int
 * L - 5 bit int
 * M - 9 bit int
 * O - 8 bit unsigned int
 *
 * Memory constraints
 *
 * Y - Xmem memory operand
 * S - Smem memory operand
 * T - Smem memory operand (write only)
 * U - dmad memory operand
 */

#define REG_CLASS_FROM_LETTER(c)  \
    \
    ( ((c) == 'a') ? A_REG         \
    : ((c) == 'b') ? B_REG         \
    : ((c) == 'c') ? ACC_REGS      \
    : ((c) == 'd') ? AR0_REG       \
    : ((c) == 'e') ? AUX_REGS      \
    : ((c) == 'f') ? T_REG         \
    : ((c) == 'g') ? TRN_REG       \
    : ((c) == 'h') ? SP_REG        \
    : ((c) == 'i') ? INT_REGS      \
    : ((c) == 'm') ? BR_REGS       \
    : ((c) == 'n') ? BRC_REG       \
    : ((c) == 'o') ? RSA_REG       \
    : ((c) == 'p') ? REA_REG       \
    : ((c) == 's') ? STAT_REGS     \
    : ((c) == 't') ? ST0_REG       \
    : ((c) == 'u') ? ST1_REG       \
    : ((c) == 'v') ? PMST_REG      \
    : ((c) == 'w') ? DP_REG        \
    : ((c) == 'x') ? XPC_REG       \
    : ((c) == 'y') ? DBL_OP_REGS   \
    : ((c) == 'z') ? MMR_REGS      \
    : NO_REGS )

/* Oohh one of those seemingly overlapping macros.
 * I'm going to set this to be true for aux regs, which might not be as broad as
 * possible. */
#define REGNO_OK_FOR_BASE_P(n) AUX_REGNO_P (n)

/* Same as REGNO_OK_FOR_BASE_P */
#define REGNO_OK_FOR_INDEX_P(n) AUX_REGNO_P (n)

/* This will work, but might not be optimal */
#define PREFERRED_RELOAD_CLASS(x, CLASS) CLASS

/* A bunch of stuff about reloading that, as far as I know, I don't need. I very
 * well could be wrong, of course. */


/* From c4x.h */
#define CLASS_MAX_NREGS(CLASS, MODE)   \
    ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

#define MODES_TIEABLE_P(MODE1, MODE2) 0

/* I might need more than this, but I decdided to err on the side of minimizing
 * bloat.
 */
#define IN_RANGE_P(val, bottom, top) \
    (bottom <= val && val <= top)

#define CONST_OK_FOR_LETTER_P(value, c)                \
    ( ((c) == 'I') ? IN_RANGE_P (value, -32768, 32767) \
    : ((c) == 'J') ? IN_RANGE_P (value, 0, 7)          \
    : ((c) == 'K') ? IN_RANGE_P (value, 0, 15)         \
    : ((c) == 'L') ? IN_RANGE_P (value, -16, 15)       \
    : ((c) == 'M') ? IN_RANGE_P (value, 0, 511)        \
    : ((c) == 'O') ? IN_RANGE_P (value, 0, 255)     \
    : 0 )

extern const enum reg_class regclass_map[FIRST_PSEUDO_REGISTER];

#define REGNO_REG_CLASS(REGNO) (regclass_map[REGNO])

#define EXTRA_CONSTRAINT(VALUE, C) \
	( ((C) == 'Y') ? c54x_xmem_p((VALUE), (C)) \
	: ((C) == 'S') ? c54x_smem_p((VALUE), (C)) \
	: ((C) == 'T') ? c54x_smem_p((VALUE), (C)) \
	: ((C) == 'U') ? c54x_dmad_p((VALUE), (C)) \
	: 0 )

#define EXTRA_MEMORY_CONSTRAINT(C, STR) \
	( (C) == 'Y' || (C) == 'S' || (C == 'T') || (C == 'U') )

/* Node: Frame Layout */
/* http://focus.ti.com/lit/ug/spru103g/spru103g.pdf Explains a great deal about the ABI and frame layout */

/* note to the humorless: the value of the preceding macro is unimportant */
#define STACK_GROWS_DOWNWARD 0xdeadbeef

/* FRAME info is stuff I'll have to figure out from the TI compiler */
#define FRAME_GROWS_DOWNWARD /* This is not important for ABI compat, should optionnaly check with TI  */

/* #define ARGS_GROW_DOWNWARD Args Grow Upward */

#define STARTING_FRAME_OFFSET -1 /* Local frame starts just below the frame pointer */

#define STACK_POINTER_OFFSET -1 /* Arguments start just below SP */

#define FIRST_PARM_OFFSET(FUNCDECL) 0

/* Node: Exception Handling */
/* ??? */

/* Node: Stack Checking */
/* hmmm. It sounds like the defaults work on most systems. I wonder if we have a
 * special case... */

/* Node: Frame Registers */

#define STACK_POINTER_REGNUM SP_REGNO

#define FRAME_POINTER_REGNUM AR7_REGNO

#define ARG_POINTER_REGNUM   ARG_REGNO

/* Node: 13.9.5 Eliminating Frame Pointer and Arg Pointer */

#define FRAME_POINTER_REQUIRED 0

/* We want to eliminate the fake argument register with the frame/stack pointer,
   we also try to eliminate the frame pointer with the stack pointer. */
#define ELIMINABLE_REGS  \
  {{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM}, \
  {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM}, \
  {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

#define CAN_ELIMINATE(FROM, TO) 1

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
do { \
	int offset = 0; \
	int regno; \
	if((FROM) == (ARG_POINTER_REGNUM) && (TO) == (FRAME_POINTER_REGNUM)) { \
		(OFFSET) = -1; /* We have a fixed offset between the frame pointer and arg pointer */ \
	} else {  \
        /* Otherwise, we start by calculating the difference between the frame pointer  \
         * and stack pointer */ \
		for(regno = 0; regno < (FIRST_PSEUDO_REGISTER); regno++) { \
			if(regs_ever_live[regno] != 0 && call_used_regs[regno] == 0) { \
				/* If we end up here, it means that this register is saved on the stack */ \
				offset -= 1; \
			} \
		} \
        offset -= get_frame_size(); \
        /* offset now contains the difference between the stack pointer \
         * and frame pointer (fp + offset = sp) */ \
        offset += (FROM) == (ARG_POINTER_REGNUM) ? -1 : 0; \
        (OFFSET) = offset; \
	} \
} while(0);

#define SMALL_REGISTER_CLASSES 1

/* Node: 13.11 Trampolines for Nested Functions */

#define TRAMPOLINE_SIZE 2 /* Just a guess for now */

#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT) \
	c54x_initialize_trampoline((TRAMP), (FNADDR), (CXT))

/* Node: Passing Function Arguments on the Stack */

#define PUSH_ARGS 1

#define PUSH_ROUNDING(BYTES) (BYTES)

/* The caller does all the popping. */
#define RETURN_POPS_ARGS(FUNDECL, FUNTYPE, STACKSIZE) 0

/* Node: Passing Arguments in Registers */

struct cumul_args {
	int has_varargs;
	int numarg;
};

#define CUMULATIVE_ARGS struct cumul_args

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, N_NAMED_ARGS) \
	(init_cumulative_args(&(CUM), (FNTYPE), (LIBNAME), (FNDECL)))

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
	(function_arg(&(CUM), (MODE), (TYPE), (NAMED)))

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
	(function_arg_advance(&(CUM), (MODE), (TYPE), (NAMED)))

#define FUNCTION_ARG_REGNO_P(REGNO) (REGNO == A_REGNO)

/* Node: 13.9.8 How Scalar Function Values Are Returned */

#define FUNCTION_VALUE(VALTYPE, FUNC) \
	gen_rtx_REG(TYPE_MODE(VALTYPE), A_REGNO) /* Values are returned in A */

#define LIBCALL_VALUE(MODE) \
	gen_rtx_REG((MODE), A_REGNO)

#define FUNCTION_VALUE_REGNO_P(REGNO) \
	((REGNO) == A_REGNO)

/* Node: 13.9.12 Generating Code for Profiling */

#define FUNCTION_PROFILER {}

/* Node: 13.13 Addressing Modes */

#define MAX_REGS_PER_ADDRESS 2

#define LEGITIMATE_CONSTANT_P(X) 1 /* Not sure */

#ifdef REG_OK_STRICT
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)                         \
do {                                                                    \
  if (legitimate_address_p ((MODE), (X), 1))                            \
    goto ADDR;                                                          \
} while (0)
#else
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)                         \
do {                                                                    \
  if (legitimate_address_p ((MODE), (X), 0))                            \
    goto ADDR;                                                          \
} while (0)
#endif

#define HAVE_POST_DECREMENT 1

#define HAVE_POST_INCREMENT 1

/* Ripped from c4x, should be ok */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR, LABEL) \
do { \
  if (GET_CODE (ADDR) == PRE_DEC        \
      || GET_CODE (ADDR) == POST_DEC    \
      || GET_CODE (ADDR) == PRE_INC     \
      || GET_CODE (ADDR) == POST_INC    \
      || GET_CODE (ADDR) == POST_MODIFY \
      || GET_CODE (ADDR) == PRE_MODIFY) \
    goto LABEL; \
} while(0);

#define CONSTANT_ADDRESS_P(X) (GET_CODE (X) == SYMBOL_REF)

/* Node: 13.15 Describing Relative Costs of Operations */

#define SLOW_BYTE_ACCESS 1

/* Node: 13.17 Dividing the Output into Sections (Texts, Data, ...) */

#define TEXT_SECTION_ASM_OP "\t.sect .text"
#define DATA_SECTION_ASM_OP "\t.data"

/* Node: 13.19.1 The Overall Framework of an Assembler File */

#define ASM_APP_ON  "#APP"
#define ASM_APP_OFF "#NO_APP"

/* Node: 13.19.3 Output of Uninitialized Variables */

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs ("\t.bss\t", FILE),                     \
  assemble_name (FILE, (NAME)),         \
  fprintf (FILE, ",%u\n", (int)(ROUNDED)))

#undef ASM_OUTPUT_COMMON
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
(  fputs ("\t.globl\t", FILE),  \
   assemble_name (FILE, (NAME)),        \
   fputs ("\n\t.bss\t", FILE),  \
   assemble_name (FILE, (NAME)),        \
   fprintf (FILE, ",%u\n", (int)(ROUNDED)))

/* Node: 13.19.4 Output and Generation of Labels */

#define ASM_GENERATE_INTERNAL_LABEL(BUFFER, PREFIX, NUM) \
	sprintf((BUFFER), "*%s%lu?", (PREFIX), (unsigned long)(NUM))

#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL c54x_globalize_label

/* Node 13.19.7 Output of Assembler Instructions */

#define REGISTER_NAMES { \
    "imr", "ifr", "st0", "st1", \
    "a", "b",  "t", "trn", "ar0", "ar1", "ar2", \
    "ar3", "ar4", "ar5", "ar6", "ar7", \
    "sp", "bk", "brc", "rsa", "rea", \
    "pmst", "xpc", "dp", "arg" }

#define PRINT_OPERAND(STREAM, X, CODE) c54x_print_operand((STREAM), (X), (CODE))

#define PRINT_OPERAND_ADDRESS(STREAM, X) c54x_print_operand_address((STREAM), (X))

/* Node: 13.19.10 Assembler Commands for Alignment */

#define ASM_OUTPUT_ALIGN(STREAM, POWER) \
	fprintf((STREAM), ".align %d", (POWER))

#define ASM_OUTPUT_SKIP(STREAM, NBYTES) \
	fprintf((STREAM), ".space %d", (int)(NBYTES))

/* Node: 13.27 Miscellaneous Parameters */

#define Pmode QImode

#define FUNCTION_MODE QImode

#define MOVE_MAX 1

#define CASE_VECTOR_MODE QImode

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1
