#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SZER_EKR 80  /* szerokosc ekranu */
#define IL_POZ   5   /* ilosc poziomow drzewa, ktore beda wydrukowane   */

char wydruk[IL_POZ+1][SZER_EKR];
char kody[256][32];

struct wezel {
    struct wezel *rodzic;
    struct wezel *lewy;
    struct wezel *prawy;
    int klucz;
    short int symbol;
};

void drukujost(struct wezel *w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==NULL)   return;
       wydruk[poziom][srodek]='*';
}

void drukujwew(struct wezel *w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[9];
       if (w==NULL)    return;
       if (w->symbol != 3000) dl=sprintf(s,"%c:%d",w->symbol,w->klucz);
       else dl=sprintf(s,"%d",w->klucz);
       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl/2+i]=s[i];
       if (++poziom<IL_POZ){
         drukujwew(w->lewy,l,srodek,poziom) ;
         drukujwew(w->prawy,srodek+1,p,poziom) ;
       }
       else {
         drukujost(w->lewy,l,srodek,poziom) ;
         drukujost(w->prawy,srodek+1,p,poziom) ;
       }
}

void drukuj(struct wezel *w){
  int j,i;
  for (i=0;i<=IL_POZ;i++)
    for (j=0;j<SZER_EKR;j++)
      wydruk[i][j] = ' ';
  drukujwew(w,0,SZER_EKR,0);
  for (i=0;i<=IL_POZ;i++){
      for (j=0;j<SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

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

int main() {
    char nazwa[32];
    int i,j;
    printf("Podaj nazwe pliku: "); scanf("%s",nazwa);
    FILE *plik = fopen(nazwa,"r");
    fseek(plik, 0L, SEEK_END);
    int dl = ftell(plik);
    rewind(plik);
    unsigned char *ciag = (char*)malloc(dl*sizeof(char));

    for (i=0;i<dl;i++) ciag[i] = fgetc(plik);
    fclose(plik);

    int liczba[256] = {};
    struct wezel *drzewa[256];
    struct wezel *drzewo;
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
        min1 = 2000000000;
        min2 = 2000000000;
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

    //drukuj(nowe);
    koduj(nowe,"");
    printf(" lp. | znak | ASCII | ilosc wystapien | kod huffmana \n");
    printf("-----+------+-------+-----------------+--------------\n");
    for (i=0,j=0;i<256;i++) {
        if (liczba[i] == 0) continue;
        if (i >= 32 && i <= 126) printf(" %3i | %4c | %5i | %15i | %13s \n",j,i,i,liczba[i],kody[i]);
        else printf(" %3i |      | %5i | %15i | %13s \n",j,i,liczba[i],kody[i]);
        j++;
    }
    return 0;
}

