#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* utworzyc program w jezyku C znajdujacy NWP dwoch ciagow znakow zapisanych w plikach
o po danych (przez uzytkownika) nazwach. Przeprowadzic pomiary czasu wykonywania algorytmu
dla dwoch ciagow o dlugosci a) 100 el. b) 200 el. c) 500 el. d) 1000 el. */
// uwaga - do wypelniania plikow znakami uzyc prograum tworzonego w poprzednim semestrze
// (pobierajacego od uzytkownika liczby znakow w pliku i zakres z ktorego maja byc losowane)

int main() {
    srand(time(NULL));
    int i,n;
    char min,max;
    char nazwa[32];
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    printf("Podaj liczbe znakow: "); scanf("%i",&n);
    getchar();
    printf("Podaj minimalny zakres (np. a): "); scanf("%c",&min);
    getchar();
    printf("Podaj maksymalny zakres (np. z): "); scanf("%c",&max);
    char *ciag;
    ciag=(char*)malloc(sizeof(char)*n+1);
    for (i=0;i<n;i++) ciag[i] = rand()%(max-min)+min;
    FILE *plik = fopen(nazwa,"w");
    fprintf(plik,"%s",ciag);
    fclose(plik);
    free(ciag);
    return 0;
}
