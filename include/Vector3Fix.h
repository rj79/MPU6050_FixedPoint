#ifndef VECTOR3_FIX_H
#define VECTOR3_FIX_H

#include "fixedpoint.h"

struct Vector3Fix
{
public:
    fixed X;
    fixed Y;
    fixed Z;

    Vector3Fix() :
        X(0),
        Y(0),
        Z(0)
    {
        // Empty
    };

    Vector3Fix(fixed x, fixed y, fixed z) :
        X(x),
        Y(y),
        Z(z)
    {
        // Empty
    };

    Vector3Fix(const Vector3Fix& other) :
        X(other.X),
        Y(other.Y),
        Z(other.Z)
    {
        // Empty
    };

    Vector3Fix& zero() {
        X = 0;
        Y = 0;
        Z = 0;
        return *this;
    }

    Vector3Fix& operator=(const Vector3Fix& other) 
    {
        if (&other == this) {
            return *this;
        }
        X = other.X;
        Y = other.Y;
        Z = other.Z;
        return *this;
    };

    inline friend bool operator==(const Vector3Fix& lhs, const Vector3Fix& rhs) 
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
    }

    inline friend Vector3Fix operator+(const Vector3Fix& lhs, const Vector3Fix& rhs)
    {
        return Vector3Fix(fix_add(lhs.X, rhs.X), fix_add(lhs.Y, rhs.Y), fix_add(lhs.Z, rhs.Z));
    }

    inline friend Vector3Fix operator-(const Vector3Fix& lhs, const Vector3Fix& rhs)
    {
        return Vector3Fix(fix_sub(lhs.X, rhs.X), fix_sub(lhs.Y, rhs.Y), fix_sub(lhs.Z, rhs.Z));
    }

    inline friend Vector3Fix operator*(const Vector3Fix& lhs, fixed rhs) 
    {
        return Vector3Fix(fix_mul(lhs.X, rhs), fix_mul(lhs.Y, rhs), fix_mul(lhs.Z, rhs));
    }

    inline friend Vector3Fix operator/(const Vector3Fix& lhs, fixed rhs) 
    {
        return Vector3Fix(fix_div(lhs.X, rhs), fix_div(lhs.Y, rhs), fix_div(lhs.Z, rhs));
    }

    Vector3Fix& operator+=(const Vector3Fix& rhs) 
    {
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
        return *this;
    }

    Vector3Fix& operator-=(const Vector3Fix& rhs) 
    {
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
        return *this;
    }

    Vector3Fix& operator*=(float rhs)
    {
        X *= rhs;
        Y *= rhs;
        Z *= rhs;
        return *this;
    }

    Vector3Fix& operator/=(float rhs)
    {
        X /= rhs;
        Y /= rhs;
        Z /= rhs;
        return *this;
    }
};

#endif