#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *pozycje;

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

int KMP_search(char *T,char *P) {
    int temp[8000];
    int n = strlen(T);
    int m = strlen(P);
    int *pi = compute_prefix(P);
    int q = 0;
    int i,j;
    for (i=0,j=0;i<n;i++) {
        while(q > 0 && P[q] != T[i]) q = pi[q-1];
        if (P[q] == T[i]) q++;
        if (q == m) {
            //printf("Znaleziono na pozycji %i.\n",i-m+1);
            temp[j] = i-m+1;
            j++;
            q = pi[q-1];
        }
    }

    pozycje = (int*)malloc(j*sizeof(int));
    for (i=0;i<j;i++) pozycje[i] = temp[i];
    return j;
}

/*
utworzyc program ktory przy uzyciu KMP znajduje w tekscie (umieszczonym w pliku o nazwie podanej przez usera)
wszystkie wystapienia wzorca (podanego przez usera jako lancuch). wyniekiem dzialania programu powinien byc wyswietlony
tekst z zaznaczonymi (np. pogrubienie,podkreslenie,inny kolor) poczatkowymi znakami wszystkich znalezionych wystapien wzora.
*/

int main() {
    // zmienne poczatkowe
    int i,n,j,k;
    char nazwa[32] = {};
    char lancuch[64] = {};
    char *text;

    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    // wczytanie pliku
    FILE *plik = fopen(nazwa,"r");
    if (plik == NULL) {
        printf("Blad z czytaniem pliku!\n");
        exit(0);
    }
    // znajduje wielkosc pliku i wczytuje zawartosc
    fseek(plik, 0L, SEEK_END);
    n = ftell(plik);
    rewind(plik);
    text = (char*)malloc(n*sizeof(char)+1);
    for (i=0;i<n;i++) text[i] = fgetc(plik);
    fclose(plik);

    // uzytkowik podaje lancuch do znalezienia
    printf("Podaj lancuch do wyszukania: "); scanf("%s",lancuch);

    // algorytm KMP
    k = KMP_search(text,lancuch);


    // drukowanie z zaznaczeniem wystapien
    printf("\nWystapien wzorca: %i\n\n",k);
    for (i=0,j=0;i<n;i++) {
        if (i == pozycje[j]) printf("\E[5m");
        putchar(text[i]);
        printf("\E[0m");
        if (i == pozycje[j]) {
            printf("\E[0m");
            j++;
        }
    }
    printf("\n\n");
    return 0;
}
