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

void startRayTrace()
{
    /* Ray generation, buffer Initialization */
    auto start = std::chrono::high_resolution_clock::now();
    auto generatedRay = generateRays(fovY, aspectRatio, eye, lookAt, up, numPixels);
    std::vector<std::vector <Color>> colorBuffer = std::vector<std::vector <Color>>(numPixels, std::vector<Color>(numPixels, Color(0.0f, 0.0f, 0.0f)));
    std::vector<std::vector<GLfloat>> depthBuffer = std::vector<std::vector <GLfloat>>(numPixels, std::vector<float>(numPixels, (GLfloat)farPlane));
    std::cout << "Far Plane: " << farPlane << "\n";
    auto boundingBox = generateBoundingBox(nearPlane, farPlane, fovY, aspectRatio, eye, lookAt, up);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Ray Generation Complete. Total Rays " << generatedRay.size() << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    /* Intersection Calculation */
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < generatedRay.size(); i ++)
    {
        int row = numPixels - (int)(i / numPixels);
        int col = (int)(i % numPixels);

        for(auto sphere: spheres){
            Intersection intersection = generatedRay[i].intersect(sphere);
            if(intersection.point.x == 0 && intersection.point.y == 0 && intersection.point.z == 0) continue;
            // if(!checkInsideBoundingBox(intersection.point, boundingBox)) continue;
            if((eye - intersection.point).length() < depthBuffer[row][col]){
                depthBuffer[row][col] = (intersection.point - eye).length();
                colorBuffer[row][col] = sphere.material.color;
            }
            
        }

        // for(auto triangle: triangles){
        //     auto intersection = generatedRay[i].intersect(triangle);
        //     // if(!checkInsideBoundingBox(intersection.point, boundingBox)) continue;
        //     if((intersection.point - eye).length() < depthBuffer[row][col]){
        //         depthBuffer[row][col] = (intersection.point - eye).length();
        //         colorBuffer[row][col] = triangle.material.color;
        //     }
        // }

        // for(auto quad: quads){
        //     auto intersection = generatedRay[i].intersect(quad);
        //     // if(!checkInsideBoundingBox(intersection.point, boundingBox)) continue;
        //     if((intersection.point - eye).length() < depthBuffer[row][col]){
        //         depthBuffer[row][col] = (intersection.point - eye).length();
        //         colorBuffer[row][col] = quad.material.color;
        //     }
        // }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Intersection Calculation Complete. Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    /* Image Generation */
    start = std::chrono::high_resolution_clock::now();
    bitmap_image image(numPixels, numPixels);
    for(int i = 0; i < numPixels; i++)
    {
        for(int j = 0; j < numPixels; j++)
        {
            image.set_pixel(i, j, colorBuffer[i][j].r * 255, colorBuffer[i][j].g * 255, colorBuffer[i][j].b * 255);
        }
    }
    image.save_image("output.bmp");
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Image Generation Complete. Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}

#endif // RAY_TRACE