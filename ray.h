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
    bool valid;

    Intersection() { valid = false; }
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
                result.valid = true;
                return result;
            }
        }
        return Intersection();
    }

    Intersection intersect(const Triangle &triangle) const
    {
        Intersection intersection;

        // Calculate the normal of the triangle
        vec3 triangleNormal = (triangle.normal).normalize();

        // Calculate the denominator of the ray-plane intersection formula
        float denominator = triangleNormal.dot(B);

        // Check if the ray is nearly parallel to the triangle
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the triangle
            vec3 originToTriangle = triangle.point1 - A;

            // Calculate the distance from the ray's origin to the intersection point
            float t = originToTriangle.dot(triangleNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                vec3 intersectionPoint = A + B * t;

                // Check if the intersection point is inside the triangle
                vec3 edge1 = triangle.point2 - triangle.point1;
                vec3 edge2 = triangle.point3 - triangle.point1;
                vec3 h = B.cross(edge2);
                float a = edge1.dot(h);

                if (a > -1e-6 && a < 1e-6)
                    return Intersection(); // Ray is parallel to the triangle

                float f = 1.0f / a;
                vec3 s = intersectionPoint - triangle.point1;
                float u = f * s.dot(h);

                if (u < 0.0f || u > 1.0f)
                    return Intersection();

                vec3 q = s.cross(edge1);
                float v = f * B.dot(q);

                if (v < 0.0f || u + v > 1.0f)
                    return Intersection();

                // The intersection point is inside the triangle
                intersection.point = intersectionPoint;
                intersection.normal = triangleNormal;
                intersection.valid = true;
                return intersection;
            }
        }

        // If there is no valid intersection, return default values
        return Intersection();
    }

    Intersection intersect(const Quad &quad) const
    {
        Intersection intersection;

        // Calculate the normal of the plane defined by the quad
        vec3 quadNormal = (quad.normal).normalize();

        // Calculate the denominator of the ray-plane intersection formula
        float denominator = quadNormal.dot(B);

        // Check if the ray is nearly parallel to the plane
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the plane
            vec3 originToQuad = quad.bottomLeftPoint - A;

            // Calculate the distance from the ray's origin to the intersection point
            float t = originToQuad.dot(quadNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                intersection.point = A + B * t;

                // Check if the intersection point is within the quad's boundaries
                vec3 quadToIntersection = intersection.point - quad.bottomLeftPoint;
                float dotBL = quadToIntersection.dot(quad.bottomRightPoint - quad.bottomLeftPoint);
                float dotTL = quadToIntersection.dot(quad.topLeftPoint - quad.bottomLeftPoint);

                if (dotBL >= 0.0f && dotBL <= (quad.bottomRightPoint - quad.bottomLeftPoint).dot(quad.bottomRightPoint - quad.bottomLeftPoint) &&
                    dotTL >= 0.0f && dotTL <= (quad.topLeftPoint - quad.bottomLeftPoint).dot(quad.topLeftPoint - quad.bottomLeftPoint))
                {
                    // The intersection point is within the quad's boundaries
                    intersection.normal = quadNormal;
                    intersection.valid = true;
                    return intersection;
                }
            }
        }

        // If there is no valid intersection, return default values
        return Intersection();
    }

    Intersection intersectCheckerBoard() const{
        Intersection intersection;
        float t = -A.y / B.y;
        if(t > 0){
            intersection.point = point_at_parameter(t);
            intersection.normal = vec3(0.0f, 1.0f, 0.0f);
            intersection.valid = true;
            return intersection;
        }
        return Intersection();
    }
};

std::vector<Ray> generateRays(const float fovY, const float aspectRatio, const vec3 &eye, const vec3 &lookAt, const vec3 &up, const int numPixels)
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
