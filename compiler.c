#include "y.tab.h"
#include <stdio.h>

int main(void) {
    
    yyparse();

    printf("Succesfully parsed");

}