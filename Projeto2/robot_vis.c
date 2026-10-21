#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0;
static int depthOn = 1;
static GLfloat camY = 0.0, camZ = 5.0;

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable (GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix();
   glTranslatef (-1.0, 0.0, 0.0);
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.4, 1.0);
   glColor3f (1.0, 0.0, 0.0);
   glutSolidCube (1.0);
   glPopMatrix();

   glTranslatef (0.5, 0.0, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 1.0, 0.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.4, 1.0);
   glColor3f (0.0, 0.0, 1.0);
   glutSolidCube (1.0);
   glPopMatrix();

   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 's':
         shoulder = (shoulder + 5) % 360;
         break;
      case 'S':
         shoulder = (shoulder - 5) % 360;
         break;
      case 'e':
         elbow = (elbow + 5) % 360;
         break;
      case 'E':
         elbow = (elbow - 5) % 360;
         break;
      case 'z':
         depthOn = !depthOn;
         if (depthOn)
            glEnable (GL_DEPTH_TEST);
         else
            glDisable (GL_DEPTH_TEST);
         printf ("z-buffer %s\n", depthOn ? "ligado" : "desligado");
         break;
      case 'a':
         if (camZ > 1.5) camZ -= 0.5;
         break;
      case 'A':
         camZ += 0.5;
         break;
      case 'c':
         camY += 0.5;
         break;
      case 'C':
         camY -= 0.5;
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
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
