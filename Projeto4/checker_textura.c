#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;
static int umQuadrado = 0, hex = 0x8;
static GLint filtro = GL_NEAREST, wrap = GL_REPEAT;
static GLfloat escala = 1.0;

void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&hex)==0)^((j&hex))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);
   makeCheckImage();
}

void display(void)
{
   char info[120];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtro);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtro);

   glBegin(GL_QUADS);
   if (!umQuadrado) {
      glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

      glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
      glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
   } else {
      glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 2.0);
      glTexCoord2f(0.0, escala); glVertex3f(-2.0, 4.0, -6.0);
      glTexCoord2f(escala, escala); glVertex3f(2.0, 4.0, -6.0);
      glTexCoord2f(escala, 0.0); glVertex3f(2.0, -1.0, 2.0);
   }
   glEnd();
   glFlush();
   glDisable(GL_TEXTURE_2D);

   sprintf(info, "0x%X  %s  escala %.0f  %s", hex,
           filtro == GL_NEAREST ? "GL_NEAREST" : "GL_LINEAR", escala,
           wrap == GL_REPEAT ? "GL_REPEAT" : "GL_CLAMP");
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
      case 'q':
         umQuadrado = !umQuadrado;
         break;
      case 'x':
         hex = (hex == 0x8) ? 0x1 : 0x8;
         makeCheckImage();
         break;
      case 'l':
         filtro = (filtro == GL_NEAREST) ? GL_LINEAR : GL_NEAREST;
         break;
      case 'e':
         escala = (escala == 1.0) ? 10.0 : 1.0;
         break;
      case 'c':
         wrap = (wrap == GL_REPEAT) ? GL_CLAMP : GL_REPEAT;
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
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
