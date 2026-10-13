/*
 * robot.c - Exercicio 2.1
 *
 * Partindo do robot.c do Red Book (que tinha so ombro e cotovelo), o braco
 * agora tem base, ombro, cotovelo, punho e garra com dois dedos, como na
 * figura 1 do roteiro.
 *
 * O modelo e montado a partir da base: a cada parte a origem e levada ate
 * a junta (glTranslatef), gira (glRotatef) e depois vai ate o centro da
 * parte para desenhar, igual ao robot.c original.
 *
 * Cada parte e um glutSolidCube colorido com um glutWireCube branco.
 *
 * Teclas (minuscula gira para um lado, maiuscula para o outro):
 *   b / B - base (gira em torno do eixo vertical)
 *   s / S - ombro (shoulder)
 *   e / E - cotovelo (elbow)
 *   w / W - punho (wrist), flexao
 *   t / T - punho, giro em torno do antebraco
 *   f / F - abre / fecha a garra (dedos)
 *   ESC   - sair
 */
#include <GL/glut.h>
#include <stdlib.h>

static int base = 0, shoulder = -20, elbow = -40;
static int wrist = -30, wristTwist = 0, finger = 15;

/* desenha um cubo solido colorido, escalado, com as arestas brancas */
void drawPart (GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b)
{
   glPushMatrix();
   glScalef (sx, sy, sz);
   glColor3f (r, g, b);
   glutSolidCube (1.0);
   glColor3f (1.0, 1.0, 1.0);
   glutWireCube (1.01);   /* um pouco maior para nao sumir dentro das faces */
   glPopMatrix();
}

/* side = 1 para o dedo da direita e -1 para o da esquerda */
void drawFinger (int side)
{
   glPushMatrix();
   glTranslatef (side * 0.3, 0.0, 0.0);
   glRotatef ((GLfloat) -side * finger, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.4, 0.0);
   drawPart (0.1, 0.8, 0.2, 0.9, 0.3, 0.3);
   glPopMatrix();
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable (GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();

   /* base */
   glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
   drawPart (3.0, 0.5, 2.0, 0.5, 0.5, 0.5);

   /* ombro + braco */
   glTranslatef (0.0, 0.25, 0.0);
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 1.0, 0.0);
   drawPart (0.4, 2.0, 0.4, 0.2, 0.4, 0.9);

   /* cotovelo + antebraco */
   glTranslatef (0.0, 1.0, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.75, 0.0);
   drawPart (0.35, 1.5, 0.35, 0.2, 0.8, 0.3);

   /* punho + mao: gira em torno do antebraco (y) e flexiona (z) */
   glTranslatef (0.0, 0.75, 0.0);
   glRotatef ((GLfloat) wristTwist, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) wrist, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.15, 0.0);
   drawPart (0.8, 0.3, 0.3, 0.9, 0.8, 0.2);

   /* garra: cada dedo salva e restaura a matriz (push/pop em drawFinger) */
   glTranslatef (0.0, 0.15, 0.0);
   drawFinger (1);
   drawFinger (-1);

   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   /* camera um pouco acima e de lado para dar nocao de profundidade */
   gluLookAt (4.0, 5.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'b':
         base = (base + 5) % 360;
         break;
      case 'B':
         base = (base - 5) % 360;
         break;
      case 's':
         if (shoulder < 90) shoulder += 5;
         break;
      case 'S':
         if (shoulder > -90) shoulder -= 5;
         break;
      case 'e':
         elbow = (elbow + 5) % 360;
         break;
      case 'E':
         elbow = (elbow - 5) % 360;
         break;
      case 'w':
         wrist = (wrist + 5) % 360;
         break;
      case 'W':
         wrist = (wrist - 5) % 360;
         break;
      case 't':
         wristTwist = (wristTwist + 5) % 360;
         break;
      case 'T':
         wristTwist = (wristTwist - 5) % 360;
         break;
      case 'f':
         if (finger < 45) finger += 5;
         break;
      case 'F':
         if (finger > -15) finger -= 5;
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
