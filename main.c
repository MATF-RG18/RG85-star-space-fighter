#include <stdio.h>
#include <stdlib.h>
#include "funkcije.h"
#include <time.h>
#include "image.h"

/* ASCII kod za ESC key */
#define ESC_KEY (27)

/* Ukljucivanje debug moda 0 -> 1 */
/*#define DEBUG (1)*/

/* OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// GLOBALNE PROMENLJIVE
_podaci glob_prom = {
    .nebo=NULL,
    .nebo_tex_id = 0,
    .timer_id = 0,
    .timer_interval = 10, // 100fps,

    /* letelica */
    .pozicija = 0.0,
    .zeljena_pozicija = 0.0,
    .rotacija = 0.0
}; 

static void on_display();
static void on_reshape(int width, int height);
static void on_key_press(unsigned char key, int x, int y);
static void on_special_key_press(int key, int x, int y);
static void on_timer(int timer_id);

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
	glutSpecialFunc(on_special_key_press);

    /* Timer */
   glutTimerFunc(glob_prom.timer_interval, on_timer, glob_prom.timer_id);

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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);

    glob_prom.nebo = image_init(800, 600);
    image_read(glob_prom.nebo, "nebo.bmp");
    glob_prom.nebo_tex_id = napravi_teksturu(glob_prom.nebo);

    /* Seed za random brojeve */
    srand(time(NULL));
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

    /* Postavljanje kamere */
	/*gluLookAt(2, 1, -2, 0, 0, 0, 0, 1, 0);*/
	// gluLookAt(2, 0, 0, 0, 0, 0, 0, 1, 0);
	 gluLookAt(0, 1.8, 3, 0, 0, -3, 0, 1, 0);
	// gluLookAt(0, 0, 1, 0, 0, -3, 0, 1, 0);
	//  gluLookAt(0, 4, 0, 0, 0, 0, 0, 0, -1);

    #if DEBUG
        iscrtaj_koordinatne_ose();
        iscrtaj_mrezu_oko_objekta();
    #endif
        iscrtaj_stazu();
        iscrtaj_letelicu(glob_prom.pozicija, glob_prom.rotacija);
        iscrtaj_nebo();

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
	gluPerspective(60, (float) width / height, 0.01, 1500);
}

static void on_key_press(unsigned char key, int x, int y)
{
	switch (key) {
		case ESC_KEY: 
            image_done(glob_prom.nebo);
			exit(EXIT_SUCCESS);
			break;
	}
}

static void on_special_key_press(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT : 
            printf("Left key is pressed\n");
            break;
        case GLUT_KEY_RIGHT: 
            printf("Right key is pressed\n");
            break;
        case GLUT_KEY_UP: 
            printf("Up key is pressed\n");
            break;
        case GLUT_KEY_DOWN:
            printf("Down key is pressed\n");
            break;
    }
}

static void on_timer(int timer_id) {
    if (timer_id != glob_prom.timer_id)
        return;
    procesuiraj_poziciju(&glob_prom.pozicija, &glob_prom.rotacija, 2.0);

    glutPostRedisplay();
    glutTimerFunc(glob_prom.timer_interval, on_timer, glob_prom.timer_id);
}
