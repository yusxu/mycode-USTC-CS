#include <stdlib.h>
#include "common.h"
#include "../lex/pl0_lex.h"

FILE * fin = NULL;
FILE * fout = NULL;
PL0Compiler * compiler = NULL;
Language default_language = LANG_ZH;
BOOL expr_plus = TRUE;

PL0Compiler * PL0Compiler_create() {
    PL0Compiler * compiler = (PL0Compiler *) calloc(1, sizeof(PL0Compiler));
    compiler->lex = PL0Lex_create(compiler);

    return compiler;
}

BOOL PL0Compiler_destroy(PL0Compiler * compiler) {
    PL0Lex_destroy(compiler->lex);

    free(compiler);
    return TRUE;
}
