#include "globalVariables.h"

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        cameraZ -= cameraSpeed; // Move camera forward
        cameraLookZ -= cameraSpeed;
        break;
    case 's':
        cameraZ += cameraSpeed; // Move camera backward
        cameraLookZ += cameraSpeed;
        break;
    case 'a':
        cameraX -= cameraSpeed; // Move camera left
        cameraLookX -= cameraSpeed;
        break;
    case 'd':
        cameraX += cameraSpeed; // Move camera right
        cameraLookX += cameraSpeed;
        break;
    case ' ':
        cameraY += cameraSpeed; // Move camera up
        cameraLookY += cameraSpeed;
        break;
    case 'c':
        cameraY -= cameraSpeed; // Move camera down
        cameraLookY -= cameraSpeed;
        break;
    }
    glutPostRedisplay(); // Trigger a repaint
}