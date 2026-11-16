#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static GLenum shade = GL_SMOOTH;
static int slices = 20, stacks = 16;

void init(void)
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

   glClearColor (0.0, 0.0, 0.0, 0.0);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   char info[100];
   int start, elapsed;

   glShadeModel (shade);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   start = glutGet(GLUT_ELAPSED_TIME);
   glutSolidSphere (1.0, slices, stacks);
   glFinish ();
   elapsed = glutGet(GLUT_ELAPSED_TIME) - start;

   sprintf(info, "%s  slices=%d  stacks=%d  tempo=%d ms",
           shade == GL_FLAT ? "FLAT" : "SMOOTH", slices, stacks, elapsed);
   glutSetWindowTitle(info);
   printf("%s\n", info);
   fflush(stdout);
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
      case 'f':
         shade = (shade == GL_FLAT) ? GL_SMOOTH : GL_FLAT;
         break;
      case 't':
         stacks *= 2;
         break;
      case 'T':
         if (stacks > 2) stacks /= 2;
         break;
      case 's':
         slices *= 2;
         break;
      case 'S':
         if (slices > 3) slices /= 2;
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
