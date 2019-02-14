cmake_minimum_required(VERSION 2.8)

add_library(tax STATIC
	pl0_tax.c
)
add_library(syntax STATIC
	pl0_syntax.c
)
add_library(expr STATIC
	pl0_expr.c
)
