/*
 * Basics types for physics engine.
 */

#pragma once

#include "DgeX/Physics/Core/Macros.h"
#include "DgeX/Utils/Math.h"

#include <glm/glm.hpp>

DPHX_BEGIN

#ifdef DPHX_HIGH_PRECISION
typedef double real_t;
constexpr real_t REAL_EPSILON = DBL_EPSILON;
#else
typedef float real_t;
constexpr real_t REAL_EPSILON = FLT_EPSILON;
#endif
constexpr real_t MAX_REAL = std::numeric_limits<real_t>::max();

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

    void Set(real_t X, real_t Y)
    {
        Set(X, Y, 0);
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

/**
 * @brief Quaternion is a 4D vector that can be used to represent orientation.
 */
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

/**
 * @brief
 * A transform matrix is a 4x3 matrix consisting of a 3x3 rotation matrix
 * and a position. It assumed that the remaining dimension is (0,0,0,1), so
 * it represents a homogeneous transformation.
 */
struct Matrix4
{
    real_t Data[12];

    Matrix4()
    {
        // Set the matrix to be the identity matrix.
        Data[1] = Data[2] = Data[3] = Data[4] = Data[6] = Data[7] = Data[8] = Data[9] = Data[11] = 0;
        Data[0] = Data[5] = Data[10] = 1;
    }

    void SetDiagonal(real_t a, real_t b, real_t c)
    {
        Data[0] = a;
        Data[5] = b;
        Data[10] = c;
        // last is always 1
    }

    Matrix4 operator*(const Matrix4& rhs) const
    {
        Matrix4 result;

        result.Data[0] = (rhs.Data[0] * Data[0]) + (rhs.Data[4] * Data[1]) + (rhs.Data[8] * Data[2]);
        result.Data[4] = (rhs.Data[0] * Data[4]) + (rhs.Data[4] * Data[5]) + (rhs.Data[8] * Data[6]);
        result.Data[8] = (rhs.Data[0] * Data[8]) + (rhs.Data[4] * Data[9]) + (rhs.Data[8] * Data[10]);

        result.Data[1] = (rhs.Data[1] * Data[0]) + (rhs.Data[5] * Data[1]) + (rhs.Data[9] * Data[2]);
        result.Data[5] = (rhs.Data[1] * Data[4]) + (rhs.Data[5] * Data[5]) + (rhs.Data[9] * Data[6]);
        result.Data[9] = (rhs.Data[1] * Data[8]) + (rhs.Data[5] * Data[9]) + (rhs.Data[9] * Data[10]);

        result.Data[2] = (rhs.Data[2] * Data[0]) + (rhs.Data[6] * Data[1]) + (rhs.Data[10] * Data[2]);
        result.Data[6] = (rhs.Data[2] * Data[4]) + (rhs.Data[6] * Data[5]) + (rhs.Data[10] * Data[6]);
        result.Data[10] = (rhs.Data[2] * Data[8]) + (rhs.Data[6] * Data[9]) + (rhs.Data[10] * Data[10]);

        result.Data[3] = (rhs.Data[3] * Data[0]) + (rhs.Data[7] * Data[1]) + (rhs.Data[11] * Data[2]) + Data[3];
        result.Data[7] = (rhs.Data[3] * Data[4]) + (rhs.Data[7] * Data[5]) + (rhs.Data[11] * Data[6]) + Data[7];
        result.Data[11] = (rhs.Data[3] * Data[8]) + (rhs.Data[7] * Data[9]) + (rhs.Data[11] * Data[10]) + Data[11];

        return result;
    }

    Vector3 operator*(const Vector3& rhs) const
    {
        return { rhs.X * Data[0] + rhs.Y * Data[1] + rhs.Z * Data[2] + Data[3],
                 rhs.X * Data[4] + rhs.Y * Data[5] + rhs.Z * Data[6] + Data[7],
                 rhs.X * Data[8] + rhs.Y * Data[9] + rhs.Z * Data[10] + Data[11] };
    }

    Vector3 Transform(const Vector3& rhs) const
    {
        return (*this) * rhs;
    }

    Vector3 TransformInverse(const Vector3& rhs) const
    {
        Vector3 temp = rhs;
        temp.X -= Data[3];
        temp.Y -= Data[7];
        temp.Z -= Data[11];
        return { temp.X * Data[0] + temp.Y * Data[4] + temp.Z * Data[8],
                 temp.X * Data[1] + temp.Y * Data[5] + temp.Z * Data[9],
                 temp.X * Data[2] + temp.Y * Data[6] + temp.Z * Data[10] };
    }

    Vector3 TransformDirection(const Vector3& rhs) const
    {
        return { rhs.X * Data[0] + rhs.Y * Data[1] + rhs.Z * Data[2],
                 rhs.X * Data[4] + rhs.Y * Data[5] + rhs.Z * Data[6],
                 rhs.X * Data[8] + rhs.Y * Data[9] + rhs.Z * Data[10] };
    }

    Vector3 TransformInverseDirection(const Vector3& rhs) const
    {
        return { rhs.X * Data[0] + rhs.Y * Data[4] + rhs.Z * Data[8],
                 rhs.X * Data[1] + rhs.Y * Data[5] + rhs.Z * Data[9],
                 rhs.X * Data[2] + rhs.Y * Data[6] + rhs.Z * Data[10] };
    }

    real_t Determinant() const
    {
        return -Data[8] * Data[5] * Data[2] + Data[4] * Data[9] * Data[2] + Data[8] * Data[1] * Data[6] -
               Data[0] * Data[9] * Data[6] - Data[4] * Data[1] * Data[10] + Data[0] * Data[5] * Data[10];
    }

    Matrix4 Inverse() const
    {
        Matrix4 result;
        result.SetInverse(*this);
        return result;
    }

    Vector3 Axis(int column) const
    {
        return { Data[column], Data[column + 4], Data[column + 8] };
    }

    void SetOrientationAndPosition(const Quaternion& q, const Vector3& p)
    {
        Data[0] = 1 - (2 * q.J * q.J + 2 * q.K * q.K);
        Data[1] = 2 * q.I * q.J + 2 * q.K * q.R;
        Data[2] = 2 * q.I * q.K - 2 * q.J * q.R;
        Data[3] = p.X;

        Data[4] = 2 * q.I * q.J - 2 * q.K * q.R;
        Data[5] = 1 - (2 * q.I * q.I + 2 * q.K * q.K);
        Data[6] = 2 * q.J * q.K + 2 * q.I * q.R;
        Data[7] = p.Y;

        Data[8] = 2 * q.I * q.K + 2 * q.J * q.R;
        Data[9] = 2 * q.J * q.K - 2 * q.I * q.R;
        Data[10] = 1 - (2 * q.I * q.I + 2 * q.J * q.J);
        Data[11] = p.Z;
    }

    void SetInverse(const Matrix4& m);

    void Invert()
    {
        SetInverse(*this);
    }
};

/**
 * @brief
 * The concept of the inertia tensor is employed to depict the mass distribution
 * and rotational inertia of a solid or rigid object.
 */
struct Matrix3
{
    real_t Data[9];

    Matrix3()
    {
        Data[0] = Data[1] = Data[2] = Data[3] = Data[4] = Data[5] = Data[6] = Data[7] = Data[8] = 0;
    }

    Matrix3(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        SetComponents(v1, v2, v3);
    }

    Matrix3(const real_t xx, const real_t xy, const real_t xz, const real_t yx, const real_t yy, const real_t yz,
            const real_t zx, const real_t zy, const real_t zz)
    {
        Data[0] = xx;
        Data[1] = xy;
        Data[2] = xz;
        Data[3] = yx;
        Data[4] = yy;
        Data[5] = yz;
        Data[6] = zx;
        Data[7] = zy;
        Data[8] = zz;
    }

    Vector3 operator*(const Vector3& rhs) const
    {
        return { rhs.X * Data[0] + rhs.Y * Data[1] + rhs.Z * Data[2],
                 rhs.X * Data[3] + rhs.Y * Data[4] + rhs.Z * Data[5],
                 rhs.X * Data[6] + rhs.Y * Data[7] + rhs.Z * Data[8] };
    }

    Matrix3 operator*(const Matrix3& rhs) const
    {
        return { Data[0] * rhs.Data[0] + Data[1] * rhs.Data[3] + Data[2] * rhs.Data[6],
                 Data[0] * rhs.Data[1] + Data[1] * rhs.Data[4] + Data[2] * rhs.Data[7],
                 Data[0] * rhs.Data[2] + Data[1] * rhs.Data[5] + Data[2] * rhs.Data[8],

                 Data[3] * rhs.Data[0] + Data[4] * rhs.Data[3] + Data[5] * rhs.Data[6],
                 Data[3] * rhs.Data[1] + Data[4] * rhs.Data[4] + Data[5] * rhs.Data[7],
                 Data[3] * rhs.Data[2] + Data[4] * rhs.Data[5] + Data[5] * rhs.Data[8],

                 Data[6] * rhs.Data[0] + Data[7] * rhs.Data[3] + Data[8] * rhs.Data[6],
                 Data[6] * rhs.Data[1] + Data[7] * rhs.Data[4] + Data[8] * rhs.Data[7],
                 Data[6] * rhs.Data[2] + Data[7] * rhs.Data[5] + Data[8] * rhs.Data[8] };
    }

    Matrix3& operator*=(const Matrix3& rhs)
    {
        real_t t1;
        real_t t2;
        real_t t3;

        t1 = Data[0] * rhs.Data[0] + Data[1] * rhs.Data[3] + Data[2] * rhs.Data[6];
        t2 = Data[0] * rhs.Data[1] + Data[1] * rhs.Data[4] + Data[2] * rhs.Data[7];
        t3 = Data[0] * rhs.Data[2] + Data[1] * rhs.Data[5] + Data[2] * rhs.Data[8];
        Data[0] = t1;
        Data[1] = t2;
        Data[2] = t3;

        t1 = Data[3] * rhs.Data[0] + Data[4] * rhs.Data[3] + Data[5] * rhs.Data[6];
        t2 = Data[3] * rhs.Data[1] + Data[4] * rhs.Data[4] + Data[5] * rhs.Data[7];
        t3 = Data[3] * rhs.Data[2] + Data[4] * rhs.Data[5] + Data[5] * rhs.Data[8];
        Data[3] = t1;
        Data[4] = t2;
        Data[5] = t3;

        t1 = Data[6] * rhs.Data[0] + Data[7] * rhs.Data[3] + Data[8] * rhs.Data[6];
        t2 = Data[6] * rhs.Data[1] + Data[7] * rhs.Data[4] + Data[8] * rhs.Data[7];
        t3 = Data[6] * rhs.Data[2] + Data[7] * rhs.Data[5] + Data[8] * rhs.Data[8];
        Data[6] = t1;
        Data[7] = t2;
        Data[8] = t3;

        return *this;
    }

    Matrix3 operator*=(const real_t rhs)
    {
        Data[0] *= rhs;
        Data[1] *= rhs;
        Data[2] *= rhs;
        Data[3] *= rhs;
        Data[4] *= rhs;
        Data[5] *= rhs;
        Data[6] *= rhs;
        Data[7] *= rhs;
        Data[8] *= rhs;

        return *this;
    }

    Matrix3 operator+=(const Matrix3& rhs)
    {
        Data[0] += rhs.Data[0];
        Data[1] += rhs.Data[1];
        Data[2] += rhs.Data[2];
        Data[3] += rhs.Data[3];
        Data[4] += rhs.Data[4];
        Data[5] += rhs.Data[5];
        Data[6] += rhs.Data[6];
        Data[7] += rhs.Data[7];
        Data[8] += rhs.Data[8];

        return *this;
    }

    Vector3 Transform(const Vector3& vector) const
    {
        return (*this) * vector;
    }

    Vector3 TransformTranspose(const Vector3& vector) const
    {
        return { vector.X * Data[0] + vector.Y * Data[3] + vector.Z * Data[6],
                 vector.X * Data[1] + vector.Y * Data[4] + vector.Z * Data[7],
                 vector.X * Data[2] + vector.Y * Data[5] + vector.Z * Data[8] };
    }

    void SetDiagonal(real_t a, real_t b, real_t c)
    {
        SetInertiaTensor(a, b, c);
    }

    void SetInertiaTensor(real_t ix, real_t iy, real_t iz, real_t ixy = 0, real_t ixz = 0, real_t iyz = 0)
    {
        Data[0] = ix;
        Data[1] = Data[3] = -ixy;
        Data[2] = Data[6] = -ixz;
        Data[4] = iy;
        Data[5] = Data[7] = -iyz;
        Data[8] = iz;
    }

    void SetBlockInertiaTensor(const Vector3& halfSizes, real_t mass)
    {
        Vector3 squares = halfSizes.ComponentProduct(halfSizes);
        SetInertiaTensor(0.3f * mass * (squares.Y + squares.Z), 0.3f * mass * (squares.X + squares.Z),
                         0.3f * mass * (squares.X + squares.Y));
    }

    /**
     * @brief
     * Sets the matrix to be a skew symmetric matrix based on
     * the given vector. The skew symmetric matrix is the equivalent
     * of the vector product. So if a and b are vectors. a x b = A_s b
     * where A_s is the skew symmetric form of a.
     */
    void SetSkewSymmetric(const Vector3& vector)
    {
        Data[0] = Data[4] = Data[8] = 0;
        Data[1] = -vector.Z;
        Data[2] = vector.Y;
        Data[3] = vector.Z;
        Data[5] = -vector.X;
        Data[6] = -vector.Y;
        Data[7] = vector.X;
    }

    void SetComponents(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        Data[0] = v1.X;
        Data[1] = v2.X;
        Data[2] = v3.X;
        Data[3] = v1.Y;
        Data[4] = v2.Y;
        Data[5] = v3.Y;
        Data[6] = v1.Z;
        Data[7] = v2.Z;
        Data[8] = v3.Z;
    }

    Vector3 Row(int row) const
    {
        return { Data[row * 3], Data[row * 3 + 1], Data[row * 3 + 2] };
    }

    Vector3 Column(int col) const
    {
        return { Data[col], Data[col + 3], Data[col + 6] };
    }

    real_t Determinant() const
    {
        return Data[0] * Data[4] * Data[8] + Data[1] * Data[5] * Data[6] + Data[2] * Data[3] * Data[7] -
               Data[0] * Data[5] * Data[7] - Data[1] * Data[3] * Data[8] - Data[2] * Data[4] * Data[6];
    }

    void SetInverse(const Matrix3& m);

    Matrix3 Inverse() const
    {
        Matrix3 result;
        result.SetInverse(*this);
        return result;
    }

    void Invert()
    {
        SetInverse(*this);
    }

    void SetTranspose(const Matrix3& m)
    {
        Data[0] = m.Data[0];
        Data[1] = m.Data[3];
        Data[2] = m.Data[6];
        Data[3] = m.Data[1];
        Data[4] = m.Data[4];
        Data[5] = m.Data[7];
        Data[6] = m.Data[2];
        Data[7] = m.Data[5];
        Data[8] = m.Data[8];
    }

    Matrix3 Transpose() const
    {
        Matrix3 result;
        result.SetTranspose(*this);
        return result;
    }

    void SetOrientation(const Quaternion& q)
    {
        Data[0] = 1 - (2 * q.J * q.J + 2 * q.K * q.K);
        Data[1] = 2 * q.I * q.J + 2 * q.K * q.R;
        Data[2] = 2 * q.I * q.K - 2 * q.J * q.R;
        Data[3] = 2 * q.I * q.J - 2 * q.K * q.R;
        Data[4] = 1 - (2 * q.I * q.I + 2 * q.K * q.K);
        Data[5] = 2 * q.J * q.K + 2 * q.I * q.R;
        Data[6] = 2 * q.I * q.K + 2 * q.J * q.R;
        Data[7] = 2 * q.J * q.K - 2 * q.I * q.R;
        Data[8] = 1 - (2 * q.I * q.I + 2 * q.J * q.J);
    }

    static Matrix3 LinearInterpolate(const Matrix3& start, const Matrix3& end, real_t step);
};

DPHX_END
