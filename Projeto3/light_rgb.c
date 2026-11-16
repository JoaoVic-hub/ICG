#include <GL/glut.h>
#include <stdlib.h>

static int spin = 0;

void init(void)
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat red[]   = { 1.0, 0.0, 0.0, 1.0 };
   GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
   GLfloat blue[]  = { 0.0, 0.0, 1.0, 1.0 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
   glLightfv(GL_LIGHT0, GL_SPECULAR, red);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
   glLightfv(GL_LIGHT1, GL_SPECULAR, green);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
   glLightfv(GL_LIGHT2, GL_SPECULAR, blue);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);
   glEnable(GL_DEPTH_TEST);
}

void placeLight (GLenum light, GLfloat x, GLfloat y, GLfloat z,
                 GLdouble ax, GLdouble ay, GLdouble az,
                 GLfloat r, GLfloat g, GLfloat b)
{
   GLfloat position[] = { x, y, z, 1.0 };

   glPushMatrix ();
   glRotated ((GLdouble) spin, ax, ay, az);
   glLightfv (light, GL_POSITION, position);

   glTranslatef (x, y, z);
   glDisable (GL_LIGHTING);
   glColor3f (r, g, b);
   glutWireCube (0.1);
   glEnable (GL_LIGHTING);
   glPopMatrix ();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   gluLookAt (0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   placeLight (GL_LIGHT0,  0.0, 1.5, 1.5,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0);
   placeLight (GL_LIGHT1,  1.5, 0.0, 1.5,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0);
   placeLight (GL_LIGHT2, -1.5, 0.0, 1.5,  0.0, 0.0, 1.0,  0.0, 0.0, 1.0);

   glutSolidSphere (1.0, 40, 32);
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

void rotateLights (void)
{
   spin = (spin + 30) % 360;
   glutPostRedisplay();
}

void toggle (GLenum light)
{
   if (glIsEnabled(light))
      glDisable(light);
   else
      glEnable(light);
   glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      rotateLights();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'r':
         toggle(GL_LIGHT0);
         break;
      case 'g':
         toggle(GL_LIGHT1);
         break;
      case 'b':
         toggle(GL_LIGHT2);
         break;
      case ' ':
         rotateLights();
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
