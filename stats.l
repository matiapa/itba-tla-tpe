%{
#include "y.tab.h"
#define LOOKUP 0

int state;

int lookup_variable(char *name);
int add_variable(int type, char *name);

%}
%%

\n      { state = LOOKUP; }
\.\n    { state = LOOKUP; return 0; }

^number     { state = NUMBER_TYPE; }
^text       { state = TEXT_TYPE; }
^boolean    { state = BOOLEAN_TYPE; }
^list       { state = ARRAY_TYPE; }

[a-zA-Z_][a-zA-Z0-9_]* {
    if(state != LOOKUP){
        add_variable(state, yytext);
    } else {
        int type = lookup_variable(yytext);
        if (type == NUMBER_TYPE || type == TEXT_TYPE || type == BOOLEAN_TYPE || type == ARRAY_TYPE) {
            printf("Variable %s is %d\n", yytext, type);
            return type;
        }else{
            printf("Unrecognized variable '%s'\n", yytext);
        }
    }
}

%%

struct variable {
    char *variable_name;
    int variable_type;
    struct variable *next;
};

struct variable *variable_list;

extern void * malloc();

int lookup_variable(char *name){
    struct variable *vp = variable_list;

    for(; vp; vp = vp->next) {
        if(strcmp(vp->variable_name, name) == 0)
            return vp->variable_type;
    }

    return LOOKUP;
}

int add_variable(int type, char *name){
    struct variable *vp;

    printf("Adding variable %s of type %d\n", name, type);

    if(lookup_variable(name) != LOOKUP){
        printf("WARNING: Variable '%s' already defined\n", name);
        return 0;
    }

    vp = (struct variable *) malloc(sizeof(struct variable));
    vp->next = variable_list;

    vp->variable_name = (char *) malloc(strlen(name)+1);
    strcpy(vp->variable_name, name);

    vp->variable_type = type;

    variable_list = vp;

    return 1;
}
