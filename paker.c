#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SZER_EKR 160  /* szerokosc ekranu */
#define IL_POZ   10   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
#define MAX 2000000000

char wydruk[IL_POZ+1][SZER_EKR];
char kody[256][32];

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

void koduj(struct wezel *x, char *k) {
    if (x == NULL) return;
    if (x->symbol != 3000) {
        strcpy(kody[(unsigned char)x->symbol],k);
        return;
    }
    int len = strlen(k);
    char *s = (char*)malloc((len+2)*sizeof(char));
    char *s2 = (char*)malloc((len+2)*sizeof(char));

    strcpy(s,k);
    strcat(s,"0");
    strcpy(s2,k);
    strcat(s2,"1");
    koduj(x->lewy,s);
    koduj(x->prawy,s2);
}

int potega2(int n) {
    int s = 1;
    int i;
    for (i=0;i<n;i++) s *= 2;
    return s;
}

void zamien(char *naz, unsigned char *ciag,int dl) {
    int i,j;
    char *nowy = (char*)malloc(sizeof(char)+1);
    char *nn = (char*)malloc(sizeof(char)+1);
    int len;
    for (i=0;i<dl;i++) {
        len = strlen(nowy);
        nn = realloc(nowy,len+sizeof(kody[ciag[i]])+2);
        nowy = nn;
        strcat(nowy,kody[ciag[i]]);
    }
    FILE *plik = fopen(naz,"a");
    dl = strlen(nowy);
    unsigned char bajt = dl%8;
    fputc(bajt,plik);
    for (i=0;i<dl;i+=8) {
        bajt = 0;
        if (dl-i < 8) {
            for (j=0;j<dl-i;j++) {
                if (n/owy[i+j] == '1') {
                    bajt += potega2(7-j);
                }
            }
        } else {
            for (j=0;j<8;j++) {
                if (nowy[i+j] == '1') {
                    bajt += potega2(7-j);
                }
            }
        }
        fputc(bajt,plik);

    }
    fclose(plik);
    free(nn);
}

void zapisz(char *naz, int *liczba) {
    FILE *plik = fopen(naz,"w");
    unsigned char test[5] = {};
    int i,j;
    for (j=0,i=0;i<256;i++) {
        if (liczba[i] > 0) {
            j++;
        }
    }
    memcpy(test,&j,2);
    fputc(test[0],plik);
    fputc(test[1],plik);
    for (i=0;i<256;i++) {
        if (liczba[i] > 0) {
            memcpy(test,&liczba[i],3);
            fputc((unsigned char)i,plik);
            fputc(test[0],plik);
            fputc(test[1],plik);
            fputc(test[2],plik);
        }
    }
    fclose(plik);
}

int main() {
    char nazwa[32],nazwa2[32];
    int i,j;
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    strcpy(nazwa2,nazwa);
    strcat(nazwa2,".huff");
    FILE *plik = fopen(nazwa,"r");
    fseek(plik, 0L, SEEK_END);
    int dl = ftell(plik);
    rewind(plik);
    unsigned char *ciag = (char*)malloc(dl*sizeof(char));

    printf("Wczytywanie pliku...\n");
    for (i=0;i<dl;i++) ciag[i] = fgetc(plik);
    fclose(plik);

    int liczba[256] = {};
    struct wezel *drzewa[256];
    struct wezel *drzewo;

    printf("Tworzenie drzewa...\n");
    for (i=0;i<256;i++) strcpy(kody[i],"");
    for (i=0;i<dl;i++) {
        liczba[ciag[i]]++;
    }
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
         //   printf("%i < %i lub %i\n",drzewa[i]->klucz,min1,min2);
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

    printf("Kodowanie znakow...\n");
    koduj(nowe,"");
    printf("Zapisywanie pliku %s...\n",nazwa2);
    zapisz(nazwa2,liczba);
    zamien(nazwa2,ciag,dl);
    printf("Zrobione.\n");
    //wczytaj();
    return 0;
}

