#include "globalVariables.h"
#include "fileUtils.h"

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Test");

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}


