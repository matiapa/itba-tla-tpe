#include <stdio.h>

double str_caller(char * str, double (*fun)(double *, int));

double mean(double * list, int size);

double median(double * list);

double mode(double * list);

double stdev(double * list);

double range(double * list);

double qtr1(double * list);

double qtr3(double * list);

double inter_qtr(double * list);

double perc_n(double * list);

int gcd(double * list);

int mcm(double * list);
