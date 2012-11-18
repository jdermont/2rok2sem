#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BS 4096 // wielkosc bufora
#define NIE 300 // liczba dla ktorej drzewo nie przechowuje symbolu // w zasadzie kazda poza 0-255 w zakresie short int

// struktura wezlu (drzewa)
struct wezel {
    //struct wezel *rodzic; // rodzic tu niepotrzebny
    struct wezel *lewy;
    struct wezel *prawy;
    int ilosc;
    short int symbol;
};

typedef unsigned char byte;
typedef struct wezel *drzewo;

// tablica kodow, pierwszy to znak, drugi to kod tego znaku w postaci binarnej
char kody[256][32];

// tworzy podstawowe "kolko", z symbolem i iloscia
void stworz_wezel(drzewo *d, int ilosc, short int symbol) {
    *d = (drzewo)malloc(sizeof(struct wezel));
    //(*d)->rodzic = NULL;
    (*d)->lewy = NULL;
    (*d)->prawy = NULL;
    (*d)->ilosc = ilosc;
    (*d)->symbol = symbol;
}

// laczy 2 "kolka" w jedno wieksze wyzej
void wstaw_drzewo(drzewo *nowe, drzewo d1, drzewo d2) {
    stworz_wezel(nowe,d1->ilosc+d2->ilosc,NIE);
    (*nowe)->lewy = d1;
    (*nowe)->prawy = d2;
}

// petla tworzaca drzewo huffmana
drzewo stworz_huffman(drzewo *drzewa,int ilosc) {
    int min1,min2;
    int j1,j2;
    int i,j=ilosc;
    drzewo nowe;
    while (ilosc > 1) {
        min1 = -1;
        min2 = -1;
        j1 = 0;
        j2 = 0;
        for (i=0;i<j;i++) {
            if (drzewa[i] == NULL) continue;
            if (min1 == -1) {
                min1 = drzewa[i]->ilosc;
                j1 = i;
                continue;
            }
            if (min2 == -1) {
                min2 = drzewa[i]->ilosc;
                j2 = i;
                continue;
            }
            if (drzewa[i]->ilosc < min1) {
                if (min2 > min1) {
                    min2 = drzewa[i]->ilosc;
                    j2 = i;
                } else {
                    min1 = drzewa[i]->ilosc;
                    j1 = i;
                }
            } else if (drzewa[i]->ilosc < min2) {
                min2 = drzewa[i]->ilosc;
                j2 = i;
            }
        }
        nowe = NULL;
        wstaw_drzewo(&nowe,drzewa[j1],drzewa[j2]);
        drzewa[j1] = nowe;
        drzewa[j2] = NULL;
        ilosc--;
    }
    return drzewa[0];
}

// TODO: zakomentarzowac
void koduj(drzewo x, char *k) {
    if (x == NULL) return;
    if (x->symbol != NIE) {
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

// struktura pliku
// #1 pierwszy bajt - ilosc roznych znakow-1 (0 do 255, czyli 1 do 256 roznych znakow)
// #2 kolejne n*5 bajtow - 1 bajt na znak, 4 bajty na ilosc (bitowo, czyli max 2^31-1 dla kazdego znaku)
// #3 kolejny bajt - reszta z dzielenia dlugosci skompresowanego pliku modulo 8
// #4 dalej - kody w postaci binarnej
// zwraca dlugosc skompresowanego pliku
int zapisz(char *nazwa, char *nazwa2, int dlugosc, int ilosc, int *znaki) {
    int i,j,r,n;
    FILE *plik = fopen(nazwa2,"w");
    fputc((byte)(ilosc-1),plik); // #1
    for (i=0,j=0;i<256;i++) { // #2
        if (znaki[i] == 0) continue;
        byte bajty[5];
        bajty[0] = (byte)i;
        memcpy(&bajty[1],&znaki[i],4); // skrotowo: int zamienia na 4 bajty
        fwrite(bajty,sizeof(byte),5,plik);
    }
    r = (int)ftell(plik); // #3, tylko pozycja pliku
    fputc('0',plik); // #3, narazie nie mamy reszty wiec tylko 0

    FILE *plik2 = fopen(nazwa,"r");
    byte buff[BS];
    byte b;
    int suma = 0;
    int t = 0;
    while (ftell(plik2) != dlugosc) { // #4
        n = fread(buff,sizeof(byte),BS,plik2);
        for (i=0;i<n;i++) {
            b = buff[i];
            for (j=0;j<strlen(kody[b]);j++) {
                if (t == 8) {
                    t = 0;
                    fputc((byte)suma,plik);
                    suma = 0;
                }
                suma <<= 1;
                if (kody[b][j] == '1') suma++;
                t++;
            }
        }
        if (t == 8) {
            t = 0;
            fputc((byte)suma,plik);
            suma = 0;
        }
    }

    if (t > 0) fputc((byte)(suma<<(8-t)),plik);
    int d = ftell(plik);
    fseek(plik,r,SEEK_SET); // #3
    fputc((byte)t,plik); // #3
    fclose(plik);
    fclose(plik2);
    return d;
}

int main() {
    // deklaracja zmiennych
    char nazwa[32],nazwa2[32];
    byte buff[BS];
    int znaki[256] = {};
    int dlugosc;
    int i,j,n;
    FILE *plik;
    drzewo drzewa[256];
    drzewo huffman;
    for (i=0;i<256;i++) strcpy(kody[i],""); // "zerowanie" kodow;

    // uzytkownik podaje nazwe
    // nowa nazwa to plik.huff
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    strcpy(nazwa2,nazwa);
    strcat(nazwa2,".huff"); // dodaje ".huff" do napisu
    plik = fopen(nazwa,"r");
    fseek(plik, 0L, SEEK_END); // przewija do konca pliku, by
    dlugosc = (int)ftell(plik); // sprawdzic dlugosc pliku
    rewind(plik); // przewija do poczatku pliku

    // zlicza ilosci jakich znakow
    printf("Zliczam znaki...\n");
    while (ftell(plik) != dlugosc) {
        n = fread(buff,sizeof(byte),BS,plik);
        for (i=0;i<n;i++) znaki[buff[i]]++;
    }
    fclose(plik);

    printf("Tworze drzewo huffmana...\n");
    for (i=0,j=0;i<256;i++) {
        if (znaki[i] == 0) continue;
        stworz_wezel(&drzewa[j],znaki[i],(short int)i);
        j++;
    }

    n = j; // ilosc znakow ktore wystapily co najmniej raz
    huffman = stworz_huffman(drzewa,n);
    printf("Koduje znaki...\n");
    if (n > 1) koduj(huffman,"");
    else strcpy(kody[huffman->symbol],"0");

    printf("Zapisuje do %s...\n",nazwa2);
    int nowa_dlugosc = zapisz(nazwa,nazwa2,dlugosc,n,znaki);
    
    printf("Dlugosc starego pliku: %i bajtow\n",dlugosc);
    printf("Dlugosc nowego pliku: %i bajtow\n",nowa_dlugosc);
    if (nowa_dlugosc > dlugosc) printf("Ostrzezenie: nowy plik wiekszy niz stary!\n");
    return 0;
}

