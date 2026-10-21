/*
 * robot_pega.c - Exercicio 3 (pegando objetos)
 *
 * Usa o braco robotico da aula pratica 02 (base, ombro, cotovelo, punho e
 * garra). Na cena ha um chao, um cubo roxo (o objeto) e uma caixa marrom.
 *
 * Como funciona:
 *  - Depois de desenhar os dedos, a matriz atual esta no ponto entre os
 *    dedos (a "pegada"). Ela e guardada em gripMatrix com glGetFloatv.
 *  - Enquanto o objeto esta preso, ele e desenhado ali mesmo, dentro da
 *    hierarquia do braco, entao acompanha todos os movimentos.
 *  - Quando o objeto e solto, a gripMatrix daquele momento e copiada para
 *    objMatrix e o objeto passa a ser desenhado com glLoadMatrixf(objMatrix),
 *    ficando parado onde foi largado.
 *  - So da para pegar se a pegada estiver perto do centro do objeto. As duas
 *    matrizes incluem a mesma camera, entao basta comparar a translacao delas.
 *
 * Teclas (minuscula gira para um lado, maiuscula para o outro):
 *   b / B - base
 *   s / S - ombro (shoulder)
 *   e / E - cotovelo (elbow)
 *   w / W - punho (wrist), flexao
 *   t / T - punho, giro em torno do antebraco
 *   f / F - abre / fecha a garra
 *   p     - pega / solta o objeto
 *   r     - recomeca
 *   ESC   - sair
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int base = 0, shoulder = -20, elbow = -40;
static int wrist = -30, wristTwist = 0, finger = 15;

static int holding = 0;   /* 1 enquanto o objeto esta na garra */
static int dropped = 0;   /* 1 depois que o objeto foi solto pela garra */
static GLfloat gripMatrix[16];
static GLfloat objMatrix[16];

/* desenha um cubo solido colorido, escalado, com as arestas brancas */
void drawPart (GLfloat sx, GLfloat sy, GLfloat sz, GLfloat r, GLfloat g, GLfloat b)
{
   glPushMatrix();
   glScalef (sx, sy, sz);
   glColor3f (r, g, b);
   glutSolidCube (1.0);
   glColor3f (1.0, 1.0, 1.0);
   glutWireCube (1.01);
   glPopMatrix();
}

void drawFinger (int side)
{
   glPushMatrix();
   glTranslatef (side * 0.3, 0.0, 0.0);
   glRotatef ((GLfloat) -side * finger, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.4, 0.0);
   drawPart (0.1, 0.8, 0.2, 0.9, 0.3, 0.3);
   glPopMatrix();
}

void drawObject (void)
{
   drawPart (0.4, 0.4, 0.4, 0.7, 0.2, 0.8);
}

void drawFloor (void)
{
   glPushMatrix();
   glTranslatef (0.0, -0.3, 0.0);
   glScalef (10.0, 0.1, 10.0);
   glColor3f (0.25, 0.25, 0.25);
   glutSolidCube (1.0);
   glPopMatrix();
}

/* caixa aberta em cima: fundo e quatro paredes, centrada em (0, 0, 2.2) */
void drawBox (void)
{
   glPushMatrix();
   glTranslatef (0.0, -0.25, 2.2);

   glPushMatrix();
   glTranslatef (0.0, 0.01, 0.0);
   drawPart (1.2, 0.02, 1.2, 0.6, 0.4, 0.2);   /* fundo */
   glPopMatrix();

   glPushMatrix();
   glTranslatef (0.0, 0.25, -0.575);
   drawPart (1.2, 0.5, 0.05, 0.6, 0.4, 0.2);   /* parede de tras */
   glPopMatrix();

   glPushMatrix();
   glTranslatef (0.0, 0.25, 0.575);
   drawPart (1.2, 0.5, 0.05, 0.6, 0.4, 0.2);   /* parede da frente */
   glPopMatrix();

   glPushMatrix();
   glTranslatef (-0.575, 0.25, 0.0);
   drawPart (0.05, 0.5, 1.1, 0.6, 0.4, 0.2);   /* parede da esquerda */
   glPopMatrix();

   glPushMatrix();
   glTranslatef (0.575, 0.25, 0.0);
   drawPart (0.05, 0.5, 1.1, 0.6, 0.4, 0.2);   /* parede da direita */
   glPopMatrix();

   glPopMatrix();
}

void drawRobot (void)
{
   glPushMatrix();

   glRotatef ((GLfloat) base, 0.0, 1.0, 0.0);
   drawPart (3.0, 0.5, 2.0, 0.5, 0.5, 0.5);

   glTranslatef (0.0, 0.25, 0.0);
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 1.0, 0.0);
   drawPart (0.4, 2.0, 0.4, 0.2, 0.4, 0.9);

   glTranslatef (0.0, 1.0, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.75, 0.0);
   drawPart (0.35, 1.5, 0.35, 0.2, 0.8, 0.3);

   glTranslatef (0.0, 0.75, 0.0);
   glRotatef ((GLfloat) wristTwist, 0.0, 1.0, 0.0);
   glRotatef ((GLfloat) wrist, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.15, 0.0);
   drawPart (0.8, 0.3, 0.3, 0.9, 0.8, 0.2);

   glTranslatef (0.0, 0.15, 0.0);
   drawFinger (1);
   drawFinger (-1);

   /* ponto entre as pontas dos dedos, onde o objeto fica preso */
   glTranslatef (0.0, 0.6, 0.0);
   glGetFloatv (GL_MODELVIEW_MATRIX, gripMatrix);
   if (holding)
      drawObject();

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

   glLoadIdentity();
   /* camera um pouco acima e de lado para dar nocao de profundidade */
   gluLookAt (4.0, 5.0, 10.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

   drawFloor();
   drawBox();
   drawRobot();

   if (!holding) {
      glPushMatrix();
      if (dropped)
         glLoadMatrixf (objMatrix);         /* onde a garra soltou */
      else
         glTranslatef (2.2, -0.05, 0.0);    /* posicao inicial, no chao */
      glGetFloatv (GL_MODELVIEW_MATRIX, objMatrix);
      drawObject();
      glPopMatrix();
   }

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
}

/* distancia entre a pegada e o centro do objeto (posicao fica em [12..14]) */
float gripDistance (void)
{
   float dx = gripMatrix[12] - objMatrix[12];
   float dy = gripMatrix[13] - objMatrix[13];
   float dz = gripMatrix[14] - objMatrix[14];
   return sqrt (dx*dx + dy*dy + dz*dz);
}

void pickOrDrop (void)
{
   int i;

   if (holding) {
      for (i = 0; i < 16; i++)
         objMatrix[i] = gripMatrix[i];
      holding = 0;
      dropped = 1;
      finger = 20;
      printf ("objeto solto\n");
   } else if (gripDistance() < 0.3) {
      holding = 1;
      finger = 0;
      printf ("objeto pego\n");
   } else {
      printf ("a garra esta longe do objeto (distancia %.2f)\n", gripDistance());
   }
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
      case 'p':
         pickOrDrop();
         break;
      case 'r':
         base = 0; shoulder = -20; elbow = -40;
         wrist = -30; wristTwist = 0; finger = 15;
         holding = 0; dropped = 0;
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
