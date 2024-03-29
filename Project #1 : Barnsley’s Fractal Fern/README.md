# Project 1 : Barnsley’s Fractal Fern


## Introduction

Barnsley's Fractal Fern is a fascinating example of how mathematics can replicate patterns found in nature. This project utilizes OpenGL and GLUT to render the fractal, providing an interactive experience to explore the complexity of fractals.

## Features

- Renders Barnsley's Fractal Fern using OpenGL.
- Interactive viewport that adjusts with the window size.
- Simple control to exit the program.

## Requirements

- OpenGL
- GLUT (FreeGLUT for Windows, GLUT for macOS)

## Installation and Running the Program

1. Ensure you have OpenGL and GLUT installed on your system.
2. Clone this repository.
3. Compile the code. For example, on a Unix-based system, you might use:
   
   g++ -o barnsley_fern main.cpp -lGL -lGLU -lglut
   
4. Run the executable:
   
   ./barnsley_fern
   

## Controls

- Press `ESC` or `q` to exit the program.

## How It Works

The program generates the fractal by applying a series of linear transformations to each point, simulating the natural growth process of a fern. These transformations are probabilistically chosen to mimic the random yet structured nature of fern growth.

