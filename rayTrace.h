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

const Color ambientColor = Color(0.1f, 0.1f, 0.1f);
std::ofstream logFile("log.txt");

bool checkLightVisibility(const vec3 &point, const vec3 &light)
{
    auto ray = Ray(point, point - light);
    GLfloat e = 0.0001f;
    auto intersection = ray.intersectCheckerBoard();
    if (intersection.valid == true && (intersection.point - point).length() > e && (intersection.point - light).length() < (point - light).length())
        return false;
    for (auto sphere : spheres)
        if (ray.intersect(sphere).valid == true && (ray.intersect(sphere).point - point).length() > e && (ray.intersect(sphere).point - light).length() < (point - light).length())
            return false;

    for (auto triangle : triangles)
        if (ray.intersect(triangle).valid == true && (ray.intersect(triangle).point - point).length() > e && (ray.intersect(triangle).point - light).length() < (point - light).length())
            return false;

    for (auto quad : quads)
        if (ray.intersect(quad).valid == true && (ray.intersect(quad).point - point).length() > e && (ray.intersect(quad).point - light).length() < (point - light).length())
            return false;

    return true;
}

Color raycast(const Ray &ray, const int iterationDeapth)
{
    if (iterationDeapth == 0)
        return Color(0.0f, 0.0f, 0.0f);

    // fong model for lighting
    GLfloat depth = 10000.0f;
    GLfloat lambert = 0, phong = 0;
    Color color = Color(0.0f, 0.0f, 0.0f);
    Intersection intersection;
    Color tempColor = Color(0.0f, 0.0f, 0.0f);
    Material material;

    logFile << "New Ray\n";
    logFile << "Ray Origin: " << ray.A.x << " " << ray.A.y << " " << ray.A.z << "\n";
    logFile << "Ray Direction: " << ray.B.x << " " << ray.B.y << " " << ray.B.z << "\n";
    logFile << "Inital Depth: " << depth << "\n";

    // check for sphere intersections
    for (auto sphere : spheres)
    {

        intersection = ray.intersect(sphere);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.A).length() > depth)
            continue;
        logFile << "Sphere Intersection\n";
        // material properties for sphere
        material = sphere.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            if (!checkLightVisibility(intersection.point, normalLight.position))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            if (!checkLightVisibility(intersection.point, spotLight.position))
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
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((ray.A - intersection.point).length() < depth)
        {
            depth = (intersection.point - ray.A).length();
            color = tempColor;
        }
    }

    // check for triangle intersections
    for (auto triangle : triangles)
    {
        intersection = ray.intersect(triangle);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.A).length() > depth)
            continue;
        logFile << "Triangle Intersection\n";
        // material properties for triangle
        material = triangle.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            if (!checkLightVisibility(intersection.point, normalLight.position))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            if (!checkLightVisibility(intersection.point, spotLight.position))
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
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((intersection.point - ray.A).length() < depth)
        {
            depth = (intersection.point - ray.A).length();
            color = tempColor;
        }
    }

    // check for quad intersections
    for (auto quad : quads)
    {
        intersection = ray.intersect(quad);
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.A).length() > depth)
            continue;
        logFile << "Quad Intersection\n";
        // material properties for quad
        material = quad.material;

        // check if lights are visible
        lambert = 0, phong = 0;

        for (auto normalLight : normalLights)
        {
            if (!checkLightVisibility(intersection.point, normalLight.position))
                continue;

            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }

        for (auto spotLight : spotLights)
        {
            if (!checkLightVisibility(intersection.point, spotLight.position))
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
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }
        tempColor = Color(0.0f, 0.0f, 0.0f);
        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((intersection.point - ray.A).length() < depth)
        {
            depth = (intersection.point - ray.A).length();
            color = tempColor;
        }
    }

    // check for checkerboard intersection
    for (int i = 0; i < 1; i++)
    {
        intersection = ray.intersectCheckerBoard();
        if (intersection.valid == false)
            continue;
        if ((intersection.point - ray.A).length() > depth)
            continue;
        // material properties for checkerBoard
        material = Material();
        material.ambient = checkerboardAmbientCoeff;
        material.diffuse = checkerboardDiffuseCoeff;
        material.reflection = checkerboardReflectionCoeff;
        material.color = ((int)(intersection.point.x / checkerboardWidth) + (int)(intersection.point.z / checkerboardWidth)) % 2 == 0 ? Color(0.0f, 0.0f, 0.0f) : Color(1.0f, 1.0f, 1.0f);
        material.specular = 0.0f;
        material.shininess = 0;

        // check if lights are visible
        for (auto normalLight : normalLights)
        {
            if (!checkLightVisibility(intersection.point, normalLight.position))
                continue;
            auto lightDirection = (normalLight.position - intersection.point).normalize();
            auto normal = intersection.normal.normalize();
            auto lightDistance = (normalLight.position - intersection.point).length();
            auto scalingFactor = exp(-lightDistance * lightDistance * normalLight.falloff);
            auto lightIntensity = normalLight.intensity * scalingFactor;

            lambert += std::max(0.0f, normal.dot(lightDirection)) * lightIntensity;
            auto reflectedRay = (normal * 2.0f * normal.dot(lightDirection) - lightDirection).normalize();
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }
        for (auto spotLight : spotLights)
        {
            if (!checkLightVisibility(intersection.point, spotLight.position))
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
            phong += pow(std::max(0.0f, reflectedRay.dot(ray.B)), material.shininess) * lightIntensity;
        }

        tempColor = tempColor + ambientColor * material.ambient + material.color * material.diffuse * lambert + material.color * material.specular * phong;
        // if distance is less than depth, update depth and color
        if ((intersection.point - ray.A).length() < depth)
        {
            depth = (intersection.point - ray.A).length();
            color = tempColor;
        }
    }

    if (depth == 10000.0f)
        logFile << "No Intersection\n";
    return color;
}

void startRayTrace()
{
    /* Ray generation, buffer Initialization */
    auto start = std::chrono::high_resolution_clock::now();
    auto generatedRay = generateRays(fovY, aspectRatio, eye, lookAt, up, numPixels);
    auto colorBuffer = std::vector<std::vector<Color>>(numPixels, std::vector<Color>(numPixels, Color(0.0f, 0.0f, 0.0f)));
    auto depthBuffer = std::vector<std::vector<GLfloat>>(numPixels, std::vector<float>(numPixels, (GLfloat)farPlane));
    auto boundingBox = generateBoundingBox(nearPlane, farPlane, fovY, aspectRatio, eye, lookAt, up);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Ray Generation Complete. Total Rays " << generatedRay.size() << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    /* Intersection Calculation */
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < generatedRay.size(); i++)
    {
        int row = numPixels - (int)(i / numPixels) - 1;
        int col = (int)(i % numPixels);

        /*
        // intersecting with the checkerboard
        auto intersection = generatedRay[i].intersectCheckerBoard();
        if (intersection.point.x != 0 || intersection.point.y != 0 || intersection.point.z != 0)
        {
            if ((intersection.point - eye).length() < depthBuffer[row][col])
            {
                depthBuffer[row][col] = (intersection.point - eye).length();
                auto newX = (int)(intersection.point.x / checkerboardWidth);
                auto newZ = (int)(intersection.point.z / checkerboardWidth);

                if ((newX + newZ) % 2 == 0)
                {
                    colorBuffer[row][col] = Color(0.0f, 0.0f, 0.0f);
                }
                else
                {
                    colorBuffer[row][col] = Color(1.0f, 1.0f, 1.0f);
                }
            }
        }

        for (auto sphere : spheres)
        {
            Intersection intersection = generatedRay[i].intersect(sphere);
            if (intersection.point.x == 0 && intersection.point.y == 0 && intersection.point.z == 0)
                continue;
            // if(!checkInsideBoundingBox(intersection.point, boundingBox)) continue;
            if ((eye - intersection.point).length() < depthBuffer[row][col])
            {
                depthBuffer[row][col] = (intersection.point - eye).length();
                colorBuffer[row][col] = sphere.material.color;
            }
        }

        for (auto triangle : triangles)
        {
            auto intersection = generatedRay[i].intersect(triangle);
            if (intersection.point.x == 0 && intersection.point.y == 0 && intersection.point.z == 0)
                continue;
            // if(!checkInsideBoundingBox(intersection.point, boundingBox)) continue;
            if ((intersection.point - eye).length() < depthBuffer[row][col])
            {
                depthBuffer[row][col] = (intersection.point - eye).length();
                colorBuffer[row][col] = triangle.material.color;
            }
        }

        for (auto quad : quads)
        {
            auto intersection = generatedRay[i].intersect(quad);
            if (intersection.point.x == 0 && intersection.point.y == 0 && intersection.point.z == 0)
                continue;
            if ((intersection.point - eye).length() < depthBuffer[row][col])
            {
                depthBuffer[row][col] = (intersection.point - eye).length();
                colorBuffer[row][col] = quad.material.color;
            }
        }
        */

        colorBuffer[row][col] = raycast(generatedRay[i], 1);
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