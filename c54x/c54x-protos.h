extern int c54x_hard_regno_mode_ok (unsigned int, enum machine_mode);
extern void init_cumulative_args (CUMULATIVE_ARGS*, tree, rtx, tree);
extern rtx function_arg (CUMULATIVE_ARGS*, enum machine_mode, tree, int);
extern void function_arg_advance (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern int legitimate_address_p (enum machine_mode, rtx, int);
