#ifndef _FUNKCIJE_H_
#define _FUNKCIJE_H_

typedef struct Image Image;

typedef struct {
    Image * nebo;
    unsigned nebo_tex_id;
    int timer_id;
    unsigned timer_interval;

    double pozicija;
    double zeljena_pozicija;
    double rotacija;
}_podaci;

/* Funkcije za Debug-ovanje */
void iscrtaj_koordinatne_ose();
void iscrtaj_mrezu_oko_objekta();
void iscrtaj_prepreku(double kompleksnost);
void iscrtaj_stazu();
void iscrtaj_nebo();
unsigned napravi_teksturu(Image *);

/* Funkcije za iscrtavanje objekata */
void iscrtaj_letelicu(double pozicija_letelice, double rotacija_letelice);
void procesuiraj_poziciju(double * pozicija_letelice, double * rotacija, double zeljena_pozicija);

#endif
