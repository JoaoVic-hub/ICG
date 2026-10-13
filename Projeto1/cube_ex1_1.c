/*
 * Exercicio 1.1 - cube.c
 *
 * Troca do gluLookAt() por glTranslatef(0.0, 0.0, -5.0) e testes com
 * outros parametros do gluLookAt().
 *
 * Teclas:
 *   1 - gluLookAt original: olho em (0,0,5) olhando para (0,0,0), vup = (0,1,0)
 *   2 - glTranslatef(0.0, 0.0, -5.0) no lugar do gluLookAt
 *   3 - gluLookAt com o olho na diagonal (lookfrom diferente)
 *   4 - gluLookAt olhando para um ponto deslocado (lookat diferente)
 *   5 - gluLookAt com vup inclinado (a imagem gira)
 *   6 - camera vendo o cubo de cima para baixo
 *   ESC - sair
 *
 * A cada desenho a matriz MODELVIEW e impressa no terminal.
 *
 * Por que os modos 1 e 2 dao o mesmo resultado?
 *   O gluLookAt(0,0,5, 0,0,0, 0,1,0) coloca a camera em z = 5 olhando na
 *   direcao -z com o "para cima" no eixo y. Essa ja e a orientacao padrao da
 *   camera no OpenGL, entao a parte de rotacao da matriz fica sendo a
 *   identidade e sobra so a translacao de -5 em z (o mundo e empurrado 5
 *   unidades para longe da camera). Ou seja, a matriz gerada e exatamente a
 *   mesma de glTranslatef(0.0, 0.0, -5.0). Da para conferir isso na matriz
 *   impressa no terminal ao apertar 1 e 2.
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int modo = 1;

void imprimeModelview(void)
{
   GLfloat m[16];
   int i;

   glGetFloatv(GL_MODELVIEW_MATRIX, m);

   /* o OpenGL guarda a matriz por colunas: m[coluna*4 + linha] */
   printf("Modo %d - matriz MODELVIEW:\n", modo);
   for (i = 0; i < 4; i++) {
      printf("  %7.3f %7.3f %7.3f %7.3f\n", m[i], m[4 + i], m[8 + i], m[12 + i]);
   }
   printf("\n");
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glLoadIdentity();             /* limpa a matriz */

   /* transformacao de visualizacao */
   if (modo == 1) {
      gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   } else if (modo == 2) {
      glTranslatef(0.0, 0.0, -5.0);
   } else if (modo == 3) {
      gluLookAt(3.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   } else if (modo == 4) {
      gluLookAt(0.0, 0.0, 5.0, 1.5, 0.0, 0.0, 0.0, 1.0, 0.0);
   } else if (modo == 5) {
      gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
   } else if (modo == 6) {
      /* olho em cima do cubo (eixo y) olhando para baixo.
         O vup nao pode ser (0,1,0), pois seria paralelo a direcao de
         visao, entao usamos o eixo -z como "para cima" da imagem. */
      gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
   }

   imprimeModelview();

   glScalef(1.0, 2.0, 1.0);      /* transformacao de modelagem */
   glutWireCube(1.0);
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   if (key >= '1' && key <= '6') {
      modo = key - '0';
      glutPostRedisplay();
   } else if (key == 27) {
      exit(0);
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Exercicio 1.1 - cube");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   printf("Teclas 1 a 6 trocam a camera (veja o comentario no inicio do codigo).\n\n");

   glutMainLoop();
   return 0;
}
