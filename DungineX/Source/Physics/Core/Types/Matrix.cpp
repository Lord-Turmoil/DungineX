#include "DgeX/Physics/Core/Types/Matrix.h"

#include "DgeX/Core/Math.h"

DPHX_BEGIN

const Vector3 Vector3::Zero = Vector3(0, 0, 0);
const Vector3 Vector3::UnitX = Vector3(1, 0, 0);
const Vector3 Vector3::UnitY = Vector3(0, 1, 0);
const Vector3 Vector3::UnitZ = Vector3(0, 0, 1);

void Matrix4::SetInverse(const Matrix4& m)
{
    // Make sure the determinant is non-zero.
    real_t det = Determinant();

    if (Math::Abs(det) < REAL_EPSILON)
    {
        DGEX_ASSERT(false, "Matrix4::SetInverse() - Determinant is zero!");
        return;
    }
    det = static_cast<real_t>(1.0) / det;

    Data[0] = (-m.Data[9] * m.Data[6] + m.Data[5] * m.Data[10]) * det;
    Data[4] = (m.Data[8] * m.Data[6] - m.Data[4] * m.Data[10]) * det;
    Data[8] = (-m.Data[8] * m.Data[5] + m.Data[4] * m.Data[9]) * det;

    Data[1] = (m.Data[9] * m.Data[2] - m.Data[1] * m.Data[10]) * det;
    Data[5] = (-m.Data[8] * m.Data[2] + m.Data[0] * m.Data[10]) * det;
    Data[9] = (m.Data[8] * m.Data[1] - m.Data[0] * m.Data[9]) * det;

    Data[2] = (-m.Data[5] * m.Data[2] + m.Data[1] * m.Data[6]) * det;
    Data[6] = (+m.Data[4] * m.Data[2] - m.Data[0] * m.Data[6]) * det;
    Data[10] = (-m.Data[4] * m.Data[1] + m.Data[0] * m.Data[5]) * det;

    Data[3] =
        (m.Data[9] * m.Data[6] * m.Data[3] - m.Data[5] * m.Data[10] * m.Data[3] - m.Data[9] * m.Data[2] * m.Data[7] +
         m.Data[1] * m.Data[10] * m.Data[7] + m.Data[5] * m.Data[2] * m.Data[11] - m.Data[1] * m.Data[6] * m.Data[11]) *
        det;
    Data[7] =
        (-m.Data[8] * m.Data[6] * m.Data[3] + m.Data[4] * m.Data[10] * m.Data[3] + m.Data[8] * m.Data[2] * m.Data[7] -
         m.Data[0] * m.Data[10] * m.Data[7] - m.Data[4] * m.Data[2] * m.Data[11] + m.Data[0] * m.Data[6] * m.Data[11]) *
        det;
    Data[11] =
        (m.Data[8] * m.Data[5] * m.Data[3] - m.Data[4] * m.Data[9] * m.Data[3] - m.Data[8] * m.Data[1] * m.Data[7] +
         m.Data[0] * m.Data[9] * m.Data[7] + m.Data[4] * m.Data[1] * m.Data[11] - m.Data[0] * m.Data[5] * m.Data[11]) *
        det;
}

void Matrix3::SetInverse(const Matrix3& m)
{
    real_t t4 = m.Data[0] * m.Data[4];
    real_t t6 = m.Data[0] * m.Data[5];
    real_t t8 = m.Data[1] * m.Data[3];
    real_t t10 = m.Data[2] * m.Data[3];
    real_t t12 = m.Data[1] * m.Data[6];
    real_t t14 = m.Data[2] * m.Data[6];

    // Calculate the determinant
    real_t det =
        (t4 * m.Data[8] - t6 * m.Data[7] - t8 * m.Data[8] + t10 * m.Data[7] + t12 * m.Data[5] - t14 * m.Data[4]);

    // Make sure the determinant is non-zero.
    if (Math::Abs(det) < REAL_EPSILON)
    {
        DGEX_ASSERT(false, "Matrix3::SetInverse() - Determinant is zero!");
        return;
    }
    det = static_cast<real_t>(1.f) / det;

    Data[0] = (m.Data[4] * m.Data[8] - m.Data[5] * m.Data[7]) * det;
    Data[1] = -(m.Data[1] * m.Data[8] - m.Data[2] * m.Data[7]) * det;
    Data[2] = (m.Data[1] * m.Data[5] - m.Data[2] * m.Data[4]) * det;
    Data[3] = -(m.Data[3] * m.Data[8] - m.Data[5] * m.Data[6]) * det;
    Data[4] = (m.Data[0] * m.Data[8] - t14) * det;
    Data[5] = -(t6 - t10) * det;
    Data[6] = (m.Data[3] * m.Data[7] - m.Data[4] * m.Data[6]) * det;
    Data[7] = -(m.Data[0] * m.Data[7] - t12) * det;
    Data[8] = (t4 - t8) * det;
}

Matrix3 Matrix3::LinearInterpolate(const Matrix3& start, const Matrix3& end, real_t step)
{
    Matrix3 result;
    for (int i = 0; i < 9; i++)
    {
        result.Data[i] = start.Data[i] * (static_cast<real_t>(1.f) - step) + end.Data[i] * step;
    }
    return result;
}

DPHX_END
