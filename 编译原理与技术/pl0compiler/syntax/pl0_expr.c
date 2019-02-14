#include <string.h>
#include "pl0_expr.h"
#include "../lex/pl0_lex.h"
#include "pl0_syntax.h"
#include "../error/pl0_error.h"
#include "../translation/pl0_translation.h"

//规约成为的文法符号
const GSymbol reduce_to[N_E_PRODUCTIONS] = {
    G_NULL,
    G_NULL,
    G_LOGICAL_EXPR,
    G_LOGICAL_EXPR,
    G_AND_EXPR,
    G_AND_EXPR,
    G_CONDITION,
    G_CONDITION,
    G_EXPRESSION,
    G_EXPRESSION,
    G_TERM,
    G_TERM,
    G_FACTOR,
    G_FACTOR,
    G_FACTOR
};

//规约时出栈符号的数量
const int reduce_num[N_E_PRODUCTIONS] = {
    0, 1,
    3, 1,   // L
    3, 1,   // A
    3, 1,   // C
    3, 1,   // E
    3, 1,   // T
    2, 1, 3 //F
};

const int action[N_E_STATUS][N_E_NONTERMINAL] = {
    //  or   and  lop  +,-  *,/  !,-  id   (    )    $
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   }, //0
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //1
    {   1,   0,   0,   0,   0,   0,   0,   0,   0,   3,   }, //2
    {   2,   1,   0,   0,   0,   0,   0,   0,   2,   2,   }, //3
    {   2,   2,   1,   0,   0,   0,   0,   0,   2,   2,   }, //4
    {   2,   2,   2,   1,   0,   0,   0,   0,   2,   2,   }, //5
    {   2,   2,   2,   2,   1,   0,   0,   0,   2,   2,   }, //6
    {   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   }, //7
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //8
    {   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   }, //9
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //10
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //11
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //12
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //13
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //14
    {   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   }, //15
    {   2,   1,   0,   0,   0,   0,   0,   0,   2,   2,   }, //16
    {   2,   2,   1,   0,   0,   0,   0,   0,   2,   2,   }, //17
    {   2,   2,   2,   1,   0,   0,   0,   0,   2,   2,   }, //18
    {   2,   2,   2,   2,   1,   0,   0,   0,   2,   2,   }, //19
    {   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   }, //20
    {   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   }, //21
    {   1,   0,   0,   0,   0,   0,   0,   0,   1,   0,   }, //22
    {   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   }, //23
};

const int goto_s[N_E_STATUS][N_E_NONTERMINAL + N_E_TERMINAL] = {
    // ||   &&   lop  +,-  *,/  !,-  id    (    )    $    L    A    C    E    T    F
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   }, //0
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   2,   3,   4,   5,   6,   7,   }, //1
    {   10,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   }, //2
    {   3,   11,  0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   0,   0,   }, //3
    {   5,   5,   12,  0,   0,   0,   0,   0,   5,   5,   0,   0,   0,   0,   0,   0,   }, //4
    {   7,   7,   7,   13,  0,   0,   0,   0,   7,   7,   0,   0,   0,   0,   0,   0,   }, //5
    {   9,   9,   9,   9,   14,  0,   0,   0,   9,   9,   0,   0,   0,   0,   0,   0,   }, //6
    {   11,  11,  11,  11,  11,  0,   0,   0,   11,  11,  0,   0,   0,   0,   0,   0,   }, //7
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   0,   0,   0,   0,   15,  }, //8
    {   13,  13,  13,  13,  13,  0,   0,   0,   13,  13,  0,   0,   0,   0,   0,   0,   }, //9
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   16,  4,   5,   6,   7,   }, //10
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   0,   17,  5,   6,   7,   }, //11
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   0,   0,   18,  6,   7,   }, //12
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   0,   0,   0,   19,  7,   }, //13
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   0,   0,   0,   0,   0,   20,  }, //14
    {   12,  12,  12,  12,  12,  0,   0,   0,   12,  12,  0,   0,   0,   0,   0,   0,   }, //15
    {   2,   11,  0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   }, //16
    {   4,   4,   12,  0,   0,   0,   0,   0,   4,   4,   0,   0,   0,   0,   0,   0,   }, //17
    {   6,   6,   6,   13,  0,   0,   0,   0,   6,   6,   0,   0,   0,   0,   0,   0,   }, //18
    {   8,   8,   8,   8,   14,  0,   0,   0,   8,   8,   0,   0,   0,   0,   0,   0,   }, //19
    {   10,  10,  10,  10,  10,  0,   0,   0,   10,  10,  0,   0,   0,   0,   0,   0,   }, //20
    {   0,   0,   0,   0,   0,   8,   9,   21,  0,   0,   22,  3,   4,   5,   6,   7,   }, //21
    {   10,  0,   0,   0,   0,   0,   0,   0,   23,  0,   0,   0,   0,   0,   0,   0,   }, //22
    {   14,  14,  14,  14,  14,  0,   0,   0,   14,  14,  0,   0,   0,   0,   0,   0,   }, //23
};

int esymbol(GSymbol gs, int status){    //表达式中出现的文法符号标号，作为goto和action的第二维
    if(gs >= G_EQU && gs <= G_GEQ){
        return 2;
    }
    switch(gs){
    case G_OR:
        return 0;
    case G_AND:
        return 1;
    case G_PLUS:
        return 3;
    case G_MINUS:   //'-' 既可能为负号，也可能为减号
        return action[status][3] != 0 ? 3 : 5;
    case G_TIMES:
    case G_SLASH:
        return 4;
    case G_NOT:
    case G_ODD:
        return 5;
    case G_VAR_CONST_NAME:
    case G_PROCEDURE_CALL:
    case G_NUMBER:
        return 6;
    case G_LPAREN:
        return 7;
    case G_RPAREN:  //右括号即可能为表达式中的部分，也可能为终结符
        return action[status][8] != 0 ? 8 : 9;
    case G_LOGICAL_EXPR:
        return 10;
    case G_AND_EXPR:
        return 11;
    case G_CONDITION:
        return 12;
    case G_EXPRESSION:
        return 13;
    case G_TERM:
        return 14;
    case G_FACTOR:
        return 15;
    default:
        return 9;
    }
}

void fputs_s(const char* str, FILE *fout){
    if(fout == NULL){
        return;
    }
    fputs(str, fout);
}

void lr_get_token(EStack *estack){
    Stack *stack = estack->stack;
    PL0Lex *lex = estack->lex;
    LR_Symbol *symbol = &(estack->symbol);
    int *parenthesis_depth = &(estack->parenthesis_depth);
    //当调用了LL分析器分析后，词法分析器已经指向下一词法单元，不能再次读取，否则出错
    if(symbol->gsymbol != G_VAR_CONST_NAME && symbol->gsymbol != G_PROCEDURE_CALL){
        PL0Lex_get_token(lex);
    }

    /*
    //强行处理括号不匹配会导致call func()的失败，故此删去，正常处理即可
    while(*parenthesis_depth == 0 && lex->last_token_type == TOKEN_RPAREN){ //处理括号不匹配
        PL0_print_error(lex, "unexpected ')' in expression. (LR-analyzer messege)");
        PL0Lex_get_token(lex);
    }
    */

    if(lex->last_token_type == TOKEN_LPAREN){
        (*parenthesis_depth)++;
    }
    else if(lex->last_token_type == TOKEN_RPAREN){
        (*parenthesis_depth)--;
    }

    //此时词法分析器所给出词法单元未下一需要分析的单元，若为ID或CALL，应先使用LL分析
    //为了防止错误分析，要先判断状态是否合法，若可以预计接下来动作不合法，则不应该分析，而是直接在此处结束
    if(lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_CALL){
        int status = stack->buffer[stack->sp - 1].status;
        int es = 6;     //两个词都意味着下个esymbol为6
        if(action[status][es] == ACTION_EMPTY){ //预计出错
            symbol->gid = stack->gid ++;
            if(lex->last_token_type == TOKEN_IDENTIFIER){
                symbol->gsymbol = G_VAR_CONST_NAME;
            }
            else{
                symbol->gsymbol = G_PROCEDURE_CALL;
            }
            return;
        }
        fputs_s("====================Analysis identifier in LL-analyzer====================\n", stack->f_stack);
        fflush(stack->f_stack);
        Attr *fattr = copy_Attr(estack->attr);
        if(lex->last_token_type == TOKEN_IDENTIFIER){
            stack_push(stack, G_VAR_CONST_NAME);
            symbol->gid = stack->buffer[stack->sp - 1].id;
            var_const_name(lex, fattr);
            code_append(fattr->code, LD);
            symbol->gsymbol = G_VAR_CONST_NAME;
        }
        else if(lex->last_token_type == TOKEN_CALL){
            stack_push(stack, G_PROCEDURE_CALL);
            symbol->gid = stack->buffer[stack->sp - 1].id;
            procedure_call(lex, fattr);
            symbol->gsymbol = G_PROCEDURE_CALL;
        }
        fputs_s("-------------------------------Analysis end-------------------------------\n", stack->f_stack);
        fflush(stack->f_stack);
        symbol->attr = fattr;
    }
    else{   //否则TOKEN_TYPE就是GSymbol
        symbol->gid = stack->gid ++;
        symbol->gsymbol = (GSymbol)lex->last_token_type;
        symbol->attr = copy_Attr(estack->attr);
        if(symbol->gsymbol == G_NUMBER){
            code_append_arg(symbol->attr->code, PUT, lex->last_num, FALSE);
        }
    }

}

void lr_expression(Stack *stack, PL0Lex *lex, Attr *attr){
    EStack _estack;             //在栈上分配空间，无需释放
    EStack *estack = &_estack;
    estack_init(estack, stack, lex, attr);
    LR_Symbol *s = &(estack->symbol);
    stack = estack->stack;                  //其实没变，只是为了清晰
    while(1){
        int status = stack->buffer[stack->sp - 1].status;
        int es = esymbol(s->gsymbol, status);
        if(action[status][es] == ACTION_SHIFT){
            int next_status = goto_s[status][es];
            estack_push(estack, s->gsymbol, next_status, s->gid);
            stack->buffer[stack->sp - 1].attr = s->attr;
            lr_get_token(estack);
        }
        else if(action[status][es] == ACTION_REDUCE){
            int production_id = goto_s[status][es];
            int pop_num = reduce_num[production_id];
            int reduce_symbol = reduce_to[production_id];
            StackElement e = {reduce_symbol, stack->gid, 0};
            StackElement old[3];
            for(int i=0; i<pop_num; i++){
                add_tree_edge(stack->f_tree, e, stack->buffer[stack->sp - 1], get_lang(default_language));
                old[i] = stack->buffer[stack->sp - 1];
                _stack_pop(stack);
            }
            int reduce_status = stack->buffer[stack->sp - 1].status;
            int next_status = goto_s[reduce_status][esymbol(reduce_symbol, status)];
            estack_push(estack, reduce_to[production_id], next_status, stack->gid++);
            s->gid = stack->gid ++;

            build_attr(estack, production_id, old);

            //estack_print(stack, get_lang(default_language));
        }
        else if(action[status][es] == ACTION_ACCEPT){
            code_extend(estack->attr->code, stack->buffer[stack->sp - 1].attr->code);
            break;
        }
        else{   //ERROR, 此处可根据状态处理不同错误
            PL0_print_error(lex, "unexpected symbol in expression. (LR-analyzer messege)");
            break;  //一般错误都直接结束，lr_get_token的实现保证了不会将错误的东西在LR分析器里提前分析
            /*
            do{
                //PL0Lex_get_token(lex);
                lr_get_token(estack);   //不断读取直至状态正常或文件尾
                es = esymbol(s->gsymbol, status);
            }while(action[status][es] == ACTION_EMPTY && s->gsymbol != G_NULL);
            break;
            */
        }
    }
    estack_destroy(estack);
}

void estack_print(EStack *estack, const char *tostring[]){
    Stack *stack = estack->stack;
    FILE *fout = stack->f_stack;
    if(fout == NULL){
        return;
    }
    fprintf(fout, "%d ", stack->buffer[stack->expr_sp].status);
    for(int i=stack->expr_sp + 1; i< stack->sp; i++){
        fprintf(fout, "%s %d ", tostring[stack->buffer[i].g], stack->buffer[i].status);
    }
    fprintf(fout, "(input: %s)", tostring[estack->symbol.gsymbol]);
    fprintf(fout, "\n");
    fflush(fout);
}

void estack_push(EStack *estack, GSymbol g, int status, int gid){
    Stack *stack = estack->stack;
    _stack_push(stack, g);
    stack->buffer[stack->sp - 1].status = status;
    stack->buffer[stack->sp - 1].id = gid;
    stack->buffer[stack->sp - 1].attr = new_Attr();
    estack_print(estack, get_lang(default_language));
}

void estack_init(EStack *estack, Stack *stack, PL0Lex *lex, Attr *attr){
    fputs_s("==============================Expression LR analysis start==============================\n", stack->f_stack);
    fflush(stack->f_stack);
    //init stack in estack
    estack->stack = stack;
    stack->expr_sp = stack->sp;
    _stack_push(stack, G_NULL);
    stack->buffer[stack->sp - 1].status = 1;

    estack->lex = lex;
    estack->parenthesis_depth = 0;
    estack->attr = attr;

    //init symbol
    estack->symbol.gsymbol = G_VAR_CONST_NAME;         //进入lr分析时，词法分析器已经指向表达式的第一个项，此处希望设定s的初始值却不读入下一个词法记号
    lr_get_token(estack);   //参见lr_get_token的实现

}

void estack_destroy(EStack *estack){
    Stack *stack = estack->stack;
    StackElement top = stack->buffer[stack->sp - 1];
    while(stack_top(stack) != G_NULL){
        top = stack->buffer[stack->sp - 1];
        _stack_pop(stack);
    }
    _stack_pop(stack);
    add_tree_edge(stack->f_tree, stack->buffer[stack->sp - 1], top, get_lang(default_language));
    fputs_s("-------------------------------Expression LR analysis end-------------------------------\n", stack->f_stack);
    fflush(stack->f_stack);
}

void build_attr(EStack *estack, int production_id, StackElement old[3]){
    Stack *stack = estack->stack;
    Attr *nattr = stack->buffer[stack->sp - 1].attr = new_Attr();
    int true_tag;
    int false_tag;
    int end_tag;
    switch(production_id){
    case 1:
        nattr->code = old[0].attr->code;
        break;
    case 2:
        true_tag = new_tag();
        end_tag = new_tag();
        nattr->code = old[2].attr->code;
        code_append_tag(nattr->code, JPC, true_tag);
        code_extend(nattr->code, old[0].attr->code);
        code_append_tag(nattr->code, JMP, end_tag);
        code_append(nattr->code, INULL);
        set_tag(true_tag, nattr->code->ed);
        code_append_arg(nattr->code, PUT, 1, FALSE);
        code_append(nattr->code, INULL);
        set_tag(end_tag, nattr->code->ed);
        break;
    case 3:
        nattr->code = old[0].attr->code;
        break;
    case 4:
        false_tag = new_tag();
        end_tag = new_tag();
        nattr->code = old[2].attr->code;
        code_append(nattr->code, NOT);
        code_append_tag(nattr->code, JPC, false_tag);
        code_extend(nattr->code, old[0].attr->code);
        code_append_tag(nattr->code, JMP, end_tag);
        code_append(nattr->code, INULL);
        set_tag(false_tag, nattr->code->ed);
        code_append_arg(nattr->code, PUT, 0, FALSE);
        code_append(nattr->code, INULL);
        set_tag(end_tag, nattr->code->ed);
        break;
    case 5:
        nattr->code = old[0].attr->code;
        break;
    case 6:
        nattr->code = old[2].attr->code;
        code_extend(nattr->code, old[0].attr->code);
        code_extend(nattr->code, gen_swap_stack_top());
        switch(old[1].g){
        case G_LES:
            code_append(nattr->code, LT);
            break;
        case G_LEQ:
            code_append(nattr->code, LE);
            break;
        case G_GTR:
            code_append(nattr->code, GT);
            break;
        case G_GEQ:
            code_append(nattr->code, GE);
            break;
        case G_EQU:
            code_append(nattr->code, EQ);
            break;
        case G_NEQ:
            code_append(nattr->code, EQ);
            code_append(nattr->code, NOT);
            break;
        default:
            break;
        }
        break;
    case 7:
        nattr->code = old[0].attr->code;
        break;
    case 8:
        nattr->code = old[2].attr->code;
        code_extend(nattr->code, old[0].attr->code);
        code_extend(nattr->code, gen_swap_stack_top());
        switch(old[1].g){
        case G_PLUS:
            code_append(nattr->code, ADD);
            break;
        case G_MINUS:
            code_append(nattr->code, SUB);
            break;
        default:
            break;
        }
        break;
    case 9:
        nattr->code = old[0].attr->code;
        break;
    case 10:
        nattr->code = old[2].attr->code;
        code_extend(nattr->code, old[0].attr->code);
        code_extend(nattr->code, gen_swap_stack_top());
        switch(old[1].g){
        case G_TIMES:
            code_append(nattr->code, MUL);
            break;
        case G_SLASH:
            code_append(nattr->code, DIV);
            break;
        default:
            break;
        }
        break;
    case 11:
        nattr->code = old[0].attr->code;
        break;
    case 12:
        nattr->code = old[0].attr->code;
        switch(old[1].g){
        case G_MINUS:
            code_append_arg(nattr->code, PUT, 0, FALSE);
            code_append(nattr->code, SUB);
            break;
        case G_NOT: //not a <=> a==0
            code_append_arg(nattr->code, PUT, 0, FALSE);
            code_append(nattr->code, EQ);
            break;
        default:
            break;
        }
        break;
    case 13:
        nattr->code = old[0].attr->code;
        break;
    case 14:
        nattr->code = old[1].attr->code;
        break;
    default:
        break;
    }
}

/*
Tree_Node node[MAX_STACK_LEN];
int tn = 0;

Tree_Edge edge_pool[MAX_STACK_LEN];
int en = 0;

Tree_Edge *edge[MAX_STACK_LEN] = {NULL};

int root = -1;

void init_tree(){
    tn = 0;
    memset(edge, 0, sizeof(edge));
    root = -1;
    en = 0;
}

int new_node(GSymbol gs){
    node[tn].attr = new_Attr();
    node[tn].gs = gs;
    node[tn].parent = 0;
    tn ++;
    return tn - 1;
}

void add_edge(int s, int t){
    edge_pool[en].to = t;
    edge_pool[en].nxt = edge[s];
    edge[s] = &edge_pool[en];
    node[t].parent = s;
}
*/
