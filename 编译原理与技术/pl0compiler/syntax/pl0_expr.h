#ifndef __PL0_EXPR_H__
#define __PL0_EXPR_H__

#include "pl0_syntax.h"
#include "pl0_tax.h"
#include "../translation/pl0_translation.h"

#define N_E_STATUS 24
#define N_E_NONTERMINAL 11
#define N_E_TERMINAL 6
#define N_E_PRODUCTIONS 15

#define ACTION_EMPTY 0
#define ACTION_SHIFT 1
#define ACTION_REDUCE 2
#define ACTION_ACCEPT 3


const GSymbol reduce_to[N_E_PRODUCTIONS];       //规约成为的文法符号
const int reduce_num[N_E_PRODUCTIONS];          //规约时出栈符号的数量
const int action[N_E_STATUS][N_E_NONTERMINAL];  //移进或规约
const int goto_s[N_E_STATUS][N_E_NONTERMINAL + N_E_TERMINAL];    //移进后状态或规约使用产生式或规约后状态

typedef struct _tLR_Symbol{
    GSymbol gsymbol;
    int gid;
    Attr *attr;
}LR_Symbol;

typedef struct _tEStack{
    Stack *stack;
    PL0Lex *lex;
    LR_Symbol symbol;
    int parenthesis_depth;
    Attr *attr;
}EStack;

void lr_get_token(EStack *estack);
int esymbol(GSymbol gs, int status);
void lr_expression(Stack *stack, PL0Lex *lex, Attr *attr);
void estack_print(EStack *estack, const char *tostring[]);
void estack_push(EStack *estack, GSymbol g, int status, int gid);
void estack_init(EStack *estack, Stack *stack, PL0Lex *lex, Attr *attr);
void estack_destroy(EStack *estack);

void build_attr(EStack *estack, int production_id, StackElement old[3]);

/*
typedef struct _tTree_Node{
    Attr *attr;
    GSymbol gs;
    GSymbol op;
    int parent;
    int son[2];
}Tree_Node;

Tree_Node node[MAX_STACK_LEN];
int tn;

typedef struct _tTree_Edge{
    int to;
    struct _tTree_Edge *nxt;
}Tree_Edge;

Tree_Edge *edge[MAX_STACK_LEN];

int gidtonode[MAX_STACK_LEN*10];

int root;

void init_tree();
int new_node(GSymbol gs);
void add_edge(int s, int t);

*/

#endif
