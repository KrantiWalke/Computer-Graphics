# Project 2 - Sphere Move

## Features

- 3D sphere simulation using OpenGL and GLUT.
- Dynamic collision detection among spheres and cube boundaries.
- Interactive camera controls for rotating the scene and zooming in/out.
- Real-time changes in sphere colors upon collisions.
- Wireframe and solid rendering modes.

## Prerequisites

To run this project, you need to have OpenGL and GLUT installed on your system. This project is platform-independent but requires a C++ compiler and a compatible OpenGL environment.

## Installation and Running

1. **Clone the repository:**
2. Compile the code:
- For Linux/Mac:
  ```
  g++ -o sphere_move main.cpp -lGL -lGLU -lglut
  ```
- For Windows:
  ```
  g++ -o sphere_move main.cpp -lopengl32 -lglu32 -lfreeglut
  ```

## Running the Program

Execute the compiled application: 
  ```
  ./sphere_move
  ```
## Controls

- **Arrow Keys**: Rotate the scene.
- **Zoom In/Out**: Up/Down arrow keys.
- **Exit Program**: Press `ESC` or `q`.

## How It Works

The application simulates spheres moving in a 3D space. Each sphere's movement is governed by basic physics principles, and collisions with the cube boundaries and other spheres are detected and handled in real-time. The lighting effects and camera controls allow users to explore the scene from different perspectives.

