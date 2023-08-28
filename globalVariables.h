#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

// for C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

// for openGL
#include <windows.h> // for MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h

float nearPlane = 0.0f;
float farPlane = 0.0f;
float fovY = 0.0f;
float aspectRatio = 0.0f;
int recursionLevel = 0;
int numPixels = 0;
int checkerboardWidth = 0;
float ambientCoeff = 0.0f;
float diffuseCoeff = 0.0f;
float reflectionCoeff = 0.0f;

char title[] = "3D Shapes";
GLfloat cameraX = 0.0f;     // Camera position X
GLfloat cameraY = 0.0f;     // Camera position Y
GLfloat cameraZ = 5.0f;     // Camera position Z
GLfloat cameraSpeed = 0.01f; // Camera movement speed
GLfloat cameraLookX = 0.0f; // Camera look-at point X
GLfloat cameraLookY = 0.0f; // Camera look-at point Y
GLfloat cameraLookZ = 0.0f; // Camera look-at point Z
GLfloat cameraUpX = 0.0f;   // Camera up vector X
GLfloat cameraUpY = 1.0f;   // Camera up vector Y
GLfloat cameraUpZ = 0.0f;   // Camera up vector Z
GLfloat tiltingAngle = 0.1f;

void resetCamera(){
    cameraX = 0.0f;     // Camera position X
    cameraY = 0.0f;     // Camera position Y
    cameraZ = 5.0f;     // Camera position Z
    cameraSpeed = 0.1f; // Camera movement speed
    cameraLookX = 0.0f; // Camera look-at point X
    cameraLookY = 0.0f; // Camera look-at point Y
    cameraLookZ = 0.0f; // Camera look-at point Z
    cameraUpX = 0.0f;   // Camera up vector X
    cameraUpY = 1.0f;   // Camera up vector Y
    cameraUpZ = 0.0f;   // Camera up vector Z
    tiltingAngle = 0.1f;
}

int prevMouseX, prevMouseY; // Previous mouse coordinates
bool mouseInitialized = false; // Flag to check if the mouse has been initialized

GLfloat ScreenWidth = 640.0f;
GLfloat ScreenHeight = 480.0f;


#endif // GLOBAL_VARIABLES_H