#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"

struct Color
{
    float r, g, b;

    Color()
    {
        this->r = 0.0f;
        this->g = 0.0f;
        this->b = 0.0f;
    } // Default Constructor

    Color(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

struct Material
{
    Color color;
    float ambient, diffuse, specular, reflection;
    int shininess;

    Material() {
        this->color = Color(0.0f, 0.0f, 0.0f);
        this->ambient = 0.0f;
        this->diffuse = 0.0f;
        this->specular = 0.0f;
        this->reflection = 0.0f;
        this->shininess = 0;
    } // Default Constructor
};
struct Sphere
{
    vec3 center;
    float radius;
    Color color;
    Material material;

    Sphere()
    {
        this->center = vec3(0.0f, 0.0f, 0.0f);
        this->radius = 0.0f;
        this->color = Color(0.0f, 0.0f, 0.0f);
    } // Default Constructor

    Sphere(vec3 center, float radius)
    {
        this->center = center;
        this->radius = radius;
    }
};

struct Pyramid
{
    vec3 lowestPoint;
    float width, height;
    Material material;

    Pyramid()
    {
        this->lowestPoint = vec3(0.0f, 0.0f, 0.0f);
        this->width = 0.0f;
        this->height = 0.0f;
    } // Default Constructor

    Pyramid(vec3 lowestPoint, float width, float height)
    {
        this->lowestPoint = lowestPoint;
        this->width = width;
        this->height = height;
    }

};

struct Cube
{
    vec3 bottomLowerLeftPoint ;
    float side;
    Material material;

    Cube()
    {
        this->bottomLowerLeftPoint = vec3(0.0f, 0.0f, 0.0f);
        this->side = 0.0f;
    } // Default Constructor

    Cube(vec3 bottomLowerLeftPoint, float side)
    {
        this->bottomLowerLeftPoint = bottomLowerLeftPoint;
        this->side = side;
    }
};

struct Quad
{
    vec3 bottomLeftPoint;
    vec3 bottomRightPoint;
    vec3 topLeftPoint;
    vec3 topRightPoint;
    vec3 normal;
    Material material;

    Quad()
    {
        this->bottomLeftPoint = vec3(0.0f, 0.0f, 0.0f);
        this->bottomRightPoint = vec3(0.0f, 0.0f, 0.0f);
        this->topLeftPoint = vec3(0.0f, 0.0f, 0.0f);
        this->topRightPoint = vec3(0.0f, 0.0f, 0.0f);
    } // Default Constructor

    Quad(vec3 bottomLeftPoint, vec3 bottomRightPoint, vec3 topRightPoint, vec3 topLeftPoint, vec3 normal, Material material)
    {
        this->bottomLeftPoint = bottomLeftPoint;
        this->bottomRightPoint = bottomRightPoint;
        this->topLeftPoint = topLeftPoint;
        this->topRightPoint = topRightPoint;
        this->normal = normal;
        this->material = material;
    }
};

struct Triangle
{
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 normal;
    Material material;

    Triangle()
    {
        this->point1 = vec3(0.0f, 0.0f, 0.0f);
        this->point2 = vec3(0.0f, 0.0f, 0.0f);
        this->point3 = vec3(0.0f, 0.0f, 0.0f);
    } // Default Constructor

    Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 normal, Material material)
    {
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
        this->normal = normal;
        this->material = material;
    }
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