#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m, k;
double input[100000][1000];
int dominateNum[100000];
int bitmap[100000][1000];

int isInteger(double x) {
    return x - (double)(int)x == 0.0;
}

void init() {
    FILE *fin;
    int i, j;
    fin = fopen("stdin.txt", "r+");
    if (fin != NULL) {
        fscanf(fin, "%d %d %d", &n, &m, &k);
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++)
                fscanf(fin, "%lf", &input[i][j]);
            for (j = 0; j < m; j++)
                fscanf(fin, "%d", &bitmap[i][j]);
        }
        fclose(fin);
    }
    for (i = 0; i < n; ++i) {
        dominateNum[i] = 0;
    }
}

int isDominate(int x1, int x2) {
    int i;
    int smallCount = 0;
    int atLeastOneSmall = 0;
    int x1IsNull, x2IsNull;
    for (i = 0; i < m; i++) {
        x1IsNull = (bitmap[x1][i] == 0);
        x2IsNull = (bitmap[x2][i] == 0);
        if (x1IsNull || x2IsNull) {
            smallCount++;
        } else {
            if (input[x1][i] <= input[x2][i]) smallCount++;
            if (input[x1][i] < input[x2][i]) atLeastOneSmall = 1;
        }
    }
    if ((smallCount == m) && atLeastOneSmall)
        return 1;
    else
        return 0;
}

void brute() {
    clock_t  start = clock(), diff;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (i != j) {
                if (isDominate(i, j)) {
                    dominateNum[j]++;
                } else if (isDominate(j, i)) {
                    dominateNum[i]++;
                }
            }
        }
    }
    diff = clock() - start;
    unsigned long msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Brute: %lu.%lus\n", msec/1000, msec%1000 );
}

void print() {
    int i, j;
    FILE *fout;
    fout = fopen("stdout.txt", "w+");
    for (i = 0; i < n; ++i) {
        if (dominateNum[i] < k) {
            for (j = 0; j < m; j++)
                fprintf(fout, "%.6lf ", input[i][j]);
            fprintf(fout, "\n");
        }
    }
    fclose(fout);
}


int main(void) {
    init();
    brute();
    print();
    return 0;
}

