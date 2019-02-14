#include <stdlib.h>
#include <string.h>
#include "pl0_translation.h"

const char *inc_str[N_INSTRUCTIONS] = {
    "",
    "PUT",
    "LD",
    "ST",
    "JMP",
    "JPC",
    "INT",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "EQ",
    "LE",
    "LT",
    "GE",
    "GT",
    "AND",
    "OR",
    "NOT",
};

int table_index = 0;
int procedure_table_index = 0;
int tag_num = 0;

/*operations for token table*/
void var_table_append(const char * name, BOOL isconst, int value, int address, int ndim, const int * dim, int parent_index) {
    strcpy(var_table[table_index].name, name);
    var_table[table_index].parent_index = parent_index;
    var_table[table_index].level = level;
    var_table[table_index].address = address;

    var_table[table_index].isconst = isconst;
    var_table[table_index].value = value;
    var_table[table_index].dynamic_len_index = -1;

    var_table[table_index].ndim = ndim;
    if(ndim != 0 && dim != NULL){
        memcpy(var_table[table_index].dim, dim, ndim*sizeof(int));
    }

    table_index += 1;

} //table_append

void procedure_table_append(const char * name, int address, int usedspace,int parent_index){
    strcpy(procedure_table[procedure_table_index].name, name);
    procedure_table[procedure_table_index].parent_index = parent_index;
    procedure_table[procedure_table_index].level = level;
    procedure_table[procedure_table_index].address = address;
    procedure_table[procedure_table_index].usedspace = usedspace;

    procedure_table_index += 1;
}

int new_tag(){
    return tag_num ++;
}

void set_tag(int tag_id, Instruction inc){
    tag_table[tag_id] = inc;
}

// return index in table
int lookup_id(const char *name, int parent_index){
    int index = 0;
    int thisparent = parent_index;
    Var_Table vinfo;

    while(TRUE){
        index = 0;
        while(index < table_index){
            vinfo = var_table[index];
            if(
                strcmp(name, vinfo.name) == 0 &&
                vinfo.parent_index == thisparent
            ){
                return index;
            }
            index += 1;
        }
        if(thisparent == 0){
            break;
        }
        thisparent = procedure_table[thisparent].parent_index;
    }
    return -1;      // not found
}

int lookup_procedure(const char *name){
    int index = 0;
    while(index < procedure_table_index){
        if(strcmp(procedure_table[index].name, name) == 0){
            return index;
        }
        index += 1;
    }
    return -1;      // not found
}


Instruction new_inc(Instruction_Type inc_type){
    Instruction inc = malloc(sizeof(List_Node));
    inc->next = NULL;
    inc->inc_type = inc_type;
    inc->has_arg = FALSE;
    inc->arg = 0;
    inc->is_offset = FALSE;
    inc->is_tag = FALSE;
    return inc;
}

Instruction new_inc_arg(Instruction_Type inc_type, int arg, BOOL is_offset, BOOL is_tag){
    Instruction inc = malloc(sizeof(List_Node));
    inc->next = NULL;
    inc->inc_type = inc_type;
    inc->has_arg = TRUE;
    inc->arg = arg;
    inc->is_offset = is_offset;
    inc->is_tag = is_tag;
    return inc;
}

void code_extend(Code c1, Code c2){ //c1 = c1 + c2
    if(c1->len != 0 && c2->len != 0){
        c1->ed->next = c2->st;
        c1->ed = c2->ed;
        c1->len = c1->len + c2->len;
    }
    else if(c1->len == 0){
        c1->st = c2->st;
        c1->ed = c2->ed;
        c1->len = c2->len;
    }
    free(c2);
    c2 = NULL;
}

void code_append(Code c1, Instruction_Type inc_type){   //c1 = c1 + inc
    Instruction inc = new_inc(inc_type);
    if(c1->st == NULL){
        c1->st = c1->ed = inc;
    }
    else{
        c1->ed->next = inc;
        c1->ed = inc;
    }
    c1->len ++;
}

void code_append_arg(Code c1, Instruction_Type inc_type, int arg, BOOL is_offset){  // c1 = c1 + inc
    Instruction inc = new_inc_arg(inc_type, arg, is_offset, FALSE);
    if(c1->st == NULL){
        c1->st = c1->ed = inc;
    }
    else{
        c1->ed->next = inc;
        c1->ed = inc;
    }
    c1->len ++;
}

void code_append_tag(Code c1, Instruction_Type inc_type, int tag_id){
    Instruction inc = new_inc_arg(inc_type, tag_id, FALSE, TRUE);
    if(c1->st == NULL){
        c1->st = c1->ed = inc;
    }
    else{
        c1->ed->next = inc;
        c1->ed = inc;
    }
    c1->len ++;
}

Code code_init(){   //init empty code list
    Code code = (Code)malloc(sizeof(Code_List));
    code->st = code->ed = NULL;
    code->len = 0;
    return code;
}

void code_prt(Code code, FILE *fout){
    if(fout == NULL){
        return;
    }

    int inc_num = 0;
    Instruction inc = code->st;
    while(inc != NULL){
        inc->id = inc_num;
        if(inc->inc_type != INULL){
            inc_num++;
        }
        inc = inc->next;
    }
    inc = code->st;
    while(inc != NULL){
        if(inc->inc_type == INULL){
            inc = inc->next;
            continue;
        }
        fprintf(fout, "%s", inc_str[inc->inc_type]);
        if(inc->has_arg){
            if(inc->is_offset){
                fprintf(fout, " %d", inc->id + inc->arg);
            }
            else if(inc->is_tag){
                fprintf(fout, " %d", tag_table[inc->arg]->id);
            }
            else{
                fprintf(fout, " %d", inc->arg);
            }
        }
        fprintf(fout, "\n");
        inc = inc->next;
    }
}

Attr * new_Attr(){
    Attr * attr = (Attr*)malloc(sizeof(Attr));
    attr->code = code_init();
    attr->procedure_index = 0;
    attr->usedspace = 0;
    attr->start_tag = 0;
    attr->value = 0;
    return attr;
}

Attr *copy_Attr(Attr *attr){
    Attr *new_attr = new_Attr();
    new_attr->procedure_index = attr->procedure_index;
    return new_attr;
}

Code gen_find_var_pointer(int table_index, int cnt_level){
    Var_Table var_info = var_table[table_index];
    Code code = code_init();
    int var_level = var_info.level;
    code_extend(code, gen_look_back(cnt_level - var_level));    //所需访问链在运算栈顶
    code_append_arg(code, PUT, var_info.address, FALSE);
    code_append(code, ADD); //将访问链位置和偏移量相加，此时栈顶为所需变量地址
    return code;
}

Code gen_find_var_address(int table_index, int cnt_level){
    Var_Table var_info = var_table[table_index];
    Code code = code_init();
    code_extend(code, gen_find_var_pointer(table_index, cnt_level));
    if(var_info.dynamic_len_index != -1){   //对于动态数组，此时放入的是存储真实位置信息的位置的地址
        code_append(code, LD);  //此时运算栈顶为真实地址开头的长度标识的地址
        code_append_arg(code, PUT, 1, FALSE);   //第一个位置用于存放长度
        code_append(code, ADD);     //此时是内容的开始地址
    }
    return code;
}

Code gen_look_back(int delta){
    Code code = code_init();
    code_append_arg(code, LD, ACS, FALSE);    //读取当前访问链
    if(delta < 0){
        printf("Fetal: Access Train Error, please check compiler and try again.\n");
        exit(-1);
    }
    for(int i=0; i<delta; i++){
        code_append(code, LD);
    }
    return code;
}

Code gen_find_var_value(int table_index, int cnt_level){
    Code code = code_init();
    code_extend(code, gen_find_var_address(table_index, cnt_level));
    code_append(code, LD);
    return code;
};

Code gen_change_sp(int delta){
    Code code = code_init();
    code_append_arg(code, LD, SP, FALSE);    //LD sp
    code_append_arg(code, PUT, delta, FALSE);   //PUT delta
    code_append(code, ADD);
    code_append_arg(code, ST, SP, FALSE);
    return code;
}

Code gen_push(){
    Code code = code_init();
    code_extend(code, gen_change_sp(-1));   //sp -= 1
    code_append_arg(code, LD, SP, FALSE);   //ld sp
    code_append(code, ST);  //mem[sp] = v
    return code;
}

Code gen_build_control_chain(){
    Code code = code_init();
    code_append_arg(code, LD, BP, FALSE);   //BP
    code_extend(code, gen_push());          //push BP
    code_append_arg(code, LD, SP, FALSE);
    code_append_arg(code, ST, BP, FALSE);   //BP = stack top
    return code;
}

Code gen_build_access_chain(int np, int nx){
    Code code = code_init();
    if(np < nx){
        code_append_arg(code, LD, ACS, FALSE);
        code_extend(code, gen_push());
    }
    else{
        code_extend(code, gen_look_back(np - nx + 1));
        code_extend(code, gen_push());
    }
    code_append_arg(code, LD, SP, FALSE);
    code_append_arg(code, ST, ACS, FALSE);   //ACS = stack top
    return code;
}

Code gen_destroy_access_chain(){
    Code code = code_init();
    code_append_arg(code, LD, BP, FALSE);   //读当前控制链
    code_append_arg(code, PUT, 1, FALSE);
    code_append(code, ADD);   //控制链+1为上一访问链位置
    code_append_arg(code, ST, ACS, FALSE);  //恢复该访问链
    code_extend(code, gen_change_sp(1));    //pop
    return code;
}

Code gen_init_machine(){
    Code code = code_init();
    code_append_arg(code, PUT, 1024, FALSE);
    code_append_arg(code, ST, SP, FALSE);
    code_append_arg(code, PUT, 1024, FALSE);
    code_append_arg(code, ST, ACS, FALSE);
    code_append_arg(code, PUT, 1023, FALSE);
    code_append_arg(code, ST, BP, FALSE);
    return code;
}

Code gen_swap_stack_top(){
    Code code = code_init();
    code_append_arg(code, ST, TMP, FALSE);
    code_append_arg(code, ST, TMP2, FALSE);
    code_append_arg(code, LD, TMP, FALSE);
    code_append_arg(code, LD, TMP2, FALSE);
    return code;
}

Code gen_build_return(int procedure_index){ //需要加入动态数组的释放
    Code code = code_init();
    Procedure_Table pinfo = procedure_table[procedure_index];

    if(procedure_index == 0){ //main
        code_append_arg(code, INT, 1, FALSE);
    }
    else{
        code_append_arg(code, PUT, 0, FALSE);   //累计长度
        for(int i=0; i<table_index; i++)
        {
            Var_Table vinfo = var_table[i];
            if(vinfo.parent_index == procedure_index && vinfo.dynamic_len_index != -1){ //释放动态数组
                code_extend(code, gen_find_var_pointer(i, level));
                code_append(code, LD);
                code_append(code, LD);  //数组长度此时在栈顶
                code_append(code, ADD); //累计长度
                code_append_arg(code, PUT, 1, FALSE);
                code_append(code, ADD); //累计长度, +1 是用于存放长度的空间
            }
        }
        code_extend(code, gen_change_sp_dynamic()); //释放相当于累计长度的空间

        int usedspace = pinfo.usedspace;
        code_extend(code, gen_change_sp(usedspace));
        code_append_arg(code, LD, SP, FALSE);
        code_append(code, LD);
        code_append_arg(code, ST, BP, FALSE);
        code_extend(code, gen_change_sp(1));
        code_append_arg(code, LD, ACS, FALSE);
        code_append_arg(code, PUT, 1, FALSE);
        code_append(code, ADD);
        code_append(code, LD);
        code_append(code, JMP);
    }
    return code;
}

Code gen_change_sp_dynamic(){
    Code code = code_init();
    code_append_arg(code, LD, SP, FALSE);    //LD sp
    code_append(code, ADD);
    code_append_arg(code, ST, SP, FALSE);
    return code;
}
