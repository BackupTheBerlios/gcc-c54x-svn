extern int c54x_hard_regno_mode_ok (unsigned int, enum machine_mode);
extern void init_cumulative_args (CUMULATIVE_ARGS*, tree, rtx, tree);
extern rtx function_arg (CUMULATIVE_ARGS*, enum machine_mode, tree, int);
extern void function_arg_advance (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern int legitimate_address_p (enum machine_mode, rtx, int);
extern int c54x_legitimize_move(enum machine_mode , rtx, rtx);
extern void c54x_initialize_trampoline(rtx, rtx, rtx);
extern void c54x_globalize_label(FILE*, const char*);
extern void c54x_override_options(void);
