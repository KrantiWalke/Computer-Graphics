# Project 6: Shaders

## Description

This OpenGL project demonstrates the use of shaders for rendering a textured teapot with various interactive features. The application utilizes vertex and fragment shaders to achieve dynamic texturing and lighting effects.

## Features

- Teapot model with texture mapping.
- Real-time texture color manipulation.
- Interactive scene with user-controlled transformations.
- Wireframe mode for viewing mesh construction.
- Lighting and material effects for enhanced realism.

## Interaction

- **Mouse Left Click**: Set control points for the Bezier curve.
- **Mouse Right Click**: Toggle visibility of control points.
- **Arrow Keys**: Rotate the model (Y-axis with left/right, X-axis with up/down).
- **Key 1**: Replace all red color in the texture with blue.
- **Key 2**: Replace all green color in the texture with blue.
- **Key 3**: Reset texture colors to original.
- **Keys W/A/S/D**: Scale teapot in different directions.
- **Spacebar**: Reset the teapot's size.
- **ESC/Q**: Exit the application.

## Shaders

- Vertex Shader: `v1.vs`
- Fragment Shader: `f1.fs`

## Setup and Execution

### Prerequisites

- OpenGL
- GLUT library
- GLEW library
- SOIL library for texture loading

### Compilation

For Linux/Mac:
```
g++ -o project6 main.cpp -lGL -lGLU -lglut -lGLEW -lSOIL
```

For Windows:
```
g++ -o project6 main.cpp -lopengl32 -lglu32 -lfreeglut -lglew32 -lSOIL
```

### Running the Program
```
./project6
```


