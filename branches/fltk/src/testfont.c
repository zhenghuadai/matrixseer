/*
 */

/*
 */
#include <GL/glut.h>
#include <stdlib.h>
#include "font.h"
static int year = 0, day = 0;
static int winw=500;
static int winh=500;
void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void displayfont()
{
   glMatrixMode (GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho (0.0, winw, 0.0, winh, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glRotatef ((GLfloat) day, 0.0, 0.0, 1.0);
	glRasterPos2i(20, 60);
	   drawString("THE QUICK BROWN FOX JUMPS");
     	 glRasterPos3i(20, 40,1);
         drawString("OVER A LAZY DOG");
  glPopMatrix(); 	 
   glMatrixMode (GL_PROJECTION);
  glPopMatrix(); 	 
   glMatrixMode(GL_MODELVIEW);
}
void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glPushMatrix();
   glutWireSphere(1.0, 20, 16);   /* draw sun */
   glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
   glTranslatef (2.0, 0.0, 0.0);
   glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */
   glPopMatrix();
   displayfont();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   winw=w;
   winh=h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'd':
         day = (day + 1) % 360;
         glutPostRedisplay();
         break;
      case 'D':
         day = (day - 10) % 360;
         glutPostRedisplay();
         break;
      case 'y':
         year = (year + 5) % 360;
         glutPostRedisplay();
         break;
      case 'Y':
         year = (year - 5) % 360;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   defaultfont();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}

