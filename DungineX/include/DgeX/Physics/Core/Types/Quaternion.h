#pragma once

#include "DgeX/Physics/Core/Precision.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

/**
 * @brief Quaternion is a 4D vector that can be used to represent orientation.
 */
#pragma warning(disable : 4201)

struct Quaternion
{
    union
    {
        struct
        {
            real_t R;
            real_t I;
            real_t J;
            real_t K;
        };

        real_t Data[4];
    };

    Quaternion() : R(1), I(0), J(0), K(0)
    {
    }

    Quaternion(real_t r, real_t i, real_t j, real_t k) : R(r), I(i), J(j), K(k)
    {
    }

    Quaternion& operator*=(const Quaternion& rhs)
    {
        Quaternion q = *this;
        R = q.R * rhs.R - q.I * rhs.I - q.J * rhs.J - q.K * rhs.K;
        I = q.R * rhs.I + q.I * rhs.R + q.J * rhs.K - q.K * rhs.J;
        J = q.R * rhs.J - q.I * rhs.K + q.J * rhs.R + q.K * rhs.I;
        K = q.R * rhs.K + q.I * rhs.J - q.J * rhs.I + q.K * rhs.R;
        return *this;
    }

    void AddScaledVector(const Vector3& vector, real_t scale)
    {
        Quaternion q(0, vector.X * scale, vector.Y * scale, vector.Z * scale);
        q *= *this;
        R += q.R * static_cast<real_t>(0.5);
        I += q.I * static_cast<real_t>(0.5);
        J += q.J * static_cast<real_t>(0.5);
        K += q.K * static_cast<real_t>(0.5);
    }

    void RotateByVector(const Vector3& vector)
    {
        Quaternion q(0, vector.X, vector.Y, vector.Z);
        *this *= q;
    }

    void Normalize()
    {
        real_t l = R * R + I * I + J * J + K * K;
        // Check for zero length quaternion, and use the no-rotation.
        if (l < REAL_EPSILON)
        {
            R = 1;
            return;
        }

        real_t s = static_cast<real_t>(1) / Math::Sqrt(l);
        R *= s;
        I *= s;
        J *= s;
        K *= s;
    }

    void Set(real_t r, real_t i, real_t j, real_t k)
    {
        R = r;
        I = i;
        J = j;
        K = k;
    }
};

DPHX_END
