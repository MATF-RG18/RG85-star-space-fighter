#include <stdio.h>
#include <stdlib.h>
#include "funkcije.h"

/* ASCII kod za ESC key */
#define ESC_KEY (27)

/* Ukljucivanje debug moda 0 -> 1 */
#define DEBUG (1)

/* OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static void on_display();
static void on_reshape(int width, int height);
static void on_key_press(unsigned char key, int x, int y);

int main(int argc, char * argv[])
{
	/* Inicijalizacija GLUT biblioteke */
	glutInit(&argc, argv);

	/* Inicijalizacija moda za prikaz */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	/* Postavljanje velicine prozora */
	glutInitWindowSize(800, 600);

	/* Pozocija prozora - koord. pocetak je cosak gore levo */
	glutInitWindowPosition(100, 100);

	/* Kreiranje prozora */
	glutCreateWindow("Titl programa");

	/* Registruju se callback funkcije */
	/* Funkcija koja se poziva za iscrtavanje */
	glutDisplayFunc(on_display);
	/* F-ja koja se poziva kada dodje do promene vel. prozora */
	glutReshapeFunc(on_reshape);
	/* F-ja koja se poziva na pritisak tastera na tastaturi */
	glutKeyboardFunc(on_key_press);

	/* Uklucivanje osvetljenja */
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };

    /* Ambijentalno osvetljenje scene. */
    GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1 };

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = { 1, 1, 1, 0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    /* Normalizacija normala */
	glEnable(GL_NORMALIZE);

    /* Pozicionira se svijetlo. */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	/* Inicijalizacija OpenGL biblioteke */
	/* Default boja u frame buffer-a kojom se cisti ekran */
	glClearColor(0, 0, 0, 0);

	/* Ukljucivanje kontrole dubine (z-buffer)*/
	glEnable(GL_DEPTH_TEST);

	/* Ulazak u glavnu petlju programa */
	glutMainLoop();

	return 0;
}

static void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Ovde se iscrtavaju sva cuda */

    /* Pozicionira se svijetlo. */
    static GLfloat light_position[] = {0, 0.5, -2.5, 0 }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	/* Ucitavanje MODELVIEW matrice */
	glMatrixMode(GL_MODELVIEW);
	/* Postavljanje MODELVIEW matrice na identity */
	glLoadIdentity();
	/* Gledaj u koordinatni pocetak sa poz. (3,2,0) */
	/*gluLookAt(2, 1, 0, 0, 0, 0, 0, 1, 0);*/
	gluLookAt(0, 2.2, 3, 0, 0, -3, 0, 1, 0);

    #if DEBUG
        iscrtaj_koordinatne_ose();
        iscrtaj_mrezu();
    #endif
        iscrtaj_letelicu();

	glutSwapBuffers();
}

static void on_reshape(int width, int height)
{
	/* Postavlja se viewport (velicina vidljive povrsine) */
	glViewport(0, 0, width, height);

	/* Postavljaju se parametri projekcije */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Ugao vidljivosti, aspect-ratio i near i far clip plane */
	gluPerspective(60, (float) width / height, 0.1, 1500);
}

static void on_key_press(unsigned char key, int x, int y)
{
	switch (key) {
		case ESC_KEY: 
			exit(EXIT_SUCCESS);
			break;
	}
}

