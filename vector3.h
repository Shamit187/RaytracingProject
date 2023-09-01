#ifndef VECTOR3_H
#define VECTOR3_H

#include <GL/glut.h>
#include <math.h>

struct vec3
{
    GLdouble x;
    GLdouble y;
    GLdouble z;

    vec3(GLdouble x, GLdouble y, GLdouble z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    vec3 operator+(const vec3 &v) const
    {
        return vec3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    vec3 operator-(const vec3 &v) const
    {
        return vec3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    vec3 operator*(const GLdouble &s) const
    {
        return vec3(this->x * s, this->y * s, this->z * s);
    }

    vec3 operator/(const GLdouble &s) const
    {
        if (s == 0)
            return vec3(0.0f, 0.0f, 0.0f);
        return vec3(this->x / s, this->y / s, this->z / s);
    }

    GLdouble dot(const vec3 &v) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    vec3 cross(const vec3 &v) const
    {
        return vec3(this->y * v.z - this->z * v.y,
                    this->z * v.x - this->x * v.z,
                    this->x * v.y - this->y * v.x);
    }

    GLdouble length() const
    {
        if (this->x == 0 && this->y == 0 && this->z == 0)
            return 0;
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    vec3 normalize() const
    {
        GLdouble len = this->length();
        if (len == 0)
            return vec3(0.0f, 0.0f, 0.0f);
        return vec3(this->x / len, this->y / len, this->z / len);
    }

    vec3 operator-() const
    {
        return vec3(-this->x, -this->y, -this->z);
    }
};

// Function to rotate a vector using the Rodrigues' rotation formula
vec3 rodrigues_rotation(const vec3 &v, const vec3 &k, GLdouble theta)
{
    GLdouble cos_theta = cos(theta);
    GLdouble sin_theta = sin(theta);

    vec3 k_cross_v = k.cross(v);
    vec3 rotated_v = v * cos_theta + k_cross_v * sin_theta + k * k.dot(v) * (1 - cos_theta);

    return rotated_v;
}


#endif