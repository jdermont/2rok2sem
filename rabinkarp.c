#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 10
#define MOD 214748357

int *pozycje;

int hash(char input[]) {
    int n = strlen(input);
    int suma = 0;
    int i;
    for (i=0;i<n;i++) suma += BASE*(int)input[i]*(int)input[i];
    suma %= MOD;
    //printf("Hash od \"%s\" == %i\n",input,suma);
    if (suma >= 0) return suma;
    else return suma+MOD;
}

int hashUpdate(int hash, char prev, char next) {

    int p = (int)prev;
    int n = (int)next;
    hash -= BASE*(p*p);
    hash += BASE*(n*n);
    hash %= MOD;
   // printf("%i\n",hash);
    if (hash >= 0) return hash;
    else return hash+MOD;
}

char *dziel(char input[],int i, int n) {
    char *temp = (char*)malloc((n-i)*sizeof(char)+1);
    strncpy(temp,&input[i],n-i+1);
    temp[n-i+1] = '\0';
    return temp;
}

int RabinKarp(char text[], char wzorzec[]) {
    int tmp[4096] = {};
    int n = strlen(text);
    int m = strlen(wzorzec);
    if (n < m) return -1;
    int i;
    int j = 0;
    int hash1 = hash(wzorzec);
    int hash2 = hash(dziel(text,0,m-1));
    for (i=0;i<n-m+1;i++) {
        if (hash1 == hash2) {
            if (!strncmp(wzorzec, &(text[i]), m)) {
                tmp[j] = i;
                j++;
            }
        }
        hash2 = hashUpdate(hash2, text[i],text[i+m]);
    }
    pozycje = (int*)malloc((j+1)*sizeof(int));
    for (i=0;i<j;i++) pozycje[i] = tmp[i];
    if (j == 0) return -1;
    else return j;
}

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

    // algorytm RK
    k = RabinKarp(text,lancuch);


    // drukowanie z zaznaczeniem wystapien
    if (k >= 0) printf("\nWystapien wzorca: %i\n\n",k);
    else printf("\nWystapien wzorca: 0\n\n");

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
