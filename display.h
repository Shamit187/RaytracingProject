#include "globalVariables.h"

void drawColorCube()
{
    // Render a color-cube consisting of 6 quads with different colors
    glPushMatrix(); // Reset the model-view matrix

    glTranslatef(1.5f, 0.0f, -7.0f); // Move right and into the screen

    glBegin(GL_QUADS); // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd(); // End of drawing color-cube

    // Render a pyramid consists of 4 triangles
    glPopMatrix(); // Reset the model-view matrix
}
void drawColorPyramid()
{
    // Render a pyramid consists of 4 triangles
    glPushMatrix(); // Reset the model-view matrix

    glTranslatef(-1.5f, 0.0f, -6.0f); // Move left and into the screen

    glBegin(GL_TRIANGLES); // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd(); // Done drawing the pyramid

    glPopMatrix(); // Reset the model-view matrix
}

#include <GL/glut.h> // or appropriate OpenGL header

void drawCube(const Cube &cube)
{
    glPushMatrix();
    // Set material properties
    glColor3f(cube.material.color.r, cube.material.color.g, cube.material.color.b);
    // Calculate vertices
    vec3 vertices[8];
    vertices[0] = cube.bottomLowerLeftPoint;
    vertices[1] = vertices[0] + vec3(cube.side, 0.0f, 0.0f);
    vertices[2] = vertices[0] + vec3(cube.side, 0.0f, cube.side);
    vertices[3] = vertices[0] + vec3(0.0f, 0.0f, cube.side);
    vertices[4] = vertices[0] + vec3(0.0f, cube.side, 0.0f);
    vertices[5] = vertices[1] + vec3(0.0f, cube.side, 0.0f);
    vertices[6] = vertices[2] + vec3(0.0f, cube.side, 0.0f);
    vertices[7] = vertices[3] + vec3(0.0f, cube.side, 0.0f);
    std::cout << "Vertices: " << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    // Draw the cube
    glBegin(GL_QUADS);
    // Bottom face
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    glBegin(GL_QUADS);
    // Side face 1 (front)
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glEnd();

    glBegin(GL_QUADS);
    // Side face 2 (right)
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glEnd();

    glBegin(GL_QUADS);
    // Side face 3 (back)
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glEnd();

    glBegin(GL_QUADS);
    // Side face 4 (left)
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glEnd();

    glBegin(GL_QUADS);
    // Top face
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glEnd();

    glPopMatrix();
}

#define M_PI 3.14159265358979323846

void drawSphere(const Sphere &sphere, int numSlices, int numStacks)
{
    glPushMatrix();
    // Draw the sphere
    float phiStep = M_PI / numStacks;
    float thetaStep = 2.0f * M_PI / numSlices;

    glColor3f(sphere.material.color.r, sphere.material.color.g, sphere.material.color.b);

    for (int i = 0; i < numStacks; ++i)
    {
        float phi = i * phiStep;
        float nextPhi = (i + 1) * phiStep;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numSlices; ++j)
        {
            float theta = j * thetaStep;

            // Calculate vertex positions
            vec3 vertex1 = sphere.center + vec3(sphere.radius * sin(nextPhi) * cos(theta),
                                                sphere.radius * cos(nextPhi),
                                                sphere.radius * sin(nextPhi) * sin(theta));
            vec3 vertex2 = sphere.center + vec3(sphere.radius * sin(phi) * cos(theta),
                                                sphere.radius * cos(phi),
                                                sphere.radius * sin(phi) * sin(theta));

            // Calculate normals
            vec3 normal1 = (vertex1 - sphere.center).normalize();
            vec3 normal2 = (vertex2 - sphere.center).normalize();

            glNormal3f(normal1.x, normal1.y, normal1.z);
            glVertex3f(vertex1.x, vertex1.y, vertex1.z);

            glNormal3f(normal2.x, normal2.y, normal2.z);
            glVertex3f(vertex2.x, vertex2.y, vertex2.z);
        }
        glEnd();
    }
    glPopMatrix();
}

void drawPyramid(const Pyramid &pyramid)
{
    glPushMatrix();
    // Set the material properties
    glColor3f(pyramid.material.color.r, pyramid.material.color.g, pyramid.material.color.b);

    // Calculate vertices of bottom face
    vec3 vertices[4];
    vertices[0] = pyramid.lowestPoint + vec3(pyramid.width / 2, 0.0f, pyramid.width / 2);
    vertices[1] = pyramid.lowestPoint + vec3(pyramid.width / 2, 0.0f, -pyramid.width / 2);
    vertices[2] = pyramid.lowestPoint + vec3(-pyramid.width / 2, 0.0f, -pyramid.width / 2);
    vertices[3] = pyramid.lowestPoint + vec3(-pyramid.width / 2, 0.0f, pyramid.width / 2);

    // Calculate vertices of top point
    vec3 topPoint = pyramid.lowestPoint + vec3(0.0f, pyramid.height, 0.0f);

    // Draw the pyramid
    glBegin(GL_QUADS);
    // Bottom face
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

    glEnd();

    glBegin(GL_TRIANGLES);
    // Side face 1
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(topPoint.x, topPoint.y, topPoint.z);

    // Side face 2
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(topPoint.x, topPoint.y, topPoint.z);

    // Side face 3
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(topPoint.x, topPoint.y, topPoint.z);

    // Side face 4
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(topPoint.x, topPoint.y, topPoint.z);

    glEnd();

    glPopMatrix();
}

void drawCheckerBoard()
{
    glPushMatrix();
    // draw a plane that is xz plane
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    glPopMatrix();
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);                         // To operate on model-view matrix

    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z); // Set the camera position and orientation

    drawCheckerBoard();

    for (auto cube : cubes)
    {
        drawCube(cube);
    }

    for (auto sphere : spheres)
    {
        drawSphere(sphere, 20, 20);
    }

    for (auto pyramid : pyramids)
    {
        drawPyramid(pyramid);
    }

    glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}
