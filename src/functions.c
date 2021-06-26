#include <stdlib.h>
#include <string.h>
#include <stdio.h>

double str_caller(char * str, double (*fun)(double *, int)) {
    double * list = malloc(sizeof(double));
    int s=1, p=1;
    for(int c=1; str[c] != ']'; c++){
        if(str[c] != ',') continue;
        list = realloc(list, sizeof(double) * (s+1));
        sscanf(str + p, "%lf", list + s - 1);
        s=s+1; p=c+1;
    }
    list = realloc(list, sizeof(double) * (s+1));
    sscanf(str + p, "%lf", list + s - 1);
    double res = fun(list, s); free(list);
    return res;
}

double mean(double * list, int size) {
    double sum = 0;
    for(int i=0; i<size; i++)
        sum += list[i];
    return sum / size;
}

double median(double * list) { }

double mode(double * list) { }

double stdev(double * list) { }

double range(double * list) { }

double qtr1(double * list) { }

double qtr3(double * list) { }

double inter_qtr(double * list) { }

double perc_n(double * list) { }

double gcd(double * list) { }

double mcm(double * list) { }
