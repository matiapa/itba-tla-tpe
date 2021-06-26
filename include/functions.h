#include <string.h>

char * str_to_fun(char * str) {
    str = strtok(str, " of");
    if(!strcmp(str, "mean")) return "mean";
    if(!strcmp(str, "median")) return "median";
    if(!strcmp(str, "mode")) return "mode";
    if(!strcmp(str, "stdev")) return "stdev";
    if(!strcmp(str, "range")) return "range";
    if(!strcmp(str, "qtr1")) return "qtr1";
    if(!strcmp(str, "qtr3")) return "qtr3";
    if(!strcmp(str, "inter-qtr")) return "inter_qtr";
    if(!strcmp(str, "perc-n")) return "perc_n";
    if(!strcmp(str, "gcd")) return "gcd";
    if(!strcmp(str, "mcm")) return "mcm";
    return NULL;
}