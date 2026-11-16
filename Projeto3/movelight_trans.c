#include <GL/glut.h>
#include <stdlib.h>

static int rotating = 1;
static int spin = 0;
static GLdouble lightZ = 2.0;

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
   gluLookAt (4.0, 2.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix ();
   if (rotating) {
      glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
      glTranslated (0.0, 0.0, 1.5);
   } else {
      glTranslated (0.0, 0.0, lightZ);
   }
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   glDisable (GL_LIGHTING);
   glColor3f (0.0, 1.0, 1.0);
   glutWireCube (0.1);
   glEnable (GL_LIGHTING);
   glPopMatrix ();

   glutSolidTorus (0.275, 0.85, 16, 30);
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

void mouse(int button, int state, int x, int y)
{
   if (state != GLUT_DOWN)
      return;

   if (rotating && button == GLUT_LEFT_BUTTON)
      spin = (spin + 30) % 360;
   else if (!rotating && button == GLUT_LEFT_BUTTON && lightZ > -2.0)
      lightZ -= 0.25;
   else if (!rotating && button == GLUT_RIGHT_BUTTON && lightZ < 2.0)
      lightZ += 0.25;

   glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'm':
         rotating = !rotating;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
   }
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
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
