#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m, k;
int input[100000][1000];
int dominateNum[100000];
int isNotNull[100000][1000];

void init() {
    FILE *fin;
    int i, j;
    fin = fopen("stdin.txt", "r+");
    if (fin != NULL) {
        fscanf(fin, "%d %d %d", &n, &m, &k);
        for (i = 0; i < n; ++i) {
            for (j = 0; j < m; j++) {
                fscanf(fin, "%d", &input[i][j]);
            }
            for (j = 0; j < m; j++) {
                fscanf(fin, "%d", &isNotNull[i][j]);
            }
        }
        fclose(fin);
    }
    for (i = 0; i < n; ++i) {
        dominateNum[i] = 0;
    }
}

int isDominate(int p1, int p2) {
    int i;
    int smallCount = 0;
    int atLeastOneSmall = 0;
    for (i = 0; i < m; ++i) {
        if (!isNotNull[p1][i] || !isNotNull[p2][i]) {
            smallCount++;
        } else if (input[p1][i] < input[p2][i]) {
            smallCount++;
            atLeastOneSmall++;
        } else if (input[p1][i] == input[p2][i]) {
            smallCount++;
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
    for (i = 0; i < n; ++i) {
        for (j = 0; j < i; ++j) {
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
            for (j = 0; j < m; j++) {
                fprintf(fout, "%d ", input[i][j]);
            }
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

