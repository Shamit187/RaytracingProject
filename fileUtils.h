#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "globalVariables.h"

void parseInputFile() {
    std::fstream inputFile;
    inputFile.open("description.txt", std::ios::in);

    //read integers from description.txt
    inputFile >> nearPlane >> farPlane;
    inputFile >> fovY >> aspectRatio;
    inputFile >> recursionLevel >> numPixels;

    //read checkerboard width
    inputFile >> checkerboardWidth;
    inputFile >> ambientCoeff >> diffuseCoeff >> reflectionCoeff;

    inputFile.close();
}

#endif // FILE_UTILS_H