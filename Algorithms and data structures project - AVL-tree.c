//####### BM40A0301 Tietorakenteet ja algoritmit ######
//Harjoitustyö AVL-puu
//Tekijä: Eemil Aspholm 0567753
//Lähteet: BM40A0301 Tietorakenteet ja algoritmit harjoitus 7 - työpaja,
//Brody, J. (2014) Pseudocode for AVL Balanced Binary Search Tree Methods [verkkojulkaisu] [viitattu 01.12.2021] saatavilla:
//https://www.cs.swarthmore.edu/~brody/cs35/f14/Labs/extras/08/avl_pseudo.pdf,


#include <math.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct solmu {
  int luku;
  int korkeus;
  struct solmu *vasen, *oikea;
}ptrsolmu;

int laskeKorkeus(ptrsolmu *);
int vertaa(int, int);
void haeAvain(ptrsolmu *, int);
ptrsolmu *vasen(ptrsolmu *);
ptrsolmu *oikea(ptrsolmu *);
ptrsolmu *kierrot(ptrsolmu *, int);
ptrsolmu *uusiSolmu(ptrsolmu *, int);
void tulosta(ptrsolmu *, int);
void kysyTiedosto(char *);
ptrsolmu *rakennaTiedostosta(ptrsolmu *);


int laskeKorkeus(ptrsolmu *ptr){
    if(ptr != NULL){
        return ptr->korkeus;
    }else{
        return 0;
    }
}

int vertaa(int i, int j){
    if(i>j){
        return i;
    }else{
        return j;
    }
}

ptrsolmu *kierrot(ptrsolmu *ptr, int luku){
    int tila = 0;
    if(ptr != NULL){//lasketaan puun vasemman ja oikean puolen korkeus = puun tila
        tila = (laskeKorkeus(ptr->vasen) - laskeKorkeus(ptr->oikea));
    }else{
       tila = 0;
    }
    if(tila > 1){ //jos tila <1 kierretään puun vasenta puolta
            if(luku < ptr->vasen->luku){
            printf("Vasen-Vasen kierto\n");
            ptr = oikea(ptr);
            tulosta(ptr, 0);
            return(ptr);
            }else{
            printf("Vasen-Oikea kierto\n");
            ptr->vasen = vasen(ptr->vasen);
            ptr = oikea(ptr);
            tulosta(ptr, 0);
            return(ptr);
            }
    }if (tila < -1){//jos tila <-1 niin kierto oikealla puolella
            if(luku > ptr->oikea->luku){
            printf("Oikea-Oikea kierto\n");
            ptr = vasen(ptr);
            tulosta(ptr, 0);
            return(ptr);
            }else{
                printf("Oikea-Vasen kierto\n");
                ptr->oikea = oikea(ptr->oikea);
                ptr = vasen(ptr);
                tulosta(ptr, 0);
                return(ptr);
            }}
        return ptr;
}

ptrsolmu *uusiSolmu(ptrsolmu *ptr, int luku){ //Varataan muistia uudelle luvulle
        if((ptr = (ptrsolmu*)malloc(sizeof(ptrsolmu*))) == NULL)
        {
            perror("Virhe muistia varatessa.\n");
            exit(0);
        }
        ptr->luku = luku;
        ptr->vasen = ptr->oikea = NULL;
        ptr->korkeus = 1;
        return ptr;
}


void tulosta(ptrsolmu *ptr, int j){
    int i = 1;
    if(ptr != NULL){
        j += 3;
        tulosta(ptr->oikea, j);
        printf("\n");
        while(i<j){ //tulostukseen välilyöntejä, jotta saadaan puumuodostelma
            printf("    ");
            i++;
        }
        printf("%d\n", ptr->luku);
        tulosta(ptr->vasen, j);
        }
    else{
        return;
    }
    return;
}


void kysyTiedosto(char *ptr){//kysytään tiedoston nimi
    printf("Anna tiedoston nimi: ");
    scanf("%s", ptr);
    return;
}

ptrsolmu *vasen(ptrsolmu *ptr){//vasemman puolen kierto
    ptrsolmu *lehti;
    
    lehti = ptr->oikea;
    ptr->oikea = lehti->vasen;
    lehti->vasen = ptr;

    ptr->korkeus = vertaa(laskeKorkeus(ptr->vasen),laskeKorkeus(ptr->oikea)) + 1;
    lehti->korkeus = vertaa(laskeKorkeus(lehti->vasen),laskeKorkeus(lehti->oikea)) +1;
    return lehti; 
}

ptrsolmu *oikea(ptrsolmu *ptr){ //oikean puolen kierto
    ptrsolmu *lehti;
    
    lehti = ptr->vasen;
    ptr->vasen = lehti->oikea;
    lehti->oikea = ptr;

    ptr->korkeus = vertaa(laskeKorkeus(ptr->vasen),laskeKorkeus(ptr->oikea)) + 1;
    lehti->korkeus = vertaa(laskeKorkeus(lehti->vasen), laskeKorkeus(lehti->oikea)) +1;
    return lehti;     
}

ptrsolmu *lisaa_solmu(ptrsolmu *ptr, int luku){
    int tila;
    if(ptr == NULL){//Luodaan uusi solmu puuhun
        ptr = uusiSolmu(ptr, luku);
        return ptr;
        }
    if(luku < ptr->luku){ //Ensin testataan kummalle puolelle luku lisätään
        ptr->vasen = lisaa_solmu(ptr->vasen, luku);
    }else if(luku > ptr->luku){
        ptr->oikea = lisaa_solmu(ptr->oikea, luku);
    }else{//jos == ptr niin luku löytyy jo puusta.
        printf("Virhe! Antamasi luku löytyy jo puusta!\n");
        return ptr;
    }
    //Päivitetään juuren korkeus solmun lisäyksen jälkeen
    ptr->korkeus = vertaa(laskeKorkeus(ptr->vasen), laskeKorkeus(ptr->oikea)) + 1;
    //sijoitetaan luku oikealle paikkaa puussa
    ptr = kierrot(ptr, luku);
    return ptr;
    }




ptrsolmu *rakennaTiedostosta(ptrsolmu *ptr){ //rakennetaan puu tiedostosta.
    char file[30];
    FILE *fptr = NULL;
    char *tdst = file;
    int rivi;
    kysyTiedosto(tdst);
    if ((fptr = fopen(tdst, "r")) == NULL){
        printf("Tiedoston avaus epäonnistui.\n");
        exit(0);
    }
    while(fscanf(fptr, "%d", &rivi) != EOF ){
        ptr = lisaa_solmu(ptr, rivi);
    }
    fclose(fptr);
    printf("Puu on valmis.\n");
    tulosta(ptr, 0);
    printf("\n");
    return ptr;  
}

void haeAvain(ptrsolmu *ptr, int avain){
    if(ptr == NULL){
        printf("Avainta ei löytynyt.\n\n");
    }else if(ptr->luku == avain){
        printf("Avain löytyi puusta!\n\n");
    }else{
        if(ptr->luku > avain){
            haeAvain(ptr->vasen, avain);
        }else{
            haeAvain(ptr->oikea, avain);
        }
    }
}


int main(){
    int valinta, syote, avain;
    ptrsolmu *puu = NULL;
    do
    {
        printf("1) Rakenna puu tiedostosta\n");
        printf("2) Lisää luku puuhun\n");
        printf("3) Tulosta puu\n");
        printf("4) Hae puusta avain\n");
        printf("0) Lopeta\n");
        printf("Valintasi: ");
        scanf("%d", &valinta);
        switch(valinta){
            case 1:
            puu = rakennaTiedostosta(puu);
            break;
            case 2:
            printf("Anna lisattava luku puuhun:\n");
            scanf("%d", &syote);
            puu = lisaa_solmu(puu, syote);
            break;
            case 3:
            if(puu != NULL){
            tulosta(puu, 0);
            printf("\n");
            }else{
                printf("Puu on tyhjä.\n");
            }
            break;
            case 4:
            printf("Anna haettava avain:\n");
            scanf("%d", &avain);
            haeAvain(puu, avain);
            break;
            case 0:
            printf("Kiitos ohjelman käytöstä.\n\n");
            break;
            default:
            printf("Tuntematon valinta, yritä uudestaan.\n\n");
        }
    } while (valinta != 0);
    return 0;
}
