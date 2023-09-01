#ifndef DISPLAY_H
#define DISPLAY_H

#include "globalVariables.h"
#include <GL/glut.h> // or appropriate OpenGL header
#define M_PI 3.14159265358979323846

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

void drawSphere(const Sphere &sphere, int numSlices, int numStacks)
{
    glPushMatrix();
    // Draw the sphere
    double phiStep = M_PI / numStacks;
    double thetaStep = 2.0f * M_PI / numSlices;

    glColor3f(sphere.material.color.r, sphere.material.color.g, sphere.material.color.b);

    for (int i = 0; i < numStacks; ++i)
    {
        double phi = i * phiStep;
        double nextPhi = (i + 1) * phiStep;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numSlices; ++j)
        {
            double theta = j * thetaStep;

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
    // draw a plane that is xz plane and has alternating color
    GLdouble xLimitLow = (eye.x - 20 * checkerboardWidth) / checkerboardWidth;
    GLdouble xLimitHigh = (eye.x + 20 * checkerboardWidth) / checkerboardWidth;
    GLdouble zLimitLow = (eye.z - 20 * checkerboardWidth) / checkerboardWidth;
    GLdouble zLimitHigh = (eye.z + 20 * checkerboardWidth) / checkerboardWidth;

    glBegin(GL_QUADS);
    for (int i = xLimitLow; i < xLimitHigh; i++)
    {
        for (int j = zLimitLow; j < zLimitHigh; j++)
        {
            if ((i + j) % 2 == 0)
            {
                glColor3f(0.0f, 0.0f, 0.0f);
            }
            else
            {
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            glVertex3f(i * checkerboardWidth, 0.0f, j * checkerboardWidth);
            glVertex3f((i + 1) * checkerboardWidth, 0.0f, j * checkerboardWidth);
            glVertex3f((i + 1) * checkerboardWidth, 0.0f, (j + 1) * checkerboardWidth);
            glVertex3f(i * checkerboardWidth, 0.0f, (j + 1) * checkerboardWidth);
        }
    }
    glEnd();

    glPopMatrix();
}

void drawTriangle(const Triangle &Triangle)
{
    glPushMatrix();
    // Set material properties
    glColor3f(Triangle.material.color.r, Triangle.material.color.g, Triangle.material.color.b);

    glBegin(GL_TRIANGLES);
    glVertex3f(Triangle.point1.x, Triangle.point1.y, Triangle.point1.z);
    glVertex3f(Triangle.point2.x, Triangle.point2.y, Triangle.point2.z);
    glVertex3f(Triangle.point3.x, Triangle.point3.y, Triangle.point3.z);
    glEnd();

    glPopMatrix();
}

void drawQuad(const Quad &quad)
{
    glPushMatrix();

    // Set material properties
    glColor3f(quad.material.color.r, quad.material.color.g, quad.material.color.b);

    glBegin(GL_QUADS);
    glVertex3f(quad.bottomLeftPoint.x, quad.bottomLeftPoint.y, quad.bottomLeftPoint.z);
    glVertex3f(quad.bottomRightPoint.x, quad.bottomRightPoint.y, quad.bottomRightPoint.z);
    glVertex3f(quad.topRightPoint.x, quad.topRightPoint.y, quad.topRightPoint.z);
    glVertex3f(quad.topLeftPoint.x, quad.topLeftPoint.y, quad.topLeftPoint.z);
    glEnd();

    glPopMatrix();
}

void drawNormalLight(const normalLight &light)
{
    glPushMatrix();
    // Set material properties
    glColor3f(1.0f, 1.0f, 1.0f);

    // draw a sphere of radius 1 with the center of light.position
    Sphere sphere;
    sphere.center = light.position;
    sphere.radius = 5.0f;
    sphere.material.color = Color(1.0f, 1.0f, 1.0f);
    drawSphere(sphere, 20, 20);

    // draw some lines outward to show the direction of the light
    for (int i = 0; i < 10; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(light.position.x, light.position.y, light.position.z);
        glVertex3f(light.position.x + i * light.falloff * light.intensity * light.color.r, light.position.y + i * light.falloff * light.intensity * light.color.g, light.position.z + i * light.falloff * light.intensity * light.color.b);
        glEnd();
    }

    glPopMatrix();
}


std::vector<vec3> getAllBasePoints(const vec3 &apex, const vec3 &direction, float length, float radius){
    std::vector<vec3> basePoints;
    std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
    vec3 directionNormalized = direction.normalize();
    vec3 directionPerpendicular = directionNormalized.cross(vec3(0.0f, 1.0f, 0.0f)).normalize();
    if(directionPerpendicular.length() == 0){
        directionPerpendicular = directionNormalized.cross(vec3(1.0f, 0.0f, 0.0f)).normalize();
    }
    vec3 directionPerpendicular2 = directionNormalized.cross(directionPerpendicular).normalize();
    for(float angle = 0; angle < 2 * M_PI; angle += 2 * M_PI / 30){
        basePoints.push_back(apex + directionNormalized * length + directionPerpendicular * radius * cos(angle) + directionPerpendicular2 * radius * sin(angle));
    }
    return basePoints;
}



void drawSpotLight(const spotLight &light)
{
    auto basePoints = getAllBasePoints(light.position, light.direction, 5.0f, 5.0f);
    glPushMatrix();
    glColor3f(light.color.r, light.color.g, light.color.b);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < basePoints.size(); i++)
    {
        glVertex3f(basePoints[i].x, basePoints[i].y, basePoints[i].z);
        glVertex3f(light.position.x, light.position.y, light.position.z);
    }
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

    // for (auto cube : cubes)
    // {
    //     drawCube(cube);
    // }

    for (auto sphere : spheres)
    {
        drawSphere(sphere, 20, 20);
    }

    // for (auto pyramid : pyramids)
    // {
    //     drawPyramid(pyramid);
    // }
    for (auto triangle : triangles)
    {
        drawTriangle(triangle);
    }

    for (auto quad : quads)
    {
        drawQuad(quad);
    }

    for (auto light : normalLights)
    {
        drawNormalLight(light);
    }

    for (auto light : spotLights)
    {
        drawSpotLight(light);
    }

    glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}

#endif