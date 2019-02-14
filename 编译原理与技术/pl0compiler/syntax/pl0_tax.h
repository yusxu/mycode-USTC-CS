#ifndef __PL0_TAX_H__
#define __PL0_TAX_H__

#include "../lex/pl0_lex.h"
#include "../translation/pl0_translation.h"

/*functions for syntax analysis*/
Attr *var_const_name(PL0Lex *lex, Attr *attr);
Attr *name_in_declaration(PL0Lex *lex, Attr *attr);
Attr *length_suffix(PL0Lex *lex, Attr *attr);
Attr *reference_suffix(PL0Lex *lex, Attr *attr);
Attr *actual_parameters_suffix(PL0Lex *lex, Attr *attr);
Attr *procedure_call(PL0Lex *lex, Attr *attr);
Attr *const_value(PL0Lex *lex, Attr *attr);
Attr *integer(PL0Lex *lex, Attr *attr);
Attr *vecs_or_scalars(PL0Lex *lex, Attr *attr);
Attr *vectors_table(PL0Lex *lex, Attr *attr);
Attr *vector(PL0Lex *lex, Attr *attr);
Attr *integers_table(PL0Lex *lex, Attr *attr);
Attr *ex_branch(PL0Lex *lex, Attr *attr);
Attr *args_table(PL0Lex *lex, Attr *attr);
Attr *logical_expr(PL0Lex *lex, Attr *attr);
Attr *ex_logical_expr(PL0Lex *lex, Attr *attr);
Attr *and_expr(PL0Lex *lex, Attr *attr);
Attr *ex_and_expr(PL0Lex *lex, Attr *attr);
Attr *statement(PL0Lex *lex, Attr *attr); //analysis the statement
Attr *statements_sequence(PL0Lex *lex, Attr *attr); //analysis the statements_sequence
Attr *const_declaration(PL0Lex *lex, Attr *attr);
Attr *consts_table(PL0Lex *lex, Attr *attr);
Attr *vars_table(PL0Lex *lex, Attr *attr);
Attr *var_declaration(PL0Lex *lex, Attr *attr);
Attr *condition(PL0Lex *lex, Attr *attr); //analysis the condition statement
Attr *expression(PL0Lex *lex, Attr *attr); //analysis the expression
Attr *term(PL0Lex *lex, Attr *attr); //analysis the term
Attr *factor(PL0Lex *lex, Attr *attr); //analysis the factor
Attr *procedure_declaration(PL0Lex *lex, Attr *attr);
Attr *program_block(PL0Lex *lex, Attr *attr); //analysis the program block
Attr *program(PL0Lex *lex, Attr *attr);
void PL0TEX_analysis(PL0Lex *lex, FILE *outfile_stack, FILE *outfile_tree); //start syntax analysis

#endif //pl0_tax.h
