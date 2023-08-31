#ifndef RAY_TRACE
#define RAY_TRACE

#include "globalVariables.h"
#include "vector3.h"
#include "object.h"
#include "ray.h"
#include "boundingBox.h"
#include "bitmap_image.hpp"
#include <vector>
#include <math.h>
#include <chrono>
#include <fstream>

Color rayCast(const Ray &ray, const int iterationDepth);
std::ofstream logFile("log.txt");

bool lightVisible(const vec3 &point, const vec3 &lightPosition, const int id)
{
    Ray ray = Ray(point, (lightPosition - point).normalize());
    Intersection intersection;
    GLdouble depth = (lightPosition - point).length();
    for (auto sphere : spheres)
    {
        if (sphere.id == id)
            continue;
        intersection = ray.intersect(sphere);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - point).length() < depth)
            return false;
    }
    for (auto triangle : triangles)
    {
        if (triangle.id == id)
            continue;
        intersection = ray.intersect(triangle);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - point).length() < depth)
            return false;
    }
    for (auto quad : quads)
    {
        if (quad.id == id)
            continue;
        intersection = ray.intersect(quad);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - point).length() < depth)
            return false;
    }
    return true;
}

Color lightInteraction(const Ray &originalRay, const Intersection &intersection, const int iterationDepth, const Material &material, const int id)
{
    auto tempColor = Color(0.0f, 0.0f, 0.0f);
    double lambert = 0, phong = 0;

    // check if lights are visible
    Ray reflectedRay;
    float e = 0.001;
    
    reflectedRay.direction = (originalRay.direction + intersection.normal * 2).normalize();
    reflectedRay.origin = intersection.point + reflectedRay.direction * e;
    for (auto light : lights)
    {
        // check if light direction is visible from plane
        if (intersection.normal.dot(light.position - intersection.point) < 0)
            continue;

        // check if angle with light is less than cutoff
        auto lightDirection = (light.position - intersection.point).normalize();
        // cutoff degree to radian
        auto angle = light.cutoff * 3.14159265358979323846 / 180.0f;
        if (lightDirection.dot(-light.direction.normalize()) < cos(angle) && light.type == SPOT_LIGHT)
            continue;

        if (!lightVisible(intersection.point, light.position, id))
            continue;

        auto normal = intersection.normal.normalize();
        auto lightDistance = (light.position - intersection.point).length();
        auto scalingFactor = exp(-lightDistance * lightDistance * light.falloff);

        lambert += lightDirection.dot(normal) * scalingFactor;

        phong += pow(reflectedRay.direction.dot(lightDirection), material.shininess) * scalingFactor;   
    }
    tempColor = material.color * material.diffuse * lambert + 
                material.color * material.specular * phong + 
                material.color * material.ambient + 
                rayCast(reflectedRay, iterationDepth - 1) * material.reflection;

    return tempColor;
}

Color rayCast(const Ray &ray, const int iterationDepth)
{
    if (iterationDepth == 0)
        return Color(0.0f, 0.0f, 0.0f);

    // fong model for lighting
    GLdouble depth = farPlane;
    GLdouble lambert = 0, phong = 0;
    Color color = Color(0.0f, 0.0f, 0.0f);
    Intersection intersection;
    Color tempColor = Color(0.0f, 0.0f, 0.0f);
    Material material;
    int id;
    Intersection tempIntersection;

    // check for sphere intersections

    for (auto sphere : spheres)
    {

        intersection = ray.intersect(sphere);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;
        // material properties for sphere
        id = sphere.id;
        depth = (intersection.point - ray.origin).length();
        material = sphere.material;
        id = sphere.id;
        tempIntersection = intersection;
    }

    // check for triangle intersections
    for (auto triangle : triangles)
    {
        intersection = ray.intersect(triangle);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;

        depth = (intersection.point - ray.origin).length();
        material = triangle.material;
        id = triangle.id;
        tempIntersection = intersection;
    }

    // check for quad intersections

    for (auto quad : quads)
    {
        intersection = ray.intersect(quad);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;

        // if distance is less than depth, update depth and color
        depth = (intersection.point - ray.origin).length();
        material = quad.material;
        id = quad.id;
        tempIntersection = intersection;
    }

    // check for checkerboard intersection
    for (int i = 0; i < 1; i++)
    {
        intersection = ray.intersectCheckerBoard();
        if (intersection.valid == false)
            continue;

        if ((intersection.point - ray.origin).length() > depth)
            continue;

        // material properties for checkerBoard
        material = Material();
        material.ambient = checkerboardAmbientCoeff;
        material.diffuse = checkerboardDiffuseCoeff;
        material.reflection = checkerboardReflectionCoeff;
        material.color = ((int)(intersection.point.x / checkerboardWidth) + (int)(intersection.point.z / checkerboardWidth)) % 2 == 0 ? Color(0.0f, 0.0f, 0.0f) : Color(1.0f, 1.0f, 1.0f);
        material.specular = 0.0f;
        material.shininess = 0;

        depth = (intersection.point - ray.origin).length();
        id = -1;
        tempIntersection = intersection;
    }

    color = lightInteraction(ray, tempIntersection, iterationDepth, material, id);
    return color;
}

void startRayTrace()
{
    /* Ray generation, buffer Initialization */
    auto start = std::chrono::high_resolution_clock::now();
    auto generatedRay = generateRays(fovY, aspectRatio, eye, lookAt, up, numPixels, numPixels, nearPlane);
    auto colorBuffer = std::vector<std::vector<Color>>(numPixels, std::vector<Color>(numPixels, Color(0.0f, 0.0f, 0.0f)));
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Ray Generation Complete. Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
    std::cout << "Generated Rays: " << generatedRay.size() << "\n";

    /* Intersection Calculation */
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < generatedRay.size(); i++)
    {
        int row = (int)(i / numPixels);
        int col = numPixels - (int)(i % numPixels) - 1;
        colorBuffer[row][col] = rayCast(generatedRay[i], recursionLevel);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Intersection Calculation Complete. Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    /* Image Generation */
    start = std::chrono::high_resolution_clock::now();
    bitmap_image image(numPixels, numPixels);
    for (int i = 0; i < numPixels; i++)
    {
        for (int j = 0; j < numPixels; j++)
        {
            image.set_pixel(i, j, colorBuffer[i][j].r * 255, colorBuffer[i][j].g * 255, colorBuffer[i][j].b * 255);
        }
    }
    image.save_image("output.bmp");
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Image Generation Complete. Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}

#endif // RAY_TRACE