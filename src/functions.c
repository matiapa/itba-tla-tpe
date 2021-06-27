#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void sort(double * arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                double temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

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
    for (int i = 0; i < size; i++)
        sum += list[i];
    return sum / size;
}

double mode(double * list, int size) {
    sort(list, size);
    double mode = 0;
    int occurrences = 0, last_occurrences = 0;
    for (int i = 0; i < size-1; i++) {
        occurrences += 1;
        if (list[i] != list[i+1]) {
            mode = occurrences > last_occurrences ? list[i] : mode;
            last_occurrences = occurrences;
            occurrences = 0;
        }
    }
    return mode;
}

double stdev(double * list, int size) {
    if (size < 2) return 0.0;
    double sum = 0, _mean = mean(list, size);
    for (int i = 0; i < size; i++)
        sum += pow(list[i] - _mean, 2);
    return sqrt(sum / (size - 1));
}

double range(double * list, int size) {
    int min = list[0], max = min;
    for (int i = 0; i < size; i++) {
        min = list[i] < min ? list[i] : min;
        max = list[i] > max ? list[i] : max;
    }
    return max - min;
}


double perc_n(double * list, int size, int n) {
    sort(list, size);
    int f = (int) floor(size * n / 100.0);
    int c = (int) ceil(size * n / 100.0);
    double v = f == c ? list[f-1] : (list[f-1] + list[c-1]) / 2;
    return v;
}

double median(double * list, int size) {
    return perc_n(list, size, 50);
}

double qtr1(double * list, int size) {
    return perc_n(list, size, 25);
}

double qtr3(double * list, int size) {
    return perc_n(list, size, 75);
}

double inter_qtr(double * list, int size) {
    return qtr3(list, size) - qtr1(list, size);
}

double print_array(double * list_var, int size) { 
    printf("[");
    for (int i = 0; i < size; i++) {
        printf(" %lf ", list_var[i]);
    }
    printf("]\n");
    return 0;
}

double factorial(double input){
    if (input < 0)
    {
        return 0;
    }
    double result =floor(input);
    for (int i = result-1; i >0; i--)
    {
        result*=i;
    }
    return result;
}

double permutation(double n,double r){
    n=floor(n);
    r=floor(r);

    if (n<r || n<0 || r<0)
    {
        return -1;
    }
    return factorial(n)/factorial(n-r);
}
double combination(double n,double r){
    n=floor(n);
    r=floor(r);
    if (n<r || n<0 || r<0)
    {
        return -1;
    }
    return factorial(n)/(factorial(n-r)*factorial(r));
}
