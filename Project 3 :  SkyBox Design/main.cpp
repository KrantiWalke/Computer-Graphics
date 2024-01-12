/// Name: Kranti Sambhaji Walke
/// Project 3 - SkyBox Design


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
#include <SOIL.h>
#define PI 3.14159

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

///-----------------------My code starts Here for variable declaration-----------------
float Wwidth,Wheight; //store the width and height of the window.
float mouseX, mouseY; //store the current X and Y coordinates of the mouse.
float numbOfClicks; //counts the number of mouse clicks.
float MAX_CLICKS; //a constant defining the maximum number of clicks to be considered.

GLfloat camscene = 1.0f; //determine the scale of the scene.
//define various size parameters, for SkyBox,Room,Ground respectively.
GLfloat size1 = 600.0f;
GLfloat size2 = 60.0f;
GLfloat size3 = 150.0f;

// Sphere rotation parameters
GLfloat rotate_angle = 0.0f; //define the current rotation angle of a sphere.
GLfloat rotate_speed = 0.0f; //define the speed of rotation for the sphere.

// determine the position of the camera in the scene.
// The camera seems to start off slightly elevated and pulled back.
GLfloat Xcam = 0.0f;
GLfloat Ycam = -(size2/1.5f);
GLfloat Zcam = size3-3;

// define the movement speed of the camera along the X and Y axis respectively.
GLfloat cam_move_speed_X = 0.9f;
GLfloat cam_move_speed_Y = 0.9f;

// Identifiers (names) for OpenGL textures.
// They will be filled when textures are loaded and can be used to bind the textures when drawing.
GLuint front_tex, back_tex, right_tex, left_tex, top_tex, bottom_tex;
GLuint frontwall_tex, backwall_tex, rightwall_tex, leftwall_tex, ceiling_tex, floor_tex;
GLuint ground_tex, planetTexture;

//Declare a pointer to a quadric object named myplanet and initializes it to NULL.
//Quadric objects can represent various 3D shapes like spheres and cylinders.
GLUquadricObj *myplanet = NULL;

//store the translation values for moving in the scene.
GLdouble X_translate=0,Y_translate=0,Z_translate=0;

//store rotation angles around the X, Y, and Z axes respectively.
GLint RotateX,RotateY,RotateZ;

float light_angle = 0.0f; //the angle at which the light is positioned.
float light_distance = 5.0f; //the distance from the origin to the light position.

///-----------------------My code Ends Here for variable declaration-----------------

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
	 gluPerspective (60.0f,Ratio,0.1f, 1000.0f);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
 }

 ///-----------------------My code starts Here for function definitions and declarations-----------------

 ///Function declaration for textureLoader that takes in a file name (as a pointer to a char) and a reference to a GLuint for the texture.
void textureLoader(char* fileName, GLuint &tex)
{
     glEnable(GL_TEXTURE_2D);  //Enables 2D texturing. In OpenGL, when you want to apply 2D textures, you need to enable this mode.
     glGenTextures(1, &tex);

     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); //Sets the texture environment mode for the current texture. In this case, it sets the mode to GL_MODULATE, which multiplies the texture color by the fragment color. This can be used, for instance, to apply lighting to a textured object.
     glBindTexture(GL_TEXTURE_2D, tex); // Binds (or selects) the texture name previously generated so that subsequent texture commands will operate on this texture.
     int width, height; // Declare local variables to store the width and height of the image being loaded and a pointer for the image data itself.
     unsigned char* image;

     //Uses the SOIL (Simple OpenGL Image Library) function to load an image from the file specified by fileName.
     //The image data will be stored in the image pointer and the image's width and height will be stored in width and height respectively.
     //The image is loaded with an RGBA format.
     image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
     //Checks if the image was successfully loaded. If not (!image is true), it prints an error message to the console.
     if (!image)cout<<"Image not found  "<< fileName <<endl;

     //Specifies the 2D texture image. This command essentially sends the image data (image) to the GPU.
     //The image is specified in RGBA format with each component as an unsigned byte.
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // binding image data
     SOIL_free_image_data(image); //Frees the image data from memory since it's now been sent to the GPU.
     //Sets the texture minification filter to linear filtering.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     //Sets the S-axis (similar to the X-axis) wrap mode to repeat.
     //This means if texture coordinates go beyond [0, 1], the texture will repeat.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

/*
 static void setupCamera() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Xcam, Ycam, 200, 0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glTranslatef(X_translate, Y_translate, Z_translate);
    glScalef(camscene, camscene, camscene);
    glRotatef(25, RotateX, RotateY, 0.0f);
    glRotatef(rotate_angle, 1.0f, 1.0f, 0.0f);
}
*/

///Draw the front face of the skybox.
static void drawFrontFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, front_tex); // This binds the texture for the front face of the skybox so it can be applied to the quad that will be drawn.
        glBegin(GL_QUADS); //Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size1, -size1, size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size1, -size1, size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size1, size1, size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size1, size1, size1);  //Top Left
        glEnd();
}

///Draw the back face of the skybox.
static void drawBackFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, back_tex); // Binds the texture for the back face.
        glBegin(GL_QUADS);//Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size1, -size1, -size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size1, -size1, -size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size1, size1, -size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size1, size1, -size1);  //Top Left
        glEnd();
}

///Draw the right face of the skybox.
static void drawRightFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, right_tex); // Binds the texture for the right face.
        glBegin(GL_QUADS);//Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(size1, -size1, size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size1, -size1, -size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size1, size1, -size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(size1, size1, size1);  //Top Left
        glEnd();
}

///Draw the left face of the skybox.
static void drawLeftFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, left_tex); // Binds the texture for the left face.
        glBegin(GL_QUADS);//Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size1, -size1, size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(-size1, -size1, -size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(-size1, size1, -size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size1, size1, size1);  //Top Left
        glEnd();
}

///Draw the top face of the skybox.
static void drawTopFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, top_tex); // Binds the texture for the top face.
        glBegin(GL_QUADS);//Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size1, size1, -size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size1, size1, -size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size1, size1, size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size1, size1, size1);  //Top Left
        glEnd();
}

///Draw the bottom face of the skybox.
static void drawBottomFaceSkyBox() {
    glBindTexture(GL_TEXTURE_2D, bottom_tex); // Binds the texture for the bottom face.
        glBegin(GL_QUADS);//Specifies that the following set of vertices will define a quad (a four-sided polygon).
        //define the texture coordinates and vertices for the front face quad, respectively.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size1, -size1, -size1);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size1, -size1, -size1);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size1, -size1, size1);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size1, -size1, size1);  //Top Left
        glEnd();
}

///Draw a textured quad (a four-sided polygon) using a given set of vertices and a texture.
//GLuint texture: The texture ID to be applied to the quad.
//GLfloat size: Not actually used in this function. Presumably, it's there for consistency with the other functions, but it could be removed without affecting the function's behavior.
//GLfloat vertices[4][3]: A 2D array specifying the 3D coordinates of the quad's four corners.
void drawRoomFace(GLuint texture, GLfloat size, GLfloat vertices[4][3]) {
    glBindTexture(GL_TEXTURE_2D, texture); //Binds the given texture.
     //The loop iterates over the vertices and maps them to the corresponding texture coordinates.
     //Texture coordinates range from 0.0 to 1.0, and the ternary conditions used help map each vertex to its respective texture coordinate.
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2f((i == 1 || i == 2) ? 1.0 : 0.0, (i > 1) ? 1.0 : 0.0);
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
    }
    glEnd();
}


///drawBottomFace(), drawBackFace(), drawRightFace(), drawLeftFace(), drawTopFace(), and drawFrontFace()
/*
Purpose: Each of these functions draws a specific face (bottom, back, right, left, top, front) of a room.

GLfloat size: Determines the dimensions of the quad.

Each function defines the vertices for its respective face in the vertices array.
Calls drawRoomFace() to actually draw the face with the appropriate texture (like floor_tex for bottom, backwall_tex for back, and so on).
*/
void drawBottomFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {-size, -size, -size},
        {size, -size, -size},
        {size, -size, size},
        {-size, -size, size}
    };
    drawRoomFace(floor_tex, size, vertices);
}

void drawBackFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {-size, -size, -size},
        {size, -size, -size},
        {size, size, -size},
        {-size, size, -size}
    };
    drawRoomFace(backwall_tex, size, vertices);
}

void drawRightFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {size, -size, size},
        {size, -size, -size},
        {size, size, -size},
        {size, size, size}
    };
    drawRoomFace(rightwall_tex, size, vertices);
}

void drawLeftFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {-size, -size, size},
        {-size, -size, -size},
        {-size, size, -size},
        {-size, size, size}
    };
    drawRoomFace(leftwall_tex, size, vertices);
}

void drawTopFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {-size, size, -size},
        {size, size, -size},
        {size, size, size},
        {-size, size, size}
    };
    drawRoomFace(ceiling_tex, size, vertices);
}

void drawFrontFace(GLfloat size) {
    GLfloat vertices[4][3] = {
        {-size, -size, size},
        {size, -size, size},
        {size, size, size},
        {-size, size, size}
    };
    drawRoomFace(frontwall_tex, size, vertices);
}

///Draw a textured quad that represents the ground. This seems to be positioned slightly below the bottom face of the room.
void drawGround() {
        glBindTexture(GL_TEXTURE_2D, ground_tex); // Binds the ground texture.
        glBegin(GL_QUADS); //Enclose the drawing commands for the quad.
        //The y-coordinate value (-(size2+0.1)) is slightly lower than the size2 variable,
        //so the ground is positioned a bit lower than the bottom face of the room.
            glTexCoord2f(0.0, 0.0); glVertex3f(-size3, -(size2+0.1), -size3);  //Bottom Left
            glTexCoord2f(1.0, 0.0); glVertex3f(size3, -(size2+0.1), -size3);  //Bottom Right
            glTexCoord2f(1.0, 1.0); glVertex3f(size3, -(size2+0.1), size3);  //Top Right
            glTexCoord2f(0.0, 1.0); glVertex3f(-size3, -(size2+0.1), size3);  //Top Left
        glEnd();
}

///This function renders the skybox,
//which is essentially a large cube that surrounds the entire scene and provides the illusion of distant surroundings.

static void drawSkyBox() {
    glDisable(GL_LIGHTING); //This turns off lighting calculations. This is because the skybox typically doesn't need to react to scene lights  it just needs to display its textures as they are.

    glColor3f(1.0f, 1.0f, 1.0f); //This sets the current drawing color to white. In the context of textured geometry, this means the texture will be drawn with its original colors, without any tint.

    //Calls to drawFrontFaceSkyBox(), drawBackFaceSkyBox(), etc.: These render each face of the skybox cube.
    drawFrontFaceSkyBox();
    drawBackFaceSkyBox();
    drawRightFaceSkyBox();
    drawLeftFaceSkyBox();
    drawTopFaceSkyBox();
    drawBottomFaceSkyBox();

    glEnable(GL_LIGHTING); // This turns lighting calculations back on so that other objects in the scene, which are drawn after the skybox, will respond to lights as expected.
}

///This function renders the interior of a room, complete with textured walls, ceiling, floor, and an additional ground layer.
void drawRoom() {
    glDisable(GL_LIGHTING); //Again, lighting is turned off. This may suggest that you want the room to display its textures without being affected by scene lights.
    glColor3f(1.0f, 1.0f, 1.0f); //Sets the drawing color to white.

    //Calls to drawBottomFace(size2), drawBackFace(size2), etc.
    //These render each face of the room, with the size of the room being defined by size2.
    drawBottomFace(size2);
    drawBackFace(size2);
    drawRightFace(size2);
    drawLeftFace(size2);
    drawTopFace(size2);
    drawFrontFace(size2);
    //Draw the ground layer beneath the room, possibly to represent an exterior or foundational layer.
    drawGround();
}

///This function renders a textured sphere to represent a planet.
static void drawPlanet() {
    //Definition of the planet's position:
    //The planetX, planetY, and planetZ variables determine where the planet will be placed in the scene.
    float planetX = 150.0f;
    float planetY = 150.0f;
    float planetZ = -50.0f;
    GLfloat planetSize = 20.0f;
    glBindTexture(GL_TEXTURE_2D, planetTexture); //This binds the texture that will be used for the planet.
    //glPushMatrix() and glPopMatrix() are used to isolate the transformations (like translation) applied to the planet so they don't affect subsequent drawing operations.
    glPushMatrix();
    glTranslatef(planetX, planetY, planetZ); //Moves the current drawing location to where the planet should be drawn.
    //Render the planet as a sphere with a given size (planetSize) and resolution (40x40 segments).
    //The myplanet parameter is presumably a quadric object, which gluSphere uses to determine specific properties of the sphere, such as normals.
    gluSphere(myplanet, planetSize, 40, 40);
    glPopMatrix();
}

///This function positions a light source in the scene and makes it revolve around a fixed point (in this case, the origin). The light moves in a circular path on the horizontal plane (i.e., the XY-plane).
//The positionLight() function makes a light source revolve around the origin in a circle on the horizontal plane.
//This revolving light can cast dynamic and changing shadows in your scene, adding visual interest and dynamism.
static void positionLight() {
    float lightPosX = light_distance * sinf(light_angle);
    float lightPosY = 0.0f;  // keeping it on the same plane for simplicity
    float lightPosZ = light_distance * cosf(light_angle);

    GLfloat light_position[] = { lightPosX, lightPosY, lightPosZ, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    light_angle += 0.01f; // Adjust the speed of rotation as needed
    if (light_angle > 2 * M_PI) light_angle -= 2 * M_PI;
}


///This function is typically called whenever the OpenGL window needs to be redrawn, whether due to being moved, resized, or any other reason that requires a refresh.
///It's where the scene is rendered using OpenGL commands.
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Sets up the camera's position (Xcam, Ycam, Zcam),
    //where the camera is looking (at the origin 0.0, 0.0, 0.0), and which direction is up (0.0, 1.0, 0.0).
    gluLookAt(Xcam, Ycam, Zcam, 0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    /// My code here

    glTranslatef(X_translate, Y_translate, Z_translate);  //This function translates (moves) the scene along the X, Y, and Z axes.Using A,W,S,D,E,R
    glScalef(camscene, camscene, camscene); // This function scales the scene, effectively zooming in or out.

    //The two glRotatef(...) calls rotate the scene.
    //One is based on mouse input (RotateX, RotateY),
    //and the other based on keyboard input (rotating around a diagonal axis with rotate_angle).
    glRotatef(25, RotateX, RotateY, 0.0f);  //Rotate using mouse
    glRotatef(rotate_angle, 1.0f, 1.0f, 0.0f);  //Rotate using j,k,i,l

   // setupCamera();
    drawSkyBox(); //Draws the skybox, which provides a distant background.
    drawPlanet(); //Draws a planet object in the scene.
    positionLight(); //Sets the position and movement of a light source.
    drawRoom(); //Draws the room in the scene.

    glutSwapBuffers();
}

///This function is designed to handle keyboard inputs.
///When a key is pressed, this function is called with the key code and the current mouse position (x and y).
///Inside, different actions are executed based on which key is pressed.
static void key(unsigned char key, int x, int y)
{
    //This structure allows the function to test for various key presses and react accordingly.
    switch (key)
    {
        ///Both are used to exit the program.
        //27 represents the ASCII value for the escape key (Esc).
        //'q' is also an option for quitting.
        case 27 :
            exit(0);
            break;

        case 'q':
            exit(0);
            break;

        ///Pressing the spacebar resets the camera and other transformation parameters to their default values.
        case 32 :
            Xcam = 0;
            Ycam = -(size2/1.5f);
            X_translate = 0;
            Y_translate = 0;
            Z_translate = 3;
            RotateX = 0 ;
            RotateY = 0;
            camscene = 1.0f;
            break;

        ///These control the zoom.
        //Pressing 'z' zooms in,
        //and pressing 'x' zooms out.
        case 'z':
            camscene += 0.5;
            break;

        case 'x':
            camscene -= 0.5;
            break;

        ///'j', 'k', 'i', 'l':
        //These keys are used to rotate the camera view.
        case 'j': // Right Rotate
            Xcam += cam_move_speed_X;
            break;

        case 'k': // Left Rotate
            Xcam -= cam_move_speed_X;
            break;

        case 'i': // Top Rotate
            Ycam += cam_move_speed_Y;
            break;

        case 'l': // Bottom Rotate
            Ycam -= cam_move_speed_Y;
            break;

        /// To reset or adjust the camera's position in the X and Y axes.
        //It centralizes the camera on the X-axis and adjusts its height based on size2.
        //This could be useful for quickly reorienting the viewer to a specific perspective or viewpoint in the scene without affecting the Z-coordinate or any rotation parameters.
        //It appears to place the camera at a location that may represent a standard or neutral viewpoint for the user.
        case 'o':
            Xcam = 0;
            Ycam = -(size2/1.5f);
            break;

        ///w', 's', 'a', 'd':
        //These are the standard keys for movement in 3D applications: forward (w), backward (s), left (a), and right (d).
        //The corresponding actions involve complex conditions to possibly limit movement within certain boundaries in the scene.

        case 'w': ///'w' key moves the viewer (or camera) forward in the scene.
            if(Z_translate <= (2*size3-10)) //The movement is done by adjusting the Z_translate variable (which presumably affects the Z-coordinate of the camera's position). The conditional checks (if statements) are there to ensure that the camera doesn't move beyond specific limits or bounds, thus preventing the viewer from going to undesired places in the scene.
            {
                if(Z_translate >= 80 && (X_translate < -48 || X_translate > 49))
                {
                    Z_translate +=1;
                }
                else if( Z_translate > 75 && (X_translate > -48 && X_translate < 49) )
                {
                    if(Z_translate < 190)
                    {
                        Z_translate += 1;
                    }
                }
                else if( (X_translate < 20 && X_translate > -1*(size2/3)) && Z_translate == 75)
                {
                    Z_translate += 1;
                }
                else if( Z_translate < 80)
                {
                    Z_translate +=1;
                }
            }
            break;

        case 's': ///'s' key moves the viewer backward in the scene.
            if( Z_translate > 99 && (X_translate > -48 && X_translate < 49) ) //Similar to the 'w' case, the movement is done by adjusting the Z_translate variable. But this time, it's decreasing its value to move backward.The conditional checks are present to ensure the camera doesn't move beyond defined limits.
            {
                Z_translate -= 1;
            }
            else if(Z_translate < 76)
            {
                Z_translate -= 1;
            }
            else if( (X_translate < 20 && X_translate > -1*(size2/3)) && Z_translate <= 99)
            {
                Z_translate -= 1;
            }
            break;

        case 'a': ///'a' key moves the viewer to the left.
            if( (Z_translate > 99 && Z_translate <= 190)) //This movement is accomplished by adjusting the X_translate variable, increasing its value. The if statements make sure that the camera stays within the bounds on the X-axis.
            {
                if( X_translate < 50 && X_translate > -48 )
                {
                    X_translate += 1;
                }
            }
            else if(X_translate <= (0.85*size3))
            {
                X_translate += 1;
            }
            break;

        case 'd': ///'d' key moves the viewer to the right.
            if( (Z_translate > 99 && Z_translate <= 176)) //As with the 'a' case, the movement is done by adjusting the X_translate variable. Here, it decreases the variable to move to the right. The conditional checks ensure the camera remains within specific boundaries.
            {
                if(X_translate < 50 && X_translate > -48){X_translate -=1;}
            //Debugging purpose:
            printf("%f",Z_translate);
            printf("   ");
            printf("%f", X_translate);
            printf("\n");
            }
            else if(X_translate >= -(0.85*size3))
            {
                X_translate -= 1;
            }
            break;

        ///If a key that doesn't match any of the specified cases is pressed, the default case will catch it, and nothing will happen.
        default:
            break;
    }
}

///This function is for handling special keys like arrow keys, function keys, etc.
//The GLUT_KEY_RIGHT and GLUT_KEY_LEFT are constants representing the right and left arrow keys, respectively.
//Currently, both these cases are empty, meaning no action is being taken if these keys are pressed.
void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_LEFT:
            break;
   }
  glutPostRedisplay();
}

///This function appears to be the idle callback for a GLUT program.
//When GLUT is idle (i.e., no events to process), this function will be called.
static void idle(void)
{
    // Update a variable called rotate_angle by adding rotate_speed to it.
    //This suggests there's a rotation animation taking place, and rotate_speed controls how fast this rotation happens.
    rotate_angle += rotate_speed;

    // Ensure the rotate_angle stays between 0 and 360 degrees, wrapping it around when it exceeds this range.
    if (rotate_angle >= 360.0f)
        rotate_angle -= 360.0f;

    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
     glutPostRedisplay();
}

///This function is a mouse movement handler.
void mouseMove(int x, int y)
{
    //The static variables prev_x and prev_y are used to store the previous positions of the mouse.
    static float prev_x =0.0;
    static float prev_y =0.0;

    //The function calculates the difference between the current mouse position (x, y) and the previous position (prev_x, prev_y).
    prev_x = (float)x-prev_x;
    prev_y = (float)y-prev_y;

    //The following if statement checks if the mouse has moved significantly (more than 15 pixels in either x or y direction) since the last call.
    //If it has, the function resets prev_x and prev_y to the current mouse position and returns without making further changes.
    if((abs((int)prev_x)>15)||(abs((int)prev_y)>15))
    {
        prev_x = (float)x;
        prev_y = (float)y;
        return;
    }

    //If the mouse movement is within the threshold, the function modifies the RotateY and RotateX variables based on the calculated difference in mouse positions.
    //These variables likely control the rotation of some object or view in the scene.
    //The % 360 ensures the rotation values are kept within the range of 0 to 360 degrees.
    RotateY = (RotateY+(int)prev_x)%360;
    RotateX = (RotateX+(int)prev_y)%360;
}

///This function is likely a callback for mouse button events in a GLUT (OpenGL Utility Toolkit) program.
///It's triggered whenever a mouse button is pressed or released. The function is provided with several pieces of information:
//btn: Indicates which mouse button event has occurred (e.g., left, middle, or right button).
//state: Specifies the state of the button (pressed down or released).
//x, y: The screen coordinates where the mouse event took place.

//The function begins by calculating a scale factor based on Wwidth and Wheight, which are probably global or external variables representing the width and height of the window.
//This scale factor appears to normalize or scale mouse coordinates to some desired range or aspect ratio.
void mouse(int btn, int state, int x, int y){
    float scale = 54*(Wwidth/Wheight);
    switch(btn){ //A switch statement is used to determine which mouse button event occurred.

        //The first case checks if the left mouse button (GLUT_LEFT_BUTTON) is pressed (GLUT_DOWN).
        case GLUT_LEFT_BUTTON:
            if(state==GLUT_DOWN){ //If the number of clicks (tracked by numbOfClicks) is less than a defined maximum (MAX_CLICKS), then the mouse coordinates are translated and scaled.
               // get new mouse coordinates for x,y
               // use scale to match right
              if(numbOfClicks <MAX_CLICKS)
                {
                    mouseX = (x - (Wwidth/2))/scale;  // mouseX and mouseY are likely global or external variables that store the translated and scaled x and y mouse positions.
                    mouseY = ((Wheight/2) - y)/scale; //The expressions (x - (Wwidth/2)) and ((Wheight/2) - y) are used to translate the mouse coordinates so that the origin (0,0) is at the center of the window.
                                                      //These translated coordinates are then divided by the scale factor to produce the final mouseX and mouseY values.
                }
             }
            break; //After handling the left button event, the function exits the switch statement.

        //Currently, no specific actions are taken for middle and right mouse button events.
        //Placeholder cases are present for potential future implementations.

        case GLUT_MIDDLE_BUTTON:
        break;

        case GLUT_RIGHT_BUTTON:
        break;
    }
  glutPostRedisplay(); //function in the GLUT library that marks the current window as needing to be redisplayed. It essentially tells GLUT to redraw the scene by invoking the display callback the next time it processes the event loop.
};

///-----------------------My code Ends Here for function definitions and declarations-----------------

///set up various properties and states for rendering
static void init(void)
{
    ///-----------------------My code Starts Here-----------------
    ///Loading SkyBox Textures:
    //This segment is responsible for loading the six textures used to create a SkyBox, which is a large cube that surrounds the entire scene to simulate distant visuals like the sky, horizon, and distant objects.
    //The textureLoader function is called with file paths for each face of the SkyBox and their respective texture variables.
    textureLoader("images/Sky_front.png", front_tex);
    textureLoader("images/Sky_back.png", back_tex);
    textureLoader("images/Sky_right.png", right_tex);
    textureLoader("images/Sky_left.png", left_tex);
    textureLoader("images/Sky_top.png", top_tex);
    textureLoader("images/Sky_bottom.png", bottom_tex);

    ///Setting up the Planet Quadric:
    myplanet = gluNewQuadric();                 //A new quadric object, myplanet, is created. Quadrics are a family of mathematical shapes, and in OpenGL, they can be used for simple 3D objects like spheres, cylinders, and discs.
    gluQuadricDrawStyle(myplanet, GLU_FILL);    //The quadric's drawing style is set to GLU_FILL, meaning it'll be filled rather than wireframe.
    gluQuadricTexture(myplanet, GL_TRUE);
    gluQuadricNormals(myplanet, GLU_SMOOTH);    //The quadric is enabled for texturing and its normals are set to be smooth. This ensures that lighting calculations will look smooth across the surface.

    ///Loading the Planet Texture:
    textureLoader("images/moon.png", planetTexture); //The texture for the planet is loaded using the textureLoader function.

    ///Enabling Various OpenGL Features:
    glEnable(GL_LIGHT0);            // This enables the first light in OpenGL's fixed-function pipeline.
    glEnable(GL_LIGHTING);          // Enables lighting calculations.
    glEnable(GL_TEXTURE_2D);        // Enables 2D texturing.
    glEnable(GL_DEPTH_TEST);        // Ensures that polygons are rendered in the correct order based on their depth.

    ///Loading Room Textures:
    //Textures for the walls, ceiling, and floor of a room are loaded.
    textureLoader("images/Room_Wall_Door.png", frontwall_tex);      //Front Wall of the room. Entrance through doore
    textureLoader("images/Room_backWall.png", backwall_tex);        //Back Wall of the room with paintings
    textureLoader("images/Room_Wall_Window.png", rightwall_tex);    //Right Wall of the room  with windows
    textureLoader("images/Room_Wall_Window.png", leftwall_tex);     //Left Wall of the room with windows
    textureLoader("images/Room_ceiling.png", ceiling_tex);          //Ceiling of the room
    textureLoader("images/Room_floor.png", floor_tex);              //Flooring of the room

    ///Loading the Ground Texture:
    textureLoader("images/Ground.jpg", ground_tex); //Ground under the room

    ///Setting the Clear Color:
    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);        // sets the color that the screen will be cleared to before each rendering. In this case, it's set to a light blue color, which may represent the sky.

    ///-----------------------My code Ends Here-----------------

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Optional:--------- The function initLighting is designed to configure the lighting setup for an OpenGL application, particularly for the GL_LIGHT0 light source, which is one of the light sources in OpenGL's fixed-function pipeline.
void initLighting() {
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

///* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 3");
    init();
    glutReshapeFunc(resize);    //Sets the callback function (resize) that will be called when the window is resized.

    glutDisplayFunc(display);   //Specifies the function (display) to be called whenever the window needs to be redrawn.
    glutMouseFunc(mouse);       //Sets the callback function (mouse) that responds to mouse button presses.
    glutKeyboardFunc(key);      //Designates the function (key) that handles standard key presses.
    glutSpecialFunc(Specialkeys);//Specifies the function (Specialkeys) to handle special keys like arrow keys, function keys, etc.
    glutMotionFunc(mouseMove);  //Sets the function (mouseMove) that will be called when the mouse is moved while a button is pressed.

    glutIdleFunc(idle);         //Specifies the function (idle) to be executed when there are no events to process. This is often used for animations or continuous updates.

    glutMainLoop();

    return EXIT_SUCCESS;
}

