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


BoundingBox generateBoundingBox(const float &nearPlane, const float &farPlane, const float &fovY, const float &aspectRatio, const vec3 &eye, const vec3 &lookAt, const vec3 &up){
    BoundingBox boundingBox;
    vec3 w = (eye - lookAt).normalize();
    vec3 u = up.cross(w).normalize();
    vec3 v = w.cross(u).normalize();
    float halfHeight = tan(fovY / 2.0f);
    float halfWidth = halfHeight * aspectRatio;
    vec3 center = eye - w;
    vec3 nearCenter = center - w * nearPlane;
    vec3 farCenter = center - w * farPlane;
    boundingBox.nearPlane[0] = nearCenter + u * halfWidth + v * halfHeight;
    boundingBox.nearPlane[1] = nearCenter - u * halfWidth + v * halfHeight;
    boundingBox.nearPlane[2] = nearCenter - u * halfWidth - v * halfHeight;
    boundingBox.nearPlane[3] = nearCenter + u * halfWidth - v * halfHeight;
    boundingBox.farPlane[0] = farCenter + u * halfWidth + v * halfHeight;
    boundingBox.farPlane[1] = farCenter - u * halfWidth + v * halfHeight;
    boundingBox.farPlane[2] = farCenter - u * halfWidth - v * halfHeight;
    boundingBox.farPlane[3] = farCenter + u * halfWidth - v * halfHeight;
    return boundingBox;
}

bool checkInsideBoundingBox(const vec3 &point, const BoundingBox &boundingBox){
    vec3 v0 = boundingBox.nearPlane[1] - boundingBox.nearPlane[0];
    vec3 v1 = boundingBox.nearPlane[2] - boundingBox.nearPlane[0];
    vec3 v2 = point - boundingBox.nearPlane[0];
    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    if (u >= 0.0f && v >= 0.0f && u + v <= 1.0f)
    {
        return true;
    }
    return false;
}

#endif // BOUNDING_BOX_H