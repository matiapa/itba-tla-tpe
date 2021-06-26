#include <stdlib.h>
#include <string.h>

int str_caller(char * str, int (*fun)(double *, int)) {
    double * list = malloc(sizeof(double));
    int s=1, p=1;
    for(int c=1; str[c] != '}'; c++){
        if(str[c] != ',') continue;
        list = realloc(list, sizeof(double) * (s+1));
        sscanf(str + p, "%lf", list + s - 1);
        s=s+1; p=c+1;
    }
    list = realloc(list, sizeof(double) * (s+1));
    sscanf(str + p, "%lf", list + s - 1);
    int res = fun(list, s); free(list);
    return res;
}

int mean(double * list, int size) {
    double sum = 0;
    for(int i=0; i<size; i++)
        sum += list[i];
    return sum / size;
}

int median(double * list) { }

int mode(double * list) { }

int stdev(double * list) { }

int range(double * list) { }

int qtr1(double * list) { }

int qtr3(double * list) { }

int inter_qtr(double * list) { }

int perc_n(double * list) { }

int gcd(double * list) { }

int mcm(double * list) { }
