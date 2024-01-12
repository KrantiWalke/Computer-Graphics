/// Student Name: Kranti Walke
/// Student ID : 301081860
/// Course : CSCI 272 (Fall 2023)
/// Assignment : Project #5 - Bezier Curve 2D Animation (using sprite sheet with animated character and parallax background)

/*
Description:
    ->This project is an OpenGL application that demonstrates the animation of a sprite along a Bezier curve.
    ->The program allows users to interactively set control points for the curve and observe how a sprite moves and rotates along the path defined by these points.

Features:
    ->Interactive setting of up to four Bezier curve control points using the mouse.
    ->Real-time rendering of a sprite moving along the Bezier curve.
    ->Control of sprite animation and orientation using the keyboard.
    ->Toggleable wireframe mode to view the mesh construction.
    ->Dynamic background and sprite textures.

Controls:
    ->Mouse Left Click: Set control points for the Bezier curve.
    ->Mouse Right Click: Toggle visibility of control points.
    ->Arrow Keys: Move the sprite along the curve(Left or Right).
    ->'W' Key: Toggle wireframe mode.
    ->Spacebar: Reset the scene.
    ->ESC or 'Q' Key: Exit the application.
*/


#include <string.h>
#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <SOIL.h>
#endif

#include <iostream>
#include <cmath>
#include <algorithm>

/// Constants
#define PI 3.14159
#define frameDuration 80 // Time for each frame in milliseconds
//#define totalAnimationFrames 10 // Number of frames in the animation

/// Global Variables
int lastFrameTimestamp  = 0;
int currentAnimationFrame  = 0;
int totalAnimationFrames = 10;

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

float Wwidth,Wheight;

/// Control Points for Bezier Curve
float *controlPointsX = new float[5];
float *controlPointsY = new float[5];

/// Bezier Curve Coefficients
GLdouble bezierCoefficients[5];

/// Control Point Index
int controlPointIndex = 1;

/// Curve Coordinates
double curveX = 0, curveY = 0;

/// Mouse Position
float mouseX, mouseY;

/// Animation Parameters
double parametricT, Sprite_Cal;

/// Direction for Animation or Movement
float directionX = 1;

/// Texture IDs for Rendering
GLuint parallex_tex, sprite_tex, idle_sprite_tex, moving_sprite_tex;

/// Scene Dimensions and Object Size
GLfloat parallexgroundSizeZ = 20.0f;
GLfloat parallexgroundSizeX = 80.0f;
GLfloat parallexgroundSizeY = 30.0f;
GLfloat objectSize = 1.8f;

GLint RotateX,RotateY;

/// Animation Speed
GLfloat Speed_Animation_Curve = 0.005;

bool showControlPoints = true;

// Additional Function Prototypes
void bezierCurveDraw();
void bezierTanCal(float parametricT, float out[2]);
void bezierCurveTracking(float parametricT, float *x, float *y);
void renderSpriteAlongBezierCurve();
void animationFrameUpdate();
void loadTexture(char* filename, GLuint &tex);
void sceneResetting();


/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }


/// This function prints Bezier Curve
void bezierCurveDraw()
{
    // Only draw if the number of control points is less than 5
    if (controlPointIndex < 5)
    {
        // Set the line width for the curve
        glLineWidth(2);

        // Begin drawing a series of quadrilateral strips
        glBegin(GL_QUAD_STRIP);

        // Iterate over the parametric value 't' from 0 to 1
        for (parametricT = 0.0; parametricT<= 1.0; parametricT += 0.001)
        {
            float curvePosX = 0.0;
            float curvePosY = 0.0;

            // Calculate the position of the curve at 't' using the Bezier formula
            for (int i = 0; i <= controlPointIndex; ++i)
            {
                float coeff = bezierCoefficients[i] * pow(1 - parametricT, controlPointIndex - i) * pow(parametricT, i);
                curvePosX += coeff * controlPointsX[i];
                curvePosY += coeff * controlPointsY[i];
            }

            // Specify the vertices of the quad strip
            glVertex3f(curvePosX, curvePosY, 0.0);
            glVertex3f(curvePosX, curvePosY - 50.0, 0.0);
        }

        // End drawing the quad strip
        glEnd();
    }
}

/// This function calculates the Tangents
void calBezierTan(float parametricT, float tangent[2]) {
    // Initialize the tangent vector to zero
    tangent[0] = 0.0f;
    tangent[1] = 0.0f;

    // Calculate the tangent vector at parametricT
    for (int j = 1; j <= controlPointIndex - 1; ++j) {
        float polynomialTerm = bezierCoefficients[j] * pow(1 - parametricT, controlPointIndex - j - 1) * pow(parametricT, j - 1);

        // X component of the tangent
        tangent[0] += polynomialTerm * (controlPointsX[j + 1] - controlPointsX[j - 1]);

        // Y component of the tangent
        tangent[1] += polynomialTerm * (controlPointsY[j + 1] - controlPointsY[j - 1]);
    }

    // Multiply by the degree of the Bezier curve (controlPointIndex - 1) for the derivative
    tangent[0] *= (controlPointIndex - 1);
    tangent[1] *= (controlPointIndex - 1);
}

/// Evaluates the position/tangent on the Bezier curve at a given parametric value 'parametricT' for each X & Y Coordinates
void evaluateBezierCurve(float parametricT, float *xPosition, float *yPosition)
{
    // Iterate through each control point
    for (int i = 0; i <= controlPointIndex; ++i) {
        float bernsteinPolynomial = bezierCoefficients[i] * pow(1 - parametricT, controlPointIndex - i) * pow(parametricT, i);

        // Accumulate the weighted position of each control point
        *xPosition += bernsteinPolynomial * controlPointsX[i];
        *yPosition += bernsteinPolynomial * controlPointsY[i];
    }
}


/// Function loadTexture that takes a string (char*) representing the filename of the texture and a reference to an unsigned integer (GLuint &) for the texture ID.
void loadTexture(char* textureFilename, GLuint &textureID)
{
    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);

    // Generate a texture ID and bind it as a 2D texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture environment parameters
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Load the image file
    int imageWidth, imageHeight;
    unsigned char* imageData = SOIL_load_image(textureFilename, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGBA);

    // Check if the image was successfully loaded
    if (!imageData) {
        cout << "Unable to load texture image: " << textureFilename << endl;
        return;
    }

    // Define the texture with the loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    // Free the image data as it is no longer needed
    SOIL_free_image_data(imageData);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture wrapping mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

/// Function to render a sprite (a 2D graphical object) along a Bezier curve.
void renderSpriteAlongBezierCurve()
{
    float spritePositionX = 0, spritePositionY = 0;

    // Clamp Sprite_Cal to the range [0, 1]
    Sprite_Cal = std::min(std::max(Sprite_Cal, 0.0), 1.0);

    // Calculate bezier curve coordinates for sprite placement
    evaluateBezierCurve(Sprite_Cal, &spritePositionX, &spritePositionY);

    // Compute the tangent vector at the sprite's position
    float tangentVector[2];
    calBezierTan(Sprite_Cal, tangentVector);

    // Normalize the tangent vector
    float vectorLength = sqrt(tangentVector[0] * tangentVector[0] + tangentVector[1] * tangentVector[1]);
    tangentVector[0] /= vectorLength;
    tangentVector[1] /= vectorLength;

    // Determine the rotation angle based on the tangent vector
    float rotationAngle = atan2(tangentVector[1], tangentVector[0]) * 180.0f / PI;

    // Apply transformations for positioning and orienting the sprite
    glTranslatef(spritePositionX, spritePositionY + 1.20f, 0.0f);
    glScalef(directionX, 1.0f, 1.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

    // Bind the sprite texture
    glBindTexture(GL_TEXTURE_2D, sprite_tex);

    // Calculate texture coordinates for sprite animation
    float spriteWidth = 1.0f / totalAnimationFrames;
    float texturePosX = currentAnimationFrame  * spriteWidth;

    // Draw the sprite as a quad
    glBegin(GL_QUADS);
        glTexCoord2f(texturePosX, 0.0f); glVertex2f(-objectSize, -objectSize);
        glTexCoord2f(texturePosX + spriteWidth, 0.0f); glVertex2f(objectSize, -objectSize);
        glTexCoord2f(texturePosX + spriteWidth, 1.0f); glVertex2f(objectSize, objectSize);
        glTexCoord2f(texturePosX, 1.0f); glVertex2f(-objectSize, objectSize);
    glEnd();
}

/// Update time frame
void updateAnimationFrame()
{
    // Get the current time from GLUT
    int currentTimestamp = glutGet(GLUT_ELAPSED_TIME);

    // Check if enough time has passed since the last frame update
    if (currentTimestamp - lastFrameTimestamp >= frameDuration) {
        // Update the current frame, wrapping around if it exceeds the total number of frames
        currentAnimationFrame = (currentAnimationFrame + 1) % totalAnimationFrames;

        // Update the last frame timestamp
        lastFrameTimestamp = currentTimestamp;
    }

    // Request a redraw of the scene
    glutPostRedisplay();
}

static void display(void)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the current matrix mode to model-view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera position
    gluLookAt(0.0, 0.0, 52.0, 0.0, 0.0, 0.0, 0.0, 1.0, 100.0);

    // Set the polygon mode based on the wireframe toggle
    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // Apply rotations based on mouse input
    glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(RotateY, 0.0f, 1.0f, 0.0f);

    // Update animation frame if necessary
    if (Sprite_Cal != 0.0 && Sprite_Cal != 1.0)
    {
        updateAnimationFrame();
    }

    // Translate for background positioning
    glTranslatef(0.0f, 0.0f, 10.0f);

    // Draw the background
    glPushMatrix();
        glTranslatef(Sprite_Cal * -30.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, parallex_tex);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(-parallexgroundSizeX, -parallexgroundSizeY, -parallexgroundSizeZ);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(parallexgroundSizeX, -parallexgroundSizeY, -parallexgroundSizeZ);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(parallexgroundSizeX, parallexgroundSizeY, -parallexgroundSizeZ);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-parallexgroundSizeX, parallexgroundSizeY, -parallexgroundSizeZ);  //Top Left
        glEnd();
    glPopMatrix();

    // Draw control points if enabled
    if (showControlPoints)
    {
        glPointSize(3.0f);
        glBegin(GL_POINTS);
            for (int i = 0; i <= 4; ++i)
            {
                glVertex2f(controlPointsX[i], controlPointsY[i]);
            }
        glEnd();
    }

    // Draw bezier curve and sprite
    if (controlPointIndex > 3)
    {
        glPushMatrix();
            bezierCurveDraw();
        glPopMatrix();

        glPushMatrix();
            renderSpriteAlongBezierCurve();
        glPopMatrix();
    }

    // Swap the buffers to display the scene
    glutSwapBuffers();
}

/// Resets the scene to its initial state
void sceneResetting()
{
    // Initialize control points to default positions
    controlPointsX[0] = -21.72;
    controlPointsY[0] = -5;
    controlPointsX[4] = 21.73;
    controlPointsY[4] = -5;

    // Set all intermediate control points to the origin
    for (int i = 1; i < 4; ++i)
    {
        controlPointsX[i] = 0;
        controlPointsY[i] = 0;
    }

    // Reset control point index and rotation angles
    controlPointIndex = 1;
    RotateX = 0;
    RotateY = 0;
}

/// Handling Key Presses:
//The function is triggered whenever a key is pressed while the OpenGL window has focus.
// The specific key that was pressed is passed to the function as the parameter key.
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 : //the program will terminate.
        case 'q': //the program will terminate.
            exit(0);
            break;
        case 'w': //When the 'w' key is pressed, the display mode of the graphics is toggled between wireframe and normal rendering.
            WireFrame =!WireFrame;
            break;
        case ' ':     //Pressing the spacebar (' ') calls the sceneResetting() function.
            sceneResetting();
            break;
    }
}


/// Handle special key presses.
// It receives the key code (key) and the mouse position (x, y) at the time the key was pressed.
void Specialkeys(int key, int x, int y)
{
    // Start of switch statement to handle different special key presses
    switch(key)
    {
        // Case for handling the right arrow key press
        case GLUT_KEY_RIGHT:
            // Set direction of movement or orientation to the right
            directionX = +1;

            // Check if the sprite's animation parameter is less than its maximum value
            if(Sprite_Cal < 1.0){
                // Increment the sprite's animation parameter to move it forward
                Sprite_Cal += Speed_Animation_Curve;

                // Set the texture for the sprite to the one that indicates movement
                sprite_tex = moving_sprite_tex;

                // Set the total number of frames in the sprite's animation
                totalAnimationFrames = 10;
            }
            // Clamp the sprite's animation parameter to its maximum value if exceeded
            else if(Sprite_Cal > 1.0){
                Sprite_Cal = 1.0;
            }
            break;

        // Case for handling the left arrow key press
        case GLUT_KEY_LEFT:
            // Set direction of movement or orientation to the left
            directionX = -1;

            // Check if the sprite's animation parameter is greater than its minimum value
            if(Sprite_Cal > 0.0){
                // Decrement the sprite's animation parameter to move it backward
                Sprite_Cal -= Speed_Animation_Curve;

                // Set the texture for the sprite to the one that indicates movement
                sprite_tex = moving_sprite_tex;

                // Set the total number of frames in the sprite's animation
                totalAnimationFrames = 10;
            }
            // Clamp the sprite's animation parameter to its minimum value if it goes below zero
            else if(Sprite_Cal < 0.0){
                Sprite_Cal = 0.0;
            }
            break;

        // Default case to handle other special keys
        default:
            // No action is performed for other keys
            break;
   }

   // Instruct GLUT to redraw the scene to reflect changes from key presses
   glutPostRedisplay();
}


/// Handling Key Releases:
// In both the GLUT_KEY_RIGHT and GLUT_KEY_LEFT cases, the function changes the sprite's texture to idle_sprite_tex, which likely represents a static or non-animated state of the sprite.
// This change suggests that when the user stops pressing either of the arrow keys, the sprite should stop moving or animating and revert to an idle state.
void SpecialkeysUp(int key, int x, int y)
{
    // Check which special key is released
    switch (key)
    {
        case GLUT_KEY_RIGHT:  // If the right arrow key is released
            sprite_tex = idle_sprite_tex;  // Change the sprite texture to the idle texture
            totalAnimationFrames = 1;      // Set the total animation frames to 1, implying no animation
            break;                         // Exit the switch statement

        case GLUT_KEY_LEFT:   // If the left arrow key is released
            sprite_tex = idle_sprite_tex;  // Similar to the right arrow, change to the idle texture
            totalAnimationFrames = 1;      // Set animation frames to 1, stopping any ongoing animation
            break;                         // Exit the switch statement
    }
    // Note: There is no default case as only specific keys are handled
}


static void idle(void)
{
    glutPostRedisplay();
}


/// Handle mouse input for interactive graphics applications,
// allowing the user to set control points for a Bezier curve and toggle the visibility of these points
void mouse(int btn, int state, int xPos, int yPos)
{
    // Calculate the scale factor based on window dimensions
    float scaleFactor = 13* (Wwidth / Wheight);

    // Check which mouse btn was pressed
    switch (btn)
    {
        case GLUT_LEFT_BUTTON:   // When the left mouse btn is clicked
            if (state == GLUT_DOWN) // Check if the btn is pressed down
            {
                // Clear the color and depth buffers
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Convert screen coordinates to world coordinates
                float mouseX = (xPos - (Wwidth / 2)) / scaleFactor;
                float mouseY = ((Wheight / 2) - yPos) / scaleFactor;

                // Store the new control point if less than four are set
                if (controlPointIndex < 4)
                {
                    controlPointsX[controlPointIndex] = mouseX;
                    controlPointsY[controlPointIndex] = mouseY;
                    controlPointIndex++;
                    Sprite_Cal = 0;  // Reset the sprite calculation variable

                    // Initialize the first value for generating Bezier coefficients
                    int previousValue = 1;
                    bezierCoefficients[0] = previousValue;

                    // Generate Pascal's triangle row for Bezier curve coefficients
                    for (int i = 1; i <= controlPointIndex; ++i)
                    {
                        int currentValue = (previousValue * (controlPointIndex - i + 1)) / i;
                        previousValue = currentValue;
                        bezierCoefficients[i] = currentValue;
                    }
                }
            }
            break;

        case GLUT_RIGHT_BUTTON:  // When the right mouse btn is clicked
            if (state == GLUT_DOWN) // Check if the btn is pressed down
            {
                // Toggle the visibility of control points
				if(showControlPoints){
                    showControlPoints = false;
                }
                else{
                    showControlPoints = true;
                }            }
            break;
    }

    // Request a redraw of the scene
    glutPostRedisplay();
}

// Calculate the change in mouse position to update the rotation angles (RotateX and RotateY) of a scene.
// It filters out minor mouse movements by only considering changes greater than a specified threshold, providing smoother and more intentional interactive rotations.
void mouseMove(int currentX, int currentY)
{
    // Static variables to hold the previous mouse position
    static float previousX = 0.0;
    static float previousY = 0.0;

    // Calculate the change in mouse position
    float deltaX = static_cast<float>(currentX) - previousX;
    float deltaY = static_cast<float>(currentY) - previousY;

    // Check if the mouse movement is significant (more than 15 units in any direction)
    if (std::abs(static_cast<int>(deltaX)) > 15 || std::abs(static_cast<int>(deltaY)) > 15)
    {
        // Update the previous mouse position and return early
        previousX = static_cast<float>(currentX);
        previousY = static_cast<float>(currentY);
        return;
    }

    // Update rotation angles based on the mouse movement
    RotateY = (RotateY + static_cast<int>(deltaX)) % 360;
    RotateX = (RotateX + static_cast<int>(deltaY)) % 360;

    // Update the previous mouse position for the next call
    previousX = static_cast<float>(currentX);
    previousY = static_cast<float>(currentY);
}


static void init(void)
{
    //The scene is first reset to a default state to ensure a clean starting point.
    sceneResetting();

    //A parallax background texture is loaded to create a sense of depth.
    loadTexture("Parallax_background.png", parallex_tex);
    //A sprite sheet containing an animated character is loaded for character animations.
    loadTexture("SpriteSheet.png", moving_sprite_tex);
    //A single frame from the sprite sheet is used for the character's idle (standing) animation.
    loadTexture("Neutral_Pose.png", idle_sprite_tex);

    //The character's initial animation state is set to 'idle or Neutral Pose', with a single frame indicating no movement.
    sprite_tex = idle_sprite_tex;
    totalAnimationFrames = 1;

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f); // assign a color you like

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
   // glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(750,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 5 - Bezier Curve 2D Animation");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutSpecialUpFunc(SpecialkeysUp);
    glutMotionFunc(mouseMove);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
