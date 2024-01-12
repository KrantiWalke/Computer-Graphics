# Project 3: SkyBox Design

## Overview

This project is a 3D graphics application developed using OpenGL and GLUT. It features an interactive 3D scene with a skybox, textured room, dynamic lighting, and a rotating planet.

## Features

- Interactive camera controls for scene exploration.
- Textured skybox and room environment.
- Dynamic lighting with a revolving light source.
- Mouse and keyboard inputs for navigation and interaction.

## Prerequisites

- OpenGL
- GLUT
- SOIL (Simple OpenGL Image Library)

## Installation

1. Clone this repository
2. Compile the code:
- For Linux/Mac:
  ```
  g++ -o Project3 main.cpp -lGL -lGLU -lglut -lSOIL
  ```
- For Windows:
  ```
  g++ -o Project3 main.cpp -lopengl32 -lglu32 -lfreeglut -lSOIL
  ```

## Running the Program

Execute the compiled application:
./Project3


## Controls

- `W`, `A`, `S`, `D`: Move forward, left, backward, right.
- `I`, `J`, `K`, `L`: Rotate the camera view.
- `Z`, `X`: Zoom in/out.
- `Space`: Reset camera position.
- `O`: Reset camera orientation.
- Mouse Movement: Rotate the scene.
- Left Mouse Button: Interaction with the scene.



