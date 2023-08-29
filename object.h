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

struct Light
{
    bool isSpot;
    float x, y, z, falloff;
    float dirX, dirY, dirZ, cutoff;
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
