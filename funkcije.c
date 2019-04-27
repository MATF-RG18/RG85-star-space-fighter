#include "funkcije.h"
#include "image.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define MAX_IN_LINE (1000)

extern _podaci glob_prom; 

/* Pomocne enkapsulirane (privatne) _funkcije */

static void _iscrtaj_valjak(double r_donje_baze, double r_gornje_baze, double visina, bool popuni_baze) 
{
    static bool is_called = 0;
    static GLUquadric * quad;

    if (!is_called) {
        quad = gluNewQuadric();

        if (quad == 0) {
               fprintf(stderr, "Neuspela alokacija quadrc objekta!\n");
               exit(1);
        }
    }

    if (popuni_baze) {
        glPushMatrix();
            glRotatef(90,1,0,0);
            glTranslatef(0,0,0.5);
            glScalef(1,1,0.001);
            glutSolidSphere(r_gornje_baze, 40, 40);
        glPopMatrix();

        glPushMatrix();
                glRotatef(90,1,0,0);
                glTranslatef(0,0,-0.5);
                glScalef(1,1,0.001);
                glutSolidSphere(r_donje_baze, 40, 40);
        glPopMatrix();
    }

    glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-0.5);
        gluCylinder(quad, r_donje_baze, r_gornje_baze, visina, 40, 40);
    glPopMatrix();
}


/* Javno vidljive funkcije (public API) */

/* Funkcije za debugovanje */
void iscrtaj_koordinatne_ose()
{
    glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

		/* X osa */
		glColor3f(1,0,0);
		glVertex3f(-1,0,0);
		glVertex3f(1,0,0);

		/* Y osa */
		glColor3f(0,1,0);
		glVertex3f(0,-1,0);
		glVertex3f(0,1,0);

		/* Z osa */
		glColor3f(0,0,1);
		glVertex3f(0,0,-1);
		glVertex3f(0,0,1);

	glEnd();
}

void iscrtaj_mrezu_oko_objekta() 
{
    glDisable(GL_LIGHTING);
    glColor3f(0,1,0);
    glutWireCube(1);
}

/* Funkcije za iscrtavanje game objekata */
void iscrtaj_letelicu(double pozicija, double rotacija, double sirina_linije_staze)
{
    glPushMatrix();
   
    /* Postavljanje pozicije na osnovu argumenata funkcije */
    glScalef(sirina_linije_staze, 1, 1);
    glTranslatef(pozicija, 0, 0);
    glScalef(1/sirina_linije_staze, 1, 1);
    glRotatef(rotacija * 50, 0, 0, -1);

    #if DEBUG
        glDisable(GL_LIGHTING);
        iscrtaj_koordinatne_ose();
        iscrtaj_mrezu_oko_objekta();
    #endif
    
    glEnable(GL_LIGHTING);

    /* Predefinisane komponente za gradjenje materijala */
    static GLfloat material_ambient[] = { 0.1, 0.1, 0.1, 1 };
    static GLfloat material_diffuse[] = { 0.47, 0.47, 0.47, 1 };
    static GLfloat material_diffuse_lblue[] = { 0.1, 0.7, 0.6, 1 };
    static GLfloat material_diffuse_dark_gray[] = { 0.2, 0.2, 0.2, 1 };
    static GLfloat material_diffuse_red[] = { 0.8, 0.0, 0.0, 1 };
    static GLfloat material_emission_red[] = { 0.2, 0.1, 0.2, 1 };
    static GLfloat material_emission_lblue[] = { 0.1, 0.25, 0.25, 1 };
    static GLfloat material_specular[] = { 0.1, 0.1, 0.1, 1 };
    static GLfloat material_high_specular[] = { 0.2, 0.2, 0.2, 1 };
    static GLfloat shininess[] = { 10 };
    static GLfloat high_shininess[] = { 100 };

    static GLfloat material_emission[] = { 0.1, 0.1, 0.1, 0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

    /* Tail fire light */
    GLfloat light_position[] = { 0, 0, 1.2, 0 };
    static GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    static GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1 };
    static GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1 };
    
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);

    /* Trup letelice */
    glPushMatrix();
        glScalef(0.4,0.08,1);
        glRotatef(90,1,0,0);
        _iscrtaj_valjak(1, 0.1, 1, 1);
    glPopMatrix();

    /* Krila letelice */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
        glScalef(0.7,0.035,0.7);
        glTranslatef(0,0.02,0.2);
        glRotatef(90,1,0,0);
        _iscrtaj_valjak(1, 0.1, 1, 1);
    glPopMatrix();

    /* Staklo na kabini */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_high_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_lblue);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_lblue);
        glTranslatef(0,0.015,-0.27);
        glScalef(0.1, 0.032, 0.3);
        glRotatef(-90, 1,0,0);
        glRotatef(-4, 1,0,0);
        static GLdouble clip_plane[] = {0, 1, 0, 0.4};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        _iscrtaj_valjak(0.2, 1, 1, 1);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);

    /* Desno peraje */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        glTranslatef(0.1,0.0,0.0);
        glRotatef(5,0,1,0);
        glRotatef(-6,1,0,0);
        glScalef(0.03,0.17,0.55);
        glTranslatef(0,0.2,0.35);
        glRotatef(90,1,0,0);

        static GLdouble clip_plane2[] = {0, 0, -1, 0.5};
        glClipPlane(GL_CLIP_PLANE2, clip_plane2);
        glEnable(GL_CLIP_PLANE2);
        _iscrtaj_valjak(1, 0, 1, 1);
        glDisable(GL_CLIP_PLANE2);
    glPopMatrix();

    /* Levo peraje */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        glTranslatef(-0.1,0.0,0.0);
        glRotatef(-5,0,1,0);
        glRotatef(-6,1,0,0);
        glScalef(0.03,0.17,0.55);
        glTranslatef(0,0.2,0.35);
        glRotatef(90,1,0,0);

        glClipPlane(GL_CLIP_PLANE2, clip_plane2);
        glEnable(GL_CLIP_PLANE2);
        _iscrtaj_valjak(1, 0, 1, 1);
        glDisable(GL_CLIP_PLANE2);
    glPopMatrix();


    /* Mlaznjak */
    glPushMatrix();
        glScalef(0.1, 0.1, 0.1);
        glTranslatef(0,0,5.5);
        glRotatef(90,1,0,0);

        static GLdouble clip_plane1[] = {0, 1, 0, 0.52};
        glClipPlane(GL_CLIP_PLANE1, clip_plane1);

        /* Desna cev mlaznjaka */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
            glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
            glTranslatef(1.45,0,0);
            glEnable(GL_CLIP_PLANE1);
            _iscrtaj_valjak(0.8, 0.8, 2, 0);
            glDisable(GL_CLIP_PLANE1);
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_red);
                glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_red);
                glutSolidSphere(0.65, 10, 10);
            glPopMatrix();
        glPopMatrix();

        /* Leva cev mlaznjaka */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
            glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
            glTranslatef(-1.45,0,0);
            glEnable(GL_CLIP_PLANE1);
            _iscrtaj_valjak(0.8, 0.8, 2, 0);
            glDisable(GL_CLIP_PLANE1);
        
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_red);
                glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_red);
                glutSolidSphere(0.65, 10, 10);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    
    glPopMatrix();
}

void iscrtaj_stazu(double sirina_linije_staze) 
{
    glDisable(GL_LIGHTING);

    glColor3f(0.02,0,0.08);
    glPushMatrix();
        glScalef(sirina_linije_staze,1,1);
        glBegin(GL_QUADS);
                glVertex3f(-2.5, -1.01, -500);
                glVertex3f(2.5, -1.01, -500);
                glVertex3f(2.5, -1.01, 10);
                glVertex3f(-2.5, -1.01, 10);
        glEnd();
    glPopMatrix();

    glColor3f(255,255,255);
    glLineWidth(1);
    glPushMatrix();
        glScalef(sirina_linije_staze,1,1);
        glBegin(GL_LINES);
            for (int i=-2; i<= 3; i++) {
                glVertex3f(i-0.5, -1, -500);
                glVertex3f(i-0.5, -1, 10);
            }
        glEnd();
    glPopMatrix();

    glColor3f(255,0,255);
    glLineWidth(5);
    glPushMatrix();
        glScalef(sirina_linije_staze,1,1);
        glBegin(GL_LINES);
            for (int i=-2; i<= 3; i++) {
                glVertex3f(i-0.5, -1, -500);
                glVertex3f(i-0.5, -1, 10);
            }
        glEnd();

    glPopMatrix();
}


unsigned napravi_teksturu(Image *img) 
{
    glEnable(GL_TEXTURE_2D);
    GLuint texture_name;
    glGenTextures(1, &texture_name);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, texture_name);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                img->width, img->height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, img->pixels);

    glDisable(GL_TEXTURE_2D);

    return texture_name;
}


static void _iscrtaj_ravan() 
{
    glPushMatrix();
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0, 0);
        glVertex2f(-0.5, 0.5);

        glTexCoord2f(0, 1);
        glVertex2f(-0.5, -0.5);

        glTexCoord2f(1, 1);
        glVertex2f(0.5, -0.5);

        glTexCoord2f(1, 0);
        glVertex2f(0.5, 0.5);
    glEnd();
    glPopMatrix();
}


static void _iscrtaj_kocku() 
{
    // prednja
    glPushMatrix();
         glTranslatef(0,0,-0.5);
        _iscrtaj_ravan();
    glPopMatrix();

    // zadnja
    glPushMatrix();
         glRotatef(-90, 0, 0, 1);
         glTranslatef(0,0,0.5);
        _iscrtaj_ravan();
    glPopMatrix();

    // desna
    glPushMatrix();
         glRotatef(90, 0, 1, 0);
         glTranslatef(0,0,0.5);
        _iscrtaj_ravan();
    glPopMatrix();

    // leva
    glPushMatrix();
         glRotatef(-90, 0, 1, 0);
         glTranslatef(0,0,0.5);
        _iscrtaj_ravan();
    glPopMatrix();

    // donja 
    glPushMatrix();
         glRotatef(90, 1, 0, 0);
         glTranslatef(0,0,0.5);
        _iscrtaj_ravan();
    glPopMatrix();

    // gornja 
    glPushMatrix();
         glRotatef(-90, 1, 0, 0);
         glTranslatef(0,0,0.5);
        _iscrtaj_ravan();
    glPopMatrix();
}

void iscrtaj_nebo(unsigned tex_id) {
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        
        glScalef(800,800,1000);
        _iscrtaj_kocku();

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

static void _stabilizacija_letelice(double *rotacija) 
{
        /* Dovodjenje letelice u horizontalan polozaj */
       if (fabs(*rotacija) > 0.001) {
            double param = fabs(*rotacija/10.0);
           *rotacija = *rotacija < 0 ? *rotacija + param : *rotacija - param;
       }
}

void procesuiraj_poziciju(double * pozicija_letelice, double * rotacija, double zeljena_pozicija) 
{
    if (*pozicija_letelice < zeljena_pozicija) {
        if (*rotacija < 0.0) 
            _stabilizacija_letelice(rotacija);
        //else {
            if (*rotacija < 1.0) *rotacija+=0.1; 
            //
            *pozicija_letelice += 0.1 * fabs(*rotacija);
            if (*pozicija_letelice > zeljena_pozicija) {
                *pozicija_letelice = zeljena_pozicija;
                _stabilizacija_letelice(rotacija);
            }
        //}
    } else if (*pozicija_letelice > zeljena_pozicija) {
        if (*rotacija > 0.0) 
            _stabilizacija_letelice(rotacija);
        //else {
            if (*rotacija > -1.0) *rotacija-=0.1; 
            //
            *pozicija_letelice -= 0.08 * fabs(*rotacija);
            if (*pozicija_letelice < zeljena_pozicija) {
                *pozicija_letelice = zeljena_pozicija;
                _stabilizacija_letelice(rotacija);
            }
        //}
    } else {
        _stabilizacija_letelice(rotacija);
    }
}

void skreni_levo(double * zeljena_pozicija, double min_poz) 
{
    if (*zeljena_pozicija <= min_poz)
        return;
    *zeljena_pozicija -= 1.0;
}

void skreni_desno(double * zeljena_pozicija, double max_poz) 
{
    if (*zeljena_pozicija >= max_poz)
        return;
    *zeljena_pozicija += 1.0;
}

static void _iscrtaj_standardnu_prepreku() {
    glPushMatrix();

        glEnable(GL_LIGHTING);
        static GLfloat material_ambient[] = { 0.0, 0.0, 0.0, 1 };
        static GLfloat material_diffuse[] = { 0.4, 0.0, 0.4, 1 };
        static GLfloat material_specular[] = { 0.1, 0.1, 0.1, 1 };
        static GLfloat shininess[] = { 30 };
        static GLfloat material_emission[] = { 0.3, 0.1, 0.3, 1 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

        glutSolidCube(1);
        glDisable(GL_LIGHTING);

        #if DEBUG
            iscrtaj_koordinatne_ose();
            iscrtaj_mrezu_oko_objekta();
        #endif

        glPushMatrix();
            glScalef(0.8, 1.0, 0.8);
            glColor3f(1,0.8,1);
            glTranslatef(0, 0.01, 0);
            glutSolidCube(1);
        glPopMatrix();
        
    glPopMatrix();
}

static void _iscrtaj_prepreku(char tip_prepreke) {
    switch (tip_prepreke) {
        case '#' : _iscrtaj_standardnu_prepreku();
           break;

    }
}

void iscrtaj_prepreke(double param_predj_puta, char ** matrica_nivoa, int br_redova, int br_prepr_u_redu, int max_vidjlivih_redova, double sirina_linije_staze)
{
    glPushMatrix();
        glTranslatef(0, -0.6, 0);
        glScalef(sirina_linije_staze, 0.4, sirina_linije_staze);
        glTranslatef(-br_prepr_u_redu/2, 0, param_predj_puta - floor(param_predj_puta));

        int m =  abs(4 > (int) param_predj_puta ? (int) param_predj_puta : -4);
        glTranslatef(0,0,m);

        int vidljivost = nearbyint(param_predj_puta) + max_vidjlivih_redova;
        int n = br_redova < vidljivost ? br_redova : vidljivost;
        for (int i = (int) param_predj_puta - m; i < n; i++) {
            int j;
            for (j = 0; j < br_prepr_u_redu; j++) {
                _iscrtaj_prepreku(matrica_nivoa[i][j]);
                glTranslatef(1,0,0);
            }
            glTranslatef(-j, 0, -1);
        }
    glPopMatrix();
}


char ** alociraj_i_ucitaj_nivo(char * putanja, int * br_redova, int * br_prepr_u_redu) {
    FILE * f;
    f = fopen(putanja, "r");
    if (f == NULL) {
        perror("Greska pri otvaranju fajla");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_IN_LINE];

    fscanf(f, "%d\n", br_redova);
    fscanf(f, "%d\n", br_prepr_u_redu);

    char ** matrica_nivoa = NULL;
    matrica_nivoa = (char **) malloc(sizeof(char*) * (*br_redova)); 
    if (*matrica_nivoa == NULL) {
       fprintf(stderr, "Greska pri alokaciji\n");
       exit(EXIT_FAILURE);
    }

    int i = 0;
    while ( fgets(buf, 100, f) != NULL ) {
       matrica_nivoa[i] = NULL;
       matrica_nivoa[i] = (char *) malloc(sizeof(char) * (*br_prepr_u_redu)); 
       if (matrica_nivoa[i] == NULL) {
                dealociraj_i_obrisi_nivo(matrica_nivoa, i);
                fprintf(stderr, "Greska pri alokaciji\n");
                exit(EXIT_FAILURE);
       } else {
            for (int j = 0; j < *br_prepr_u_redu; j++) {
               matrica_nivoa[i][j] = buf[j*2];
           }
        }
        i++;
    }

    return matrica_nivoa;
}

void dealociraj_i_obrisi_nivo(char ** matrica_nivoa, int br_redova) 
{
    for (int i=0; i < br_redova; i++) {
                free(matrica_nivoa[i]);
    }
    free(matrica_nivoa);
}

bool proveri_koliziju(double tr_poz, double param_predj_puta, double min_poz, char ** matrica_nivoa, int br_redova) 
{
    int i = nearbyint(param_predj_puta);
    int j = nearbyint(tr_poz+fabs(min_poz));
    if (i < br_redova) 
        if (matrica_nivoa[i][j] == '#')
            return true;

    return false;
}

void ispisi_tekst(char * tekst, int x, int y, float r, float g, float b, int sirina_ekrana, int duzina_ekrana)
{
	glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(r, g, b, 1.0 );
	glOrtho(0, sirina_ekrana, 0, duzina_ekrana, -1, 1);

	glRasterPos2f(x, y);

	int len= strlen(tekst);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tekst[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void zaustavi_igru() 
{
    glob_prom.igra_aktivna = false;
}

void restartuj_igru() 
{
    glob_prom.igra_aktivna = false;
    glob_prom.param_predj_puta = 0.0;
    glob_prom.pozicija = 0.0;
    glob_prom.rotacija = 0.0;
    glob_prom.zeljena_pozicija = 0.0;
}
