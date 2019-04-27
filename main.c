#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "funkcije.h"
#include "image.h"
    
/* ASCII kod za ESC key */
#define ESC_KEY (27)

/* OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// GLOBALNE PROMENLJIVE
_podaci glob_prom = {
    .tex_nebo_id = 0,

    .sirina_ekrana = 0,
    .duzina_ekrana = 0,
    .tekst_na_centru_ekrana = "Da zapocnes igru pritisni 'G'",
    .tekst_u_donjem_levom_uglu = "",
    .tekst_u_donjem_desnom_uglu = "Score: 0",
    .view_distance = 100,
    .timer_id = 0,
    .timer_interval = 17, // oko 60 fps,
    .igra_aktivna = false,
    .param_predj_puta = 0.0,

    /* letelica */
    .pozicija = 0.0,
    .zeljena_pozicija = 0.0,
    .rotacija = 0.0,
    .min_poz = -2,
    .max_poz = 2,
    .sirina_linije_staze = 2.0,

    /* matrica nivoa */
    .matrica_nivoa = NULL,
    .br_redova = 0,
    .br_prepr_u_redu = 0
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

    /* Pokretanje preko celog ekrana */
    glutFullScreen();

	/* Registruju se callback funkcije */
	/* Funkcija koja se poziva za iscrtavanje */
	glutDisplayFunc(on_display);
	/* F-ja koja se poziva kada dodje do promene vel. prozora */
	glutReshapeFunc(on_reshape);
	/* F-ja koja se poziva na pritisak tastera na tastaturi */
	glutKeyboardFunc(on_key_press);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glutSpecialFunc(on_special_key_press);

    glutSetCursor(GLUT_CURSOR_NONE);

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

    /* Uvitavnje tekstura */
    Image * nebo = image_init(1024, 1024);
    image_read(nebo, "nebo.bmp");
    glob_prom.tex_nebo_id = napravi_teksturu(nebo);
    image_done(nebo);

    /* Seed za random brojeve */
    srand(time(NULL));

    /* Ucitavanje nivoa */

    glob_prom.matrica_nivoa = alociraj_i_ucitaj_nivo("nivo1.txt", &glob_prom.br_redova, &glob_prom.br_prepr_u_redu);

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
	gluLookAt(glob_prom.pozicija*glob_prom.sirina_linije_staze, 1.8, 3, glob_prom.pozicija*glob_prom.sirina_linije_staze, 0, -3, 0, 1, 0);

    iscrtaj_nebo(glob_prom.tex_nebo_id);
    iscrtaj_stazu(glob_prom.sirina_linije_staze);
    iscrtaj_letelicu(glob_prom.pozicija, glob_prom.rotacija, glob_prom.sirina_linije_staze);
    iscrtaj_prepreke(glob_prom.param_predj_puta, glob_prom.matrica_nivoa, glob_prom.br_redova, glob_prom.br_prepr_u_redu, glob_prom.view_distance, glob_prom.sirina_linije_staze);

    // Ako ima kolizije sa preprekom
    if (proveri_koliziju(glob_prom.pozicija, glob_prom.param_predj_puta, glob_prom.min_poz, glob_prom.matrica_nivoa, glob_prom.br_redova)) {
            zaustavi_igru();
            glutPostRedisplay();
            strcpy(glob_prom.tekst_na_centru_ekrana, "Kraj igre, pritisnite 'R' za restart");
    }

    ispisi_tekst(glob_prom.tekst_na_centru_ekrana, glob_prom.sirina_ekrana/2 - strlen(glob_prom.tekst_na_centru_ekrana)*5, glob_prom.duzina_ekrana/2 - 10, 1, 0, 1, glob_prom.sirina_ekrana, glob_prom.duzina_ekrana); 

    sprintf(glob_prom.tekst_u_donjem_levom_uglu, "Score: %d", (int) glob_prom.param_predj_puta * 10);
    ispisi_tekst(glob_prom.tekst_u_donjem_levom_uglu, 10, 24, 1, 0, 1, glob_prom.sirina_ekrana, glob_prom.duzina_ekrana); 

    //sprintf(glob_prom.tekst_u_donjem_desnom_uglu, "Score: %d", (int) glob_prom.param_predj_puta * 10);
    //ispisi_tekst(glob_prom.tekst_u_donjem_desnom_uglu, glob_prom.sirina_ekrana-140, 24, 1, 0, 1, glob_prom.sirina_ekrana, glob_prom.duzina_ekrana); 

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
    glob_prom.sirina_ekrana = width;
    glob_prom.duzina_ekrana = height;
}

static void on_key_press(unsigned char key, int x, int y)
{
	switch (key) {
		case ESC_KEY: 
            dealociraj_i_obrisi_nivo(glob_prom.matrica_nivoa, glob_prom.br_redova);
			exit(EXIT_SUCCESS);
			break;
        case 'g':
        case 'G':
            if (glob_prom.igra_aktivna == false && glob_prom.param_predj_puta < glob_prom.br_redova) {
                glob_prom.igra_aktivna = true;
                glutTimerFunc(glob_prom.timer_interval, on_timer, glob_prom.timer_id); 
                strcpy(glob_prom.tekst_na_centru_ekrana, "");
            }
            break;
        case 'r': 
        case 'R':
            restartuj_igru();
            glutPostRedisplay();
            strcpy(glob_prom.tekst_na_centru_ekrana, "Da zapocnes igru pritisni 'G'");
            break;
	}
}

static void on_special_key_press(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT : 
            if (glob_prom.igra_aktivna)
                skreni_levo(&glob_prom.zeljena_pozicija, glob_prom.min_poz);
            break;
        case GLUT_KEY_RIGHT: 
            if (glob_prom.igra_aktivna)
                skreni_desno(&glob_prom.zeljena_pozicija, glob_prom.max_poz);
            break;
        case GLUT_KEY_UP: 
            break;
        case GLUT_KEY_DOWN:
            break;
    }
}

static void on_timer(int timer_id) {
    if (timer_id != glob_prom.timer_id || !glob_prom.igra_aktivna)
        return;
    procesuiraj_poziciju(&glob_prom.pozicija, &glob_prom.rotacija, glob_prom.zeljena_pozicija);

    glob_prom.param_predj_puta += 0.125 + 0.00001*glob_prom.param_predj_puta;

    if (glob_prom.br_redova < glob_prom.param_predj_puta) {
        sprintf(glob_prom.tekst_na_centru_ekrana, "Presli ste igru, osvojeno: %d poena", (int)glob_prom.param_predj_puta*10);
        zaustavi_igru();
    }

    glutPostRedisplay();
    glutTimerFunc(glob_prom.timer_interval, on_timer, glob_prom.timer_id);
}
