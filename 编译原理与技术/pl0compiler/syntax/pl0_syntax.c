#include <stdio.h>
#include <stdlib.h>
#include "../common/common.h"
#include "pl0_tax.h"
#include "pl0_syntax.h"

const char *zh_GSymbols[N_GSYMBOL] = {
    "NULL",
    "ident",
    "number",
    "符号",
    "+",
    "-",
    "*",
    "/",
    "=",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "(",
    ")",
    ",",
    ";",
    ".",
    ":=",
    "&&",
    "||",
    "!",
    "[",
    "]",
    "{",
    "}",
    "保留字",
    "var",
    "const",
    "procedure",
    "begin",
    "end",
    "if",
    "then",
    "do",
    "while",
    "call",
    "odd",
    "return",
    "else",
    "elif",
    "endif",
    "print",
    "非终结符",
    "程序",
    "程序体",
    "声明",
    "常量定义",
    "常量名表",
    "余常量名表",
    "变量定义",
    "变量名表",
    "余变量名表",
    "过程",
    "条件",
    "余条件",
    "表达式",
    "项",
    "余表达式",
    "因子",
    "余项",
    "语句序列",
    "语句",
    "变量常量名",
    "长度后缀",
    "引用后缀",
    "常量值",
    "整数",
    "数或向量表",
    "向量表",
    "向量",
    "余向量表",
    "整数表",
    "余整数表",
    "余分支",
    "参数表",
    "余参数表",
    "逻辑式",
    "余逻辑式",
    "与式",
    "余与式",
    "可空名表",
    "形参后缀",
    "实参后缀",
    "表达式表",
    "余式表",
    "过程调用",
    "定义名",
    "数或标识",
    "ε",
};

const char *en_GSymbols[N_GSYMBOL] = {
    "NULL",
    "ident",
    "number",
    "Symbols",
    "+",
    "-",
    "*",
    "/",
    "=",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "(",
    ")",
    ",",
    ";",
    ".",
    ":=",
    "&&",
    "||",
    "!",
    "[",
    "]",
    "{",
    "}",
    "Reswords",
    "var",
    "const",
    "procedure",
    "begin",
    "end",
    "if",
    "then",
    "do",
    "while",
    "call",
    "odd",
    "return",
    "else",
    "elif",
    "endif",
    "print",
    "Nonterminal_Symbols",
    "Program",
    "Program_Body",
    "Declaration",
    "Const_Declaration",
    "Consts_Table",
    "EX_Consts_Table",
    "Var_Declaration",
    "Vars_Table",
    "EX_Vars_Table",
    "Procedure_Declaration",
    "Condition",
    "EX_Condition",
    "Expression",
    "Term",
    "EX_Expression",
    "Factor",
    "EX_Term",
    "Seqence",
    "Statement",
    "Var_Const_Name",
    "Length_Suffix",
    "Reference_Suffix",
    "Const_Value",
    "Integer",
    "Vecs_Or_Scalars",
    "Vectors_Table",
    "Vector",
    "EX_Vectors_Table",
    "Integers_Table",
    "EX_Integers_Table",
    "EX_Branch",
    "Args_Table",
    "EX_Args_Table",
    "Logical_Expr",
    "EX_Logical_Expr",
    "And_Expr",
    "EX_And_Expr",
    "Empty_Or_Vars",
    "Formal_Parameters_Table",
    "Actual_Parameters_Table",
    "Expressions_Table",
    "EX_Expressions_Table",
    "Procedure_Call",
    "Name_In_Declaration",
    "Number_Or_ID",
    "epsilon",
};

const char *sen_GSymbols[N_GSYMBOL] = { //英文缩写
    "NULL",
    "ident",
    "number",
    "Symbols",
    "+",
    "-",
    "*",
    "/",
    "=",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "(",
    ")",
    ",",
    ";",
    ".",
    ":=",
    "&&",
    "||",
    "!",
    "[",
    "]",
    "{",
    "}",
    "Reswords",
    "var",
    "const",
    "procedure",
    "begin",
    "end",
    "if",
    "then",
    "do",
    "while",
    "call",
    "odd",
    "return",
    "else",
    "elif",
    "endif",
    "print",
    "Nonterminal_Symbols",
    "P",
    "PB",
    "D",
    "CD",
    "CT",
    "ECT",
    "VD",
    "VT",
    "EVT",
    "PD",
    "C",
    "EC",
    "E",
    "T",
    "EE",
    "F",
    "ET",
    "SS",
    "S",
    "VCN",
    "LS",
    "RS",
    "CV",
    "I",
    "VOS",
    "VS",
    "VEC",
    "EVS",
    "IS",
    "EIS",
    "EBR",
    "AT",
    "EAT",
    "LE",
    "ELE",
    "AE",
    "EAE",
    "EOV",
    "FPS",
    "APS",
    "EST",
    "EEST",
    "PC",
    "NID",
    "NOI",
    "ε",
};


const GSymbol productions[N_PRODUCTIONS][MAX_PRODUCTION_LEN] = {
    {G_PROGRAM, G_PROGRAM_BODY, G_PERIOD, G_EPSILON},          //P --> PB .
    {G_PROGRAM_BODY, G_DECLARATION, G_PROGRAM_BODY, G_EPSILON},    //PB --> D PB
    {G_PROGRAM_BODY, G_STATEMENT, G_EPSILON},
    {G_DECLARATION, G_CONST_DECLARATION, G_EPSILON},
    {G_DECLARATION, G_VAR_DECLARATION, G_EPSILON},
    {G_DECLARATION, G_PROCEDURE_DECLARATION, G_EPSILON},
    {G_CONST_DECLARATION, G_CONST, G_CONSTS_TABLE, G_SEMICOLON, G_EPSILON},
    {G_CONSTS_TABLE, G_NAME_IN_DECLARATION, G_EQU, G_CONST_VALUE, G_EX_CONSTS_TABLE, G_EPSILON},
    {G_EX_CONSTS_TABLE, G_COMMA, G_CONSTS_TABLE, G_EPSILON},
    {G_EX_CONSTS_TABLE, G_EPSILON},
    {G_VAR_DECLARATION, G_VAR, G_VARS_TABLE, G_SEMICOLON, G_EPSILON},
    {G_VARS_TABLE, G_NAME_IN_DECLARATION, G_EX_VARS_TABLE, G_EPSILON},
    {G_EX_VARS_TABLE, G_COMMA, G_VARS_TABLE, G_EPSILON},
    {G_EX_VARS_TABLE, G_EPSILON},
    {G_PROCEDURE_DECLARATION, G_PROCEDURE, G_IDENTIFIER, G_FORMAL_PARAMETERS_SUFFIX, G_SEMICOLON, G_PROGRAM_BODY, G_SEMICOLON, G_EPSILON},
    {G_CONDITION, G_ODD, G_EXPRESSION, G_EPSILON},
    {G_CONDITION, G_EXPRESSION, G_EX_CONDITION, G_EPSILON},
    {G_EX_CONDITION, G_EQU, G_EXPRESSION, G_EPSILON},
    {G_EX_CONDITION, G_NEQ, G_EXPRESSION, G_EPSILON},
    {G_EX_CONDITION, G_LES, G_EXPRESSION, G_EPSILON},
    {G_EX_CONDITION, G_GTR, G_EXPRESSION, G_EPSILON},
    {G_EX_CONDITION, G_LEQ, G_EXPRESSION, G_EPSILON},
    {G_EX_CONDITION, G_GEQ, G_EXPRESSION, G_EPSILON},
    {G_EXPRESSION, G_TERM, G_EX_EXPRESSION, G_EPSILON},
    {G_TERM, G_FACTOR, G_EX_TERM, G_EPSILON},
    {G_EX_EXPRESSION, G_PLUS, G_TERM, G_EX_EXPRESSION, G_EPSILON},
    {G_EX_EXPRESSION, G_MINUS, G_TERM, G_EX_EXPRESSION, G_EPSILON},
    {G_EX_EXPRESSION, G_EPSILON},
    {G_FACTOR, G_VAR_CONST_NAME, G_EPSILON},
    {G_FACTOR, G_NUMBER, G_EPSILON},
    {G_FACTOR, G_MINUS, G_FACTOR, G_EPSILON},
    {G_FACTOR, G_LPAREN, G_EXPRESSION, G_RPAREN, G_EPSILON},
    {G_EX_TERM, G_TIMES, G_FACTOR, G_EX_TERM, G_EPSILON},
    {G_EX_TERM, G_SLASH, G_FACTOR, G_EX_TERM, G_EPSILON},
    {G_EX_TERM, G_EPSILON},
    {G_SEQUENCE, G_STATEMENT, G_SEMICOLON, G_SEQUENCE, G_EPSILON},
    {G_STATEMENT, G_VAR_CONST_NAME, G_BECOMES, G_EXPRESSION, G_EPSILON},
    {G_STATEMENT, G_PROCEDURE_CALL, G_EPSILON},
    {G_STATEMENT, G_BEGIN, G_SEQUENCE, G_END, G_EPSILON},
    {G_STATEMENT, G_IF, G_LOGICAL_EXPR, G_THEN, G_STATEMENT, G_EX_BRANCH, G_EPSILON},
    {G_STATEMENT, G_WHILE, G_LOGICAL_EXPR, G_DO, G_STATEMENT, G_EPSILON},
    //productions below are added in lab-3
    {G_STATEMENT, G_DO, G_STATEMENT, G_WHILE, G_LOGICAL_EXPR, G_EPSILON},
    {G_STATEMENT, G_RETURN, G_EXPRESSION, G_EPSILON},
    {G_NAME_IN_DECLARATION, G_IDENTIFIER, G_LENGTH_SUFFIX, G_EPSILON},
    {G_LENGTH_SUFFIX, G_LSQUARE, G_NUMBER_OR_ID, G_RSQUARE, G_LENGTH_SUFFIX, G_EPSILON},
    {G_LENGTH_SUFFIX, G_EPSILON},
    {G_REFERENCE_SUFFIX, G_LSQUARE, G_EXPRESSION, G_RSQUARE, G_REFERENCE_SUFFIX, G_EPSILON},
    {G_REFERENCE_SUFFIX, G_EPSILON},
    {G_CONST_VALUE, G_INTEGER, G_EPSILON},
    {G_CONST_VALUE, G_LCURLY, G_VECS_OR_SCALARS, G_RCURLY, G_EPSILON},
    {G_INTEGER, G_NUMBER, G_EPSILON},
    {G_INTEGER, G_MINUS, G_NUMBER, G_EPSILON},
    {G_VECS_OR_SCALARS, G_VECTORS_TABLE, G_EPSILON},
    {G_VECS_OR_SCALARS, G_INTEGERS_TABLE, G_EPSILON},
    {G_VECTORS_TABLE, G_VECTOR, G_EX_VECTORS_TABLE, G_EPSILON},
    {G_VECTOR, G_LCURLY, G_INTEGERS_TABLE, G_RCURLY, G_EPSILON},
    {G_EX_VECTORS_TABLE, G_COMMA, G_VECTORS_TABLE, G_EPSILON},
    {G_EX_VECTORS_TABLE, G_EPSILON},
    {G_INTEGERS_TABLE, G_INTEGER, G_EX_INTEGERS_TABLE, G_EPSILON},
    {G_EX_INTEGERS_TABLE, G_COMMA, G_INTEGERS_TABLE, G_EPSILON},
    {G_EX_INTEGERS_TABLE, G_EPSILON},
    {G_EX_BRANCH, G_ELIF, G_LOGICAL_EXPR, G_THEN, G_STATEMENT, G_EX_BRANCH, G_EPSILON},
    {G_EX_BRANCH, G_ELSE, G_STATEMENT, G_EPSILON},
    {G_EX_BRANCH, G_EPSILON},
    {G_ARGS_TABLE, G_IDENTIFIER, G_EX_ARGS_TABLE, G_EPSILON},
    {G_EX_ARGS_TABLE, G_COMMA, G_IDENTIFIER, G_EX_ARGS_TABLE, G_EPSILON},
    {G_EX_ARGS_TABLE, G_EPSILON},
    {G_LOGICAL_EXPR, G_AND_EXPR, G_EX_LOGICAL_EXPR, G_EPSILON},
    {G_EX_LOGICAL_EXPR, G_OR, G_AND_EXPR, G_EX_LOGICAL_EXPR, G_EPSILON},
    {G_EX_LOGICAL_EXPR, G_EPSILON},
    {G_AND_EXPR, G_CONDITION, G_EX_AND_EXPR, G_EPSILON},
    {G_EX_AND_EXPR, G_AND, G_CONDITION, G_EX_AND_EXPR, G_EPSILON},
    {G_EX_AND_EXPR, G_EPSILON},
    {G_CONDITION, G_NOT, G_CONDITION, G_EPSILON},
    {G_ARGS_TABLE, G_EPSILON},
    {G_STATEMENT, G_PRINT, G_EXPRESSION, G_EPSILON},
    {G_EPSILON},
    {G_EPSILON},
    {G_SEQUENCE, G_EPSILON},
    {G_FORMAL_PARAMETERS_SUFFIX, G_LPAREN, G_ARGS_TABLE, G_RPAREN, G_EPSILON},
    {G_FORMAL_PARAMETERS_SUFFIX, G_EPSILON},
    {G_ACTUAL_PARAMETERS_SUFFIX, G_LPAREN, G_EXPRESSIONS_TABLE, G_RPAREN, G_EPSILON},
    {G_ACTUAL_PARAMETERS_SUFFIX, G_EPSILON, G_EPSILON},
    {G_EXPRESSIONS_TABLE, G_EXPRESSION, G_EX_EXPRESSIONS_TABLE, G_EPSILON},
    {G_EXPRESSIONS_TABLE, G_EPSILON},
    {G_EX_EXPRESSIONS_TABLE, G_COMMA, G_EXPRESSION, G_EX_EXPRESSIONS_TABLE, G_EPSILON},
    {G_EX_EXPRESSIONS_TABLE, G_EPSILON},
    {G_FACTOR, G_PROCEDURE_CALL, G_EPSILON},
    {G_PROCEDURE_CALL, G_CALL, G_IDENTIFIER, G_ACTUAL_PARAMETERS_SUFFIX, G_EPSILON},
    {G_VAR_CONST_NAME, G_IDENTIFIER, G_REFERENCE_SUFFIX, G_EPSILON},
    {G_NUMBER_OR_ID, G_NUMBER, G_EPSILON},
    {G_NUMBER_OR_ID, G_IDENTIFIER, G_EPSILON},
};

int productions_len(int num){
    for(int i=0;;i++){
        if(productions[num][i] == G_EPSILON){
            return i;
        }
    }
}

const char** get_lang(Language l){
    switch(l){
    case LANG_ZH:
        return zh_GSymbols;
    case LANG_EN:
        return en_GSymbols;
    case LANG_SEN:
        return sen_GSymbols;
    default:
        return zh_GSymbols;
    }
}

BOOL is_terminal(GSymbol g) {
    return g < G_NONTERMINAL && g > G_TERMINAL && g != G_SYMBOL && g != G_RESWORDS;
}

BOOL is_nonterminal(GSymbol g) {
    return g > G_NONTERMINAL && g < N_GSYMBOL && g != G_EPSILON;
}

BOOL is_valid(GSymbol g) {
    return is_terminal(g) || is_nonterminal(g);
}

const GSymbol *get_alpha(int n) {
    return productions[n] + 1;
}

const GSymbol get_A(int n) {
    return productions[n][0];
}

void print_syntax(FILE *fout, Language lan) {
    /*
     * 用指定语言打印句法的产生式。
     * 参数：
     *   fout: 要打印到的文件流
     *   lan: 需要的语言
     */
    if(fout == NULL)
        return;
    for(int i=0; i<N_PRODUCTIONS; i++) {
        fprintf(fout, "[%-2d]: ", i);
        print_production(fout, lan, i);
        fprintf(fout, "\n");
    }
}

BOOL check_syntax() {
    //检查句法是否符合要求
    for(int i=0; i<N_PRODUCTIONS; i++) {
        const GSymbol *p = productions[i];
        if(p[0] == G_EPSILON){
            continue;
        }
        if(!is_nonterminal(p[0])) {
            printf("ERROR: In production 'A --> alpha', 'A' must be nonterminal. (in production %d)\n", i);
            return FALSE;
        }
        if(productions_len(i) > MAX_PRODUCTION_LEN) {
            printf("ERROR: Production too long. (in production %d)\n", i);
            return FALSE;
        }
        for(int j=1; p[j]!=G_EPSILON;j++){
            if(!is_valid(p[j])){
                printf("ERROR: Invalid G_symbol is production:\n");
                print_production(stdout, default_language, i);
                printf("\n");
                return FALSE;
            }
        }
    }
    return TRUE;
}

void print_production(FILE *fout, Language lan, int i) {
    if(fout == NULL)
        return;
    const char **tostring = get_lang(lan);
    const GSymbol *p = productions[i];
    if(p[0] == G_EPSILON){
        fputs("epsilon", fout);
        return;
    }
    fputs(tostring[p[0]], fout);
    fprintf(fout, " -->");
    for(int j=1; p[j] != G_EPSILON; j++) {
        fprintf(fout, " ");
        fputs(tostring[p[j]], fout);
    }
    if(p[1] == G_EPSILON) {
        fprintf(fout, " epsilon");
    }
}

void print_stack(Stack *stack, const char *tostring[]) {
    FILE *fout = stack->f_stack;
    if(fout == NULL)
        return;
    for(int i=0; i<stack->sp; i++) {
        fprintf(fout, "%s ", tostring[stack->buffer[i].g]);
    }
    fprintf(fout, "\n");
    fflush(fout);
}

void add_tree_edge(
    FILE *fout,
    StackElement parent,
    StackElement son,
    const char *tostring[]
) {
    if(fout == NULL)
        return;
    fprintf(
        fout,
        "\t%d(\"%s\") --> %d(\"%s\") ;\n",
        parent.id,
        tostring[parent.g],
        son.id,
        tostring[son.g]
    );
    fflush(fout);
};

void stack_init(Stack *stack, FILE *f_stack, FILE *f_tree) {
    stack->sp = 0;
    stack->f_stack = f_stack;
    stack->f_tree = f_tree;
    stack->gid = 1;
    stack->expr_sp = 0;
    if(f_tree != NULL){
        fprintf(f_tree, "```mermaid\ngraph TD;\n");
    }
}

BOOL _stack_push(Stack *stack, GSymbol t) {
    if(stack->sp == MAX_STACK_LEN) {
        return FALSE;
    }
    else {
        StackElement e = {t, 0};
        stack->buffer[stack->sp] = e;
        stack->sp ++;
        return TRUE;
    }
}

BOOL stack_push(Stack *stack, GSymbol t) {
    BOOL flag = _stack_push(stack, t);
    stack->buffer[stack->sp-1].id = stack->gid;
    stack->gid ++;
    print_stack(stack, get_lang(default_language));
    return flag;
}


GSymbol stack_top(Stack *stack) {
    if(stack->sp == 0) {
        return G_NULL;
    }
    else {
        return stack->buffer[stack->sp - 1].g;
    }
}

GSymbol _stack_pop(Stack *stack) {
    if(stack->sp == 0) {
        return G_NULL;
    }
    else {
        stack->sp --;
        return stack->buffer[stack->sp].g;
    }
}

GSymbol stack_pop(Stack *stack) {
    BOOL flag = _stack_pop(stack);
    print_stack(stack, get_lang(default_language));
    return flag;
}

BOOL produce(Stack *stack, int n) {   //按第i条产生式规约， i应为0到N_PRODUCTIONS-1的整数
    const GSymbol *p = productions[n];
    if(stack_top(stack) != p[0]) {
        printf("Fetal: Can not produce by production: \n\t");
        print_production(stdout, default_language, n);
        printf("\nPlease check complier and try again.\n");
        exit(-1);
        return FALSE;
    }
    else {
        StackElement e = stack->buffer[stack->sp-1];
        _stack_pop(stack);
        int plen = productions_len(n);
        for(int i=plen-1; i>0; i--) {
            _stack_push(stack, p[i]);
        }
        for(int i=1; i<plen; i++) {
            stack->buffer[stack->sp - i].id = stack->gid ++;
            add_tree_edge(stack->f_tree, e, stack->buffer[stack->sp - i], get_lang(default_language));
        }
        if(plen == 1){    //to epsilon
            StackElement se = {G_EPSILON, stack->gid++};
            add_tree_edge(stack->f_tree, e, se, get_lang(default_language));
        }
        print_stack(stack, get_lang(default_language));
        return TRUE;
    }
}
