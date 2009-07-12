#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
int a=0;
void display(void)
{
   GLfloat white[3] = { 1.0, 0.0, 1.0 };

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3fv(white);
	
   glRotatef(0,0,1,a);
  //glTranslatef(10,10,0);
  /*
   glRasterPos2i(20, 60);
   drawString("THE QUICK BROWN FOX JUMPS");
   glRasterPos3i(20, 40,1);
   drawString("OVER A LAZY DOG");
   */
  glPushMatrix();
  glScalef(2,2,2);
  glTranslatef(20,20,0);
   glutWireSphere(20,20,16);  
   glPopMatrix();
   glFlush ();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho (0.0, w, 0.0, h, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
	 case 'a': a+=5;// glRotatef(0,0,1,25);
	 glutPostRedisplay();
	 break;
   }
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(300, 100);
   glutInitWindowPosition (100, 100);
   glutCreateWindow(argv[0]);
   defaultfont();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}

