#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "common/common.h"
#include "lex/pl0_lex.h"
#include "syntax/pl0_tax.h"
#include "syntax/pl0_syntax.h"
#include "error/pl0_error.h"

void show_help(const char * arg0) {
    printf("Usage: %s <src>\n\n", arg0);
    printf("----CLI MODE----\n");
}

void open_file(const char *filename) {
    fin = fopen(filename, "rb");
    if (!fin) {
        printf("Fatal: Cannot open file for reading: %s\n", filename);
        exit(-1);
    }
}

void open_out_file(const char *filename) {
    fout = fopen(filename, "w");
    if (!fout) {
        printf("Fatal: Cannot open file for writing: %s\n", filename);
        exit(-1);
    }
}

void init(const char *filename) {
    open_file(filename);
    compiler = PL0Compiler_create();
}

void set_output(const char *filename) {
    open_out_file(filename);
}

void reset(){       //reset compiler and file.
    rewind(fin);
    PL0Compiler_destroy(compiler);
    compiler = PL0Compiler_create();
}

void cleanup(){     //close file, destroy compiler.
    fclose(fin);
    fin = NULL;
    PL0Compiler_destroy(compiler);
}

void lexical_analysis_test() {
    /* Lexical Analysis */
    printf("--- Lexical Analysis testing ---\n");
    PL0Lex * lex = compiler->lex;
    while (PL0Lex_get_token(lex)) {
        assert(lex->last_token_type != TOKEN_NULL);
        assert(lex->last_token_type != TOKEN_RESWORDS);
        assert(lex->last_token_type != TOKEN_SYMBOL);
        if (lex->last_token_type == TOKEN_IDENTIFIER) {
            printf("Identifier:    %-10s\t", lex->last_id);
        }
        else if (lex->last_token_type == TOKEN_NUMBER) {
            printf("Number:        %-10d\t", lex->last_num);
        }
        else if (lex->last_token_type > TOKEN_RESWORDS) {
            printf("Reserved word: %-10s\t", TOKEN_RESERVED_WORDS[lex->last_token_type - TOKEN_RESWORDS - 1]);
        }
        else {
            printf("Symbol:        %-10s\t", TOKEN_SYMBOLS[lex->last_token_type - TOKEN_SYMBOL - 1]);
        }
        printf("%d:%d-%d\n", lex->start_line_number, lex->first_letter_index, lex->first_letter_index + lex->length - 1);
    }
    printf("--- Lexical Analysis testing end. ---\n");
}

void syntax_analysis_test(){
    /*test syntax analysis*/
    printf("--- Syntax Analysis testing ---\n");
    PL0Lex * lex = compiler->lex;
    FILE *o_stack = fopen("./temp/stack_log.txt", "w");
    FILE *o_tree = fopen("./temp/tree_log.md", "w");
    PL0TEX_analysis(lex, o_stack, o_tree);
    printf("--- Syntax Analysis testing end. ---\n");
}

void compiler_test(){
    /* Start */
    printf("=== This is the program for PL/0 testing ===\n");
    /* Testing */
    lexical_analysis_test();
    reset();
    syntax_analysis_test();
    /* Cleanup */
    cleanup();
    /* Finish */
    printf("=== Normal end for testing. ===\n");
}

void compile(){ //compile without log
    PL0Lex * lex = compiler->lex;
    PL0TEX_analysis(lex, NULL, NULL);
}


void command_line(){
    char command[256];
    printf("PL0_Compiler >>> ");
    while(scanf("%s", command) == 1) {
        if(!strcmp(command, "file")) {
            char filename[256];
            scanf("%s", filename);
            if(fin != NULL){
                cleanup();
            }
            init(filename);
        }
        else if(!strcmp(command, "lex")) {
            lexical_analysis_test();
            reset();
        }
        else if(!strcmp(command, "tax")){
            syntax_analysis_test();
            reset();
        }
        else if(!strcmp(command, "lan")){
            char lan[256];
            scanf("%s", lan);
            if(!strcmp(lan, "zh")){
                default_language = LANG_ZH;
            }
            else if(!strcmp(lan, "en")){
                default_language = LANG_EN;
            }
            else if(!strcmp(lan, "sen")){
                default_language = LANG_SEN;
            }
            else {
                printf("ERROR: Can not find language: %s\n", lan);
            }
        }
        else if(!strcmp(command, "prt")){
            print_syntax(stdout, default_language);
        }
        else if(!strcmp(command, "fprt")){
            char filename[256];
            scanf("%s", filename);
            FILE *fout = fopen(filename, "w");
            if(fout == NULL){
                printf("ERROR: Can not open file: '%s'\n", filename);
                continue;
            }
            print_syntax(fout, default_language);
            fclose(fout);
        }
        else if(!strcmp(command, "check")){
            if(check_syntax()){
                printf("Result: syntax valid!\n");
            }
            else{
                printf("Result: syntax invalid!!!!!\n");
            }
        }
        else if(!strcmp(command, "test")){
            compiler_test();
        }
        else if(!strcmp(command, "e+")){
            expr_plus = TRUE;
        }
        else if(!strcmp(command, "e-")){
            expr_plus = FALSE;
        }
        else if(!strcmp(command, "q")){
            return;
        }
        else{
            printf("ERROR: Can not find command: %s\n", command);
        }
        printf("PL0_Compiler >>> ");
    }
}

int main(int argc, char * argv[]) {
    /* Parse arguments */
    if(argc == 2) {
        init(argv[1]);
        compile();
    }
    else if(argc == 3){
        init(argv[1]);
        fout = stdout;
        set_output(argv[2]);
        //compiler_test();
        compile();
    }
    else{
        show_help(argv[0]);
        command_line();
    }
    return 0;
}
