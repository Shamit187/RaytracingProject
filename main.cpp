#include "globalVariables.h"
#include "keycontrol.h"
#include "display.h"
#include "parser.h"
#include "ray.h"

/* Initialize OpenGL Graphics */
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              // Set background color to black and opaque
    glClearDepth(1.0f);                                // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                           // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                            // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                           // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouse);
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLdouble aspect = (GLdouble)width / (GLdouble)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(fovY, aspect, nearPlane, farPlane);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char **argv)
{
    parseSceneFile("description.txt");
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480);     // Set the window's initial width & height
    glutInitWindowPosition(50, 50);   // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);         // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);         // Register callback handler for window re-size event
    initGL();                         // Our own OpenGL initialization
    glutMainLoop();                   // Enter the infinite event-processing loop
    return 0;
}

/* Command For Running the code */
// g++ main.cpp -o demo.exe -lfreeglut -lglew32 -lopengl32 -lglu32
// start demo.exe