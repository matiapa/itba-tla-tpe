#include <stdio.h>

double str_caller(char * str, double (*fun)(double *, int));

double mean(double * list, int size);

double median(double * list, int size);

double mode(double * list, int size);

double stdev(double * list, int size);

double range(double * list, int size);

double qtr1(double * list, int size);

double qtr3(double * list, int size);

double inter_qtr(double * list, int size);

double perc_n(double * list, int size);

int gcd(double * list, int size);

int mcm(double * list, int size);

double print_array(double * list_var, int size);
