/*
 * cube_vis.c - Exercicio 1 (oclusao)
 *
 * Feito a partir do cube.c do Red Book. Desenha um cubo vermelho na origem
 * e um cubo azul na mesma posicao (x, y), porem mais longe da camera.
 * O z-buffer NAO esta ligado, entao quem aparece na frente e sempre o
 * ultimo cubo desenhado.
 *
 * Teclas:
 *   s / S - move o cubo azul para a direita / esquerda
 *   i     - inverte a ordem em que os cubos sao desenhados
 *   ESC   - sair
 */
#include <GL/glut.h>
#include <stdlib.h>

static GLfloat blueX = 0.0;
static int redFirst = 1;   /* 1: vermelho e depois azul, 0: azul e depois vermelho */

void drawRed (void)
{
   glPushMatrix();
   glColor3f (1.0, 0.0, 0.0);
   glutSolidCube (1.0);
   glPopMatrix();
}

void drawBlue (void)
{
   glPushMatrix();
   glTranslatef (blueX, 0.0, -3.0);   /* mais afastado da camera */
   glColor3f (0.0, 0.0, 1.0);
   glutSolidCube (1.0);
   glPopMatrix();
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity ();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   if (redFirst) {
      drawRed();
      drawBlue();
   } else {
      drawBlue();
      drawRed();
   }

   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 's':
         blueX += 0.1;
         break;
      case 'S':
         blueX -= 0.1;
         break;
      case 'i':
         redFirst = !redFirst;
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
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
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
