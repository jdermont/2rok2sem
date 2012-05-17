#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *compute_prefix(char *P) {
    int n = strlen(P);
    int *pi = (int*)malloc(n*sizeof(int)+1);
    pi[0] = 0;
    int k = 0;
    int q;
    for (q=1;q<n;q++) {
        k = pi[q-1];
        while (k > 0 && P[k] != P[q]) k = pi[k-1];
        if (P[k] == P[q]) k++;
        pi[q] = k;
    }
    return pi;
}

void KMP_search(char *T,char *P) {
    int n = strlen(T);
    int m = strlen(P);
    int *pi = compute_prefix(P);
    int q = 0;
    int i;
    for (i=0;i<n;i++) {
        while(q > 0 && P[q] != T[i]) q = pi[q-1];
        if (P[q] == T[i]) q++;
        if (q == m) {
            printf("Znaleziono '%s' na pozycji %i.\n",P,i-m+1);
            q = pi[q-1];
        }
    }
    free(pi);
}

int main() {
    char text[] = "przeczyscilem zlew, bo go olewalem. teraz ide zrobic sobie lewatywe. polewka lew nalewka lwy. trololo";
    char wzorzec[] = "lew";
    printf("%s\n",text);
    KMP_search(text,wzorzec);
    return 0;
}
