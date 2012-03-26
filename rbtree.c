#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 2

#define SZER_EKR 80
#define IL_POZ 5

char wydruk[IL_POZ+1][SZER_EKR];

struct wezel {
    struct wezel *rodzic;
    struct wezel *lewy;
    struct wezel *prawy;
    int kolor;
    int klucz;
};

int pierwszy = 1;
struct wezel *NIL;
struct wezel *drzewo;

void rotate_left(struct wezel *t) {
    struct wezel *y = t->prawy;
    struct wezel *x = t;
    x->prawy = y->lewy;
    if (y->lewy != NIL) y->lewy->rodzic = x;
    y->rodzic = x->rodzic;
    if (x->rodzic == NIL) drzewo = y;
    else if (x == x->rodzic->lewy) x->rodzic->lewy = y;
    else x->rodzic->prawy = y;
    y->lewy = x;
    x->rodzic = y;
}


void rotate_right(struct wezel *y) {
    struct wezel *x = y->lewy;
    y->lewy = x->prawy;
    if (x->prawy != NIL) x->prawy->rodzic = y;
    x->rodzic = y->rodzic;
    if (y->rodzic == NIL) drzewo = x;
    else if (y == y->rodzic->prawy) y->rodzic->prawy = x;
    else y->rodzic->lewy = x;
    x->prawy = y;
    y->rodzic = x;
}


void rb_insert_fix(struct wezel *z) {
    struct wezel *y; // = (struct wezel*)malloc(sizeof(struct wezel));

    while (z->rodzic != NIL && z->rodzic->kolor == RED) {
        if (z->rodzic == z->rodzic->rodzic->lewy) {
            y = z->rodzic->rodzic->prawy;
            if (y->kolor == RED) {
                z->rodzic->kolor = BLACK;
                y->kolor = BLACK;
                z->rodzic->rodzic->kolor = RED;
                z = z->rodzic->rodzic;
            } else {
                if (z == z->rodzic->prawy) {
                    z = z->rodzic;
                    rotate_left(z);
                }
                z->rodzic->kolor = BLACK;
                z->rodzic->rodzic->kolor = RED;
                rotate_right(z->rodzic->rodzic);
            }
        } else {
            y = z->rodzic->rodzic->lewy;
            if (y->kolor == RED) {
                z->rodzic->kolor = BLACK;
                y->kolor = BLACK;
                z->rodzic->rodzic->kolor = RED;
                z = z->rodzic->rodzic;
            } else {
                if (z == z->rodzic->lewy) {
                    z = z->rodzic;
                    rotate_right(z);
                }
                z->rodzic->kolor = BLACK;
                z->rodzic->rodzic->kolor = RED;
                rotate_left(z->rodzic->rodzic);
            }
        }

        drzewo->kolor = BLACK;
    }
}

void rb_insert(int n) {
    struct wezel *z = (struct wezel*)malloc(sizeof(struct wezel));
    z->klucz = n;
    struct wezel *y = NIL;
    struct wezel *x = drzewo;
    while (x != NIL) {
        y = x;
        if (z->klucz < x->klucz) x = x->lewy;
        else x = x->prawy;
    }
    z->rodzic = y;
    if (y == NIL) drzewo = z;
    else if (z->klucz < y->klucz) y->lewy = z;
    else y->prawy = z;
    z->lewy = NIL;
    z->prawy = NIL;
    z->kolor = RED;

    rb_insert_fix(z);
}

void wstaw() {
    int n;
    printf("Podaj element: "); scanf("%i",&n);
    if (pierwszy == 1) {
        drzewo->klucz = n;
        drzewo->rodzic = NIL;
        drzewo->lewy = NIL;
        drzewo->prawy = NIL;
        drzewo->kolor = BLACK;
        pierwszy = 0;
    } else rb_insert(n);
}

void drukujost(struct wezel *w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==NULL)    return;
       wydruk[poziom][srodek]='*';
}

void drukujwew(struct wezel *w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[9];
       if (w==NIL)    return;
       if (w->kolor == RED) dl=sprintf(s,"%dR",w->klucz);
       else dl=sprintf(s,"%dB",w->klucz);
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
      for (j=0;j<=SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

int main() {
    NIL = (struct wezel*)malloc(sizeof(struct wezel));
    NIL->kolor = BLACK;
    NIL->klucz = 0;
    NIL->rodzic = NULL;
    NIL->lewy = NULL;
    NIL->prawy = NULL;
    drzewo = (struct wezel*)malloc(sizeof(struct wezel));
    int pierwszy = 1;
    int wybor;
    while (1) {
        printf("Menu:\n");
        printf("0 - wstaw element.\n");
        printf("1 - wydrukuj drzewo.\n");
        printf("2 - wyjdz.\n");
        printf("Wybor: "); scanf("%i",&wybor);
        switch (wybor) {
            case 0: wstaw(drzewo); break;
            case 1: drukuj(drzewo); break;
            case 2: return 0; break;
        }
    }

    return 0;
}
