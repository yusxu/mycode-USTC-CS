#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>

struct _tPL0Lex;

typedef struct _tPL0Compiler {
    struct _tPL0Lex * lex;
} PL0Compiler;


typedef enum BOOLEAN {
    FALSE,
    TRUE,
} BOOL;

typedef enum _tLanguage{   //可用的文法符号描述语言
    LANG_ZH,     //中文
    LANG_EN,     //完整英文
    LANG_SEN,    //英文缩写
} Language;

/* Global variables */
extern FILE * fin;
extern FILE * fout;
extern PL0Compiler * compiler;
extern Language default_language;
extern BOOL expr_plus;

/* Functions */
PL0Compiler * PL0Compiler_create();
BOOL PL0Compiler_destroy(PL0Compiler * compiler);

#endif /* common.h */
