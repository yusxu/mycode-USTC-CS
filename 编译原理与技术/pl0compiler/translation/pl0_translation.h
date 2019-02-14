#ifndef __PL0_TRANSLATION_H_
#define __PL0_TRANSLATION_H_

#include "../lex/pl0_lex.h"

#define TXMAX       500         // max length of token table
#define MAXADDRESS  32767       // max address
#define MAX_TAG     500
#define N_INSTRUCTIONS (NOT+1)    // number of instructions
#define DIMMAX 10

#define SP 0
#define BP 1
#define ACS 2
#define TMP 3
#define TMP2 4

/*variables for token table*/
int procedure_table_index;
int table_index;
int level;


typedef enum _tInstruction_Type{
    INULL,
    PUT,
    LD,
    ST,
    JMP,
    JPC,
    INT,
    ADD,
    SUB,
    MUL,
    DIV,
    EQ,
    LE,
    LT,
    GE,
    GT,
    AND,
    OR,
    NOT,
}Instruction_Type;

const char *inc_str[N_INSTRUCTIONS];

typedef struct _tList_Node{
    Instruction_Type inc_type;
    BOOL has_arg;
    int arg;
    BOOL is_offset;
    BOOL is_tag;
    int id;
    struct _tList_Node *next;
}List_Node;

typedef List_Node *Instruction;

typedef struct _tCode_List{
    List_Node *st;
    List_Node *ed;
    int len;
}Code_List;

typedef Code_List *Code;

/*struct for token table*/
typedef struct _token_table{
    char name[MAX_ID_LEN + 1];
    int ndim;
    int dim[DIMMAX];
    int parent_index;
    BOOL isconst;
    short level;
    int value;
    int address;            // offset
    int dynamic_len_index;  //储存动态数组长度的id的table_index
} Var_Table;

/*definition for token table*/
Var_Table var_table[TXMAX];

/*struct for procedure table*/
typedef struct _procedure_table{
    char name[MAX_ID_LEN + 1];
    int parent_index;
    int usedspace;
    short level;
    int address;            // offset
} Procedure_Table;

/*definition for procedure table*/
Procedure_Table procedure_table[TXMAX];


typedef struct _tAttr{
    Code code;
    int procedure_index;
    int usedspace;
    int value;
    int var_table_index;
    int start_tag;  //用于设置过程的开始地址
}Attr;

Instruction tag_table[MAX_TAG];
int tag_num;

int new_tag();  //分配新的tag_id
void set_tag(int tag_id, Instruction inc);  //将tag_id对应的tag关联到语句

Attr *new_Attr();
Attr *copy_Attr(Attr *attr);

/*operations for token table*/
void var_table_append(const char * name, BOOL isconst, int value, int address, int ndim, const int * dim, int parent_index);
void procedure_table_append(const char * name, int address, int usedspace, int parent_index);
int lookup_id(const char *name, int parent_index);
int lookup_procedure(const char *name);


Instruction new_inc(Instruction_Type inc_type);
Instruction new_inc_arg(Instruction_Type inc_type, int arg, BOOL is_offset, BOOL is_tag);
void code_extend(Code c1, Code c2);
void code_append(Code c1, Instruction_Type inc_type);
void code_append_arg(Code c1, Instruction_Type inc_type, int arg, BOOL is_offset);
void code_append_tag(Code c1, Instruction_Type inc_type, int tag);
Code code_init();
void code_prt(Code code, FILE *fout);

Code gen_find_var_address(int table_index, int cnt_level);  //将var的地址放在运算栈顶
Code gen_find_var_pointer(int table_index, int cnt_level);    //用于寻址储存动态数组真实位置的栈空间，对普通变量和find_var_address一致
Code gen_look_back(int delta);  //将在访问链上跳动delta次后的访问链位置放入运算栈顶
Code gen_find_var_value(int table_index, int cnt_level); //值放栈顶
Code gen_change_sp(int delta);  //将内存栈顶指针sp += delta, 可小于0
Code gen_push();    //将运算栈顶压入内存栈
Code gen_build_control_chain();
Code gen_build_access_chain(int np, int nx);    //level np call level nx
Code gen_destroy_access_chain();
Code gen_init_machine();
Code gen_swap_stack_top();  //交换运算栈顶两个元素
Code gen_build_return(int procedure_index);
Code gen_change_sp_dynamic();
#endif // __PL0_TRANSLATION_H_
