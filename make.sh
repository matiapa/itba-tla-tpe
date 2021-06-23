lex stats.l
yacc -d stats.y

gcc -c compiler.c lex.yy.c y.tab.c
gcc -o bin/compiler compiler.o lex.yy.o y.tab.o -ll

rm lex.yy.c y.tab.c y.tab.h *.o