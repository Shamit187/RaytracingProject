#include "globalVariables.h"
#include "fileUtils.h"
#include "display.h"

int main(int argc, char **argv)
{
    parseInputFile();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("OpenGL Test");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
