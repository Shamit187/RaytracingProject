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

const Color ambientColor = Color(0.0f, 0.0f, 0.0f);
// std::ofstream logFile("log.txt");

bool lightVisible(const vec3 &point, const vec3 &lightPosition, const int id)
{
    Ray ray = Ray(point, (lightPosition - point).normalize());
    Intersection intersection;
    GLfloat depth = (lightPosition - point).length();
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

Color raycast(const Ray &ray, const int iterationDeapth)
{
    if (iterationDeapth == 0)
        return Color(0.0f, 0.0f, 0.0f);

    // fong model for lighting
    GLfloat depth = farPlane;
    GLfloat lambert = 0, phong = 0;
    Color color = Color(0.0f, 0.0f, 0.0f);
    Intersection intersection;
    Color tempColor = Color(0.0f, 0.0f, 0.0f);
    Material material;

    // check for sphere intersections
    /*
    for (auto sphere : spheres)
    {

        intersection = ray.intersect(sphere);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;
        // material properties for sphere
        material = sphere.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(normalLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from sphere
            if (!checkLightVisibility(intersection.point, normalLight.position, sphere.id))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(spotLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from sphere
            if (!checkLightVisibility(intersection.point, spotLight.position, sphere.id))
                continue;
            // check cutoff
            auto lightDirection = (spotLight.position - intersection.point).normalize();
            // check if angle with light is less than cutoff
            if (lightDirection.dot(spotLight.direction) < cos(spotLight.cutoff))
                continue;
            auto normal = intersection.normal.normalize();
            auto lightDistance = (spotLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * spotLight.falloff);
            auto lightIntensity = spotLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((ray.origin - intersection.point).length() < depth)
        {
            depth = (intersection.point - ray.origin).length();
            color = tempColor;
        }
    }

    */

    
    // check for triangle intersections
    /*
    for (auto triangle : triangles)
    {
        intersection = ray.intersect(triangle);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;

        // material properties for triangle
        material = triangle.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(normalLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from triangle
            if (!checkLightVisibility(intersection.point, normalLight.position, triangle.id))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(spotLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from triangle
            if (!checkLightVisibility(intersection.point, spotLight.position, triangle.id))
                continue;
            // check cutoff
            auto lightDirection = (spotLight.position - intersection.point).normalize();
            // check if angle with light is less than cutoff
            if (lightDirection.dot(spotLight.direction) < cos(spotLight.cutoff))
                continue;
            auto normal = intersection.normal.normalize();
            auto lightDistance = (spotLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * spotLight.falloff);
            auto lightIntensity = spotLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((intersection.point - ray.origin).length() < depth)
        {
            depth = (intersection.point - ray.origin).length();
            color = tempColor;
        }
    }

    */


    // check for quad intersections
    /*
    for (auto quad : quads)
    {
        intersection = ray.intersect(quad);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.origin).length() > depth)
            continue;

        // material properties for quad
        material = quad.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(normalLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from quad
            if (!checkLightVisibility(intersection.point, normalLight.position, quad.id))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(spotLight.position - intersection.point) < 0)
                continue;

            // check if light is visible from quad
            if (!checkLightVisibility(intersection.point, spotLight.position, quad.id))
                continue;
            // check cutoff
            auto lightDirection = (spotLight.position - intersection.point).normalize();
            // check if angle with light is less than cutoff
            if (lightDirection.dot(spotLight.direction) < cos(spotLight.cutoff))
                continue;
            auto normal = intersection.normal.normalize();
            auto lightDistance = (spotLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * spotLight.falloff);
            auto lightIntensity = spotLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((intersection.point - ray.origin).length() < depth)
        {
            depth = (intersection.point - ray.origin).length();
            color = tempColor;
        }
    }

    */

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
        material.color = ((int)(intersection.point.x / checkerboardWidth) + (int)(intersection.point.z / checkerboardWidth)) % 2 == 0 ? Color(1.0f, 1.0f, 1.0f) : Color(1.0f, 1.0f, 1.0f);
        material.specular = 0.0f;
        material.shininess = 0;
        tempColor = Color(0.0f, 0.0f, 0.0f);

        // check if lights are visible
        for (auto normalLight : normalLights)
        {

            // check if light direction is visible from plane
            if (intersection.normal.dot(normalLight.position - intersection.point) < 0)
                continue;

            if(!lightVisible(intersection.point, normalLight.position, -1))
                continue;
            
            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
            tempColor = tempColor + normalLight.color * material.color * material.diffuse * lambert + normalLight.color * material.color * material.specular * phong;
        }
        for (auto spotLight : spotLights)
        {
            // check if light direction is visible from plane
            if (intersection.normal.dot(spotLight.position - intersection.point) < 0)
                continue;

            if(!lightVisible(intersection.point, spotLight.position, -1))
                continue;

            // check if angle with light is less than cutoff
            auto lightDirection = (spotLight.position - intersection.point).normalize();
            if (lightDirection.dot(spotLight.direction) < cos(spotLight.cutoff))
                continue;

            auto normal = intersection.normal.normalize();
            auto lightDistance = (spotLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * spotLight.falloff);
            auto lightIntensity = spotLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.direction)), material.shininess) * lightIntensity;
            tempColor = tempColor + spotLight.color * material.color * material.diffuse * lambert + spotLight.color * material.color * material.specular * phong;
        }

        // tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        depth = (intersection.point - ray.origin).length();
        color = tempColor;
    }

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
        int row = numPixels - (int)(i / numPixels) - 1;
        int col = numPixels - (int)(i % numPixels) - 1;
        colorBuffer[col][row] = raycast(generatedRay[i], 1);
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