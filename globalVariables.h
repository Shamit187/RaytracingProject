#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

#endif // GLOBAL_VARIABLES_H