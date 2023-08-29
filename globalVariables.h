#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

// for C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include "vector3.h"
#include "object.h"

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
float checkerboardAmbientCoeff = 0.0f;
float checkerboardDiffuseCoeff = 0.0f;
float checkerboardReflectionCoeff = 0.0f;

char title[] = "3D Shapes";

vec3 eye = vec3(0.0f, 100.0f, 10.0f);
vec3 lookAt = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

void resetCamera()
{
    eye = vec3(0.0f, 100.0f, 10.0f);
    lookAt = vec3(0.0f, 0.0f, 10.0f);
    up = vec3(0.0f, 1.0f, 0.0f);
    return;
}

int prevMouseX, prevMouseY;    // Previous mouse coordinates
bool mouseInitialized = false; // Flag to check if the mouse has been initialized

GLfloat ScreenWidth = 640.0f;
GLfloat ScreenHeight = 480.0f;

/* All objects for scene */
std::vector<Sphere> spheres;
std::vector<Pyramid> pyramids;
std::vector<Cube> cubes;

/* All lights for scene */
std::vector<normalLight> normalLights;
std::vector<spotLight> spotLights;

#endif // GLOBAL_VARIABLES_H