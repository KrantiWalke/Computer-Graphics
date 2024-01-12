/*
Name:Kranti Sambhaji Walke
ID: 301081860
Class: CSCI 272
Project Name: Assignment 2 - Sphere Move
*/

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
#include<time.h>

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/*
 initial x, y, and z coordinates of the 4 balls in the 3D space.
 For instance, x1_ball1, y1_ball1, and z1_ball1 represent the x, y, and z coordinates of the first ball, respectively.
*/
float x1_ball1=3.0, x2_ball2=-4.0, x3_ball3=5.0, x4_ball4=0.0;
float y1_ball1=-4.0, y2_ball2=-9.0, y3_ball3=6.0, y4_ball4=2.0;
float z1_ball1=1.0, z2_ball2=8.0, z3_ball3=7.0, z4_ball4=0.0;

/*This variable represents the rotation angle around the y-axis for a rotating effect.
This angle will probably be updated during the animation to give a rotating view of the scene.*/
static float rotationAngle=0.0;
/*These variables determine the position of the camera or the viewer's "eye" in the 3D space.
This affects how the scene looks from the viewer's perspective.*/
float eyeX=0, eyeY=5, eyeZ=35.0;
//float cameraDistance  = 5.0f; // Camera cameraDistance  from the object

/*the speeds of the balls in x, y, and z directions.
For example, x1_speed, y1_speed, and z1_speed are the speeds of the first ball in the x, y, and z axes, respectively.*/
float x1_speed=0.1, x2_speed=0.1, x3_speed=0.1, x4_speed=0.1;
float y1_speed=0.1, y2_speed=0.1, y3_speed=0.1, y4_speed=0.1;
float z1_speed=0.1, z2_speed=0.1, z3_speed=0.1, z4_speed=0.1;

/*the size of the cube.
Given the context of the previous code, the cube likely encapsulates the balls, and the balls bounce around inside this cube.*/
float cubesize= 20;

/*These variables set the initial RGB (Red, Green, Blue) values for the 4 balls.
For instance, the first ball (ball1) is yellow (red and green mixed, but no blue). Each component (R, G, B) can have a
value between 0.0 and 1.0, where 0.0 means none of that color is present, and 1.0 means the maximum amount of that color is present.*/
float ball1_R=1.0, ball1_G=1.0, ball1_B=0.0;
float ball2_R=1.0, ball2_G=0.0, ball2_B=0.0;
float ball3_R=0.0, ball3_G=1.0, ball3_B=0.0;
float ball4_R=0.0, ball4_G=0.0, ball4_B=1.0;

//These variables are of type clock_t, which is used for measuring time intervals.
clock_t StartTime;
clock_t endtime;

//The changeColor function is designed to change the color of a specific ball based on its index, given by the parameter i.
void changeColor(int i){
    switch(i){
        case 0: //If the value of i is 0, then the color of the first ball (ball1) will be changed.
            ball1_R = (float)(rand()%10 + 1)/10; //The red component
            ball1_G = (float)(rand()%10 + 1)/10; //The Green component
            ball1_B = (float)(rand()%10 + 1)/10; //The Blue component
            break;
        case 1:
            ball2_R = (float)(rand()%10 + 1)/10;
            ball2_G = (float)(rand()%10 + 1)/10;
            ball2_B = (float)(rand()%10 + 1)/10;
            break;
        case 2:
            ball3_R = (float)(rand()%10 + 1)/10;
            ball3_G = (float)(rand()%10 + 1)/10;
            ball3_B = (float)(rand()%10 + 1)/10;
            break;
        case 3:
            ball4_R = (float)(rand()%10 + 1)/10;
            ball4_G = (float)(rand()%10 + 1)/10;
            ball4_B = (float)(rand()%10 + 1)/10;
            break;
    }
}

/*This function checks whether a ball (based on its index i) collides with the cube's boundaries or with other balls.
If there's a collision, it modifies the ball's speed, making it move in the opposite direction (i.e., reflecting its direction).*/

bool checkCollision(int i) {
    bool result = false;

    float x, y, z;
    float x_speed, y_speed, z_speed;

    // Getting the position and speed of the ball by its index
    /*Depending on the value of i (which specifies the ball's index),
    this switch-case sets the position (x, y, z) and speed (x_speed, y_speed, z_speed) of the ball.*/
    switch(i) {
        case 0:
            x = x1_ball1; y = y1_ball1; z = z1_ball1;
            x_speed = x1_speed; y_speed = y1_speed; z_speed = z1_speed;
            break;
        case 1:
            x = x2_ball2; y = y2_ball2; z = z2_ball2;
            x_speed = x2_speed; y_speed = y2_speed; z_speed = z2_speed;
            break;
        case 2:
            x = x3_ball3; y = y3_ball3; z = z3_ball3;
            x_speed = x3_speed; y_speed = y3_speed; z_speed = z3_speed;
            break;
        case 3:
            x = x4_ball4; y = y4_ball4; z = z4_ball4;
            x_speed = x4_speed; y_speed = y4_speed; z_speed = z4_speed;
            break;
        default:
            return false;
    }

    //Checking collision with the cube
    float m = cubesize / 2.0f;  //m is half the size of the cube (since the cube's center is at the origin, its edges go from -m to m along each axis).

    // Check collision with the bounding box
    //three if-conditions check whether the ball's x, y, or z coordinates are outside the bounds of the cube.
    //If the ball is out of bounds in any dimension, its speed in that dimension is reversed, and result is set to true to indicate a collision occurred.
    if(x > m || x < -m) {
        x_speed = -x_speed;
        result = true;
    }
    if(y > m || y < -m) {
        y_speed = -y_speed;
        result = true;
    }
    if(z > m || z < -m) {
        z_speed = -z_speed;
        result = true;
    }

    // Check collision with other balls
    //This loop iterates over all balls (including the current one).
    for(int k = 0; k < 4; k++) {
        if(k != i) {
                //The nested switch-case within the loop gets the position (x1, y1, z1) of another ball k:
            float x1, y1, z1;
            switch(k) {
                case 0: x1 = x1_ball1; y1 = y1_ball1; z1 = z1_ball1; break;
                case 1: x1 = x2_ball2; y1 = y2_ball2; z1 = z2_ball2; break;
                case 2: x1 = x3_ball3; y1 = y3_ball3; z1 = z3_ball3; break;
                case 3: x1 = x4_ball4; y1 = y4_ball4; z1 = z4_ball4; break;
            }
            //Then, it calculates the Euclidean distance dist between the current ball and ball k:
            float dist = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1) + (z - z1) * (z - z1));
            /*If this distance is less than or equal to 2.6 (assuming this is the sum of the radii of two balls),
             the speeds of the current ball are reversed, indicating a collision with another ball*/
            if(dist <= 2.6) {
                x_speed = -x_speed;
                y_speed = -y_speed;
                z_speed = -z_speed;
                result = true;
            }
        }
    }

    // Updating the speed variables of the original ball
    switch(i) {
        case 0: x1_speed = x_speed; y1_speed = y_speed; z1_speed = z_speed; break;
        case 1: x2_speed = x_speed; y2_speed = y_speed; z2_speed = z_speed; break;
        case 2: x3_speed = x_speed; y3_speed = y_speed; z3_speed = z_speed; break;
        case 3: x4_speed = x_speed; y4_speed = y_speed; z4_speed = z_speed; break;
    }
//The function then returns result, which will be true if a collision occurred and false otherwise.
    return result;
}

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }


static void display(void)
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
// Compute camera position based on rotationAngle and cameraDistance
   // float camX = cameraDistance;
    //float camZ = cameraDistance;
    // Setup camera using gluLookAt
    gluLookAt(eyeX, eyeY, eyeZ,  // Camera position
              0.0f, 0.0f, 0.0f,  // Look at point (center of the scene)
              0.0f, 1.0f, 0.0f); // Up vector
   // gluLookAt(eyeX, eyeY, eyeZ,0.0,0.0,0.0,0.0,1.0,0.0);     //using this method we resize the cube frame

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        //Toggle WIRE FRAME

//----------------Draw Cube-----------------------------------

    //using rotateAngle we rotate the cube using up down keys
     glRotatef(rotationAngle,0,1,0);
    //creating cube using this method
     glPushMatrix();
     glutWireCube(cubesize);
     glPopMatrix();


//------------------Draw Ball 1-----------------------
    glPushMatrix();
    glColor3f(ball1_R,ball1_G,ball1_B);    //this functions decides the color of the ball
    glTranslatef(x1_ball1,y1_ball1,z1_ball1);
    glutSolidSphere(1.3,40,40);    //this method is used for creating the sphere
    glPopMatrix();

//------------------Draw Ball 2-----------------------
    glPushMatrix();
    glColor3f(ball2_R,ball2_G,ball2_B);
    glTranslatef(x2_ball2,y2_ball2,z2_ball2);
    glutSolidSphere(1.3,40,40);
    glPopMatrix();

//------------------Draw Ball 3-----------------------

    glPushMatrix();
    glColor3f(ball3_R,ball3_G,ball3_B);
    glTranslatef(x3_ball3,y3_ball3,z3_ball3);
    glutSolidSphere(1.3,40,40);
    glPopMatrix();

//------------------Draw Ball 4-----------------------

    glPushMatrix();
    glColor3f(ball4_R,ball4_G,ball4_B);
    glTranslatef(x4_ball4,y4_ball4,z4_ball4);
    glutSolidSphere(1.3,40,40);
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

//method to handle the orientation of the cube

void Specialkeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        eyeZ  -= 0.5f;  //Zoom In: Up_Key  (0.5 increment)
        break;
    case GLUT_KEY_DOWN:
        eyeZ  += 0.5f; //Zoom Out: Down_Key (0.5 decrement)
        break;
    case GLUT_KEY_LEFT:
        rotationAngle += 5.0f; //Rotate Left (around y-axis): Left_key (by 5 degrees increment)
        break;
    case GLUT_KEY_RIGHT:
        rotationAngle -= 5.0f; //Rotate Right (around y-axis): Right_key (by5 degrees decrement)
        break;
    }

    // Clamp the cameraDistance  if necessary
//    if (cameraDistance  < 0.5f) cameraDistance  = 0.5f;

    // Normalize rotationAngle
    if (rotationAngle > 360.0f || rotationAngle < -360.0f) rotationAngle = fmod(rotationAngle, 360.0f);

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
    /*Time Control:
Here, the current time (in terms of clock ticks since the program started) is fetched using the clock() function from the <ctime> (or <time.h>) library.*/
    endtime = clock();

    /*This section ensures that the code inside the if-statement is executed only after a certain number of clock ticks (in this case, 30) have passed since the last execution.
    This mechanism can be used to control the rate at which scene updates happen.*/
    if (endtime - StartTime > 30) {
        StartTime = clock();

        /* Update Ball State:
        Inside the time-controlled if-statement, there's a loop that goes over the state of each ball:
        For each ball (0 through 3), the program checks for collisions using the checkCollision(i) function.
        If a collision is detected (c is true), the ball's color is changed using the changeColor(i) function.*/
        for (int i = 0; i < 4; i++) {
            bool c = checkCollision(i);
            if (c) {
                changeColor(i); // Assuming this function has been similarly modified to accommodate changes
            }

            /*Update Ball Position:
            Still inside the for-loop, there's a switch-case structure to update the position of each ball based on its speed:
            Depending on the value of i, the position of the corresponding ball is incremented by its speed in the x, y, and z directions.
            This effectively moves each ball.*/
            switch (i) {
                case 0:
                    x1_ball1 += x1_speed;
                    y1_ball1 += y1_speed;
                    z1_ball1 += z1_speed;
                    break;

                case 1:
                    x2_ball2 += x2_speed;
                    y2_ball2 += y2_speed;
                    z2_ball2 += z2_speed;
                    break;

                case 2:
                    x3_ball3 += x3_speed;
                    y3_ball3 += y3_speed;
                    z3_ball3 += z3_speed;
                    break;

                case 3:
                    x4_ball4 += x4_speed;
                    y4_ball4 += y4_speed;
                    z4_ball4 += z4_speed;
                    break;
            }
        }
    }
}

static void init(void)
{
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    StartTime=clock(); //Start clock time
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
