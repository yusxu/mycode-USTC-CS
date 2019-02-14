#ifndef __PL0_SYNTAX_H__
#define __PL0_SYNTAX_H__

#include "../common/common.h"
#include "../translation/pl0_translation.h"

#define N_TERMINAL (G_NONTERMINAL-3)    //去掉三个占位符号
#define N_NONTERMINAL (G_EPSILON-G_NONTERMINAL-1)
#define N_GSYMBOL (G_EPSILON + 1)
#define MAX_PRODUCTION_LEN 10    //产生式最大长度
#define N_PRODUCTIONS 92    //产生式数量
#define MAX_STACK_LEN 1000

#define G_NULL G_TERMINAL   //G_NULL和G_TERMINAL共用枚举值，保证GSymbol和Token类型的一致

typedef enum _tGSymbol {    //grammar symbol
    G_TERMINAL,    /* Below are terminal grammar symbol, same as lexical token */
    G_IDENTIFIER,
    G_NUMBER,
    G_SYMBOL,   /* Below are accepted symbols */
    G_PLUS,     /* + */
    G_MINUS,    /* - */
    G_TIMES,    /* * */
    G_SLASH,    /* / */
    G_EQU,      /* = */
    G_NEQ,      /* != */
    G_LES,      /* < */
    G_LEQ,      /* <= */
    G_GTR,      /* > */
    G_GEQ,      /* >= */
    G_LPAREN,   /* ( */
    G_RPAREN,   /* ) */
    G_COMMA,    /* , */
    G_SEMICOLON,/* ; */
    G_PERIOD,   /* . */
    G_BECOMES,  /* := */
    G_AND,       /* && */
    G_OR,        /* || */
    G_NOT,       /* ! */
    G_LSQUARE,   /* [ */
    G_RSQUARE,   /* ] */
    G_LCURLY,    /* { */
    G_RCURLY,    /* } */
    G_RESWORDS, /* Below are reserved words */
    G_VAR,
    G_CONST,
    G_PROCEDURE,
    G_BEGIN,
    G_END,
    G_IF,
    G_THEN,
    G_DO,
    G_WHILE,
    G_CALL,
    G_ODD,
    G_RETURN,
    G_ELSE,
    G_ELIF,
    G_ENDIF,
    G_PRINT,
    G_NONTERMINAL,    /* Below are nonterminal symbols */
    G_PROGRAM,
    G_PROGRAM_BODY,
    G_DECLARATION,
    G_CONST_DECLARATION,
    G_CONSTS_TABLE,
    G_EX_CONSTS_TABLE,
    G_VAR_DECLARATION,
    G_VARS_TABLE,
    G_EX_VARS_TABLE,
    G_PROCEDURE_DECLARATION,
    G_CONDITION,
    G_EX_CONDITION,
    G_EXPRESSION,
    G_TERM,
    G_EX_EXPRESSION,
    G_FACTOR,
    G_EX_TERM,
    G_SEQUENCE,
    G_STATEMENT,
    G_VAR_CONST_NAME,
    G_LENGTH_SUFFIX,
    G_REFERENCE_SUFFIX,
    G_CONST_VALUE,
    G_INTEGER,
    G_VECS_OR_SCALARS,
    G_VECTORS_TABLE,
    G_VECTOR,
    G_EX_VECTORS_TABLE,
    G_INTEGERS_TABLE,
    G_EX_INTEGERS_TABLE,
    G_EX_BRANCH,
    G_ARGS_TABLE,
    G_EX_ARGS_TABLE,
    G_LOGICAL_EXPR,
    G_EX_LOGICAL_EXPR,
    G_AND_EXPR,
    G_EX_AND_EXPR,
    G_EMPTY_OR_VARS,
    G_FORMAL_PARAMETERS_SUFFIX,
    G_ACTUAL_PARAMETERS_SUFFIX,
    G_EXPRESSIONS_TABLE,
    G_EX_EXPRESSIONS_TABLE,
    G_PROCEDURE_CALL,
    G_NAME_IN_DECLARATION,
    G_NUMBER_OR_ID,
    G_EPSILON       //epsilon
} GSymbol;


const char *zh_GSymbols[N_GSYMBOL];

const char *en_GSymbols[N_GSYMBOL];

const char *sen_GSymbols[N_GSYMBOL];

const GSymbol productions[N_PRODUCTIONS][MAX_PRODUCTION_LEN];

Language default_language;
BOOL expr_plus;

const char** get_lang(Language l);
BOOL is_terminal(GSymbol g);
BOOL is_nonterminal(GSymbol g);
BOOL is_valid(GSymbol g);
const GSymbol *get_alpha(int n);
const GSymbol get_A(int n);
void print_syntax(FILE *fout, Language lan);
void print_production(FILE *fout, Language lan, int i);
BOOL check_syntax();

typedef struct _tStackElemet {
    GSymbol g;
    int id;
    int status;
    Attr *attr;
} StackElement;

typedef struct _tStack {
    FILE *f_stack;
    FILE *f_tree;
    int gid;
    StackElement buffer[MAX_STACK_LEN];
    int sp;
    int expr_sp;
} Stack;

void add_tree_edge(
    FILE *fout,
    StackElement parent,
    StackElement son,
    const char *tostring[]
);

void stack_init(Stack *stack, FILE *f_stack, FILE *f_tree);
BOOL _stack_push(Stack *stack, GSymbol t);
BOOL stack_push(Stack *stack, GSymbol t);
GSymbol stack_top(Stack *stack);
GSymbol _stack_pop(Stack *stack);
GSymbol stack_pop(Stack *stack);
BOOL produce(Stack *stack, int n);


#endif //pl0_syntax.h
