#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "globalVariables.h"
#include "vector3.h"
#include "object.h"
#include "ray.h"

struct BoundingBox
{
    vec3 nearPlane[4];
    vec3 farPlane[4];
};

BoundingBox generateBoundingBox(const float &nearPlaneDist, const float &farPlaneDist, const float &fovY, const float &aspectRatio, const vec3 &eye, const vec3 &lookAt, const vec3 &up)
{
    BoundingBox boundingBox;

    // Calculate the dimensions of the near and far planes
    float halfHeightNear = tan(fovY / 2.0f) * nearPlaneDist;
    float halfWidthNear = halfHeightNear * aspectRatio;
    float halfHeightFar = tan(fovY / 2.0f) * farPlaneDist;
    float halfWidthFar = halfHeightFar * aspectRatio;

    // Calculate the camera basis vectors
    vec3 forward = (lookAt - eye).normalize();
    vec3 right = (forward.cross(up)).normalize();
    vec3 realUp = right.cross(forward).normalize();

    // Calculate the corner points of the near plane
    boundingBox.nearPlane[0] = eye + forward * nearPlaneDist - right * halfWidthNear - realUp * halfHeightNear;
    boundingBox.nearPlane[1] = eye + forward * nearPlaneDist + right * halfWidthNear - realUp * halfHeightNear;
    boundingBox.nearPlane[2] = eye + forward * nearPlaneDist + right * halfWidthNear + realUp * halfHeightNear;
    boundingBox.nearPlane[3] = eye + forward * nearPlaneDist - right * halfWidthNear + realUp * halfHeightNear;

    // Calculate the corner points of the far plane
    boundingBox.farPlane[0] = eye + forward * farPlaneDist - right * halfWidthFar - realUp * halfHeightFar;
    boundingBox.farPlane[1] = eye + forward * farPlaneDist + right * halfWidthFar - realUp * halfHeightFar;
    boundingBox.farPlane[2] = eye + forward * farPlaneDist + right * halfWidthFar + realUp * halfHeightFar;
    boundingBox.farPlane[3] = eye + forward * farPlaneDist - right * halfWidthFar + realUp * halfHeightFar;

    return boundingBox;
}

bool checkInsideBoundingBox(const vec3 &point, const BoundingBox &boundingBox)
{
    // Check if the point is inside the bounding box

    // Check against the near and far planes
    for (int i = 0; i < 4; ++i)
    {
        if ((boundingBox.nearPlane[i] - boundingBox.farPlane[i]).dot(point - boundingBox.farPlane[i]) > 0.0f)
        {
            return false; // Point is outside one of the planes
        }
    }

    // Check against the side planes (formed by connecting near and far plane corner points)
    for (int i = 0; i < 4; ++i)
    {
        int nextIndex = (i + 1) % 4;
        vec3 edgeDir = (boundingBox.nearPlane[nextIndex] - boundingBox.nearPlane[i]).normalize();
        vec3 edgeNormal = edgeDir.cross(boundingBox.farPlane[i] - boundingBox.nearPlane[i]);

        if (edgeNormal.dot(point - boundingBox.nearPlane[i]) > 0.0f)
        {
            return false; // Point is outside one of the side planes
        }
    }

    return true; // Point is inside the bounding box
}

#endif // BOUNDING_BOX_H