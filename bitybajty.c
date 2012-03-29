#include <stdio.h>
#include <stdlib.h>

// funkcja pomocnicza potega 2
int pot2(int n) {
    int i,s = 1;
    for (i=0;i<n;i++) s *= 2;
    return s;
}

// zamienia ciag kodow na bity i zapisuje do pliku
// nazwa - nazwa pliku
// kod = "101010001011101..."
void StrToBit(char *nazwa,char *kod) {
    int i,j,bajt;
    int dl = strlen(kod);
    FILE *plik = fopen(nazwa,"w");
    bajt = dl%8; // reszta, potrzebna potem przy wczytywaniu zeby obciac bity
    if (bajt == 0) bajt = 8;
    fputc(bajt,plik);
    // petla zamienia stringa na bity/bajty i zapisuje do pliku
    for (i=0;i<dl;i+=8,bajt=0) {
        if (dl-i < 8) {
            for (j=0;j<dl-i;j++) {
                if (kod[i+j] == '1') bajt += pot2(7-j);
            }
        } else {
            for (j=0;j<8;j++) {
                if (kod[i+j] == '1') bajt += pot2(7-j);
            }
        }
        fputc(bajt,plik);
    }
    fclose(plik);
}

// wczytuje plik o nazwie nazwa
// drukuje bity
void BitToStr(char *nazwa) {
    int a,c,i,reszta;
    int x = 8;
    FILE *plik = fopen(nazwa,"r");
    // drukuje bity
    reszta = fgetc(plik); // patrz StrToBit :O
    c = fgetc(plik);
    do {
        a = c;
        c = fgetc(plik);
        if (c == EOF) x = reszta;
        for (i=0;i<x;i++) {
            // drukuje bity
            // tutaj zmienic na cos przydatnego
            if (a&pot2(7-i)) printf("1");
            else printf("0");
        }
    } while (!feof(plik));
    fclose(plik);
}

int main() {
    char ciag[] = "110101010001010101010111101010101001010101011";
    StrToBit("trololo",ciag);
    printf("%s\n",ciag);
    BitToStr("trololo");
    printf("\n");
    return 0;
}
