#include <GL/glut.h>
#include <stdlib.h>

static GLdouble camX = 0.0, camY = 0.0, camZ = 5.0;

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   glLightfv (GL_LIGHT0, GL_POSITION, position);
   gluLookAt (camX, camY, camZ, camX, camY, camZ - 5.0, 0.0, 1.0, 0.0);

   glutSolidTorus (0.275, 0.85, 16, 30);

   glPushMatrix ();
   glTranslated (0.0, 0.0, -3.0);
   glutSolidTorus (0.275, 0.85, 16, 30);
   glPopMatrix ();

   glPopMatrix ();
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'w':
         camZ -= 0.25;
         break;
      case 's':
         camZ += 0.25;
         break;
      case 'a':
         camX -= 0.25;
         break;
      case 'd':
         camX += 0.25;
         break;
      case 'q':
         camY += 0.25;
         break;
      case 'e':
         camY -= 0.25;
         break;
      case 27:
         exit(0);
         break;
      default:
         return;
   }
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
