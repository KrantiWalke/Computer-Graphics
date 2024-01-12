/// Author Name: Kranti Walke
/// Project 6: Shaders


///Rendering Process:
/*  ->The program uses shaders for rendering. Vertex shader (vs) and fragment shader (fs) are compiled and linked into a shader program (program).
    ->Textures are loaded and can be modified in real-time based on user input to change colors.
    ->The scene is rendered with lighting and material properties, and transformations like scaling and rotation are applied based on user input.
*/

///Interaction:
/*    ->The user can interact with the application via keyboard inputs,
                ->->->changing how the scene is rendered (like toggling wireframe mode,
                                                          scaling the object,
                                                          changing texture colors,
                                                          or rotating the object).
*/

///Shaders:
/*   ->The shaders (v1.vs for the vertex shader and f1.fs for the fragment shader) are crucial for rendering.
    ->The vertex shader typically handles vertex transformations, while the fragment shader handles per-pixel operations like texture mapping and color manipulation.
*/

///TASKS DONE:
/*
1. Drew a teapot and wrap a texture around it.
2. Loaded vertex shader and fragment shader files.
3. Implemented the loadFile function to read your v.vs and f.fs text files and load shader source code.
4. Implemented the loadShader function to compile shaders and return their IDs.
5. Implemented the initShader function to initialize and link vertex and fragment shaders into a shader program.
6. Implemented the clean function to detach and delete shaders and the shader program.

7. Arrow keys to rotate your model using the Specialkeys function:
    ->Right arrow key: Increase rotation around the Y-axis.
    ->Left arrow key: Decrease rotation around the Y-axis.
    ->Up arrow key: Increase rotation around the X-axis.
    ->Down arrow key: Decrease rotation around the X-axis.

8. Implemented key presses for the following actions:
    ->Key 1: Replace all red color of the texture with blue.
    ->Key 2: Replace all green color of the texture with blue.
    ->Key 3: Reset the texture colors to their original values.
    ->Key A: Scale down X coordinates of the teapot.
    ->Key D: Scale up X coordinates of the teapot.
    ->Key W: Scale up Y coordinates of the teapot.
    ->Key S: Scale up Z coordinates of the teapot.
    ->Key Spacebar: Reset the sizes of the teapot.
*/

#include <string.h>
#define GLEW_STAIC
#include<GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>
#include <fstream>

#define PI 3.14159

using namespace std;
//unsigned integers used to store the IDs of the vertex shader (vs), fragment shader (fs), and the shader program (program), respectively.
unsigned int vs,fs,program;

unsigned char* textureData;  // Global variable to store texture data
int texWidth, texHeight;     // Texture dimensions

int colorMode = 0; // 0 for normal, 1 for red to blue, 2 for green to blue

// for scaling transformations in the rendered scene.
float scl=1.0;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

bool WireFrame= false;

float i=0;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//const GLubyte* openglVersion = glGetString(GL_VERSION);
//Floats used for storing positions and dimensions, potentially for the window or viewport.
float xpos=0;
float ypos=0;
float Wwidth,Wheight;

int RotateX=0;
int RotateY=0;

//texture identifier
GLuint tex1;

/// -----------------------CODE STARTS HERE-----------------------------------------------------

/// Sets the viewport and projection matrix, usually called when the window is resized.
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

/// Reads shader source code from a file into a string.
void loadFile(const char* fileName, string &str) {
    // Open the file stream with the given filename
    ifstream in(fileName);

    // Temporary buffer to store each line read from the file
    char tmp[1024];

    // Check if the file was successfully opened
    if (!in.is_open()) {
        // If the file could not be opened, print an error message
        cout << "The File " << fileName << " - Not Found" << endl;
    } else {
        // Read the file until the end of file (EOF) is reached
        while (!in.eof()) {
            // Read a line from the file into the temporary buffer
            in.getline(tmp, 1024);
            // Append the line to the output string 'str'
            str += tmp;
            // Append a newline character to 'str' after each line
            str += '\n';
        }
    }
}

/// Compiles a shader from its source code and returns its ID.
unsigned int loadShader(string& source, unsigned int mode)
{
    // Variable to store the shader ID
    unsigned int id;

    // Array to store error messages from shader compilation
    char error[1024];

    // Create a shader object of the given type (vertex or fragment)
    id = glCreateShader(mode);

    // Convert the shader source code from string to C-style string
    const char* cSrc = source.c_str();

    // Associate the shader source code string with the shader object
    glShaderSource(id, 1, &cSrc, NULL);

    // Compile the shader
    glCompileShader(id);

    // Retrieve any error messages or logs from the shader compilation process
    glGetShaderInfoLog(id, 1024, NULL, error);

    // Output the compilation status and any error messages
    //cout << "Compile status: \n" << error << endl;

    // Return the shader ID
    return id;
}


/// Initializes and links vertex and fragment shaders into a shader program.
void initShader(const char* vName, const char* fName, unsigned int& program)
{
    // Variable to store shader source code
    string source;

    // Check if a vertex shader name was provided
    if (vName) {
        // Load the vertex shader source code from a file
        loadFile(vName, source);
        // Compile the vertex shader and store its ID in 'vs'
        vs = loadShader(source, GL_VERTEX_SHADER);
        // Clear the source string for potential reuse
        source = "";
    }

    // Check if a fragment shader name was provided
    if (fName) {
        // Load the fragment shader source code from a file
        loadFile(fName, source);
        // Compile the fragment shader and store its ID in 'fs'
        fs = loadShader(source, GL_FRAGMENT_SHADER);
    }

    // Create a shader program
    program = glCreateProgram();
    // Attach the vertex shader to the program
    glAttachShader(program, vs);
    // Attach the fragment shader to the program
    glAttachShader(program, fs);

    // Link the shader program
    glLinkProgram(program);
    // Set the shader program as the current program
    glUseProgram(program);
    // The following lines are commented out - they would detach the shaders
    // glDetachShader(program, vs);
    // glDetachShader(program, fs);
}

/// Detaches shaders from the program and deletes them, used for cleanup.
void clean() {
    // Detach the vertex shader from the program
    glDetachShader(program, vs);
    // Detach the fragment shader from the program
    glDetachShader(program, fs);
    // Delete the vertex shader to free up resources
    glDeleteShader(vs);
    // Delete the fragment shader to free up resources
    glDeleteShader(fs);
    // Delete the shader program to free up resources
    glDeleteProgram(program);
}


/// Loads a texture from a file, updates textureData, and sets texture parameters.
void TLoad(char* fileName, GLuint &tex)
{
    // Bind the texture object to the 2D texture target
    glBindTexture(GL_TEXTURE_2D, tex);

    // Load the image from the file using SOIL (Simple OpenGL Image Library)
    // SOIL_load_image will return a pointer to the loaded image data
    // and also set the width and height of the texture
    textureData = SOIL_load_image(fileName, &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);

    // Check if the texture data was loaded successfully
    if (!textureData) {
        // If the texture data was not loaded, print an error message
        cout << "Image not found: " << fileName << endl;
    }
    else {
        // Define the 2D texture image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        // Free the image data as it's no longer needed after glTexImage2D
        SOIL_free_image_data(textureData);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}



/// (Optional) on CPU side: Modifies the texture data based on the colorMode.
void ModifyTextureData(bool redToBlue, bool greenToBlue, bool reset = false) {
    // Check if the reset flag is true
    if (reset) {
        // Reload the original texture if reset is requested
        TLoad("teapot.jpg", tex1);
        // Exit the function early as no further processing is needed
        return;
    }
    // Iterate over each pixel in the texture
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            // Calculate the index in the texture data array
            // Assuming 4 components per pixel (RGBA)
            int index = (y * texWidth + x) * 4;
            // Check if red to blue conversion is requested and if the red component is dominant
            if (redToBlue && textureData[index] > 128) {
                // Set the red component to 0 (removing red)
                textureData[index] = 0;
                // Set the blue component to maximum (255), effectively turning red to blue
                textureData[index + 2] = 255;
            }
            // Check if green to blue conversion is requested and if the green component is dominant
            if (greenToBlue && textureData[index + 1] > 128) {
                // Set the green component to 0 (removing green)
                textureData[index + 1] = 0;
                // Set the blue component to maximum (255), effectively turning green to blue
                textureData[index + 2] = 255;
            }
        }
    }
}


///  (Optional) on CPU side:  Updates the texture in OpenGL with any changes made to textureData.
void UpdateTexture() {
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}


/// The main rendering function; called to draw each frame.
static void display(void)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the current matrix mode to modelview
    glMatrixMode(GL_MODELVIEW);
    // Reset the modelview matrix
    glLoadIdentity();

    // Use the shader program for rendering
    glUseProgram(program);

    // Set up the camera position and orientation
    gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 100.0);

    // Check if wireframe mode is enabled
    if (WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw in wireframe mode
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Draw in fill mode (default)

    // Apply scaling transformation
    glScalef(scaleX, scaleY, scaleZ);

    // Set the current color to red
    glColor3f(1.0, 0.0, 0.0);

    // Apply rotation transformations
    glRotated(RotateX, 1, 0, 0); // Rotate around the X-axis
    glRotated(RotateY, 0, 1, 0); // Rotate around the Y-axis

    // Bind the texture for use
    glBindTexture(GL_TEXTURE_2D, tex1);

    // Draw a solid teapot
    glutSolidTeapot(2.0);

    // Get the location of the 'scale' uniform in the shader and set its value
    GLint loc = glGetUniformLocation(program, "scale");
    if (loc != -1) {
        glUniform1f(loc, scl);
    }

    // Get the location of the 'colorMode' uniform in the shader and set its value
    GLint colorModeLoc = glGetUniformLocation(program, "colorMode");
    glUniform1i(colorModeLoc, colorMode);

    // Stop using the shader program
    glUseProgram(0);

    // Swap the front and back buffers (double buffering)
    glutSwapBuffers();
}

///  Handles keyboard input for scaling, changing color modes, and other actions.
static void key(unsigned char key, int x, int y) {
    // Switch statement to handle various key presses
    switch (key) {
        case 27:  // ASCII value for the ESC key
        case 'q': // 'q' key
            clean();  // Call the clean function to free resources
            exit(0);  // Exit the program
            break;

        case 'w': // 'w' key
            scaleY += 0.1f;  // Increase Y scaling factor
            break;
        case 's': // 's' key
            scaleZ += 0.1f;  // Increase Z scaling factor
            break;
        case 'a': // 'a' key
            // Decrease X scaling factor, with a minimum limit of 0.1
            scaleX = max(0.1f, scaleX - 0.1f);
            break;
        case 'd': // 'd' key
            scaleX += 0.1f;  // Increase X scaling factor
            break;

        case '1': // '1' key
            // Set color mode to 1 (change red to blue)
            colorMode = 1;
            break;
        case '2': // '2' key
            // Set color mode to 2 (change green to blue)
            colorMode = 2;
            break;
        case '3': // '3' key
            // Reset color mode to 0 (original colors)
            colorMode = 0;
            break;

        case ' ': // Spacebar key
            // Reset all scaling factors to 1.0
            scaleX = scaleY = scaleZ = 1.0f;
            break;
    }
    // Trigger a redraw after a key is pressed
    glutPostRedisplay();
}


/// Specialkeys: Handles special key inputs like arrow keys for rotation.
void Specialkeys(int key, int x, int y)
{
    // Switch statement to check which key was pressed
    switch (key) {
        case GLUT_KEY_RIGHT:
            // If the right arrow key is pressed, increase rotation around Y-axis
            RotateY += 5;
            break;
        case GLUT_KEY_LEFT:
            // If the left arrow key is pressed, decrease rotation around Y-axis
            RotateY -= 5;
            break;
        case GLUT_KEY_UP:
            // If the up arrow key is pressed, increase rotation around X-axis
            RotateX += 5;
            break;
        case GLUT_KEY_DOWN:
            // If the down arrow key is pressed, decrease rotation around X-axis
            RotateX -= 5;
            break;
    }
    // Redraw the scene with the updated rotations
    glutPostRedisplay();
}


/// The idle function is called whenever the application is idle
static void idle(void)
{
    // Increment the scale (scl) variable slightly
    // This could be used for a continuous effect like pulsing or breathing
    scl += 0.0005;

    // Request a redisplay
    // This tells GLUT to call the display function again,
    // leading to continuous animation as long as the application is idle
    glutPostRedisplay();
}



/// Function to handle mouse button events
void mouse(int btn, int state, int xPos, int yPos)
{
    // Calculate a scaling factor based on the window dimensions
    float scale = 11 * (Wwidth / Wheight);

    // Switch statement to handle different mouse buttons
    switch (btn) {
        case GLUT_LEFT_BUTTON: // Check if the left mouse button was pressed or released
            if (state == GLUT_DOWN) { // Check if the button is in the 'down' state
                // If the left button was pressed, you can add code here to handle the event.
                // For example, you might want to record the mouse position, start a drag operation, etc.
                // This is where you could use the 'scale' variable, perhaps to adjust how much the scene zooms in or out.
            }
            break;
        // You can add cases for other mouse buttons (e.g., GLUT_RIGHT_BUTTON) here if needed.
    }

    // After handling the mouse event, request a redraw of the scene
    glutPostRedisplay();
};


/// Function to handle mouse movement for rotation
void mouseMove(int x, int y) {
    // Static variables to keep track of the previous mouse position
    static float prev_x = 0.0;
    static float prev_y = 0.0;

    // Calculate the change in mouse position since the last event
    prev_x = (float)x - prev_x;
    prev_y = (float)y - prev_y;

    // Check if the change in position is greater than a threshold (15 here)
    // This can prevent small, unintentional movements from affecting the rotation
    if ((abs((int)prev_x) > 15) || (abs((int)prev_y) > 15)) {
        // If the movement is too large, reset the previous position
        // and exit the function without modifying the rotation
        prev_x = (float)x;
        prev_y = (float)y;
        return;
    }

    // Update the rotation angles based on the mouse movement
    // The change in mouse position is added to the current rotation
    // and then taken modulo 360 to keep the angles within a full rotation
    RotateY = (RotateY + (int)prev_x) % 360;
    RotateX = (RotateX + (int)prev_y) % 360;
}



/// Function to initialize OpenGL settings
static void init(void) {
    // Initialize GLEW, a library to handle OpenGL extensions
    glewInit();
    // Set the clear color (background color) of the OpenGL context
    glClearColor(0.5f, 0.5f, 1.0f, 0.0f); // Set to a light blue color
    // Enable automatic normalization of normals
    glEnable(GL_NORMALIZE);
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    // Set the quality of color, texture coordinate, and fog coordinate interpolation
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // Set the shading model to smooth
    glShadeModel(GL_SMOOTH);
    // Set up the ambient light component of the light source
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    // Set up the diffuse light component (light color)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    // Set up the specular light component (shininess)
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    // Set the position of the light source
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    // Set material properties for the front face of polygons
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // Enable the first light source
    glEnable(GL_LIGHT0);
    // Enable lighting calculations
    glEnable(GL_LIGHTING);

    // Initialize shaders by loading, compiling, and linking them
    initShader("V1.vs", "F1.fs", program);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Generate texture object IDs
    glGenTextures(1, &tex1);

    // Enable blending for transparency
    glEnable(GL_BLEND);

    // Set blending function for source and destination factors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load the texture for the teapot
    TLoad("teapot.jpg", tex1);
}

/* Program entry point */

/// The main function, entry point of the application
int main(int argc, char *argv[]) {
    // Initialize GLUT library
    glutInit(&argc, argv);

    // Set initial window size to 800x600 pixels
    glutInitWindowSize(800, 600);
    // Set the initial position of the window (top-left corner of the screen)
    glutInitWindowPosition(0, 0);
    // Set the display mode: use RGBA colors, double buffering, and a depth buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // Create a window with a title
    glutCreateWindow("Project Assignment 6 - Shaders");
    // Call the initialization function to set up shaders and other settings
    init();
    // Register the function to be called when the window is resized
    glutReshapeFunc(resize);
    // Register the function to handle the display/rendering
    glutDisplayFunc(display);
    // Register the function to handle mouse button events
    glutMouseFunc(mouse);
    // Register the function to handle key presses
    glutKeyboardFunc(key);
    // Register the function to handle special key events (like arrow keys)
    glutSpecialFunc(Specialkeys);
    // Register the function to handle continuous motion when a mouse button is pressed
    glutMotionFunc(mouseMove);
    // Register the function to be called when the system is idle
    glutIdleFunc(idle);

    // Start the main loop of GLUT
    glutMainLoop();

    // Cleanup: delete the shaders and the shader program after the main loop ends
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);

    // Return success
    return EXIT_SUCCESS;
}

