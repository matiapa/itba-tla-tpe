#include <stdio.h>
#include <math.h>

double str_caller(char * str, double arg, double (*fun)(double *, int, double));

double mean(double * list, int size, double arg);

double median(double * list, int size, double arg);

double mode(double * list, int size, double arg);

double stdev(double * list, int size, double arg);

double range(double * list, int size, double arg);

double qtr1(double * list, int size, double arg);

double qtr3(double * list, int size, double arg);

double iqtr(double * list, int size, double arg);

double perc_n(double * list, int size, int n);

double elem_at(double * list, int size, double i);

double contains(double * list, int size, double n);

double print_array(double * list_var, int size, double arg);
