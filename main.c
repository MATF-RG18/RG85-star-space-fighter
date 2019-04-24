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

	/* Inicijalizacija OpenGL biblioteke */
	/* Default boja u frame buffer-a kojom se cisti ekran */
	glClearColor(1, 1, 1, 0);

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

	/* Ucitavanje MODELVIEW matrice */
	glMatrixMode(GL_MODELVIEW);
	/* Postavljanje MODELVIEW matrice na identity */
	glLoadIdentity();
	/* Gledaj u koordinatni pocetak sa poz. (3,2,0) */
	gluLookAt(3, 2, 0, 0, 0, 0, 0, 1, 0);

	/* Iscrtaj plav cajnik u koordinatnom pocetku */
	glColor3f(0, 0, 1);

    #if DEBUG
        iscrtaj_koordinatne_ose();
        iscrtaj_mrezu();
    #endif

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

