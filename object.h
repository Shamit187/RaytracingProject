#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"


struct Color
{
    double r, g, b;

    Color()
    {
        this->r = 0.0f;
        this->g = 0.0f;
        this->b = 0.0f;
    } // Default Constructor

    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    Color operator+(const Color &c) const
    {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator*(const Color &c) const
    {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator*(double f) const
    {
        return Color(r * f, g * f, b * f);
    }
};

struct Material
{
    Color color;
    double ambient, diffuse, specular, reflection, refraction, shininess;

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
    int id;
    vec3 center;
    double radius;
    Color color;
    Material material;

    Sphere()
    {
        this->center = vec3(0.0f, 0.0f, 0.0f);
        this->radius = 0.0f;
        this->color = Color(0.0f, 0.0f, 0.0f);
    } // Default Constructor

    Sphere(vec3 center, double radius)
    {
        this->center = center;
        this->radius = radius;
    }
};

struct Pyramid
{
    int id;
    vec3 lowestPoint;
    double width, height;
    Material material;

    Pyramid()
    {
        this->lowestPoint = vec3(0.0f, 0.0f, 0.0f);
        this->width = 0.0f;
        this->height = 0.0f;
    } // Default Constructor

    Pyramid(vec3 lowestPoint, double width, double height)
    {
        this->lowestPoint = lowestPoint;
        this->width = width;
        this->height = height;
    }

};

struct Cube
{
    int id;
    vec3 bottomLowerLeftPoint ;
    double side;
    Material material;

    Cube()
    {
        this->bottomLowerLeftPoint = vec3(0.0f, 0.0f, 0.0f);
        this->side = 0.0f;
    } // Default Constructor

    Cube(vec3 bottomLowerLeftPoint, double side)
    {
        this->bottomLowerLeftPoint = bottomLowerLeftPoint;
        this->side = side;
    }
};

struct Quad
{
    int id;
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

    Quad(vec3 bottomLeftPoint, vec3 bottomRightPoint, vec3 topRightPoint, vec3 topLeftPoint, vec3 normal, Material material, int id)
    {
        this->bottomLeftPoint = bottomLeftPoint;
        this->bottomRightPoint = bottomRightPoint;
        this->topLeftPoint = topLeftPoint;
        this->topRightPoint = topRightPoint;
        this->normal = normal;
        this->material = material;
        this->id = id;
    }
};

struct Triangle
{
    int id;
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

    Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 normal, Material material, int id)
    {
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
        this->normal = normal;
        this->material = material;
        this->id = id;
    }
};

struct normalLight
{
    vec3 position;
    double falloff;
    double intensity;
    Color color;
};

struct spotLight
{
    vec3 position;
    double falloff;
    vec3 direction;
    double cutoff;
    double intensity;
    Color color;
};

enum lightType {NORMAL_LIGHT, SPOT_LIGHT};

struct Light{
    lightType type;
    vec3 position;
    double falloff;
    vec3 direction;
    double cutoff;
    double intensity;
    Color color;
};


#endif // OBJECT_H