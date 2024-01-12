//Author: Kranti Sambhaji Walke
//Computer Graphics Project #1 : Barnsley’s Fractal Fern


#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

float a,b,c,d,e,f;
float x,y,oldX,oldY;
int n=200000;
int num;


void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 oldX=1.0;
 oldY=1.0;

 glOrtho(-2.5,2.75,0,10.5,-1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{

    glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
  // your code here
  glColor3f(0,1,0);
  for(int i=0;i<n;i++){
        num=rand()%100;
          if (num <= 1) {
            a = 0;
            b = 0;
            c = 0;
            d = 0.16;
            e = 0;
            f = 0;
        }
        else if (num <= 86) {
            a = 0.85;
            b = 0.04;
            c = -0.04;
            d = 0.85;
            e = 0;
            f = 1.6;
        }
        else if (num <= 93) {
            a = 0.2;
            b = 0.23;
            c = -0.26;
            d = 0.22;
            e = 0;
            f = 1.6;
        }
        else {
            a = -0.15;
            b = 0.26;
            c = 0.28;
            d = 0.24;
            e = 0;
            f = 0.44;
        }
        x = a*oldX+  c*oldY + e;
        y = b*oldX + d*oldY + f;
        glBegin(GL_POINTS);
        glVertex3f(x, y, 0) ;
        //glVertex3f(-x, y, 0) ;   //for mirror image
        glEnd();
        oldX=x;
        oldY=y;
  }
  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);


   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (800, 800);                //window screen

   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display

   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard

   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}


