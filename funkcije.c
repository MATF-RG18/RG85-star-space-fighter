#include "funkcije.h"
#include "image.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
		glColor3f(0,0,1);
		glVertex3f(0,-1,0);
		glVertex3f(0,1,0);

		/* Z osa */
		glColor3f(0,1,0);
		glVertex3f(0,0,-1);
		glVertex3f(0,0,1);

	glEnd();
}

void iscrtaj_mrezu_oko_objekta() 
{
    glDisable(GL_LIGHTING);
    glutWireCube(1);
}

/* Funkcije za iscrtavanje game objekata */
void iscrtaj_letelicu() 
{
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
}

void iscrtaj_prepreku(double kompleksnost) 
{
    int br_celija = 9;
    int max_celija = floor(kompleksnost*br_celija);
    int velicina_prepreke = rand() % (max_celija+1);

    int i=rand()%(max_celija+1);
    bool niz[max_celija]; 

    for (int i=0; i<max_celija; i++)
        niz[i] = false;

    //printf("%d ", velicina_prepreke);
    //fflush(stdout);

    while (velicina_prepreke--) {
        niz[i] = true;
        i = (i+1)%(max_celija+1);
    }

    /* Predefinisane komponente za gradjenje materijala */
    static GLfloat material_ambient[] = { 0.1, 0.1, 0.1, 1 };
    static GLfloat material_diffuse[] = { 0.8, 0.2, 0.5, 1 };
    static GLfloat material_specular[] = { 0.07, 0.07, 0.07, 1 };
    static GLfloat shininess[] = { 5 };
    static GLfloat material_emission[] = { 0.1, 0.1, 0.1, 0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);


    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (niz[i+i*j] == true) {
                    glPushMatrix();
                        glScalef(1.5, 1, 1);
                        glTranslatef(j, i, 0);
                        glutSolidCube(1);
                    glPopMatrix();
            }
        }
    }
}

void iscrtaj_stazu() {

    glDisable(GL_LIGHTING);

    glColor3f(0.02,0,0.08);
    glPushMatrix();
        glScalef(2,1,1);
        glBegin(GL_QUADS);
                glVertex3f(-2.5, -1.01, -1000);
                glVertex3f(2.5, -1.01, -1000);
                glVertex3f(2.5, -1.01, 10);
                glVertex3f(-2.5, -1.01, 10);
        glEnd();
    glPopMatrix();

    glColor3f(255,0,255);
    glLineWidth(2);
    glPushMatrix();
        glScalef(2,1,1);
        glBegin(GL_LINES);
            for (int i=-2; i<= 3; i++) {
                glVertex3f(i-0.5, -1, -1500);
                glVertex3f(i-0.5, -1, 10);
            }
        glEnd();
    glPopMatrix();
}


unsigned napravi_teksturu(Image *img) {
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


void _iscrtaj_ravan() {
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


void _iscrtaj_kocku() {
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

void iscrtaj_nebo() {
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, glob_prom.nebo_tex_id);
        
        glScalef(800,800,1000);
        _iscrtaj_kocku();

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
