#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int max(int a,int b) {
    if (a >= b) return a;
    return b;
}

void odwroc(char *text) {
    int i;
    int n = strlen(text)-1;
    char z;
    for (i=0;i<n/2;i++) {
        z = text[i];
        text[i] = text[n-i];
        text[n-i] = z;
    }
}

int main() {
    // deklaracja poczatkowych zmiennych
    int i,j,m,n;
    char nazwa1[32], nazwa2[32];
    struct timeval tv;
    time_t czas_start_sek,czas_start_usek;
    time_t czas_stop_sek,czas_stop_usek;
    double czas;

    // odczyt ciagow z plikow
    printf("Podaj nazwe 1. pliku: "); scanf("%s",nazwa1);
    printf("Podaj nazwe 2. pliku: "); scanf("%s",nazwa2);
    FILE *plik1 = fopen(nazwa1,"r");
    fseek(plik1, 0L, SEEK_END);
    n = ftell(plik1);
    rewind(plik1);
    char *ciag1 = (char*)malloc((n+1)*sizeof(char));
    for (i=0;i<n;i++) ciag1[i] = fgetc(plik1);
    fclose(plik1);
    FILE *plik2 = fopen(nazwa2,"r");
    fseek(plik2, 0L, SEEK_END);
    m = ftell(plik2);
    rewind(plik2);
    char *ciag2 = (char*)malloc((m+1)*sizeof(char));
    for (i=0;i<m;i++) ciag2[i] = fgetc(plik2);
    fclose(plik2);

    // deklaracja tablicy
    int **tablica = (int**)malloc((n+1)*sizeof(int*));
    for (i=0;i<n+1;i++) *(tablica+i) = (int*)malloc((m+1)*sizeof(int*));

    // poczatek czasu
    gettimeofday(&tv, NULL);
    czas_start_sek = tv.tv_sec;
    czas_start_usek = tv.tv_usec;
    // START poczatek algorytmu
    for (i=0;i<n+1;i++) tablica[i][0] = 0;
    for (j=1;j<m+1;j++) tablica[0][j] = 0;

    for (i=1;i<n+1;i++) {
        for (j=1;j<m+1;j++) {
            if (ciag1[i-1] == ciag2[j-1]) tablica[i][j] = tablica[i-1][j-1]+1;
            else tablica[i][j] = max(tablica[i-1][j],tablica[i][j-1]);
        }
    }

    /*for (i=0;i<m+1;i++) {
        for (j=0;j<n+1;j++) {
            printf("%i,",tablica[j][i]);
        }
        printf("\n");
    }*/

    i = n;
    j = m;
    int teraz;
    int z = 0;
    char *wynik = (char*)malloc((n+1)*sizeof(char));
    while (1) {
        teraz = tablica[i][j];
        if (i > 0 && (tablica[i-1][j] == teraz)) {
            i--;
            if (i<=0 && j<=0) break;
        } else if (j > 0 && (tablica[i][j-1] == teraz)) {
            j--;
            if (i<=0 && j<=0) break;
        } else {
            wynik[z] = ciag1[i-1];
            z++;
            i--;
            j--;
            if (i<=0 && j<=0) break;
        }
    }
    wynik[z] = '\0';
    odwroc(wynik);
    // STOP koniec algorytmu
    // koniec czasu
    gettimeofday(&tv, NULL);
    czas_stop_sek = tv.tv_sec;
    czas_stop_usek = tv.tv_usec;

    czas = czas_stop_sek+(czas_stop_usek/1000000.0) - czas_start_sek-(czas_start_usek/1000000.0);

    printf("\n1. ciag ma %i elementow.\n",n);
    printf("2. ciag ma %i elementow.\n",m);
    printf("\nNajwiekszy wspolny podciag to: %s\n",wynik);
    printf("\nJego dlugosc to: %i\n",strlen(wynik));
    printf("\nCzas trwania algorytmu: %.4lf s\n",czas);

    // zwalnianie pamieci
    free(ciag1);
    free(ciag2);
    for (i=0;i<n+1;i++) free(tablica[i]);
    free(tablica);
    free(wynik);

    return 0;
}
