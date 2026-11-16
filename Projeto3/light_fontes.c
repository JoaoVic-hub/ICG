#include <GL/glut.h>
#include <stdlib.h>

static int ambientOn = 0, diffuseOn = 1, specularOn = 1;
static int red = 0;
static GLfloat light0_position[] = { 1.0, 1.0, 1.0, 0.0 };

void init(void)
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };

   GLfloat light1_ambient[]  = { 0.0, 0.2, 0.0, 1.0 };
   GLfloat light1_diffuse[]  = { 0.0, 1.0, 0.0, 1.0 };
   GLfloat light1_specular[] = { 0.0, 1.0, 0.0, 1.0 };
   GLfloat light1_position[] = { -2.0, 0.0, 1.0, 1.0 };
   GLfloat spot_direction[]  = { 1.0, 0.0, -0.5 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
   glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
   glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);

   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void setLight0 (void)
{
   GLfloat g = red ? 0.0 : 1.0;
   GLfloat ambient[]  = { 0.4 * ambientOn, 0.4 * ambientOn * g, 0.4 * ambientOn * g, 1.0 };
   GLfloat diffuse[]  = { diffuseOn, diffuseOn * g, diffuseOn * g, 1.0 };
   GLfloat specular[] = { specularOn, specularOn * g, specularOn * g, 1.0 };

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   setLight0();
   glutSolidSphere (1.0, 40, 32);
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'a':
         ambientOn = !ambientOn;
         break;
      case 'd':
         diffuseOn = !diffuseOn;
         break;
      case 'e':
         specularOn = !specularOn;
         break;
      case 'c':
         red = !red;
         break;
      case 'p':
         light0_position[3] = 1.0 - light0_position[3];
         break;
      case '1':
         if (glIsEnabled(GL_LIGHT1))
            glDisable(GL_LIGHT1);
         else
            glEnable(GL_LIGHT1);
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
