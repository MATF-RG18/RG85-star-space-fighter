#ifndef _FUNKCIJE_H_
#define _FUNKCIJE_H_

typedef struct Image Image;

typedef struct {
    Image * nebo;
    unsigned nebo_tex_id;
}_podaci;

/* Funkcije za Debug-ovanje */
void iscrtaj_koordinatne_ose();
void iscrtaj_mrezu_oko_objekta();
void iscrtaj_prepreku(double kompleksnost);
void iscrtaj_stazu();
void iscrtaj_nebo();
unsigned napravi_teksturu(Image *);

/* Funkcije za iscrtavanje objekata */
void iscrtaj_letelicu();

#endif
