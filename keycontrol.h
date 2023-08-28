#include "globalVariables.h"

void keyboard(unsigned char key, int x, int y)
{
    GLfloat forwardX = cameraLookX - cameraX;
    GLfloat forwardY = cameraLookY - cameraY;
    GLfloat forwardZ = cameraLookZ - cameraZ;
    GLfloat backwardX = cameraX - cameraLookX;
    GLfloat backwardY = cameraY - cameraLookY;
    GLfloat backwardZ = cameraZ - cameraLookZ;
    GLfloat leftX = forwardZ;
    GLfloat leftY = forwardY;
    GLfloat leftZ = -forwardX;
    GLfloat rightX = -forwardZ;
    GLfloat rightY = forwardY;
    GLfloat rightZ = forwardX;
    GLfloat upX = forwardX;
    GLfloat upY = -forwardZ;
    GLfloat upZ = forwardY;
    GLfloat downX = forwardX;
    GLfloat downY = forwardZ;
    GLfloat downZ = -forwardY;
    GLfloat cosTheta = cos(tiltingAngle);
    GLfloat sinTheta = sin(tiltingAngle);
    GLfloat oneMinusCos = 1.0f - cosTheta;
    GLfloat dot = cameraUpX * forwardX + cameraUpY * forwardY + cameraUpZ * forwardZ;

    std::cout << "Camera Position (" << cameraX << ", " << cameraY << ", " << cameraZ << ")" << std::endl;
    std::cout << "Camera Look (" << cameraLookX << ", " << cameraLookY << ", " << cameraLookZ << ")" << std::endl;
    std::cout << "Camera Up (" << cameraUpX << ", " << cameraUpY << ", " << cameraUpZ << ")" << std::endl;
    switch (key)
    {
    case 'w': // Move camera forward
        cameraX += cameraSpeed * forwardX;
        cameraY += cameraSpeed * forwardY;
        cameraZ += cameraSpeed * forwardZ;
        cameraLookX += cameraSpeed * forwardX;
        cameraLookY += cameraSpeed * forwardY;
        cameraLookZ += cameraSpeed * forwardZ;
        break;
    case 's': // Move camera backward
        cameraX += cameraSpeed * backwardX;
        cameraY += cameraSpeed * backwardY;
        cameraZ += cameraSpeed * backwardZ;
        cameraLookX += cameraSpeed * backwardX;
        cameraLookY += cameraSpeed * backwardY;
        cameraLookZ += cameraSpeed * backwardZ;
        break;
    case 'a': // Move camera left
        cameraX += cameraSpeed * leftX;
        cameraY += cameraSpeed * leftY;
        cameraZ += cameraSpeed * leftZ;
        cameraLookX += cameraSpeed * leftX;
        cameraLookY += cameraSpeed * leftY;
        cameraLookZ += cameraSpeed * leftZ;
        break;

    case 'd': // Move camera right
        cameraX += cameraSpeed * rightX;
        cameraY += cameraSpeed * rightY;
        cameraZ += cameraSpeed * rightZ;
        cameraLookX += cameraSpeed * rightX;
        cameraLookY += cameraSpeed * rightY;
        cameraLookZ += cameraSpeed * rightZ;
        break;
    case ' ': // Move camera up
        cameraX += cameraSpeed * upX;
        cameraY += cameraSpeed * upY;
        cameraZ += cameraSpeed * upZ;
        cameraLookX += cameraSpeed * upX;
        cameraLookY += cameraSpeed * upY;
        cameraLookZ += cameraSpeed * upZ;
        break;
    case 'c': // Move camera down
        cameraX += cameraSpeed * downX;
        cameraY += cameraSpeed * downY;
        cameraZ += cameraSpeed * downZ;
        cameraLookX += cameraSpeed * downX;
        cameraLookY += cameraSpeed * downY;
        cameraLookZ += cameraSpeed * downZ;
        break;
    case 'q':
        cameraUpX = forwardX * dot * oneMinusCos + cameraUpX * cosTheta + (-cameraUpZ * forwardY + cameraLookY * forwardZ) * sinTheta,
        cameraUpY = forwardY * dot * oneMinusCos + cameraUpY * cosTheta + (cameraUpZ * forwardX - cameraLookX * forwardZ) * sinTheta,
        cameraUpZ = forwardZ * dot * oneMinusCos + cameraUpZ * cosTheta + (-cameraUpY * forwardX + cameraLookX * forwardY) * sinTheta;
        break;
    case 'e':
        cameraUpX = forwardX * dot * oneMinusCos + cameraUpX * cosTheta + (cameraUpZ * forwardY - cameraLookY * forwardZ) * sinTheta,
        cameraUpY = forwardY * dot * oneMinusCos + cameraUpY * cosTheta + (-cameraUpZ * forwardX + cameraLookX * forwardZ) * sinTheta,
        cameraUpZ = forwardZ * dot * oneMinusCos + cameraUpZ * cosTheta + (cameraUpY * forwardX - cameraLookX * forwardY) * sinTheta;
        break;
    case 'l':
        // mouse is initialized
        mouseInitialized = true;
        break;
    case 'k':
        // mouse is not initialized
        mouseInitialized = false;
        break;
    case 'r':
        resetCamera();
        break;
    case 27: // Escape key
        exit(0);
        break;
    }
    glutPostRedisplay(); // Trigger a repaint
}

void mouse(int x, int y)
{
    std::cout << "Mouse at (" << x << ", " << y << ")" << std::endl;
    const float sensitivity = 0.05f;
    int deltaX = x - prevMouseX;
    int deltaY = y - prevMouseY;
    prevMouseX = x;
    prevMouseY = y;

    if (!mouseInitialized)
        return;

    std::cout << "Mouse Delta (" << deltaX << ", " << deltaY << ")" << std::endl;
    std::cout << "Camera Look (" << cameraLookX << ", " << cameraLookY << ")" << std::endl;
    cameraLookX += deltaX * sensitivity;
    cameraLookY -= deltaY * sensitivity;

    glutPostRedisplay(); // Trigger a repaint
}   