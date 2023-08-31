#ifndef KEYCONTROL_H
#define KEYCONTROL_H

#include "globalVariables.h"
#include "rayTrace.h"

void keyboard(unsigned char key, int x, int y)
{
    vec3 forward = (lookAt - eye).normalize();
    vec3 right = forward.cross(up).normalize();
    GLdouble cameraSpeed = 1.0f;
    GLdouble cameraTiltSpeed = 0.1f;
    switch (key)
    {
    case 'w': // Move camera forward
        eye = eye + forward * cameraSpeed;
        lookAt = lookAt + forward * cameraSpeed;
        break;
    case 'W':
        eye = eye + forward * cameraSpeed * 10.0f;
        lookAt = lookAt + forward * cameraSpeed * 10.0f;
        break;
    case 's': // Move camera backward
        eye = eye - forward * cameraSpeed;
        lookAt = lookAt - forward * cameraSpeed;
        break;
    case 'S':
        eye = eye - forward * cameraSpeed * 10.0f;
        lookAt = lookAt - forward * cameraSpeed * 10.0f;
        break;
    case 'a': // Move camera left
        eye = eye - right * cameraSpeed;
        lookAt = lookAt - right * cameraSpeed;
        break;
    case 'A': // Move camera left
        eye = eye - right * cameraSpeed * 10.0f;
        lookAt = lookAt - right * cameraSpeed * 10.0f;
        break;
    case 'd': // Move camera right
        eye = eye + right * cameraSpeed;
        lookAt = lookAt + right * cameraSpeed;
        break;
    case 'D': // Move camera right
        eye = eye + right * cameraSpeed * 10.0f;
        lookAt = lookAt + right * cameraSpeed * 10.0f;
        break;
    case ' ': // Move camera up
        eye = eye + up.normalize() * cameraSpeed;
        lookAt = lookAt + up.normalize() * cameraSpeed;
        break;
    case 'c': // Move camera down
        eye = eye - up.normalize() * cameraSpeed;
        lookAt = lookAt - up.normalize() * cameraSpeed;
        break;
    case 'q': // tilt camera left
        up = rodrigues_rotation(up, forward, cameraTiltSpeed);
        std::cout << "up vector: " << up.x << " " << up.y << " " << up.z << "\n";
        break;
    case 'e': // tilt camera right
        up = rodrigues_rotation(up, forward, -cameraTiltSpeed);
        std::cout << "up vector: " << up.x << " " << up.y << " " << up.z << "\n";
        break;
    case 'l': // mouse is initialized
        mouseInitialized = true;
        break;
    case 'k': // mouse is not initialized
        mouseInitialized = false;
        break;
    case 'r':
        resetCamera();
        break;
    case 13: // Enter key
        startRayTrace();
        break;
    case 27: // Escape key
        exit(0);
        break;
    }
    glutPostRedisplay(); // Trigger a repaint
}

void mouse(int x, int y)
{
    const double sensitivity = 0.05f;
    int deltaX = x - prevMouseX;
    int deltaY = y - prevMouseY;
    prevMouseX = x;
    prevMouseY = y;

    if (!mouseInitialized)
        return;

    vec3 forward = (lookAt - eye).normalize();
    vec3 right = forward.cross(up).normalize();

    // Rotate the camera
    forward = rodrigues_rotation(forward, up, -deltaX * sensitivity);
    forward = rodrigues_rotation(forward, right, -deltaY * sensitivity);
    lookAt = eye + forward;

    glutPostRedisplay(); // Trigger a repaint
}

#endif // KEYCONTROL_H