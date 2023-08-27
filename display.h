#include "globalVariables.h"

void drawCheckerboard() {
    int numSquares = 8; // Number of squares per side
    float squareSize = checkerboardWidth / numSquares;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the checkerboard
    for (int i = 0; i < numSquares; ++i) {
        for (int j = 0; j < numSquares; ++j) {
            glPushMatrix();
            glTranslatef(i * squareSize, 0.0f, j * squareSize);

            // Alternate colors
            if ((i + j) % 2 == 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // White
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            }

            glBegin(GL_QUADS);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(squareSize, 0.0f, 0.0f);
            glVertex3f(squareSize, 0.0f, squareSize);
            glVertex3f(0.0f, 0.0f, squareSize);
            glEnd();

            glPopMatrix();
        }
    }

    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawCheckerboard();
    glutSwapBuffers();
}