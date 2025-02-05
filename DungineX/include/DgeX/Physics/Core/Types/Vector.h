/*
 * Basics types for physics engine.
 */

#pragma once

#include "DgeX/Physics/Core/Precision.h"

#include "DgeX/Utils/Math.h"

#include <glm/glm.hpp>

DPHX_BEGIN

struct Vector3
{
    // Warning: These may not be initialized when constructing global variables!
    static const Vector3 Zero;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;

    real_t X;
    real_t Y;
    real_t Z;

    /** The default constructor creates a zero vector. */
    Vector3() : X(0), Y(0), Z(0)
    {
    }

    /**
     * The explicit constructor creates a vector with the given
     * components.
     */
    Vector3(const real_t x, const real_t y, const real_t z = 0) : X(x), Y(y), Z(z)
    {
    }

    /*
     * =========================================================================
     * --------------------------- Vector Operations ---------------------------
     *  =========================================================================
     */

    real_t operator[](const unsigned i) const
    {
        return (i == 0) ? X : (i == 1) ? Y : Z;
    }

    real_t& operator[](const unsigned i)
    {
        return (i == 0) ? X : (i == 1) ? Y : Z;
    }

    void operator+=(const Vector3& rhs)
    {
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
    }

    Vector3 operator+(const Vector3& rhs) const
    {
        return { X + rhs.X, Y + rhs.Y, Z + rhs.Z };
    }

    void operator-=(const Vector3& rhs)
    {
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
    }

    Vector3 operator-(const Vector3& rhs) const
    {
        return { X - rhs.X, Y - rhs.Y, Z - rhs.Z };
    }

    Vector3 operator-() const
    {
        return { -X, -Y, -Z };
    }

    void operator*=(const real_t scale)
    {
        X *= scale;
        Y *= scale;
        Z *= scale;
    }

    /** Returns a copy of this vector scaled the given value. */
    Vector3 operator*(const real_t scale) const
    {
        return { X * scale, Y * scale, Z * scale };
    }

    /**
     * @brief
     * Component product is the same as the dot product, but return as
     * vector instead of scalar.
     */
    Vector3 ComponentProduct(const Vector3& rhs) const
    {
        return { X * rhs.X, Y * rhs.Y, Z * rhs.Z };
    }

    void ComponentProductUpdate(const Vector3& rhs)
    {
        X *= rhs.X;
        Y *= rhs.Y;
        Z *= rhs.Z;
    }

    /**
     * @brief a.k.a. the cross product.
     */
    Vector3 VectorProduct(const Vector3& rhs) const
    {
        return { Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X };
    }

    // Use % for cross product
    void operator%=(const Vector3& rhs)
    {
        *this = VectorProduct(rhs);
    }

    Vector3 operator%(const Vector3& rhs) const
    {
        return VectorProduct(rhs);
    }

    real_t ScalarProduct(const Vector3& rhs) const
    {
        return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
    }

    // Use * for scalar product
    real_t operator*(const Vector3& rhs) const
    {
        return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
    }

    // A shortcut for add and scale
    void AddScaledVector(const Vector3& vec, real_t scale)
    {
        X += vec.X * scale;
        Y += vec.Y * scale;
        Z += vec.Z * scale;
    }

    void AddVector(const Vector3& vec)
    {
        X += vec.X;
        Y += vec.Y;
        Z += vec.Z;
    }

    void AddVector(real_t x, real_t y, real_t z)
    {
        X += x;
        Y += y;
        Z += z;
    }

    /*
     * =========================================================================
     * --------------------------- Vector Comparison ---------------------------
     * =========================================================================
     */

    // We're not doing scientific calculations, so == is fine.
    bool operator==(const Vector3& other) const
    {
        return (X == other.X) && (Y == other.Y) && (Z == other.Z);
    }

    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @warning This does not behave like a single-value comparison:
     * !(a < b) does not imply (b >= a).
     */
    bool operator<(const Vector3& other) const
    {
        return X < other.X && Y < other.Y && Z < other.Z;
    }

    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @warning This does not behave like a single-value comparison:
     * !(a < b) does not imply (b >= a).
     */
    bool operator>(const Vector3& other) const
    {
        return X > other.X && Y > other.Y && Z > other.Z;
    }

    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @warning This does not behave like a single-value comparison:
     * !(a <= b) does not imply (b > a).
     */
    bool operator<=(const Vector3& other) const
    {
        return X <= other.X && Y <= other.Y && Z <= other.Z;
    }

    /**
     * Checks if this vector is component-by-component less than
     * the other.
     *
     * @warning This does not behave like a single-value comparison:
     * !(a <= b) does not imply (b > a).
     */
    bool operator>=(const Vector3& other) const
    {
        return X >= other.X && Y >= other.Y && Z >= other.Z;
    }

    /*
     * =========================================================================
     * --------------------------- Vector Properties ---------------------------
     * =========================================================================
     */

    real_t Magnitude() const
    {
        return Math::Sqrt(X * X + Y * Y + Z * Z);
    }

    // A faster version of magnitude without the square root
    real_t MagnitudeSquare() const
    {
        return X * X + Y * Y + Z * Z;
    }

    // Normalise the vector
    void Normalize()
    {
        real_t l = Magnitude();
        if (l > 0)
        {
            (*this) *= static_cast<real_t>(1) / l;
        }
    }

    Vector3 Normalized() const
    {
        Vector3 v = *this;
        v.Normalize();
        return v;
    }

    void Invert()
    {
        X = -X;
        Y = -Y;
        X = -Z;
    }

    Vector3 Inverse() const
    {
        return { -X, -Y, -Z };
    }

    void Clear()
    {
        X = Y = Z = static_cast<real_t>(0);
    }

    /**
     * @brief Rotate the vector counter-clockwise around the Z axis.
     */
    void RotateZ(real_t radian)
    {
        real_t c = Math::Cos(radian);
        real_t s = Math::Sin(radian);
        real_t x = X * c - Y * s;
        real_t y = X * s + Y * c;
        X = x;
        Y = y;
    }

    Vector3 RotatedZ(real_t radian) const
    {
        Vector3 v = *this;
        v.RotateZ(radian);
        return v;
    }

    void Set(real_t x, real_t y, real_t z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    void Set(const Vector3& v)
    {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
    }

    void Set(real_t x, real_t y)
    {
        Set(x, y, 0);
    }

    bool IsZero() const
    {
        return (X == 0) && (Y == 0) && (Z == 0);
    }

    glm::vec3 ToGlmVec3() const
    {
        return glm::vec3(X, Y, Z);
    }

    glm::vec3 ToGlmVec3(float z) const
    {
        return glm::vec3(X, Y, z);
    }
};

DPHX_END
