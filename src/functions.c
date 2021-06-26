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

double median(double * list, int size) { }

double mode(double * list, int size) { }

double stdev(double * list, int size) { }

double range(double * list, int size) { }

double qtr1(double * list, int size) { }

double qtr3(double * list, int size) { }

double inter_qtr(double * list, int size) { }

double perc_n(double * list, int size) { }

double gcd(double * list, int size) { }

double mcm(double * list, int size) { }
