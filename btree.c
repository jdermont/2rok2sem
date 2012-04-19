#include <stdio.h>
#include <stdlib.h>
// ustalone T
// indeksowane jest [1...n] bo w Cormenie tak jest :<
#define T 2

// zmienne do drukowania
#define SZER_EKR 80
#define IL_POZ 5
char wydruk[IL_POZ+1][SZER_EKR];

struct node {
    unsigned int number; // aktualna liczba kluczy w wezle
    //enum {0,1} is_leaf; // czy jest lisciem
    enum {false,true} is_leaf;
    unsigned int key[2*T]; // tablica kluczy, maks rozmiar
    struct node *son[2*T+1]; // tablica wskaznikow do potomkow
};

typedef struct node* Wezel;

Wezel drzewo;

// tworzy nowy pusty wezel / alokuje pamiec
Wezel allocate_node() {
    Wezel x = (Wezel)malloc(sizeof(struct node));
    x->number = 0;
    return x;
}

// tworzy drzewo, funkcja poczatkowa
void bt_create() {
    Wezel x = allocate_node();
    x->is_leaf = true;
    x->number = 0;
    int i;
    for (i=1;i<2*T;i++) x->key[i] = 0;
    drzewo = x;
}

// funkcja pomocnicza, dzieli wezel na kilka mniejszych
void bt_split_child(Wezel *ptr, int i) {
    Wezel x = *ptr;
    Wezel z = allocate_node();
    Wezel y = x->son[i];
    z->is_leaf = y->is_leaf;
    z->number = T-1;
    int j;
    for (j=1;j<=T-1;j++) {
        z->key[j] = y->key[j+T];
    }
    if (y->is_leaf == false) {
        for (j=1;j<=T;j++) {
            z->son[j] = y->son[j+T];
        }
    }
    y->number = T-1;

    for (j=x->number+1;j>=i+1;j--) {
        x->son[j+1] = x->son[j];
    }

    x->son[i+1] = z;

    for (j=x->number;j>=i;j--) {
        x->key[j+1] = x->key[j];
    }

    x->key[i] = y->key[T];
    x->number = x->number+1;
    *ptr = x;
}

// funkcja pomocnicza do wstawiania
void bt_insert_nonfull(Wezel *ptr, int k) {
    Wezel x = *ptr;
    int i = x->number;
    if (x->is_leaf == true) {
        while (i >= 1 && k < x->key[i]) {
            x->key[i+1] = x->key[i];
            i--;
        }
        x->key[i+1] = k;
        x->number = x->number + 1;
    } else {
        while (i >= 1 && k < x->key[i]) {
            i--;
        }
        i++;
        if (x->son[i]->number == 2*T-1) {
            bt_split_child(&x,i);
            if (k > x->key[i]) i++;
        }
        bt_insert_nonfull(&x->son[i],k);
    }
    *ptr = x;
}

// wstawia k do drzewa
void bt_insert(int k) {
    Wezel r = drzewo;
    if (r->number == 2*T-1) {
        Wezel s = allocate_node();
        drzewo = s;
        s->is_leaf = false;
        s->number = 0;
        s->son[1] = r;
        bt_split_child(&s,1);
        bt_insert_nonfull(&s,k);
    } else {
        bt_insert_nonfull(&r,k);
    }
}

// funkcje do drukowania drzew binarnaych Paczka
// zmodyfikowane do potrzeb b-drzew
// drukuje poziomo i troche nierowno :<
void drukujost(Wezel w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==NULL)    return;
       wydruk[poziom][srodek]='*';
}

void drukujwew(Wezel w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[20] = {};
       if (w==NULL)    return;
       //dl=sprintf(s,"%d",w->klucz);
        for (i=1;i<=w->number;i++) {
             dl=sprintf(s,"%s %d",s,w->key[i]);
        }

       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl+i]=s[i];
       if (++poziom<IL_POZ){
           for (i=1;i<=w->number+1;i++) {
             drukujwew(w->son[i],l,srodek+dl+22*i,poziom) ;
           }
       //  drukujwew(w->lewy,l,srodek,poziom) ;
       //  drukujwew(w->prawy,srodek+1,p,poziom) ;
       }
       else {
            for (i=1;i<=w->number+1;i++) {
             drukujost(w->son[i],l,srodek+i,poziom) ;
           }
        // drukujost(w->lewy,l,srodek,poziom) ;
        // drukujost(w->prawy,srodek+1,p,poziom) ;
       }
}

void drukuj(Wezel w){
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
    bt_create();
    int wybor,n;
    while(1) {
        printf("1 - wstaw element.\n");
        printf("2 - pokaz drzewo.\n");
        printf("3 - wyjscie.\n");
        printf("Wybor: "); scanf("%i",&wybor);
        switch(wybor) {
            case 1: printf("Dodaj element: "); scanf("%i",&n); bt_insert(n); break;
            case 2: drukuj(drzewo); break;
            case 3: exit(0); break;
        }
    }

    return 0;
}
