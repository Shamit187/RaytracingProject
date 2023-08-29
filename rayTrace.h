#ifndef RAY_TRACE
#define RAY_TRACE

#include "globalVariables.h"
#include "vector3.h"
#include "object.h"
#include "ray.h"
#include <vector>
#include <math.h>
#include <chrono>

void startRayTrace()
{
    auto start = std::chrono::high_resolution_clock::now();

    auto generatedRay = generateRays(fovY, aspectRatio, eye, lookAt, up, numPixels);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Ray Generation Complete. Total Rays " << generatedRay.size() << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    start = std::chrono::high_resolution_clock::now();
}

#endif // RAY_TRACE