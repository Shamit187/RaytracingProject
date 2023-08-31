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

double nearPlane = 0.0f;
double farPlane = 0.0f;
double fovY = 0.0f;
double aspectRatio = 0.0f;
int recursionLevel = 0;
int numPixels = 0;
int checkerboardWidth = 0;
double checkerboardAmbientCoeff = 0.0f;
double checkerboardDiffuseCoeff = 0.0f;
double checkerboardReflectionCoeff = 0.0f;

char title[] = "3D Shapes";

vec3 eye = vec3(0.0f, 30.0f, 100.0f);
vec3 lookAt = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

void resetCamera()
{
    eye = vec3(0.0f, 30.0f, 100.0f);
    lookAt = vec3(0.0f, 0.0f, 10.0f);
    up = vec3(0.0f, 1.0f, 0.0f);
    return;
}

int prevMouseX, prevMouseY;    // Previous mouse coordinates
bool mouseInitialized = false; // Flag to check if the mouse has been initialized

GLdouble ScreenWidth = 640.0f;
GLdouble ScreenHeight = 480.0f;

/* All objects for scene */
std::vector<Sphere> spheres;
std::vector<Pyramid> pyramids;
std::vector<Cube> cubes;
std::vector<Quad> quads;
std::vector<Triangle> triangles;

/* All lights for scene */
std::vector<normalLight> normalLights;
std::vector<spotLight> spotLights;
std::vector<Light> lights;

#endif // GLOBAL_VARIABLES_H