# Project 5: Bezier Curve 2D Animation (using sprite sheet with animated character and parallax background)


## Project Overview

This project demonstrates a 2D animation of a sprite moving along a Bezier curve in an OpenGL application. It features interactive control point placement for the curve, real-time sprite animation, and dynamic background textures, creating an engaging and interactive graphics demonstration.

## Features

- Interactive setting of up to four Bezier curve control points using mouse input.
- Real-time rendering of a sprite moving and rotating along the Bezier curve.
- Control of sprite animation orientation using arrow keys.
- Wireframe mode toggle for mesh visualization.
- Dynamic and visually appealing background and sprite textures.

## Controls

- **Left Mouse Click**: Set control points for the Bezier curve.
- **Right Mouse Click**: Toggle visibility of control points.
- **Arrow Keys**: Navigate the sprite along the curve.
- **'W' Key**: Toggle wireframe mode.
- **Spacebar**: Reset the scene.
- **ESC** or **'Q' Key**: Exit the application.

## Installation and Running

### Prerequisites

- OpenGL
- GLUT library
- SOIL library for texture loading

### Setup

1. **Clone the repository**
2. **Compile the code:**
- For Linux/Mac:
  ```
  g++ -o project5 main.cpp -lGL -lGLU -lglut -lSOIL
  ```
- For Windows:
  ```
  g++ -o project5 main.cpp -lopengl32 -lglu32 -lfreeglut -lSOIL
  ```

### Execution

Run the compiled application:
 ```
 ./project5
```
