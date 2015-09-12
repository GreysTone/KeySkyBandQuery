#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m, k;
double input[100000][1000];
int cnt_domi[100000];
int bitmap[100000][1000];

int IsInteger(double x) {
    return x - (double)(int)x == 0.0;
}

void Init() {
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
        cnt_domi[i] = 0;
    }
}

int IsDominate(int x1, int x2) {
    int i;
    int is_small = 0;
    int cnt_small_or_equal = 0;
    int is_null_x1, is_null_x2;
    for (i = 0; i < m; i++) {
        is_null_x1 = (bitmap[x1][i] == 0);
        is_null_x2 = (bitmap[x2][i] == 0);
        if (is_null_x1 || is_null_x2) {
            cnt_small_or_equal++;
        } else {
            if (input[x1][i] <= input[x2][i]) cnt_small_or_equal++;
            if (input[x1][i] < input[x2][i]) is_small = 1;
        }
    }
    if ((cnt_small_or_equal == m) && is_small)
        return 1;
    else
        return 0;
}

void BruteForce() {
    clock_t  start = clock(), diff;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (i != j) {
                if (IsDominate(i, j)) {
                    cnt_domi[j]++;
                } else if (IsDominate(j, i)) {
                    cnt_domi[i]++;
                }
            }
        }
    }
    diff = clock() - start;
    unsigned long msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Brute: %lus %lums\n", msec/1000, msec%1000);
}

void Print() {
    int i, j;
    FILE *fout;
    fout = fopen("stdout.txt", "w+");
    for (i = 0; i < n; ++i) {
        if (cnt_domi[i] < k) {
            for (j = 0; j < m; j++)
                fprintf(fout, "%.6lf ", input[i][j]);
            fprintf(fout, "\n");
        }
    }
    fclose(fout);
}


int main(void) {
    Init();
    BruteForce();
    Print();
    return 0;
}
