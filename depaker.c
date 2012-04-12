#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SZER_EKR 160  /* szerokosc ekranu */
#define IL_POZ   10   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
#define MAX 2000000000

char wydruk[IL_POZ+1][SZER_EKR];

struct wezel {
    struct wezel *rodzic;
    struct wezel *lewy;
    struct wezel *prawy;
    int klucz;
    short int symbol;
};

void wstaw(struct wezel **drzewo, int n) {
    struct wezel *temp = *drzewo;
    struct wezel *y = NULL;
    while(temp != NULL) {
        y = temp;
        if (n >= temp->klucz) temp = temp->prawy;
        else temp = temp->lewy;
    }

    struct wezel *nowy = (struct wezel*)malloc(sizeof(struct wezel));
    nowy->rodzic = y;
    nowy->lewy = NULL;
    nowy->prawy = NULL;
    nowy->klucz = n;
    nowy->symbol = 3000;

    if (y == NULL) *drzewo = nowy;
}

void wstaw2(struct wezel **drzewo, int n,char symbol) {
    struct wezel *temp = *drzewo;
    struct wezel *y = NULL;
    while(temp != NULL) {
        y = temp;
        if (n >= temp->klucz) temp = temp->prawy;
        else temp = temp->lewy;
    }

    struct wezel *nowy = (struct wezel*)malloc(sizeof(struct wezel));
    nowy->rodzic = y;
    nowy->lewy = NULL;
    nowy->prawy = NULL;
    nowy->klucz = n;
    nowy->symbol = (short int)symbol;

    if (y == NULL) *drzewo = nowy;
}

void wstaw_drzewo(struct wezel **drzewo,struct wezel *drzewo1,struct wezel *drzewo2) {
    struct wezel *y = *drzewo;

    struct wezel *nowy = (struct wezel*)malloc(sizeof(struct wezel));
    nowy->rodzic = y;
    nowy->lewy = NULL;
    nowy->prawy = NULL;
    nowy->klucz = drzewo1->klucz+drzewo2->klucz;
    nowy->symbol = 3000;

    *drzewo = nowy;
    (*drzewo)->lewy = drzewo1;
    (*drzewo)->prawy = drzewo2;
}

int potega2(int n) {
    int s = 1;
    int i;
    for (i=0;i<n;i++) s *= 2;
    return s;
}

int main() {
    char nazwa[32],nazwa2[32];
    int i,j;
    unsigned char test[5] = {};
    int x = 0;
    int liczba[256] = {};
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    memcpy(nazwa2,nazwa,strlen(nazwa)-5); // tu czasami wali sie nazwa pliku, pewnie brak \0 czy cos
    strcat(nazwa2,".nowy");
    printf("Otwieranie pliku...\n");
    FILE *plik = fopen(nazwa,"r");
    test[0] = fgetc(plik);
    test[1] = fgetc(plik);
    memcpy(&x,&test,2);
    j = 4*x;

    for(i=0;i<j;i+=4) {
        x = fgetc(plik);
        test[0] = fgetc(plik);
        test[1] = fgetc(plik);
        test[2] = fgetc(plik);
        memcpy(&liczba[x],&test,3);
    }

    struct wezel *drzewa[256];
    struct wezel *drzewo;
    printf("Odtwarzanie drzewa...\n");
    for (i=0,j=0;i<256;i++) {
        if (liczba[i] == 0) continue;
        drzewo = NULL;
        wstaw2(&drzewo,liczba[i],(short int)i);
        drzewa[j] = drzewo;
        j++;
    }

    int size = j;
    int min1,min2;
    int j1,j2;
    struct wezel *nowe;
    while (size > 1) {
        min1 = MAX;
        min2 = MAX;
        j1 = 0;
        j2 = 0;
        for (i=0;i<j;i++) {
            if (drzewa[i] == NULL) continue;
            if (drzewa[i]->klucz < min1) {
                if (min2 > min1) {
                    min2 = drzewa[i]->klucz;
                    j2 = i;
                } else {
                min1 = drzewa[i]->klucz;
                j1 = i; }
            } else if (drzewa[i]->klucz < min2) {
                min2 = drzewa[i]->klucz;
                j2 = i;
            }
        }
        nowe = NULL;
        wstaw_drzewo(&nowe,drzewa[j1],drzewa[j2]);
        drzewa[j1] = nowe;
        drzewa[j2] = NULL;
        size--;
    }

    printf("Rozpakowywanie...\n");
    FILE *plik2 = fopen(nazwa2,"w");
    struct wezel *temp = nowe;
    int a,b=fgetc(plik),c=fgetc(plik),xxx=8;
    do {
        a = c;
        c = fgetc(plik);
        if (c == EOF && b > 0) xxx = b;
        for (j=0;j<xxx;j++) {
            if (a&potega2(7-j)) {
                temp = temp->prawy;
            } else temp = temp->lewy;
            if (temp->symbol != 3000) {
                fputc(temp->symbol,plik2);
                temp = nowe;
            }
        }

    } while (!feof(plik));
    fclose(plik);
    fclose(plik2);
    printf("Rozpakowano do %s. Koniec.\n",nazwa2);
    return 0;
}

