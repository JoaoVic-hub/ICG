/*
 * Exercicio 1.2 - cube.c
 *
 * Troca do glFrustum() por gluPerspective(60.0, 1.0, 1.5, 20.0) e depois
 * por glOrtho() com os mesmos parametros do glFrustum().
 *
 * Teclas:
 *   1 - glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0)   (original)
 *   2 - gluPerspective(fovy, aspect, 1.5, 20.0)     (comeca com 60 e 1.0)
 *   3 - glOrtho(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0)
 *   f / F - diminui / aumenta o fovy   (gluPerspective)
 *   a / A - diminui / aumenta o aspect (gluPerspective)
 *   r     - volta fovy = 60 e aspect = 1.0
 *   ESC   - sair
 *
 * Observacoes:
 *   - fovy maior = angulo de visao maior, o cubo parece menor (zoom out);
 *     fovy menor = zoom in.
 *   - aspect = largura/altura do volume de visao. Se ele for diferente da
 *     proporcao da janela, o cubo aparece esticado ou achatado.
 *   - No glOrtho nao ha perspectiva: o tamanho do cubo nao depende da
 *     distancia, entao a face da frente e a de tras se sobrepoem e o cubo
 *     vira um retangulo.
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int projecao = 1;
float fovy = 60.0;
float aspect = 1.0;

void ajustaProjecao(void)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (projecao == 1) {
      glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
      printf("glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0)\n");
   } else if (projecao == 2) {
      gluPerspective(fovy, aspect, 1.5, 20.0);
      printf("gluPerspective(%.1f, %.2f, 1.5, 20.0)\n", fovy, aspect);
   } else {
      glOrtho(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
      printf("glOrtho(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0)\n");
   }

   glMatrixMode(GL_MODELVIEW);
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
   glLoadIdentity();
   gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glScalef(1.0, 2.0, 1.0);
   glutWireCube(1.0);
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   ajustaProjecao();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case '1':
   case '2':
   case '3':
      projecao = key - '0';
      break;
   case 'f':
      if (fovy > 10.0) fovy -= 5.0;
      projecao = 2;
      break;
   case 'F':
      if (fovy < 170.0) fovy += 5.0;
      projecao = 2;
      break;
   case 'a':
      if (aspect > 0.25) aspect -= 0.25;
      projecao = 2;
      break;
   case 'A':
      aspect += 0.25;
      projecao = 2;
      break;
   case 'r':
      fovy = 60.0;
      aspect = 1.0;
      break;
   case 27:
      exit(0);
   default:
      return;
   }

   ajustaProjecao();
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Exercicio 1.2 - cube");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   printf("1 = glFrustum, 2 = gluPerspective, 3 = glOrtho\n");
   printf("f/F muda o fovy, a/A muda o aspect, r reseta\n\n");

   glutMainLoop();
   return 0;
}
