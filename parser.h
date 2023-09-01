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
    int id = 0;

    for (int i = 0; i < numberOfObject; i++)
    {
        std::string objectType;
        inputFile >> objectType;

        if (objectType == "sphere")
        {
            double x, y, z, radius;
            inputFile >> x >> y >> z >> radius;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection >> material.refraction;
            inputFile >> material.shininess;

            Sphere sphere;
            sphere.center = vec3(x, y, z);
            sphere.radius = radius;
            sphere.material = material;
            sphere.id = id++;

            spheres.push_back(sphere);
        }
        else if (objectType == "pyramid")
        {
            double x, y, z;
            inputFile >> x >> y >> z;
            double width, height;
            inputFile >> width >> height;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection >> material.refraction;
            inputFile >> material.shininess;

            Pyramid pyramid;
            pyramid.lowestPoint = vec3(x, y, z);
            pyramid.width = width;
            pyramid.height = height;
            pyramid.material = material;
            pyramid.id = id++;

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
            vec3 normal = vec3({0.0f, -1.0f, 0.0f});

            // Calculate normal of side face 1
            vec3 normal1 = (vertices[0] - topPoint).cross(vertices[1] - topPoint).normalize();

            // Calculate normal of side face 2
            vec3 normal2 = (vertices[1] - topPoint).cross(vertices[2] - topPoint).normalize();

            // Calculate normal of side face 3
            vec3 normal3 = (vertices[2] - topPoint).cross(vertices[3] - topPoint).normalize();

            // Calculate normal of side face 4
            vec3 normal4 = (vertices[3] - topPoint).cross(vertices[0] - topPoint).normalize();

            quads.push_back(Quad{vertices[0], vertices[1], vertices[2], vertices[3], normal, material, pyramid.id});
            triangles.push_back(Triangle{vertices[0], vertices[1], topPoint, normal1, material, pyramid.id});
            triangles.push_back(Triangle{vertices[1], vertices[2], topPoint, normal2, material, pyramid.id});
            triangles.push_back(Triangle{vertices[2], vertices[3], topPoint, normal3, material, pyramid.id});
            triangles.push_back(Triangle{vertices[3], vertices[0], topPoint, normal4, material, pyramid.id});
        }
        else if (objectType == "cube")
        {
            double x, y, z;
            inputFile >> x >> y >> z;
            double side;
            inputFile >> side;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection >> material.refraction;
            inputFile >> material.shininess;

            Cube cube;
            cube.bottomLowerLeftPoint = vec3(x, y, z);
            cube.side = side;
            cube.material = material;
            cube.id = id++;

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
            vec3 normal = (vec3{0.0f, -1.0f, 0.0f}).normalize();

            // Calculate normal of side face 1 (front)
            vec3 normal1 = (vec3{0.0f, 0.0f, 1.0f}).normalize();

            // Calculate normal of side face 2 (right)
            vec3 normal2 = (vec3{1.0f, 0.0f, 0.0f}).normalize();

            // Calculate normal of side face 3 (back)
            vec3 normal3 = (vec3{0.0f, 0.0f, -1.0f}).normalize();

            // Calculate normal of side face 4 (left)
            vec3 normal4 = (vec3{-1.0f, 0.0f, 0.0f}).normalize();

            // Calculate normal of top face
            vec3 normal5 = (vec3{0.0f, 1.0f, 0.0f}).normalize();

            quads.push_back(Quad{vertices[0], vertices[1], vertices[2], vertices[3], normal, material, cube.id});
            quads.push_back(Quad{vertices[0], vertices[1], vertices[5], vertices[4], normal1, material, cube.id});
            quads.push_back(Quad{vertices[1], vertices[2], vertices[6], vertices[5], normal2, material, cube.id});
            quads.push_back(Quad{vertices[2], vertices[3], vertices[7], vertices[6], normal3, material, cube.id});
            quads.push_back(Quad{vertices[3], vertices[0], vertices[4], vertices[7], normal4, material, cube.id});
            quads.push_back(Quad{vertices[4], vertices[5], vertices[6], vertices[7], normal5, material, cube.id});
        }
        else if (objectType == "quad")
        {
            vec3 vertices[4];
            inputFile >> vertices[0].x >> vertices[0].y >> vertices[0].z;
            inputFile >> vertices[1].x >> vertices[1].y >> vertices[1].z;
            inputFile >> vertices[2].x >> vertices[2].y >> vertices[2].z;
            inputFile >> vertices[3].x >> vertices[3].y >> vertices[3].z;
            double normalX, normalY, normalZ;
            inputFile >> normalX >> normalY >> normalZ;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection >> material.refraction;
            inputFile >> material.shininess;

            Quad quad;
            quad.bottomLeftPoint = vertices[0];
            quad.bottomRightPoint = vertices[1];
            quad.topRightPoint = vertices[2];
            quad.topLeftPoint = vertices[3];
            quad.normal = vec3(normalX, normalY, normalZ);
            quad.material = material;
            quad.id = id++;

            quads.push_back(quad);
        }
        else if (objectType == "triangle")
        {
            vec3 vertices[3];
            inputFile >> vertices[0].x >> vertices[0].y >> vertices[0].z;
            inputFile >> vertices[1].x >> vertices[1].y >> vertices[1].z;
            inputFile >> vertices[2].x >> vertices[2].y >> vertices[2].z;
            double normalX, normalY, normalZ;
            inputFile >> normalX >> normalY >> normalZ;

            Material material;
            inputFile >> material.color.r >> material.color.g >> material.color.b;
            inputFile >> material.ambient >> material.diffuse >> material.specular >> material.reflection >> material.refraction;
            inputFile >> material.shininess;

            Triangle triangle;
            triangle.point1= vertices[0];
            triangle.point2 = vertices[1];
            triangle.point3 = vertices[2];
            triangle.normal = vec3(normalX, normalY, normalZ);
            triangle.material = material;
            triangle.id = id++;

            triangles.push_back(triangle);
        }
    }

    int numberOfNormalLight;
    inputFile >> numberOfNormalLight;

    for (int i = 0; i < numberOfNormalLight; i++)
    {
        double x, y, z, falloff;
        inputFile >> x >> y >> z >> falloff;

        normalLight light;
        light.position = vec3(x, y, z);
        light.falloff = falloff;
        light.intensity = 1.0f;
        light.color = Color(1.0f, 1.0f, 1.0f);

        normalLights.push_back(light);
        Light l;
        l.type = NORMAL_LIGHT;
        l.position = light.position;
        l.falloff = light.falloff;
        l.intensity = light.intensity;
        l.color = light.color;
        lights.push_back(l);
    }

    int numberOfSpotLight;
    inputFile >> numberOfSpotLight;

    for (int i = 0; i < numberOfSpotLight; i++)
    {
        double x, y, z, falloff;
        inputFile >> x >> y >> z >> falloff;
        double dirX, dirY, dirZ, cutoff;
        inputFile >> dirX >> dirY >> dirZ >> cutoff;

        spotLight light;
        light.position = vec3(x, y, z);
        light.falloff = falloff;
        light.direction = vec3(dirX - x, dirY - y, dirZ- z).normalize();
        light.cutoff = cutoff;
        light.intensity = 1.0f;
        light.color = Color(1.0f, 1.0f, 1.0f);

        spotLights.push_back(light);
        Light l;
        l.type = SPOT_LIGHT;
        l.position = light.position;
        l.falloff = light.falloff;
        l.direction = light.direction;
        l.cutoff = light.cutoff;
        l.intensity = light.intensity;
        l.color = light.color;
        lights.push_back(l);
    }

    return true;
}
#endif // SCENE_H
