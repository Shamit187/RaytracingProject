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
    vec3 origin;
    vec3 direction;
    float time;

    vec3 point_at_parameter(float t) const { return origin + direction * t; }

    Ray() {}
    
    Ray(const vec3 &a, const vec3 &b, float ti = 0.0)
    {
        origin = a;
        direction = b;
        time = ti;
    }

    Intersection intersect(const Sphere &sphere) const
    {
        vec3 oc = origin - sphere.center;
        float a = direction.dot(direction);
        float b = oc.dot(direction);
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
        float denominator = triangleNormal.dot(direction);

        // Check if the ray is nearly parallel to the triangle
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the triangle
            vec3 originToTriangle = triangle.point1 - origin;

            // Calculate the distance from the ray's origin to the intersection point
            float t = originToTriangle.dot(triangleNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                vec3 intersectionPoint = origin + direction * t;

                // Check if the intersection point is inside the triangle
                vec3 edge1 = triangle.point2 - triangle.point1;
                vec3 edge2 = triangle.point3 - triangle.point1;
                vec3 h = direction.cross(edge2);
                float a = edge1.dot(h);

                if (a > -1e-6 && a < 1e-6)
                    return Intersection(); // Ray is parallel to the triangle

                float f = 1.0f / a;
                vec3 s = intersectionPoint - triangle.point1;
                float u = f * s.dot(h);

                if (u < 0.0f || u > 1.0f)
                    return Intersection();

                vec3 q = s.cross(edge1);
                float v = f * direction.dot(q);

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
        float denominator = quadNormal.dot(direction);

        // Check if the ray is nearly parallel to the plane
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the plane
            vec3 originToQuad = quad.bottomLeftPoint - origin;

            // Calculate the distance from the ray's origin to the intersection point
            float t = originToQuad.dot(quadNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                intersection.point = origin + direction * t;

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
        // checker board is in xz plane
        // from origin check if the ray intersects the plane in front of it
        if(direction.y < 0.0f){
            float t = -origin.y / direction.y;
            if(t > 0.0f){
                intersection.point = origin + direction * t;
                intersection.normal = vec3(0.0f, 1.0f, 0.0f);
                intersection.valid = true;
                return intersection;
            }
        }
        return Intersection();
    }
};

std::vector<Ray> generateRays(const float fovY, const float aspectRatio, const vec3 &eye, const vec3 &lookAt, const vec3 &up, const int numPixelsX, const int numPixelsY, const float nearPlane)
{
    std::vector<Ray> rays;

    // Calculate the half height and width of the near plane
    float halfHeight = std::tan(fovY * 0.5f) * nearPlane;
    float halfWidth = halfHeight * aspectRatio;

    // Calculate the basis vectors for the camera coordinate system
    vec3 w = (eye - lookAt).normalize();
    vec3 u = up.cross(w).normalize();
    vec3 v = w.cross(u);

    // Generate rays for each pixel
    for (int y = 0; y < numPixelsY; ++y) {
        for (int x = 0; x < numPixelsX; ++x) {
            // Calculate the normalized device coordinates
            float ndcX = (2.0f * (x + 0.5f) / numPixelsX) - 1.0f;
            float ndcY = 1.0f - (2.0f * (y + 0.5f) / numPixelsY);

            // Calculate the ray direction in camera space
            vec3 rayDirection = (u * (ndcX * halfWidth) + v * (ndcY * halfHeight) - w * nearPlane).normalize();

            // Transform the ray direction to world space
            vec3 worldRayDirection = u * rayDirection.x + v * rayDirection.y + w * rayDirection.z;

            // Create a ray and add it to the vector
            Ray ray;
            ray.origin = eye + (worldRayDirection * nearPlane);
            ray.direction = worldRayDirection;
            rays.push_back(ray);
        }
    }

    return rays;
}

#endif
