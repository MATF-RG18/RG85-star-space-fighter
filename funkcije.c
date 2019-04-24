#include "funkcije.h"

#include <stdio.h>
#include <stdbool.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

void iscrtaj_mrezu() 
{
    glDisable(GL_LIGHTING);
    glutWireCube(1);
}

void _iscrtaj_valjak(double r_donje_baze, double r_gornje_baze, double visina) 
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

    glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(0,0,-0.5);
        gluCylinder(quad, r_donje_baze, r_gornje_baze, visina, 40, 40);
    glPopMatrix();
}


void iscrtaj_letelicu() 
{
    glEnable(GL_LIGHTING);

    /* Predefinisane komponente za gradjenje materijala */
    static GLfloat material_ambient[] = { 0.1, 0.1, 0.1, 1 };
    static GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1 };
    static GLfloat material_diffuse_lblue[] = { 0.0, 0.7, 0.1, 1 };
    static GLfloat material_diffuse_dark_gray[] = { 0.4, 0.4, 0.4, 1 };
    static GLfloat material_diffuse_red[] = { 0.8, 0.0, 0.0, 1 };
    static GLfloat material_emission_red[] = { 0.2, 0.1, 0.2, 1 };
    static GLfloat material_specular[] = { 0.07, 0.07, 0.07, 1 };
    static GLfloat shininess[] = { 5 };
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
    static GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1 };
    static GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1 };
    
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);

    /* Trup letelice */
    glPushMatrix();
        glScalef(0.4,0.08,1);
        glRotatef(90,1,0,0);
        _iscrtaj_valjak(1, 0.1, 1);
    glPopMatrix();

    /* Krila letelice */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
        glScalef(0.7,0.035,0.7);
        glTranslatef(0,0,0.2);
        glRotatef(90,1,0,0);
        _iscrtaj_valjak(1, 0.1, 1);
    glPopMatrix();

    /* Staklo na kabini */
    glPushMatrix();
        static GLdouble clip_plane[] = {0, 1, 0, 0.5};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_lblue);
        glScalef(1,0.4,1.3);
        glTranslatef(0, 0.04, -0.1);
        glutSolidSphere(0.1, 20, 20);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();


    /* Cevi mlaznjaka */
    glPushMatrix();
        glScalef(0.1, 0.1, 0.1);
        glTranslatef(0,0,5.5);
        glRotatef(90,1,0,0);

        static GLdouble clip_plane1[] = {0, 1, 0, 0.52};
        glClipPlane(GL_CLIP_PLANE1, clip_plane1);

        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
            glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
            glTranslatef(1.45,0,0);
            glEnable(GL_CLIP_PLANE1);
            _iscrtaj_valjak(0.8, 0.8, 2);
            glDisable(GL_CLIP_PLANE1);
            glPushMatrix();
                glTranslatef(0,0,0.2);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_red);
                glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_red);
                glutSolidSphere(0.6, 10, 10);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_dark_gray);
            glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
            glTranslatef(-1.45,0,0);
            glEnable(GL_CLIP_PLANE1);
            _iscrtaj_valjak(0.8, 0.8, 2);
            glDisable(GL_CLIP_PLANE1);
        
            glPushMatrix();
                glTranslatef(0,0,0.2);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_red);
                glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_red);
                glutSolidSphere(0.6, 10, 10);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}
