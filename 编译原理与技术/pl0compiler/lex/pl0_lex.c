#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "../common/common.h"
#include "pl0_lex.h"

const char * TOKEN_RESERVED_WORDS[NRW] = {"var", "const", "procedure", "begin", "end", "if", "then", "do", "while", "call", "odd", "return", "else", "elif", "endif", "print" };
const char * TOKEN_SYMBOLS[NSYM] = { "+", "-", "*", "/", "=", "!=", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=", "&&", "||", "!", "[", "]", "{", "}" };
const char BLANK_CHARACTERS[NBLK] = {' ', '\t', '\n', '\r' };



BOOL is_identifier_characters(char c) {
    return isalnum(c) || c=='_';
}

BOOL is_identifier_start_characters(char c) {
    return isalpha(c) || c=='_';
}

BOOL is_symbol_start_characters(char c) {
    for(int i=0; i<NSYM; i++) {
        if(c == TOKEN_SYMBOLS[i][0])
            return TRUE;
    }
    return FALSE;
}

BOOL is_number_characters(char c) {
    return isdigit(c);
}

BOOL is_number_start_characters(char c) {
    return isdigit(c) || c=='-';
}

BOOL is_blank(char c) {
    for(int i=0; i<NBLK; i++) {
        if(c == BLANK_CHARACTERS[i]){
            return TRUE;
        }
    }
    return FALSE;
}

int PL0Lex_getc(PL0Lex *lex) {
    int c=fgetc(fin);
    if(c == '\n') {
        lex->last_line_length = lex->letter_index;
        lex->line_number ++;
        lex->letter_index = -1;
    }
    else {
        lex->letter_index ++;
    }
    return c;
}

int PL0Lex_ungetc(PL0Lex *lex, int c) {
    if(c == '\n') {
        lex->letter_index = lex->last_line_length;
        lex->line_number --;
    }
    else {
        lex->letter_index --;
    }
    return ungetc(c, fin);
}

PL0Lex * PL0Lex_create(PL0Compiler * parent) {
    PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
    lex->compiler = parent;
    lex->line_number = 1;
    lex->letter_index = -1;
    lex->last_line_length = 0;
    return lex;
}

BOOL PL0Lex_destroy(PL0Lex * lex) {
    free(lex);
    return TRUE;
}

BOOL PL0Lex_skip_comments(PL0Lex *lex) {
    int c;
    while((c=PL0Lex_getc(lex))) {
        if(c == '*') {
            int next_c = PL0Lex_getc(lex);
            if(next_c == '/') {
                break;
            }
            else {
                PL0Lex_ungetc(lex, next_c);
            }
        }
    }
    return TRUE;
}

BOOL PL0Lex_skip_line(PL0Lex *lex) {
    int c;
    while((c=PL0Lex_getc(lex))) {
        if(c == EOF || c == '\n') {
            break;
        }
    }
    return TRUE;
}

BOOL PL0Lex_caculate_number(PL0Lex *lex, int signal) {
    lex->last_token_type = TOKEN_NUMBER;
    int c;
    while((c=PL0Lex_getc(lex))) {
        if(is_number_characters(c)) {
            if(lex->length < MAX_NUM_LEN) {
                lex->last_num *= 10;
                lex->last_num += (c - '0');
            }
            lex->length ++;
        }
        else if(is_identifier_start_characters(c)) { //error, such as '123var'
            lex->last_token_type = TOKEN_NULL;
        }
        else {
            PL0Lex_ungetc(lex, c);
            if(lex->length > MAX_NUM_LEN) {
                lex->last_token_type = TOKEN_NULL;
            }
            break;
        }
    }
    lex->last_num *= signal;
    return TRUE;
}

BOOL PL0Lex_get_id_or_resword(PL0Lex *lex) {
    int c;
    while((c=PL0Lex_getc(lex))!=EOF) {
        if(is_identifier_characters(c)) {
            if(lex->length < MAX_ID_LEN) {
                lex->last_id[lex->length] = c;
            }
            lex->length ++;
        }
        else {
            PL0Lex_ungetc(lex, c);
            break;
        }
    }

    if(lex->length > MAX_ID_LEN) {  //id too long
        lex->last_token_type = TOKEN_NULL;
        return TRUE;
    }

    for(int i=0; i<NRW; i++) {
        if(strcmp(lex->last_id, TOKEN_RESERVED_WORDS[i]) == 0) {
            lex->last_token_type = TOKEN_RESWORDS + i + 1;  //与数组中排列一致
            return TRUE;
        }
    }
    lex->last_token_type = TOKEN_IDENTIFIER;
    return TRUE;
}

int In_Symbol(char ch) {
    int k;
    for(k = 0; k < NSYM; k++) {
        if(ch == TOKEN_SYMBOLS[k][0]) {
            return 1;
        }
    }
    return 0;
}


int row = 0;    //remember the row, vary with the appearance of '\n'
int l = 0;  //remember the left of chars
int r = 0;  //remember the right of chars

BOOL PL0Lex_get_token(PL0Lex * lex) {
    int c;
    while((c=PL0Lex_getc(lex))!=EOF) {
        lex->first_letter_index = lex->letter_index;
        lex->start_line_number = lex->line_number;
        lex->length = 1;
        if(is_blank(c)) {
            ;   //pass
        }
        else if(is_identifier_start_characters(c)) {
            memset(lex->last_id, 0, sizeof(lex->last_id));
            lex->last_id[0] = c;
            lex->length = 1;
            return PL0Lex_get_id_or_resword(lex);
        }
        else if(c == '/') {
            int next_c = PL0Lex_getc(lex);
            if(next_c == '*') { //comment
                PL0Lex_skip_comments(lex);
            }
            else if(next_c == '/') { //line comment
                PL0Lex_skip_line(lex);
            }
            else {
                PL0Lex_ungetc(lex, next_c);
                lex->last_token_type = TOKEN_SLASH;
                return TRUE;
            }
        }
        else if(is_symbol_start_characters(c)) {
            char temp_symbol[3]= {'\0'};
            temp_symbol[0] = c;
            temp_symbol[1] = PL0Lex_getc(lex);
            for(int i=0; i<NSYM; i++) { //先考虑两个字符的符号
                if(strcmp(temp_symbol, TOKEN_SYMBOLS[i]) == 0) {
                    lex->last_token_type = TOKEN_SYMBOL + i + 1;    //数组中的排布和枚举中的排布一致
                    lex->length = 2;
                    return TRUE;
                }
            }
            PL0Lex_ungetc(lex, temp_symbol[1]);
            temp_symbol[1] = '\0';
            for(int i=0; i<NSYM; i++) {
                if(strcmp(temp_symbol, TOKEN_SYMBOLS[i]) == 0) {
                    lex->last_token_type = TOKEN_SYMBOL + i + 1;
                    lex->length = 1;
                    return TRUE;
                }
            }

            lex->last_token_type = TOKEN_NULL;
            return TRUE;   //不可能运行
        }
        else if(is_number_start_characters(c)) {
            lex->last_num = c-'0';
            return PL0Lex_caculate_number(lex, 1);
        }
        else {
            lex->last_token_type = TOKEN_NULL;
            return TRUE;
        }
    }
    lex->last_token_type = TOKEN_NULL;
    return FALSE;

}

