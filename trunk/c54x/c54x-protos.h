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

extern int  c54x_hard_regno_mode_ok (unsigned int, enum machine_mode);
extern void init_cumulative_args (CUMULATIVE_ARGS*, tree, rtx, tree);
extern rtx  function_arg (CUMULATIVE_ARGS*, enum machine_mode, tree, int);
extern void function_arg_advance (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern int  legitimate_address_p (enum machine_mode, rtx, int);
extern int  c54x_expand_movqi(rtx[]);
extern void c54x_expand_addqi(rtx []);
extern void c54x_initialize_trampoline(rtx, rtx, rtx);
extern void c54x_globalize_label(FILE*, const char*);
extern void c54x_override_options(void);
extern int  c54x_xmem_p(rtx, char);
extern int  c54x_smem_p(rtx, char);
extern int  c54x_dmad_p(rtx, char);
extern void c54x_print_operand(FILE*, rtx, char);
extern void c54x_print_operand_address(FILE*, rtx);
extern void c54x_expand_prologue(void);
extern void c54x_expand_epilogue(void);
extern int  c54x_save_register_p(int);
extern void c54x_file_start(void);
