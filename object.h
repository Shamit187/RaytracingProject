#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"

struct Color
{
    float r, g, b;
};

struct Material
{
    Color color;
    float ambient, diffuse, specular, reflection;
    int shininess;
};
struct Sphere
{
    vec3 center;
    float radius;
    Color color;
    Material material;
};

struct Pyramid
{
    vec3 lowestPoint;
    float width, height;
    Material material;

};

struct Cube
{
    vec3 bottomLowerLeftPoint ;
    float side;
    Material material;
};

struct Quad
{
    vec3 bottomLeftPoint;
    vec3 bottomRightPoint;
    vec3 topLeftPoint;
    vec3 topRightPoint;
    vec3 normal;
    Material material;
};

struct Triangle
{
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 normal;
    Material material;
};

struct normalLight
{
    vec3 position;
    float falloff;
};

struct spotLight
{
    vec3 position;
    float falloff;
    vec3 direction;
    float cutoff;
};

#endif // OBJECT_H