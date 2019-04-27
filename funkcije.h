#ifndef _FUNKCIJE_H_
#define _FUNKCIJE_H_

/* Otkomentarisati za ukljucivanje 
 * debug mod-a */
// #define DEBUG (1)

#include <stdbool.h>

typedef struct Image Image;

typedef struct {
    unsigned tex_nebo_id;

    int sirina_ekrana;
    int duzina_ekrana;
    char tekst_na_centru_ekrana[255];
    char tekst_u_donjem_levom_uglu[255];
    char tekst_u_donjem_desnom_uglu[255];
    int view_distance;
    int timer_id;
    unsigned timer_interval;
    bool igra_aktivna;
    double param_predj_puta;
    double sirina_linije_staze;

    double pozicija;
    double zeljena_pozicija;
    double rotacija;
    double min_poz;
    double max_poz;
    char ** matrica_nivoa;
    int br_redova;
    int br_prepr_u_redu;
}_podaci;

/* Funkcije za Debug-ovanje */
void iscrtaj_koordinatne_ose();
void iscrtaj_mrezu_oko_objekta();

/* Funkcije za iscrtavanje objekata */
void iscrtaj_letelicu(double pozicija_letelice, double rotacija_letelice, double sirina_linije_staze);
void iscrtaj_prepreku(double kompleksnost);
void iscrtaj_stazu(double razmak);
void iscrtaj_nebo(unsigned tex_nebo_id);
void iscrtaj_prepreke(double param_predj_puta, char ** matrica_nivoa, int br_redova, int br_prepr_u_redu, int max_vidjlivih_redova, double sirina_linije_staze);
void restartuj_igru();
void zaustavi_igru();

/* Funkcije */
void procesuiraj_poziciju(double * pozicija_letelice, double * rotacija, double zeljena_pozicija);
void skreni_levo(double * zeljena_pozicija, double min_poz);
void skreni_desno(double * zeljena_pozicija, double max_poz);
unsigned napravi_teksturu(Image *);
char** alociraj_i_ucitaj_nivo(char * putanja, int * br_redova, int * br_prepr_u_redu);
void dealociraj_i_obrisi_nivo(char ** matrica_nivoa, int br_redova);
bool proveri_koliziju(double tr_poz, double param_predj_puta, double min_poz, char ** matrica_nivoa, int br_redova);
void ispisi_tekst(char * tekst, int x, int y, float r, float g, float b, int sirina_ekrana, int duzina_ekrana);

#endif
