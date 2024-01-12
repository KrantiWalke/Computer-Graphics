/// Student Name: Kranti Walke
/// Student ID : 301081860
/// Course : CSCI 272 (Fall 2023)
/// Assignment : Project #4 -Bezier Curve

/*
In this project I am following below instructions to draw Bezier Curve:
            • Use given Framework to draw 3D (x,y,z) Bezier Curve and do the following
            • Define initial maximum number of control points to 35 (program stop drawing after 35 clicks)
            • Write function to calculate factorials
            • Mark the control points on the screen for each left mouse click: use glPointSize(5);
            (note: mouse click gives you only x,y values. For the z value assign random value in visible range for each click).
            • Start drawing Bézier curve when number of control points reach 3 and beyond:
            use glBegin(GL_POINTS) to plot the graph. Set glPointSize(2);
            • Each click where n>3 must plot a unique Bézier curve Animate sphere move along the path
            • Use right mouse click to toggle between hide/show control points
            • Use middle mouse button or “space” key for reset the program
            • Have Left key and Right key to rotate your whole scene around the Y-Axis
            • Have repeated sphere animation by pressing “Enter” key

mouse and keys functionality:
            -> control points on the screen for each left mouse click
            -> middle mouse button or “space” : press space key to reset everything and draw a new bizer curve
            -> 'w' : to view the
            -> "Enter" : Enter key for restarting sphere animation
            -> Left key and Right key : to rotate your whole scene around the Y-Axis
            -> Right mouse click : to toggle between hide/show control points

*/

#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI 3.14159
#include <vector>
#define MAX_CLICKS 35

using namespace std;
//Defines a 3D point with x, y, and z coordinates.
struct Point {
    double x, y, z;
};

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


double Wwidth,Wheight; //Window width and height.
vector<Point> controlPoints; //Stores control points for the Bézier curve.
//Control the animation and display of control points.
bool showControlPoints = true;
double t = 0.0f; // Parameter for sphere's position on the curve
bool animate = true; // Control animation

GLfloat angle=0;//for rotating the screen over  y-axis
double zoomKey=15.0f;//for zoom in and zoom out the screen over y-axis

/// Function to compute factorials of 'n'
int factorial(double n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

/// Compute a point on the Bézier curve based on the parameter t.
Point calculateBezierPoint(double t) {
    Point p = {0.0f, 0.0f, 0.0f};  //This point will store the computed coordinates (x, y, z) of the Bézier curve at parameter t.
    double n = controlPoints.size() - 1; //The degree is given by n = controlPoints.size() - 1;. This is based on the number of control points minus one.
    for (double i = 0; i <= n; i++) {
        //The loop runs from i = 0 to i = n (inclusive). Each iteration calculates a portion of the point's final coordinates based on the current control point.
        double binomial = factorial(n) / (double)(factorial(i) * factorial(n - i)); //This is a part of the Bézier curve formula. It determines how much each control point influences the curve at parameter t. The binomial coefficient is calculated using the factorial function:
        double term = binomial * pow(t, i) * pow(1 - t, n - i); //This term uses the binomial coefficient, t raised to the power of i, and (1 - t) raised to the power of n - i.
        p.x += term * controlPoints[i].x; //For each control point, this term is multiplied by the control point's x, y, and z coordinates and added to the corresponding coordinates of point p.
        p.y += term * controlPoints[i].y;
        p.z += term * controlPoints[i].z;
      //  cout<<n<<endl;
    }
    return p;
}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (double)width;
     Wheight = (double)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }


/// The main rendering function. It clears the buffer, sets up the view, and draws the control points, Bézier curve, and a moving sphere along the curve.
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10.0,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME
    glPushMatrix(); //To Rotating the Scene
    glRotatef(angle,0,1,0); //Rotates the scene around the y-axis by angle degrees.

 // your code here

 /// Draw control points
    if (showControlPoints) { //If showControlPoints is true, control points are drawn as small dots.
        glPointSize(5); //sets the size of these points.
        glBegin(GL_POINTS);
        for (const auto& point : controlPoints) {
            glVertex3f(point.x, point.y, point.z);
        }
        glEnd();
    }

    /// Draw Bézier curve
    /*  Checks if there are at least 3 control points.
        Sets up drawing state: glPointSize(2) for point size, and glLineWidth(3.0f) for line width.
        Draws the curve using GL_LINE_STRIP, which connects a series of points. The curve is computed point-by-point using calculateBezierPoint(t) within a loop from t = 0.0 to 1.0.*/
    if (controlPoints.size() >= 3) {
        glPointSize(2);
        glLineWidth(3.0f); // Set line width
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1.0; t += 0.01) {
            Point p = calculateBezierPoint(t);
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
        /*glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(3.0f); // Set line width
        glBegin(GL_LINE_STRIP); // Use line strip instead of points
        for (float t = 0.0; t <= 1.0; t += 0.01) {
            Point p = calculateBezierPoint(t);
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();*/

    /// Draw the moving sphere
        Point spherePos = calculateBezierPoint(t);  //A sphere is placed at a position calculated by calculateBezierPoint(t), representing the current position on the curve.
        glPushMatrix();
        glTranslatef(spherePos.x, spherePos.y, spherePos.z);
        glutSolidSphere(0.3, 20, 20); // Draws a solid sphere with a radius of 0.3 and 20 segments both horizontally and vertically.
        glPopMatrix();
    }
    glPopMatrix ();
    glutSwapBuffers();  //Swaps the front and back buffers.
}

/// Handles keyboard input for quitting, toggling wireframe mode, and resetting or restarting the animation.
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':  // to exit the program
            exit(0);
            break;
        case 'w':
            WireFrame =!WireFrame;
            break;
        case ' ': // Space key for resetting
            controlPoints.clear();
            t = 0.0f; // Reset t for the animation
            animate = true; // Enable animation
            break;
        case 13: // Enter key for restarting animation
            if (!animate) {
                t = 0.0f; // Reset t for the animation
                animate = true; // Restart animation
            }
            break;
    }
}

/// Handles special keys for rotating the view.
void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT: // this will rotate the wireframe in y axis
            angle+=5;
            break;
     case GLUT_KEY_LEFT: // this will rotate the wireframe in y axis
            angle-=5;
            break;
   }
  glutPostRedisplay();
}


/// The idle function for animating the moving sphere along the curve.
static void idle(void) {
    if (animate) {
        t += 0.001f;
        if (t > 1.0f) {
            t = 1.0f; // Stop at the end of the curve
            animate = false; // Stop animation
        }
        glutPostRedisplay();
    }
}

/// Handles mouse input for adding control points, clearing them, and toggling their visibility.
void mouse(int btn, int state, int x, int y) {  //Handles mouse button events, where btn is the mouse button, state is the button state (pressed or released), x and y are the mouse coordinates.
    double scale = 54 * (Wwidth / Wheight);
    switch (btn) {
        case GLUT_LEFT_BUTTON:  //If the left button is pressed (GLUT_DOWN), and the number of control points is less than MAX_CLICKS:
            if (state == GLUT_DOWN) {
                if (controlPoints.size() < MAX_CLICKS) {
                    Point newPoint; //A new Point is created with its x and y values adjusted based on the current mouse position and the scaling factor.
                    newPoint.x = (x - (Wwidth / 2)) / scale;
                    newPoint.y = ((Wheight / 2) - y) / scale;
                    newPoint.z = rand() % 10 - 5; // Random Z value between -5 and 5
                    controlPoints.push_back(newPoint);  //This new point is added to the controlPoints vector.

                    // Reset and activate the animation
                    t = 0.0f;
                    animate = true;
                }
            }
            break;
        case GLUT_MIDDLE_BUTTON: //If the middle button is pressed:
                                 // The controlPoints vector is cleared, removing all existing control points.
            if (state == GLUT_DOWN) {
                controlPoints.clear();
            }
            break;

        case GLUT_RIGHT_BUTTON: // If the right button is pressed:
                                // Toggles the visibility of control points (showControlPoints).
            if (state == GLUT_DOWN) {
                showControlPoints = !showControlPoints;
            }
            break;
    }
    glutPostRedisplay();  //Signals GLUT to redraw the window.
}


static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */
/// Initializes GLUT, sets up the window, registers callback functions, and starts the GLUT main loop.
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 3");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
