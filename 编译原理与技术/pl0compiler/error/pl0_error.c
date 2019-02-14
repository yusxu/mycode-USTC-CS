#include "pl0_error.h"
#include "../lex/pl0_lex.h"
#include "../syntax/pl0_syntax.h"

void PL0_print_error(PL0Lex *lex, char * err_msg)
{
    const char** tostring = get_lang(default_language);

    #ifdef __linux
        fprintf(stderr, "\e[31mERROR: \e[0m");
    #else
        fprintf(stderr, "ERROR: ");
    #endif
    char str_position[30];
    char str_token[15] = "EOF";
    PL0TokenType type = lex->last_token_type;
    sprintf(str_position, "%d-%d", lex->line_number, lex->letter_index);
    if(type != TOKEN_NULL){
        sprintf(str_token, "'%s'", tostring[lex->last_token_type]);
    }
    fprintf(stderr, "before %-5s %-10s\t",str_position, str_token);
    fprintf(stderr, "%s\n", err_msg);

}

