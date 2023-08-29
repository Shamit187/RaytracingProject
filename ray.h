#ifndef RAY_H
#define RAY_H
#include "vector3.h"
#include "object.h"
#include <vector>
#include <math.h>

struct Intersection
{
    vec3 point;
    vec3 normal;
};

struct Ray
{
    vec3 A;
    vec3 B;
    float time;

    vec3 point_at_parameter(float t) const { return A + B * t; }

    Ray() {}
    Ray(const vec3 &a, const vec3 &b, float ti = 0.0)
    {
        A = a;
        B = b;
        time = ti;
    }

    Intersection intersect(const Sphere &sphere) const
    {
        vec3 oc = A - sphere.center;
        float a = B.dot(B);
        float b = oc.dot(B);
        float c = oc.dot(oc) - sphere.radius * sphere.radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0)
        {
            float temp = (-b - sqrt(discriminant)) / a;
            
            if (temp < 0.0f)
            {
                temp = (-b + sqrt(discriminant)) / a;
            }
            if (temp > 0.0f)
            {
                Intersection result;
                result.point = point_at_parameter(temp);
                result.normal = (result.point - sphere.center) / sphere.radius;
                return result;
            }
        }
        return Intersection();
    }

    Intersection intersect(const Triangle &triangle) const
    {
        vec3 normal = triangle.normal;
        float t = (triangle.point1 - A).dot(normal) / B.dot(normal);
        if (t > 0.0f)
        {
            vec3 point = point_at_parameter(t);
            vec3 v0 = triangle.point2 - triangle.point1;
            vec3 v1 = triangle.point3 - triangle.point1;
            vec3 v2 = point - triangle.point1;
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
                Intersection result;
                result.point = point;
                result.normal = normal;
                return result;
            }
        }
        return Intersection();
    }

    Intersection intersect(const Quad &quad) const
    {
        vec3 normal = quad.normal;
        float t = (quad.bottomLeftPoint - A).dot(normal) / B.dot(normal);
        if (t > 0.0f)
        {
            vec3 point = point_at_parameter(t);
            vec3 v0 = quad.bottomRightPoint - quad.bottomLeftPoint;
            vec3 v1 = quad.topLeftPoint - quad.bottomLeftPoint;
            vec3 v2 = point - quad.bottomLeftPoint;
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
                Intersection result;
                result.point = point;
                result.normal = normal;
                return result;
            }
        }
        return Intersection();
    }
};

std::vector<Ray> generateRays(const float fovY,const float aspectRatio,const vec3 &eye,const vec3 &lookAt,const vec3 &up,const int numPixels)
{
    std::vector<Ray> rays;
    vec3 w = (eye - lookAt).normalize();
    vec3 u = up.cross(w).normalize();
    vec3 v = w.cross(u);
    float fovX = fovY * aspectRatio;
    float halfWidth = tan(fovX / 2.0f);
    float halfHeight = tan(fovY / 2.0f);
    vec3 lowerLeftCorner = eye - u * halfWidth - v * halfHeight - w;
    vec3 horizontal = u * halfWidth * 2.0f;
    vec3 vertical = v * halfHeight * 2.0f;
    for (int i = 0; i < numPixels; i++)
    {
        for (int j = 0; j < numPixels; j++)
        {
            float x = (float)i / (float)numPixels;
            float y = (float)j / (float)numPixels;
            vec3 direction = lowerLeftCorner + horizontal * x + vertical * y - eye;
            direction = direction.normalize();
            Ray r = Ray(eye, direction);
            rays.push_back(r);
        }
    }
    return rays;
}

#endif
