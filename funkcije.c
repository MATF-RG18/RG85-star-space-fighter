#include "funkcije.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void iscrtaj_koordinatne_ose()
{
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
    glColor3f(0,1,0);
    glutWireCube(1);
}
