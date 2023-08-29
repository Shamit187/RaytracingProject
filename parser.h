#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "globalVariables.h"
#include "vector3.h"
#include "object.h"

bool parseSceneFile(const std::string &filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return false;
    }

    inputFile >> nearPlane >> farPlane >> fovY >> aspectRatio;
    inputFile >> recursionLevel >> numPixels;
    inputFile >> checkerboardWidth;
    inputFile >> checkerboardAmbientCoeff >> checkerboardDiffuseCoeff >> checkerboardReflectionCoeff;

    int numberOfObject;
    inputFile >> numberOfObject;

    for (int i = 0; i < numberOfObject; i++)
    {
        std::string objectType;
        inputFile >> objectType;

        if (objectType == "sphere")
        {
            float x, y, z, radius;
            inputFile >> x >> y >> z >> radius;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection;
            inputFile >> material.shininess;

            Sphere sphere;
            sphere.center = vec3(x, y, z);
            sphere.radius = radius;
            sphere.material = material;

            spheres.push_back(sphere);
        }
        else if (objectType == "pyramid")
        {
            float x, y, z;
            inputFile >> x >> y >> z;
            float width, height;
            inputFile >> width >> height;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection;
            inputFile >> material.shininess;

            Pyramid pyramid;
            pyramid.lowestPoint = vec3(x, y, z);
            pyramid.width = width;
            pyramid.height = height;
            pyramid.material = material;

            pyramids.push_back(pyramid);

            // Calculate vertices of bottom face
            vec3 vertices[4];
            vertices[0] = pyramid.lowestPoint + vec3(pyramid.width / 2, 0.0f, pyramid.width / 2);
            vertices[1] = pyramid.lowestPoint + vec3(pyramid.width / 2, 0.0f, -pyramid.width / 2);
            vertices[2] = pyramid.lowestPoint + vec3(-pyramid.width / 2, 0.0f, -pyramid.width / 2);
            vertices[3] = pyramid.lowestPoint + vec3(-pyramid.width / 2, 0.0f, pyramid.width / 2);

            // Calculate vertices of top point
            vec3 topPoint = pyramid.lowestPoint + vec3(0.0f, pyramid.height, 0.0f);

            // Calculate normal of bottom face
            vec3 normal = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]).normalize();

            // Calculate normal of side face 1
            vec3 normal1 = (vertices[0] - topPoint).cross(vertices[1] - topPoint).normalize();

            // Calculate normal of side face 2
            vec3 normal2 = (vertices[1] - topPoint).cross(vertices[2] - topPoint).normalize();

            // Calculate normal of side face 3
            vec3 normal3 = (vertices[2] - topPoint).cross(vertices[3] - topPoint).normalize();

            // Calculate normal of side face 4
            vec3 normal4 = (vertices[3] - topPoint).cross(vertices[0] - topPoint).normalize();

            quads.push_back(Quad{vertices[0], vertices[1], vertices[2], vertices[3], normal, material});
            triangles.push_back(Triangle{vertices[0], vertices[1], topPoint, normal1, material});
            triangles.push_back(Triangle{vertices[1], vertices[2], topPoint, normal2, material});
            triangles.push_back(Triangle{vertices[2], vertices[3], topPoint, normal3, material});
            triangles.push_back(Triangle{vertices[3], vertices[0], topPoint, normal4, material});
        }
        else if (objectType == "cube")
        {
            float x, y, z;
            inputFile >> x >> y >> z;
            float side;
            inputFile >> side;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection;
            inputFile >> material.shininess;

            Cube cube;
            cube.bottomLowerLeftPoint = vec3(x, y, z);
            cube.side = side;
            cube.material = material;

            cubes.push_back(cube);

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

            // Calculate normal of bottom face
            vec3 normal = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]).normalize();
            
            // Calculate normal of side face 1 (front)
            vec3 normal1 = (vertices[0] - vertices[4]).cross(vertices[1] - vertices[4]).normalize();

            // Calculate normal of side face 2 (right)
            vec3 normal2 = (vertices[1] - vertices[5]).cross(vertices[2] - vertices[5]).normalize();

            // Calculate normal of side face 3 (back)
            vec3 normal3 = (vertices[2] - vertices[6]).cross(vertices[3] - vertices[6]).normalize();

            // Calculate normal of side face 4 (left)
            vec3 normal4 = (vertices[3] - vertices[7]).cross(vertices[0] - vertices[7]).normalize();

            // Calculate normal of top face
            vec3 normal5 = (vertices[4] - vertices[5]).cross(vertices[6] - vertices[5]).normalize();

            quads.push_back(Quad{vertices[0], vertices[1], vertices[2], vertices[3], normal, material});
            quads.push_back(Quad{vertices[0], vertices[1], vertices[5], vertices[4], normal1, material});
            quads.push_back(Quad{vertices[1], vertices[2], vertices[6], vertices[5], normal2, material});
            quads.push_back(Quad{vertices[2], vertices[3], vertices[7], vertices[6], normal3, material});
            quads.push_back(Quad{vertices[3], vertices[0], vertices[4], vertices[7], normal4, material});
            quads.push_back(Quad{vertices[4], vertices[5], vertices[6], vertices[7], normal5, material});
        }
    }

    int numberOfNormalLight;
    inputFile >> numberOfNormalLight;

    for (int i = 0; i < numberOfNormalLight; i++)
    {
        float x, y, z, falloff;
        inputFile >> x >> y >> z >> falloff;

        normalLight light;
        light.position = vec3(x, y, z);
        light.falloff = falloff;
        light.intensity = 1.0f;
        light.color = Color(1.0f, 1.0f, 1.0f);

        normalLights.push_back(light);
    }

    int numberOfSpotLight;
    inputFile >> numberOfSpotLight;

    for (int i = 0; i < numberOfSpotLight; i++)
    {
        float x, y, z, falloff;
        inputFile >> x >> y >> z >> falloff;
        float dirX, dirY, dirZ, cutoff;
        inputFile >> dirX >> dirY >> dirZ >> cutoff;

        spotLight light;
        light.position = vec3(x, y, z);
        light.falloff = falloff;
        light.direction = vec3(dirX, dirY, dirZ);
        light.cutoff = cutoff;
        light.intensity = 1.0f;
        light.color = Color(1.0f, 1.0f, 1.0f);

        spotLights.push_back(light);
    }

    return true;
}

void printDescriptionOfScene()
{
    std::cout << "nearPlane: " << nearPlane << std::endl;
    std::cout << "farPlane: " << farPlane << std::endl;
    std::cout << "fovY: " << fovY << std::endl;
    std::cout << "aspectRatio: " << aspectRatio << std::endl;
    std::cout << "recursionLevel: " << recursionLevel << std::endl;
    std::cout << "numPixels: " << numPixels << std::endl;
    std::cout << "checkerboardWidth: " << checkerboardWidth << std::endl;
    std::cout << "checkerboardAmbientCoeff: " << checkerboardAmbientCoeff << std::endl;
    std::cout << "checkerboardDiffuseCoeff: " << checkerboardDiffuseCoeff << std::endl;
    std::cout << "checkerboardReflectionCoeff: " << checkerboardReflectionCoeff << std::endl;

    std::cout << "spheres: " << spheres.size() << std::endl;
    for (int i = 0; i < spheres.size(); i++)
    {
        std::cout << "sphere " << i << std::endl;
        std::cout << "center: " << spheres[i].center.x << " " << spheres[i].center.y << " " << spheres[i].center.z << std::endl;
        std::cout << "radius: " << spheres[i].radius << std::endl;
        std::cout << "color: " << spheres[i].material.color.r << " " << spheres[i].material.color.g << " " << spheres[i].material.color.b << std::endl;
        std::cout << "ambient: " << spheres[i].material.ambient << std::endl;
        std::cout << "diffuse: " << spheres[i].material.diffuse << std::endl;
        std::cout << "specular: " << spheres[i].material.specular << std::endl;
        std::cout << "reflection: " << spheres[i].material.reflection << std::endl;
        std::cout << "shininess: " << spheres[i].material.shininess << std::endl;
    }

    std::cout << "pyramids: " << pyramids.size() << std::endl;
    for (int i = 0; i < pyramids.size(); i++)
    {
        std::cout << "pyramid " << i << std::endl;
        std::cout << "lowestPoint: " << pyramids[i].lowestPoint.x << " " << pyramids[i].lowestPoint.y << " " << pyramids[i].lowestPoint.z << std::endl;
        std::cout << "width: " << pyramids[i].width << std::endl;
        std::cout << "height: " << pyramids[i].height << std::endl;
        std::cout << "color: " << pyramids[i].material.color.r << " " << pyramids[i].material.color.g << " " << pyramids[i].material.color.b << std::endl;
        std::cout << "ambient: " << pyramids[i].material.ambient << std::endl;
        std::cout << "diffuse: " << pyramids[i].material.diffuse << std::endl;
        std::cout << "specular: " << pyramids[i].material.specular << std::endl;
        std::cout << "reflection: " << pyramids[i].material.reflection << std::endl;
        std::cout << "shininess: " << pyramids[i].material.shininess << std::endl;
    }

    std::cout << "cubes: " << cubes.size() << std::endl;
    for (int i = 0; i < cubes.size(); i++)
    {
        std::cout << "cube " << i << std::endl;
        std::cout << "bottomLowerLeftPoint: " << cubes[i].bottomLowerLeftPoint.x << " " << cubes[i].bottomLowerLeftPoint.y << " " << cubes[i].bottomLowerLeftPoint.z << std::endl;
        std::cout << "side: " << cubes[i].side << std::endl;
        std::cout << "color: " << cubes[i].material.color.r << " " << cubes[i].material.color.g << " " << cubes[i].material.color.b << std::endl;
        std::cout << "ambient: " << cubes[i].material.ambient << std::endl;
        std::cout << "diffuse: " << cubes[i].material.diffuse << std::endl;
        std::cout << "specular: " << cubes[i].material.specular << std::endl;
        std::cout << "reflection: " << cubes[i].material.reflection << std::endl;
        std::cout << "shininess: " << cubes[i].material.shininess << std::endl;
    }

    std::cout << "normalLights: " << normalLights.size() << std::endl;
    for (int i = 0; i < normalLights.size(); i++)
    {
        std::cout << "normalLight " << i << std::endl;
        std::cout << "position: " << normalLights[i].position.x << " " << normalLights[i].position.y << " " << normalLights[i].position.z << std::endl;
        std::cout << "falloff: " << normalLights[i].falloff << std::endl;
    }

    std::cout << "spotLights: " << spotLights.size() << std::endl;
    for (int i = 0; i < spotLights.size(); i++)
    {
        std::cout << "spotLight " << i << std::endl;
        std::cout << "position: " << spotLights[i].position.x << " " << spotLights[i].position.y << " " << spotLights[i].position.z << std::endl;
        std::cout << "falloff: " << spotLights[i].falloff << std::endl;
        std::cout << "direction: " << spotLights[i].direction.x << " " << spotLights[i].direction.y << " " << spotLights[i].direction.z << std::endl;
        std::cout << "cutoff: " << spotLights[i].cutoff << std::endl;
    }
}

#endif // SCENE_H
