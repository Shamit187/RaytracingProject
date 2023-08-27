#pragma once 
#include "globalVariables.h"

void parseInputFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    inputFile >> nearPlane >> farPlane >> fovY >> aspectRatio;
    inputFile >> std::ws; // Skip any whitespace

    inputFile >> recursionLevel >> numPixels;
    inputFile >> checkerboardWidth;
    inputFile >> ambientCoeff >> diffuseCoeff >> reflectionCoeff;

    inputFile.close();
}

