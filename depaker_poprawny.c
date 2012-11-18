#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BS 4096 // wielkosc bufora
#define NIE 300 // liczba dla ktorej drzewo nie przechowuje symbolu

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

// wziety z: http://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
// odwraca bajt (np. 10010000 -> 00001001)
byte odwroc(byte b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

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
// zwraca pozycje na ktorej jest reszta
int wczytaj_znaki(char *nazwa, int *znaki) {
    int i,n;
    FILE *plik = fopen(nazwa,"r");
    n = (int)(fgetc(plik)+1); // #1
    for (i=0;i<n;i++) { // #2
        byte bajty[5];
        fread(bajty,sizeof(byte),5,plik);
        memcpy(&znaki[bajty[0]],&bajty[1],4);
    }
    int p = (int)ftell(plik);
    fclose(plik);
    return p;
}

int main() {
    // deklaracja zmiennych
    char nazwa[32],nazwa2[32];
    byte buff[BS];
    int znaki[256] = {};
    int dlugosc;
    int i,j,n,r,t;
    FILE *plik,*plik2;
    drzewo drzewa[256];
    drzewo huffman;
    for (i=0;i<256;i++) strcpy(kody[i],""); // "zerowanie" kodow;

    // uzytkownik podaje nazwe (plik.huff)
    // nazwa2 to plik.nowy
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    strcpy(nazwa2,nazwa);
    strcpy(&nazwa2[strlen(nazwa)-5],".nowy");
    
    printf("Odczytuje ilosci znakow...\n");
    r = wczytaj_znaki(nazwa,znaki);

    printf("Tworze drzewo huffmana...\n");
    for (i=0,j=0;i<256;i++) {
        if (znaki[i] == 0) continue;
        stworz_wezel(&drzewa[j],znaki[i],(short int)i);
        j++;
    }

    n = j; // ilosc znakow ktory wystapily co najmniej raz
    huffman = stworz_huffman(drzewa,n);
    printf("Koduje znaki...\n");
    if (n > 1) koduj(huffman,"");
    else strcpy(kody[huffman->symbol],"0");

    plik = fopen(nazwa,"r");
    fseek(plik, 0L, SEEK_END); // przewija do konca pliku, by
    dlugosc = (int)ftell(plik); // sprawdzic dlugosc pliku
    fseek(plik,r,SEEK_SET);
    r = fgetc(plik); // #3

    plik2 = fopen(nazwa2,"w");
    byte b;
    drzewo temp = huffman;
    t = 8;
    printf("Dekompresuje i zapisuje do %s...\n",nazwa2);
    while (ftell(plik) != dlugosc) { // #4
        n = fread(buff,sizeof(byte),BS,plik);
        for (i=0;i<n;i++) {
            b = odwroc(buff[i]);
            if (ftell(plik) == dlugosc && i == n-1 && r > 0) t = r; // #3
            for (j=0;j<t;j++) {
                if (temp->symbol != NIE) {
                    fputc((byte)temp->symbol,plik2);
                    temp = huffman;
                    continue;
                }
                if (b&1) temp = temp->prawy;
                else temp = temp->lewy;
                b >>= 1;
                if (temp->symbol != NIE) {
                    fputc((byte)temp->symbol,plik2);
                    temp = huffman;
                }
            }
        }
    }
    fclose(plik);
    fclose(plik2);

    printf("Ukonczono dekompresje.\n");
    return 0;
}

