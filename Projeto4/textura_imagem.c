#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint texture;
static GLUquadric *quad;
static int modo = 0, vermelho = 0, esfera = 0;
static GLint modos[] = { GL_DECAL, GL_REPLACE, GL_MODULATE, GL_BLEND, GL_ADD };
static char *nomes[] = { "GL_DECAL", "GL_REPLACE", "GL_MODULATE", "GL_BLEND", "GL_ADD" };

void loadTexture(const char* filename)
{
   int width, height, nrChannels;
   unsigned char* data;

   stbi_set_flip_vertically_on_load(1);
   data = stbi_load(filename, &width, &height, &nrChannels, 0);
   if (data) {
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (nrChannels == 3) {
         gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
                           GL_RGB, GL_UNSIGNED_BYTE, data);
      } else if (nrChannels == 4) {
         gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height,
                           GL_RGBA, GL_UNSIGNED_BYTE, data);
      }

      stbi_image_free(data);
   } else {
      fprintf(stderr, "Failed to load texture: %s\n", filename);
   }
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   quad = gluNewQuadric();
   gluQuadricTexture(quad, GL_TRUE);
}

void display(void)
{
   char info[100];

   if (texture == 0)
      loadTexture("terra.jpg");

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modos[modo]);
   glBindTexture(GL_TEXTURE_2D, texture);

   if (vermelho)
      glColor3f(1.0, 0.0, 0.0);
   else
      glColor3f(1.0, 1.0, 1.0);

   if (esfera) {
      glPushMatrix();
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      glRotatef(70.0, 0.0, 0.0, 1.0);
      gluSphere(quad, 1.0, 32, 32);
      glPopMatrix();
   } else {
      glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 1.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -1.0, 0.0);
      glEnd();
   }
   glFlush();
   glDisable(GL_TEXTURE_2D);

   sprintf(info, "%s  cor %s  %s", nomes[modo],
           vermelho ? "vermelha" : "branca", esfera ? "esfera" : "quadrado");
   glutSetWindowTitle(info);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'v':
         vermelho = !vermelho;
         break;
      case 'm':
         modo = (modo + 1) % 5;
         break;
      case '1': case '2': case '3': case '4': case '5':
         modo = key - '1';
         break;
      case 'e':
         esfera = !esfera;
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
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(600, 400);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
