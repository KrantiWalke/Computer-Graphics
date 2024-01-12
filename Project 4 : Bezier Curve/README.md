# Project 4: Bezier Curve

## Overview

This project, showcases an interactive 3D Bezier Curve implementation using OpenGL and GLUT. It allows users to dynamically create and visualize Bezier curves by placing control points within a 3D environment.

## Features

- Interactive creation of a 3D Bezier Curve with up to 35 control points.
- Addition of control points via left mouse click, each assigned a random Z-value.
- Toggle visibility of control points using right mouse click.
- Reset functionality using the middle mouse button or the `space` key.
- Sphere animation along the Bezier curve, restartable with the `Enter` key.
- Scene rotation around the Y-Axis with `Left` and `Right` arrow keys.
- Control points displayed with `glPointSize(5)`.
- Bezier curve plotting with `glPointSize(2)`.

## Getting Started

### Prerequisites

- OpenGL
- GLUT library
- Basic knowledge of C++ and computer graphics concepts.

### Installation

1. Clone the project repository
2. Compile the code:
- For Linux/Mac:
  ```
  g++ -o bezier_curve main.cpp -lGL -lGLU -lglut
  ```
- For Windows:
  ```
  g++ -o bezier_curve main.cpp -lopengl32 -lglu32 -lfreeglut
  ```

### Running the Program

Execute the compiled application:
```
  ./project4
```

## Controls

- **Left Mouse Click**: Add a new control point.
- **Right Mouse Click**: Toggle visibility of control points.
- **Middle Mouse Button** or **Space Key**: Reset the drawing.
- **Enter Key**: Restart the sphere animation.
- **Left/Right Arrow Keys**: Rotate the entire scene.



