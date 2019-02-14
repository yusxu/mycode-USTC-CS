#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "../lex/pl0_lex.h"
#include "../error/pl0_error.h"
#include "pl0_tax.h"
#include "pl0_syntax.h"
#include "pl0_expr.h"
#include "../translation/pl0_translation.h"

Stack __stack;
Stack *stack = &__stack;

Attr *integer(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_NUMBER)
    {
        produce(stack,50);

        attr->value = lex->last_num;

        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else if(lex->last_token_type == TOKEN_MINUS)
    {
        produce(stack,51);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        if(lex->last_token_type == TOKEN_NUMBER)
        {
            attr->value = - lex->last_num;

            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be a number behind '-'.");
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
    }
    else
    {
        PL0_print_error(lex, "integer must start with number or '-'.");
    }
    return NULL;
}


Attr *args_table(PL0Lex *lex, Attr *attr) {
    int narg = 0;
    if(lex->last_token_type != TOKEN_IDENTIFIER){
        produce(stack, 74);
        return NULL;
    }
    else
    {
        produce(stack, 64);
        narg += 1;
        var_table_append(lex->last_id, FALSE, 0, narg + 1,
            0, NULL,
            attr->procedure_index
        );

        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    while(lex->last_token_type == TOKEN_COMMA)
    {
        produce(stack, 65);
        stack_pop(stack);
        PL0Lex_get_token(lex);

        if(lex->last_token_type == TOKEN_IDENTIFIER){
            narg += 1;
            var_table_append(lex->last_id, FALSE, 0, narg + 1,
                0, NULL,
                attr->procedure_index
            );
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "missing identifier after ','.");
        }
        stack_pop(stack);
    }
    produce(stack, 66);
    return NULL;
}

Attr *var_const_name(PL0Lex *lex, Attr *attr)
{
    int pindex = attr->procedure_index;
    //Procedure_Table pinfo = procedure_table[attr->procedure_index];
    int index = -1;
    Code var_addr = code_init();
    produce(stack, 89);
    if(lex->last_token_type == TOKEN_IDENTIFIER)
    {
        index = lookup_id(lex->last_id, pindex);
        var_addr = gen_find_var_address(index, level);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "var_name or const_name must start with a id.");
    }

    // base
    code_extend(attr->code, var_addr);

    stack_pop(stack);
    attr->var_table_index = index;

    // calculate offset
    reference_suffix(lex, attr);

    // base + offset
    code_append(attr->code, ADD);
    return NULL;
}

Attr *name_in_declaration(PL0Lex *lex, Attr *attr)
{
    produce(stack, 43);
    if(lex->last_token_type == TOKEN_IDENTIFIER)
    {
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "var_name or const_name must start with a id.");
    }
    stack_pop(stack);
    length_suffix(lex, attr);
    return NULL;
}

Attr *length_suffix(PL0Lex *lex, Attr *attr)
{
    int ndim = 0;
    int usedspace = 0;
    while(lex->last_token_type == TOKEN_LSQUARE)
    {
        produce(stack, 44);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        if(lex->last_token_type == TOKEN_NUMBER)
        {
            produce(stack, 90);
            var_table[table_index - 1].dim[ndim] = lex->last_num;
            PL0Lex_get_token(lex);
        }
        else if(lex->last_token_type == TOKEN_IDENTIFIER){
            produce(stack, 91);
            int len_var_index = lookup_id(lex->last_id, attr->procedure_index);
            var_table[table_index - 1].dynamic_len_index = len_var_index;
            if(ndim >= 1){
                PL0_print_error(lex, "only support one dimensionality dynamic array.");
            }
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be number in [].");
        }
        stack_pop(stack);
        if(lex->last_token_type == TOKEN_RSQUARE)
        {
            PL0Lex_get_token(lex);
            //PL0_print_error(lex, "after]:%d\n",lex->last_token_type);
        }
        else
        {
            PL0_print_error(lex, "missing ].");
        }
        stack_pop(stack);

        if(var_table[table_index - 1].dynamic_len_index == -1){ //动态内存无需更多静态空间
            if(ndim == 0)
            {
                usedspace += var_table[table_index - 1].dim[ndim];
            }
            else
            {
                usedspace *= var_table[table_index - 1].dim[ndim];
            }
        }
        ndim += 1;
    }
    var_table[table_index - 1].ndim = ndim;
    if(ndim != 0 && var_table[table_index - 1].dynamic_len_index == -1) //动态数组usedspace应=1
    {
        attr->usedspace += usedspace - 1;
        var_table[table_index - 1].address = -attr->usedspace - 1;
    }
    produce(stack, 45);
    return NULL;
}

Attr *reference_suffix(PL0Lex *lex, Attr *attr)
{
    int ndim = 0;
    code_append_arg(attr->code, PUT, 0, FALSE);     //put 0
    while(lex->last_token_type == TOKEN_LSQUARE)
    {
        code_append_arg(attr->code, PUT, var_table[attr->var_table_index].dim[ndim], 0);    //put维数
        code_append(attr->code, MUL);

        produce(stack, 46);
        stack_pop(stack);
        PL0Lex_get_token(lex);

        expression(lex, attr);

        if(lex->last_token_type == TOKEN_RSQUARE)
        {
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "missing ].");
        }
        stack_pop(stack);

        code_append(attr->code, ADD);

        ndim += 1;
    }
    //address + offset
    //code_append(attr, ADD);
    produce(stack, 47);
    return NULL;
}


Attr *actual_parameters_suffix(PL0Lex *lex, Attr *attr){
    int npara = 0;
    if(lex->last_token_type == TOKEN_LPAREN) {
        produce(stack, 81);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        if(lex->last_token_type != TOKEN_RPAREN)
        {
            produce(stack, 83);

            expression(lex, attr);
            npara += 1;
            while(lex->last_token_type == TOKEN_COMMA){
                produce(stack, 85);
                stack_pop(stack);
                PL0Lex_get_token(lex);

                expression(lex, attr);
                npara += 1;
            }
            produce(stack, 86);
        }
        else
        {
            produce(stack, 84);
        }
        if(lex->last_token_type == TOKEN_RPAREN)
        {
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be a ')' to follow.");
            stack_pop(stack);
        }
    }
    else
    {
        produce(stack, 82);
    }
    // backward put into mem
    int i;
    for(i = 0; i < npara; i++){
        code_extend(attr->code, gen_push());
    }
    attr->usedspace = npara;
    return NULL;
}

//应该生成将函数返回值放在运行栈顶的代码
Attr *procedure_call(PL0Lex *lex, Attr *attr)
{
    char name[MAX_ID_LEN+1];
    produce(stack, 88);
    if(lex->last_token_type == TOKEN_CALL)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
        if(lex->last_token_type == TOKEN_IDENTIFIER)
        {
            memcpy(name, lex->last_id, sizeof(name));
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be an identifier to follow.");
            stack_pop(stack);
        }

        Attr *new_attr = copy_Attr(attr);
        int return_tag = new_tag();
        int pindex = lookup_procedure(name);
        Procedure_Table pinfo = procedure_table[pindex];

        actual_parameters_suffix(lex, new_attr); //实参表在内部被push至运行栈
        code_extend(attr->code, new_attr->code);
        code_extend(attr->code, gen_change_sp(-1)); //sp-1, 返回地址
        code_append_tag(attr->code, PUT, return_tag);
        code_append_arg(attr->code, LD, SP, FALSE);
        code_append(attr->code, ST);
        code_extend(attr->code, gen_build_access_chain(level, pinfo.level));

        code_append_tag(attr->code, JMP, pinfo.address);

        code_append(attr->code, INULL);
        set_tag(return_tag, attr->code->ed);
        code_extend(attr->code, gen_destroy_access_chain());
        code_extend(attr->code, gen_change_sp(1 + new_attr->usedspace));
        //pop return address and args
        //注意参数表要传回usedspace，代表参数个数
    }
    else
    {
        PL0_print_error(lex, "procedure call must start with a 'call'.");
    }
    return NULL;
}

Attr *const_value(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_LCURLY)
    {
        produce(stack, 49);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        vecs_or_scalars(lex, NULL);
        if(lex->last_token_type == TOKEN_RCURLY)
        {
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "missing } in the end of initial list.");
        }
        stack_pop(stack);
    }
    else if(lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_NUMBER)
    {
        produce(stack, 48);

        Attr * value = copy_Attr(attr);
        integer(lex, value);
        var_table[table_index - 1].value = value->value;
    }
    else
    {
        //错误处理
        PL0_print_error(lex, "missing value after '=' in const declaration.");
        stack_pop(stack);   //不继续推导，简单弹出记号
    }
    return NULL;
}

Attr *vecs_or_scalars(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_NUMBER)
    {
        produce(stack, 53);
        integers_table(lex, NULL);
    }
    else if(lex->last_token_type == TOKEN_LCURLY)
    {
        produce(stack, 52);
        vectors_table(lex, NULL);
    }
    else
    {
        /*while(lex->last_token_type != TOKEN_RCURLY){
            if(!PL0Lex_get_token(lex)){
                break;
            }
        }*/
        stack_pop(stack);
        PL0_print_error(lex, "unexpected token in initial list.");
    }
    return NULL;
}

Attr *vectors_table(PL0Lex *lex, Attr *attr)
{
    do
    {
        produce(stack, 54);
        vector(lex, NULL);
        if(lex->last_token_type == TOKEN_COMMA)
        {
            produce(stack, 56);
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            produce(stack, 57);
            break;
        }
    }
    while(1);
    return NULL;
}

Attr *vector(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_LCURLY)
    {
        produce(stack, 55);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        integers_table(lex, NULL);
        if(lex->last_token_type == TOKEN_RCURLY)
        {
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "missing '}' in the end of initial list.");
        }
        stack_pop(stack);
    }
    else
    {
        stack_pop(stack);
        PL0_print_error(lex, "initial list must start with '{'.");
    }
    return NULL;
}

Attr *integers_table(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_NUMBER)
    {
        produce(stack, 58);
        integer(lex, NULL);
    }
    else
    {
        PL0_print_error(lex, "unexpected token in initial list.");
        //exit(0);
    }

    while(lex->last_token_type == TOKEN_COMMA)
    {
        produce(stack, 59);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        if(lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_NUMBER)
        {
            produce(stack, 58);
            integer(lex, NULL);
        }
        else
        {
            PL0_print_error(lex, "unexpected token in initial list.");
            //exit(0);
        }
    }
    produce(stack, 60);
    return NULL;
}

Attr *ex_branch(PL0Lex *lex, Attr *attr)
{
    Attr *le,*st;
    int tag = new_tag();    //label the end place

    while(lex->last_token_type == TOKEN_ELIF)
    {
        // else&elseif -> elif condition than statement ; else&elseif
        produce(stack, 61);

        //pop(elif)
        stack_pop(stack);
        PL0Lex_get_token(lex);

        st = copy_Attr(attr);
        le = copy_Attr(attr);

        logical_expr(lex, le);
        if(lex->last_token_type == TOKEN_THEN)
        {
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "missing 'then' after condition.");
        }
        stack_pop(stack);

        statement(lex, st);

        int next_tag = new_tag();

        code_extend(attr->code, le->code);
        code_append(attr->code, NOT);
        code_append_tag(attr->code, JPC, next_tag);
        code_extend(attr->code, st->code);
        code_append_tag(attr->code, JPC, tag);
        code_append(attr->code, INULL);
        set_tag(next_tag, attr->code->ed);
        //??code_append_arg(attr.code, JMP, );
    }

    if(lex->last_token_type == TOKEN_ELSE)
    {
        // else&elseif -> else statement
        produce(stack, 62);
        stack_pop(stack);

        PL0Lex_get_token(lex);

        st = copy_Attr(attr);
        statement(lex, st);
        code_extend(attr->code, st->code);
    }
    else
    {
        produce(stack, 63);
    }
    code_append(attr->code, INULL);
    set_tag(tag, attr->code->ed);
    return NULL;
}

// return attr * trans
// maybe pass global attr through func
// have result attr return
Attr *statement(PL0Lex *lex, Attr *attr)
{

    Attr *trans = attr;
    //trans->code = code_init();

    //PL0_print_error(lex, "analysis the statement");
    //if(stack_top() == "S")
    //{
    //pop();
    //trans->code = code_init();
    if(lex->last_token_type == TOKEN_IDENTIFIER)
    {
        produce(stack,36);
        //printfstack()
        //stack_pop(stack);

        Attr *var_const_loc = copy_Attr(attr);
        var_const_name(lex, var_const_loc);
        /*
        if lex is right ,get the next lex and break;
        if lex is = , get the next lex and break;
        if := is lost ,break
        if lex is other lex , continue until find a lex in aboving conditions
        */
        if(lex->last_token_type == TOKEN_BECOMES)
        {
            stack_pop(stack);
            //printstack();
            PL0Lex_get_token(lex);
            //expression(lex, NULL);
        }
        else if(lex->last_token_type == TOKEN_EQU)
        {
            PL0_print_error(lex, " found '=' when expecting ':='.");
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else if(
            lex->last_token_type == TOKEN_IDENTIFIER ||
            lex->last_token_type == TOKEN_LPAREN ||
            lex->last_token_type == TOKEN_MINUS
        )
        {
            //means lost :=
            PL0_print_error(lex, " there must be a ':=' to follow.");
            stack_pop(stack);
            //expression(lex, NULL);
        }
        else
        {
            PL0_print_error(lex, " there must be a ':=' to follow ,but the lex is illegal.");
            stack_pop(stack);
        }

        Attr * exp = copy_Attr(attr);
        expression(lex, exp);

        code_extend(attr->code, exp->code);
        code_extend(attr->code, var_const_loc->code);
        code_append(attr->code, ST);

    }
    else if(lex->last_token_type == TOKEN_CALL)
    {
        produce(stack,37);
        procedure_call(lex, attr);
    }
    else if(lex->last_token_type == TOKEN_BEGIN)
    {
        produce(stack,38);
        //printfstack
        stack_pop(stack);
        //printfstack
        PL0Lex_get_token(lex);

        statements_sequence(lex, attr);

        if(lex->last_token_type == TOKEN_END)
        {
            stack_pop(stack);
            //printfstack()
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be an 'end' to follow.");
            stack_pop(stack);
        }
        return NULL;
    }
    else if(lex->last_token_type == TOKEN_IF)
    {
        produce(stack,39);

        stack_pop(stack);
        PL0Lex_get_token(lex);

        Attr *c1 = copy_Attr(attr);
        logical_expr(lex, c1);

        if(lex->last_token_type == TOKEN_THEN)
        {
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be an 'then' to follow.");
            stack_pop(stack);
        }

        Attr *c2 = copy_Attr(attr);
        statement(lex, c2);
        Attr *c3 = copy_Attr(attr);
        ex_branch(lex, c3);

        int false_tag = new_tag();
        int end_tag = new_tag();
        set_tag(false_tag, c3->code->st);

        //begin to extend
        code_extend(trans->code, c1->code);
        code_append(trans->code, NOT);
        code_append_tag(trans->code, JPC, false_tag);
        code_extend(trans->code, c2->code);
        code_append_tag(trans->code, JMP, end_tag);
        code_extend(trans->code, c3->code);
        code_append(trans->code, INULL);
        set_tag(end_tag, trans->code->ed);

        return NULL;
    }
    else if(lex->last_token_type == TOKEN_WHILE)
    {
        produce(stack,40);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        Attr *logicexpattr = copy_Attr(attr);
        logical_expr(lex, logicexpattr);

        if(lex->last_token_type == TOKEN_DO)
        {
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be an 'do' to follow.");
        }
        stack_pop(stack);
        Attr *statmattr = copy_Attr(attr);
        statement(lex, statmattr);

        // concat the code
        /*
        code_extend(attr->code, logicexpattr->code);    //logical_expr in top
        code_append(attr->code, NOT);                   //
        code_append_arg(attr->code, JPC, statmattr->code->len + 2, TRUE);
        code_extend(attr->code, statmattr->code);
        code_append_arg(attr->code, JMP, -logicexpattr->code->len - statmattr->code->len - 2, 1);
        */
        int start_tag = new_tag();
        int end_tag = new_tag();
        set_tag(start_tag, logicexpattr->code->st);
        code_extend(attr->code, logicexpattr->code);
        code_append(attr->code, NOT);
        code_append_tag(attr->code, JPC, end_tag);
        code_extend(attr->code, statmattr->code);
        code_append_tag(attr->code, JMP, start_tag);
        code_append(attr->code, INULL);
        set_tag(end_tag, attr->code->ed);

    }
    else if(lex->last_token_type == TOKEN_DO)
    {
        produce(stack,41);   //语句 --> do 语句 while 条件
        stack_pop(stack);
        PL0Lex_get_token(lex);
        Attr *statmattr = copy_Attr(attr);
        statement(lex, statmattr);
        Attr *logicexpattr = copy_Attr(attr);

        if(lex->last_token_type == TOKEN_WHILE)
        {
            stack_pop(stack);
            PL0Lex_get_token(lex);
            logical_expr(lex, logicexpattr);
        }
        else
        {
            PL0_print_error(lex, "there must be an ‘while’ to follow.");
            stack_pop(stack);
        }

        int start_tag = new_tag();
        set_tag(start_tag, statmattr->code->st);
        // concat
        code_extend(attr->code, statmattr->code);
        code_extend(attr->code, logicexpattr->code);
        code_append_tag(attr->code, JPC, start_tag);
    }
    else if(lex->last_token_type == TOKEN_RETURN)
    {
        produce(stack,42);	//语句 --> return 表达式
        stack_pop(stack);
        PL0Lex_get_token(lex);
        expression(lex, attr);
        code_extend(attr->code, gen_build_return(attr->procedure_index));
    }
    else if(lex->last_token_type == TOKEN_PRINT)
    {
        produce(stack, 75);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        expression(lex, attr);

        // translate
        //code_extend(attr->code, exp->code);
        code_append_arg(attr->code, INT, 0, FALSE);
        // pop value in calculate stack top
        // use mem[10] as trash bin
        code_append_arg(attr->code, ST, TMP, FALSE);

    }
    else
    {
        PL0_print_error(lex, "there must be a member of first(statement).");
    }
    //}

    return NULL;
}

Attr *statements_sequence(PL0Lex *lex, Attr *attr)
{
    //printf("analysis the statements_sequence");
    //Attr *trans;
    //trans->code = code_init();
    Attr *st;
    while(
        lex->last_token_type == TOKEN_IDENTIFIER ||
        lex->last_token_type == TOKEN_CALL ||
        lex->last_token_type == TOKEN_BEGIN ||
        lex->last_token_type == TOKEN_IF ||
        lex->last_token_type == TOKEN_WHILE ||
        lex->last_token_type == TOKEN_DO ||
        lex->last_token_type == TOKEN_RETURN ||
        lex->last_token_type == TOKEN_PRINT
    )
    {
        produce (stack,35);
        st = copy_Attr(attr);
        statement(lex, st);

        if(lex->last_token_type == TOKEN_SEMICOLON)
        {
            //printfstack
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be a ; to follow statement.");
        }
        stack_pop(stack);
        code_extend(attr->code, st->code);
        free(st);
    }

    if(lex->last_token_type == TOKEN_END)
    {
        produce(stack, 78);
    }
    else
    {
        PL0_print_error(lex, "missing 'end' in the end of statement_block.");
    }
    return NULL;
}

Attr *ex_and_expr(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_AND)
    {
        // moreand_expr -> && condition moreand_expr
        produce(stack, 71);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        condition(lex, attr);
        code_append(attr->code, AND);
        ex_and_expr(lex, attr);
    }
    else
    {
        // moreand_expr -> epsilon
        produce(stack, 72);
    }
    return NULL;
}

Attr *and_expr(PL0Lex *lex, Attr *attr)
{
    // and_expr -> condition moreand_expr
    produce(stack, 70);
    condition(lex, attr);
    ex_and_expr(lex, attr);
    return NULL;
}

Attr *ex_logical_expr(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_OR)
    {
        // ex logicalexp -> || and_expr exlogicexp
        produce(stack, 68);
        stack_pop(stack);
        PL0Lex_get_token(lex);
        and_expr(lex, attr);
        code_append(attr->code, OR);
        ex_logical_expr(lex, attr);
    }
    else
    {
        // exlogicexp -> epsilon
        produce(stack, 69);
    }
    return NULL;
}

Attr *logical_expr(PL0Lex *lex, Attr *attr)
{
    if(expr_plus == TRUE)
    {
        lr_expression(stack, lex, attr);
        stack_pop(stack);
        return NULL;
    }
    // logical -> andexp morelogical
    //code_append_arg(attr->code, PUT, 0);
    produce(stack, 67);
    and_expr(lex, attr);
    ex_logical_expr(lex, attr);
    //code_append(attr->code, OR);
    return NULL;
}

Attr *condition(PL0Lex *lex, Attr *attr)
{
    //printf("analysis the condition expression");
    if(lex->last_token_type == TOKEN_ODD)
    {
        // stack_op( condition -> odd express );
        produce(stack, 15);
        // stack_pop( odd );
        stack_pop(stack);

        PL0Lex_get_token(lex);
        expression(lex, NULL);
    }
    else if(lex->last_token_type == TOKEN_NOT)
    {
        // condition -> ! condition
        produce(stack, 73);

        // pop(!)
        stack_pop(stack);

        PL0Lex_get_token(lex);
        condition(lex, attr);
        code_append(attr->code, NOT);
    }
    else
    {
        // stack_op( condition -> express relation&express );
        produce(stack, 16);
        expression(lex, attr);

        int oper = 0;
        // PL0Lex_get_token(lex);
        switch(lex->last_token_type)
        {
        case TOKEN_EQU:
            // stack_op( relation&express -> = expression);
            produce(stack, 17);
            // stack_pop(=);
            stack_pop(stack);
            oper = 1;
            break;
        case TOKEN_NEQ:
            // stack_op(relation&express -> != expression);
            produce(stack, 18);
            // stack_pop(!=);
            stack_pop(stack);
            oper = 2;
            break;
        case TOKEN_LES:
            // stack_op(relation&express -> < expression);
            produce(stack, 19);
            // stack_pop(<);
            stack_pop(stack);
            oper = 3;
            break;
        case TOKEN_GTR:
            // stack_op(relation&express -> > expression);
            produce(stack, 20);
            // stack_pop(>);
            stack_pop(stack);
            oper = 4;
            break;
        case TOKEN_LEQ:
            // stack_op(relation&express -> <= expression);
            produce(stack, 21);
            // stack_pop(<=);
            stack_pop(stack);
            oper = 5;
            break;
        case TOKEN_GEQ:
            // stack_op(relation&express -> >= expression);
            produce(stack, 22);
            // stack_pop(>=);
            stack_pop(stack);
            oper = 6;
            break;
        default:
            PL0_print_error(lex, "condition wrong! need a relation symbol between two expressions.");
            /* ignore */
            // stack_pop( relation&express );
            stack_pop(stack);
            oper = 0;
            break;
        }

        PL0Lex_get_token(lex);
        expression(lex, attr);
        code_extend(attr->code, gen_swap_stack_top());
        if(1 == oper)
        {
            code_append(attr->code, EQ);
        }
        else if(2 == oper)
        {
            code_append(attr->code, EQ);
            code_append(attr->code, NOT);
        }
        else if(3 == oper)
        {
            code_append(attr->code, LT);
        }
        else if(4 == oper)
        {
            code_append(attr->code, GT);
        }
        else if(5 == oper)
        {
            code_append(attr->code, LE);
        }
        else if(6 == oper)
        {
            code_append(attr->code, GE);
        }
        else
        {
            //do nothing
        }
    }
    return NULL;
}

//需要传入的属性为：调用函数名
Attr *expression(PL0Lex *lex, Attr *attr)
{
    if(expr_plus == TRUE)
    {
        lr_expression(stack, lex, attr);
        stack_pop(stack);
        return NULL;
    }
    //printf("analysis the expression");
    // stack_op(expression -> term plusorminus&term);
    produce(stack, 23);
    term(lex, attr);
    //code_extend(attr->code, tm->code);
    //code_append_arg(attr->code, ST, TMP, FALSE);	//put the number into temp
    int oper = 0;
    while(lex->last_token_type == TOKEN_PLUS || lex->last_token_type == TOKEN_MINUS)
    {
        if(lex->last_token_type == TOKEN_PLUS)
        {
            // stack_op( plusorminus&term -> + term plusorminus&term);
            produce(stack, 25);
            // stack_pop(+);
            stack_pop(stack);
            oper = 0;
        }
        else if(lex->last_token_type == TOKEN_MINUS)
        {
            // stack_op( plusorminus&term -> - term plusorminus&term);
            produce(stack, 26);
            // stack_pop(-);
            stack_pop(stack);
            oper = 1;
        }

        PL0Lex_get_token(lex);
        term(lex, attr);
        code_extend(attr->code, gen_swap_stack_top());
        //code_append_arg(attr->code, LD, TMP, FALSE);	//load temp into stack stack_top
        if(oper == 0)
        {
            code_append(attr->code, ADD);
        }
        else
        {
            code_append(attr->code, SUB);
        }
    }

    // plusorminus&term -> epsilon
    produce(stack, 27);
    return NULL;
}

Attr *term(PL0Lex *lex, Attr *attr)
{
    //printf("analysis the term");
    // stack_op( term -> factor mulordiv&factor )
    produce(stack, 24);
    factor(lex, attr);
    //code_extend(trans->code, ft->code);
    //code_append_arg(attr->code, ST, TMP, FALSE);	//put the number into temp

    /* it is stupid, so not completed.  2018.11.29 done*/
    /* special judge for missing left parenthesize or unexpecting right parenthesize  */
    /*if(lex->last_token_type == TOKEN_RPAREN){
        PL0_print_error(lex, "unexpecting token.");

        PL0Lex_get_token(lex);
    }*/

    int oper = 0;   //用于判断*或/
    while(lex->last_token_type == TOKEN_TIMES || lex->last_token_type == TOKEN_SLASH)
    {
        if(lex->last_token_type == TOKEN_TIMES)
        {
            // stack_op( mulordiv&factor -> * factor mulordiv&factor);
            produce(stack, 32);
            // stack_pop(*);
            stack_pop(stack);
            oper = 0;
        }
        else if(lex->last_token_type == TOKEN_SLASH)
        {
            // stack_op( mulordiv&factor -> / factor mulordiv&factor);
            produce(stack, 33);
            // stack_pop(/);
            stack_pop(stack);
            oper = 1;
        }

        PL0Lex_get_token(lex);
        factor(lex, attr);
        //code_extend(trans->code, ft->code);
        //code_append_arg(attr->code, LD, TMP, FALSE);
        code_extend(attr->code, gen_swap_stack_top());
        if(oper == 0)
        {
            code_append(attr->code, MUL);
        }
        else
        {
            code_append(attr->code, DIV);
        }
    }
    // mulordiv&factor -> epsilon
    produce(stack, 34);
    return NULL;
}

Attr *factor(PL0Lex *lex, Attr *attr)
{
    //传入函数名
    /*
    Attr *trans;
    trans->code = code_init();
    */
    //printf("analysis the factor");
    if(lex->last_token_type == TOKEN_IDENTIFIER)
    {
        // stack_op(factor -> ident);
        produce(stack, 28);
        // stack_pop(ident);
        //stack_pop(stack);
        //int index = lookup_id(lex->last_id, procedure_table[attr->procedure_index].name); //找到在表中的位置
        //Code ad = gen_find_var_address(index, level);
        //code_extend(attr->code, ad);
        //attr->var_table_index = index;
        var_const_name(lex, attr);
        //此时栈顶为地址
        //code_extend(attr->code, ad);
        //code_extend(attr->code, attr1->code);
        code_append(attr->code, LD);
    }
    else if(lex->last_token_type == TOKEN_NUMBER)
    {
        // stack_op(factor -> number);
        produce(stack, 29);
        // stack_pop(number);
        stack_pop(stack);

        code_append_arg(attr->code, PUT, lex->last_num, FALSE);

        PL0Lex_get_token(lex);
    }
    else if(lex->last_token_type == TOKEN_MINUS)
    {
        // stack_op(factor -> - expression);
        produce(stack, 30);
        // stack_pop(-);
        stack_pop(stack);

        PL0Lex_get_token(lex);
        Attr *fa = factor(lex, attr);
        code_extend(attr->code, fa->code);
        code_append_arg(attr->code, PUT, 0, FALSE);
        code_append(attr->code, SUB);
    }
    else if(lex->last_token_type == TOKEN_LPAREN)
    {
        // stack_op(factor -> (expression));
        produce(stack, 31);
        // stack_pop( ( );
        stack_pop(stack);

        PL0Lex_get_token(lex);

        expression(lex, attr);
        if(lex->last_token_type == TOKEN_RPAREN)
        {
            // stack_pop( ) );
            stack_pop(stack);
        }
        else
        {
            PL0_print_error(lex, "expecting a right parenthesis.");
            // stack_pop( ) );
            stack_pop(stack);
        }
        //code_extend(attr->code, ex->code);
        PL0Lex_get_token(lex);
    }
    else if(lex->last_token_type == TOKEN_CALL)
    {
        produce(stack, 87);
        procedure_call(lex, attr);
    }
    else
    {
        PL0_print_error(lex, "expecting a factor.");
        // stack_pop( factor );
        stack_pop(stack);

        PL0Lex_get_token(lex);
    }
    return NULL;
}

Attr *procedure_declaration(PL0Lex *lex, Attr *attr)
{
    level += 1;

    produce(stack, 14);
    if(lex->last_token_type == TOKEN_PROCEDURE)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "procedure must begin with 'procedure'.");
    }
    if(lex->last_token_type == TOKEN_IDENTIFIER)
    {
        procedure_table_append(lex->last_id, new_tag(), 0, attr->procedure_index);
        attr->procedure_index = procedure_table_index - 1;
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "there must be a id after procedure.");
        stack_pop(stack);
    }

    //(can shu biao)
    if(lex->last_token_type == TOKEN_LPAREN)
    {
        produce(stack, 79);
        stack_pop(stack);
        PL0Lex_get_token(lex);

        Attr * info = copy_Attr(attr);
        args_table(lex, info);
        if(lex->last_token_type == TOKEN_RPAREN) {
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be a ')' after args_table.");
            stack_pop(stack);
        }
    }
    else
    {
        produce(stack, 80);
    }

    if(lex->last_token_type == TOKEN_SEMICOLON)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "missing ';' after id or ')'.");
        stack_pop(stack);
    }
    program_block(lex, attr);
    if(lex->last_token_type == TOKEN_SEMICOLON)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "missing ';' after program_block.");
        stack_pop(stack);
    }

    level -= 1;
    return NULL;
}

Attr *vars_table(PL0Lex *lex, Attr *attr)
{
    do
    {
        produce(stack, 11);
        if(lex->last_token_type == TOKEN_IDENTIFIER)
        {
            attr->usedspace += 1;
            var_table_append(lex->last_id, 0, 0, -attr->usedspace - 1, 0, NULL, attr->procedure_index);
            name_in_declaration(lex, attr);
        }
        else
        {
            PL0_print_error(lex, "expecting a id.");
            stack_pop(stack);
        }

        if(lex->last_token_type == TOKEN_COMMA)
        {
            // more_varlist -> , varlist
            produce(stack, 12);
            // pop(,)
            stack_pop(stack);

            PL0Lex_get_token(lex);
        }
        else if(lex->last_token_type == TOKEN_IDENTIFIER)
        {
            PL0_print_error(lex, "expecting ','.");

            // more_varlist -> , varlist
            produce(stack, 12);
            // pop(,)
            stack_pop(stack);
        }
        else if(lex->last_token_type == TOKEN_SEMICOLON || lex->last_token_type == TOKEN_RPAREN)
        {
            // more var list -> epsilon
            produce(stack, 13);
            break;
        }
        else
        {
            PL0_print_error(lex, "expecting ',' or ';'.");
            produce(stack, 13);
            break;
        }
    }
    while(1);
    return NULL;
}

Attr *var_declaration(PL0Lex *lex, Attr *attr)
{
    if(lex->last_token_type == TOKEN_VAR)
    {
        // var_declare -> var varlist ;
        produce(stack, 10);
        // pop( var )
        stack_pop(stack);
    }
    else
    {
        PL0_print_error(lex, "expecting a 'var'.");
    }


    PL0Lex_get_token(lex);
    vars_table(lex, attr);
    if(lex->last_token_type == TOKEN_SEMICOLON)
    {
        // pop( ; )
        stack_pop(stack);

        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "expecting a ';'.");

        // pop(;)
        stack_pop(stack);
    }
    return NULL;
}

Attr *consts_table(PL0Lex *lex, Attr *attr)
{
    do
    {
        produce(stack, 7);
        if (lex->last_token_type == TOKEN_IDENTIFIER)
        {
            attr->usedspace += 1;
            var_table_append(lex->last_id, 1, 0, -attr->usedspace - 1, 0, NULL, attr->procedure_index);
            name_in_declaration(lex, attr);
        }
        else
        {
            PL0_print_error(lex, "there must be an identifier to follow 'const' or ','.");
            stack_pop(stack);
        }
        if (lex->last_token_type == TOKEN_EQU)
        {
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be an '=' to follow the identifier.");
            stack_pop(stack);
        }
        if (
            lex->last_token_type == TOKEN_NUMBER ||
            lex->last_token_type == TOKEN_MINUS  ||
            lex->last_token_type == TOKEN_LCURLY
        )
        {
            //table_append(lex, ID_CONSTANT);
            const_value(lex, attr);
            //PL0Lex_get_token(lex);
        }
        else
        {
            PL0_print_error(lex, "there must be a integer or initial list to follow '='.");
            stack_pop(stack);
        }

        if(lex->last_token_type == TOKEN_COMMA)
        {
            produce(stack, 8);
            stack_pop(stack);
            PL0Lex_get_token(lex);
        }
        else if (lex->last_token_type == TOKEN_SEMICOLON)
        {
            produce(stack, 9);
            break;
        }
        else
        {
            PL0_print_error(lex, "missing ',' or ';'.");
            produce(stack, 9);
            break;
        }
    }
    while(1);
    return NULL;
}

Attr *const_declaration(PL0Lex *lex, Attr *attr)
{
    produce(stack, 6);
    if(lex->last_token_type == TOKEN_CONST)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "const declaration must begin with 'const'.");
    }
    consts_table(lex, attr);
    if(lex->last_token_type == TOKEN_SEMICOLON)
    {
        stack_pop(stack);
        PL0Lex_get_token(lex);
    }
    else
    {
        PL0_print_error(lex, "missing ';' after const_declaration.");
        stack_pop(stack);
    }
    return NULL;
} //const_declaration

Attr *declaration(PL0Lex *lex, Attr *attr)
{
    //Attr *dec = copy_Attr(attr);
    if (lex->last_token_type == TOKEN_CONST)
    {
        produce(stack, 3);
        const_declaration(lex, attr);
    }
    else if(lex->last_token_type == TOKEN_VAR)
    {
        produce(stack, 4);
        var_declaration(lex, attr);
    }
    else if(lex->last_token_type == TOKEN_PROCEDURE)
    {
        produce(stack, 5);
        Attr *procedure_attr = copy_Attr(attr);  //每个子过程要建立一个新的属性用于传递，防止被覆盖
        procedure_declaration(lex, procedure_attr);
        code_extend(attr->code, procedure_attr->code);

    }
    else
    {
        //impossible
        PL0_print_error(lex, "declaration begin with wrong token.");
    }
    return NULL;
}

Attr *program_block(PL0Lex *lex, Attr *attr)
{
    //Procedure_Table pinfo = procedure_table[attr->procedure_index];

    while(
        lex->last_token_type == TOKEN_CONST ||
        lex->last_token_type == TOKEN_VAR ||
        lex->last_token_type == TOKEN_PROCEDURE
    )
    {
        produce(stack, 1);
        declaration(lex, attr);
    }
    if(
        lex->last_token_type == TOKEN_IDENTIFIER ||
        lex->last_token_type == TOKEN_CALL ||
        lex->last_token_type == TOKEN_BEGIN ||
        lex->last_token_type == TOKEN_IF ||
        lex->last_token_type == TOKEN_WHILE
    )
    {
        int pindex = attr->procedure_index;
        procedure_table[pindex].usedspace = attr->usedspace;
        Procedure_Table pinfo = procedure_table[pindex];

        code_append(attr->code, INULL);
        set_tag(pinfo.address, attr->code->ed);
        code_extend(attr->code, gen_build_control_chain()); //建立控制链
        code_extend(attr->code, gen_change_sp(-pinfo.usedspace));   //分配局部空间

        for(int i=0; i<table_index; i++) //给常量赋值,动态数组分配空间
        {
            Var_Table vinfo = var_table[i];
            if(vinfo.parent_index == pindex && vinfo.isconst)
            {
                code_append_arg(attr->code, PUT, vinfo.value, FALSE);
                code_extend(attr->code, gen_find_var_address(i, level));
                code_append(attr->code, ST);
            }
            if(vinfo.parent_index == pindex && vinfo.dynamic_len_index != -1){ //动态数组
                code_extend(attr->code, gen_find_var_value(vinfo.dynamic_len_index, level));    //得到动态数组长度
                code_append_arg(attr->code, ST, TMP, FALSE);    //保存一下长度
                code_append_arg(attr->code, LD, TMP, FALSE);
                code_append_arg(attr->code, PUT, 1, FALSE);
                code_append(attr->code, ADD);   //长度+1为真实应分配空间
                code_append_arg(attr->code, PUT, 0, FALSE);
                code_append(attr->code, SUB);   //取相反数
                code_extend(attr->code, gen_change_sp_dynamic());   //分配空间
                code_append_arg(attr->code, LD, TMP, FALSE);
                code_append_arg(attr->code, LD, SP, FALSE);
                code_append(attr->code, ST);    //当前栈顶为分配的空间的开始，记录长度
                code_append_arg(attr->code, LD, SP, FALSE);
                code_extend(attr->code, gen_find_var_pointer(i, level));    //给动态数组分配的空间用于存放真实地址
                code_append(attr->code, ST);    //将当前栈顶存放在动态数组分配的单个空间
            }
        }
        Attr *statement_attr = copy_Attr(attr);
        produce(stack, 2);
        statement(lex, statement_attr);
        code_extend(attr->code, statement_attr->code);
        code_extend(attr->code, gen_build_return(attr->procedure_index));
    }
    else   //ERROR
    {
        PL0_print_error(lex, "missing statement after declaration.");
    }
    return NULL;
} //program_block

Attr *program(PL0Lex *lex, Attr *attr)
{
    produce(stack, 0);
    attr->start_tag = new_tag();
    procedure_table_append("main", attr->start_tag, 0, -1);
    attr->procedure_index = procedure_table_index - 1;

    code_extend(attr->code, gen_init_machine());
    code_append_tag(attr->code, JMP, attr->start_tag);


    program_block(lex, attr);
    if(lex->last_token_type == TOKEN_PERIOD)
    {
        stack_pop(stack);
    }
    else
    {
        PL0_print_error(lex, "missing '.' after program body.");
        stack_pop(stack);
    }
    return NULL;
}

void PL0TEX_analysis(PL0Lex *lex, FILE *outfile_stack, FILE *outfile_tree)
{
    stack_init(stack, outfile_stack, outfile_tree);
    stack_push(stack, G_PROGRAM);
    Attr *attr = new_Attr();
    PL0Lex_get_token(lex);
    program(lex, attr);
    if(PL0Lex_get_token(lex))
    {
        PL0_print_error(lex, "redundant code behind program.");
    }

    if(fout != NULL)
    {
        code_prt(attr->code, fout);
    }
}
