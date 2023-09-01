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
    double time;

    vec3 point_at_parameter(double t) const { return origin + direction * t; }

    Ray() {}
    
    Ray(const vec3 &a, const vec3 &b, double ti = 0.0)
    {
        origin = a;
        direction = b;
        time = ti;
    }

    Intersection intersect(const Sphere &sphere) const
    {
        vec3 oc = origin - sphere.center;
        double a = direction.dot(direction);
        double b = oc.dot(direction);
        double c = oc.dot(oc) - sphere.radius * sphere.radius;
        double discriminant = b * b - a * c;
        if (discriminant > 0 && a != 0)
        {
            double temp = (-b - sqrt(discriminant)) / a;

            if (temp < 0.0f)
            {
                temp = (-b + sqrt(discriminant)) / a;
            }
            if (temp > 0.0f)
            {
                Intersection result;
                result.point = point_at_parameter(temp);
                result.normal = (result.point - sphere.center).normalize();
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
        double denominator = triangleNormal.dot(direction);

        // Check if the ray is nearly parallel to the triangle
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the triangle
            vec3 originToTriangle = triangle.point1 - origin;

            // Calculate the distance from the ray's origin to the intersection point
            double t = originToTriangle.dot(triangleNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                vec3 intersectionPoint = origin + direction * t;

                // Check if the intersection point is inside the triangle
                vec3 edge1 = triangle.point2 - triangle.point1;
                vec3 edge2 = triangle.point3 - triangle.point1;
                vec3 h = direction.cross(edge2);
                double a = edge1.dot(h);

                if (a > -1e-6 && a < 1e-6)
                    return Intersection(); // Ray is parallel to the triangle

                double f = 1.0f / a;
                vec3 s = intersectionPoint - triangle.point1;
                double u = f * s.dot(h);

                if (u < 0.0f || u > 1.0f)
                    return Intersection();

                vec3 q = s.cross(edge1);
                double v = f * direction.dot(q);

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
        double denominator = quadNormal.dot(direction);

        // Check if the ray is nearly parallel to the plane
        if (std::abs(denominator) > 1e-6)
        {
            // Calculate the vector from the ray's origin to a point on the plane
            vec3 originToQuad = quad.bottomLeftPoint - origin;

            // Calculate the distance from the ray's origin to the intersection point
            double t = originToQuad.dot(quadNormal) / denominator;

            // Check if the intersection point is in front of the ray
            if (t >= 0.0f)
            {
                // Calculate the intersection point
                intersection.point = origin + direction * t;

                // Check if the intersection point is within the quad's boundaries
                vec3 quadToIntersection = intersection.point - quad.bottomLeftPoint;
                double dotBL = quadToIntersection.dot(quad.bottomRightPoint - quad.bottomLeftPoint);
                double dotTL = quadToIntersection.dot(quad.topLeftPoint - quad.bottomLeftPoint);

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
            double t = -origin.y / direction.y;
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

std::vector<Ray> generateRays(const double fovY, const double aspectRatio, const vec3 &eye, const vec3 &lookAt, const vec3 &up, const int numPixelsX, const int numPixelsY, const double nearPlane)
{
    std::vector<Ray> rays;

    for(int row = 0; row < numPixelsX; row++){
        for(int col = 0; col < numPixelsY; col++){
            double upAmount = (double)col / (double)numPixelsY;
            double rightAmount = (double)row / (double)numPixelsX;

            vec3 forward = (lookAt - eye).normalize();
            vec3 right = forward.cross(up).normalize();

            vec3 screenPosition = eye + forward + right * (2.0f * rightAmount - 1.0f) + up * (2.0f * upAmount - 1.0f);
            Ray ray;
            ray.origin = screenPosition;
            ray.direction = (screenPosition - eye).normalize();
            // std::cout << "ray origin: "  << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
            // std::cout << "ray direction: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;

            rays.push_back(ray);
        }
    }

    return rays;
}

#endif
