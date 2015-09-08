#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m, k;

void generate() {
    FILE *fout;
    int i, j, tmp;
    fout = fopen("stdin.txt", "w");
    if (fout != NULL) {
        fprintf(fout, "%d %d %d\n", n, m, k);
        for (i = 0; i < n; ++i) {
            for (j = 0; j < m; ++j)
                fprintf(fout, "%lf ", rand() % 50000 / 100.0);
            fprintf(fout, "\n");
            for (j = 0; j < m; ++j) {
                tmp = rand() % 100;
                if (tmp > 95)
                    fprintf(fout, "%d ", 1);
                else
                    fprintf(fout, "%d ", 0);
            }
            fprintf(fout, "\n");
        }
    }
}

int main(int argc, char *argv[]) {
    n = strtol(argv[1], NULL, 10);
    m = strtol(argv[2], NULL, 10);
    k = strtol(argv[3], NULL, 10);
    srand((unsigned int)time(NULL));
    generate();
    return 0;
}
