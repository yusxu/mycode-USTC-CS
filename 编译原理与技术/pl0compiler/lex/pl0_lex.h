#ifndef __PL0_LEX_H__
#define __PL0_LEX_H__

#include "../common/common.h"

#define NRW        (TOKEN_END_OF_TOKENS-TOKEN_RESWORDS-1)  /* Number of reserved words */
#define NSYM        (TOKEN_RESWORDS-TOKEN_SYMBOL-1)  /* Number of symbols */
#define NBLK        4   /* Number of blank characters */
#define MAX_NUM_LEN 9   /* Maximum length of a number, set to 9 to fit in range of C (-2147483648 to 2147483647) */
#define MAX_ID_LEN  10  /* Maximum length of an identifier */

struct _tPL0Compiler;

typedef enum _tPL0TokenType {
    TOKEN_NULL,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,   /* Below are accepted symbols */
    TOKEN_PLUS,     /* + */
    TOKEN_MINUS,    /* - */
    TOKEN_TIMES,    /* * */
    TOKEN_SLASH,    /* / */
    TOKEN_EQU,      /* = */
    TOKEN_NEQ,      /* != */
    TOKEN_LES,      /* < */
    TOKEN_LEQ,      /* <= */
    TOKEN_GTR,      /* > */
    TOKEN_GEQ,      /* >= */
    TOKEN_LPAREN,   /* ( */
    TOKEN_RPAREN,   /* ) */
    TOKEN_COMMA,    /* , */
    TOKEN_SEMICOLON,/* ; */
    TOKEN_PERIOD,   /* . */
    TOKEN_BECOMES,  /* := */
    TOKEN_AND,       /* && */
    TOKEN_OR,        /* || */
    TOKEN_NOT,       /* ! */
    TOKEN_LSQUARE,   /* [ */
    TOKEN_RSQUARE,   /* ] */
    TOKEN_LCURLY,    /* { */
    TOKEN_RCURLY,    /* } */
    TOKEN_RESWORDS, /* Below are reserved words */
    TOKEN_VAR,
    TOKEN_CONST,
    TOKEN_PROCEDURE,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_DO,
    TOKEN_WHILE,
    TOKEN_CALL,
    TOKEN_ODD,
    TOKEN_RETURN,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_ENDIF,
    TOKEN_PRINT,
    TOKEN_END_OF_TOKENS,        //无意义，作为中止
} PL0TokenType;

extern const char * TOKEN_RESERVED_WORDS[NRW];
extern const char * TOKEN_SYMBOLS[NSYM];

typedef struct _tPL0Lex {
    /* Parent return pointer */
    struct _tPL0Compiler * compiler;
    /* For output */
    PL0TokenType last_token_type;
    char last_id[MAX_ID_LEN + 1];
    int last_num;
    int last_level;
    int start_line_number;
    int first_letter_index;
    int length;

    /* File position */
    int line_number;
    int letter_index;
    int last_line_length;   //上一行的长度，用于回退

    /** --------------------------
     * TODO: Your variables here
     */
} PL0Lex;

PL0Lex * PL0Lex_create(struct _tPL0Compiler * parent);
BOOL PL0Lex_destroy(PL0Lex * lex);
BOOL PL0Lex_get_token(PL0Lex * lex);

#endif /* pl0_lex.h */
