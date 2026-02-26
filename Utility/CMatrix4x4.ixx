export module math.matrix;

import <cmath>;
import <span>;
import "CVector3.ixx";

export struct CMatrix4x4
{
    float e00 = 0, e01 = 0, e02 = 0, e03 = 0;
    float e10 = 0, e11 = 0, e12 = 0, e13 = 0;
    float e20 = 0, e21 = 0, e22 = 0, e23 = 0;
    float e30 = 0, e31 = 0, e32 = 0, e33 = 0;

    CMatrix4x4() = default;

    constexpr void MakeIdentity()
    {
        *this = { 1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1 };
    }

    constexpr void SetRow(int row, const CVector3& v)
    {
        std::span<float, 16> elts{ &e00, 16 };
        elts[row * 4 + 0] = v.x;
        elts[row * 4 + 1] = v.y;
        elts[row * 4 + 2] = v.z;
    }


    [[nodiscard]] constexpr CVector3 GetRow(int row) const
    {
        std::span<const float, 16> elts{ &e00, 16 };
        return CVector3{ elts[row * 4 + 0], elts[row * 4 + 1], elts[row * 4 + 2] };
    }

    constexpr CMatrix4x4& operator*=(const CMatrix4x4& m)
    {
        if (this == &m) { *this = m * m; }
        else
        {
            float t0, t1, t2;

            t0 = e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30;
            t1 = e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31;
            t2 = e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32;
            e03 = e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33;
            e00 = t0; e01 = t1; e02 = t2;

            t0 = e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30;
            t1 = e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31;
            t2 = e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32;
            e13 = e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33;
            e10 = t0; e11 = t1; e12 = t2;

            t0 = e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30;
            t1 = e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31;
            t2 = e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32;
            e23 = e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33;
            e20 = t0; e21 = t1; e22 = t2;

            t0 = e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30;
            t1 = e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31;
            t2 = e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32;
            e33 = e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33;
            e30 = t0; e31 = t1; e32 = t2;
        }
        return *this;
    }
};


[[nodiscard]] export constexpr CMatrix4x4 operator*(const CMatrix4x4& m1, const CMatrix4x4& m2)
{
    CMatrix4x4 out;

    out.e00 = m1.e00 * m2.e00 + m1.e01 * m2.e10 + m1.e02 * m2.e20 + m1.e03 * m2.e30;
    out.e01 = m1.e00 * m2.e01 + m1.e01 * m2.e11 + m1.e02 * m2.e21 + m1.e03 * m2.e31;
    out.e02 = m1.e00 * m2.e02 + m1.e01 * m2.e12 + m1.e02 * m2.e22 + m1.e03 * m2.e32;
    out.e03 = m1.e00 * m2.e03 + m1.e01 * m2.e13 + m1.e02 * m2.e23 + m1.e03 * m2.e33;

    out.e10 = m1.e10 * m2.e00 + m1.e11 * m2.e10 + m1.e12 * m2.e20 + m1.e13 * m2.e30;
    out.e11 = m1.e10 * m2.e01 + m1.e11 * m2.e11 + m1.e12 * m2.e21 + m1.e13 * m2.e31;
    out.e12 = m1.e10 * m2.e02 + m1.e11 * m2.e12 + m1.e12 * m2.e22 + m1.e13 * m2.e32;
    out.e13 = m1.e10 * m2.e03 + m1.e11 * m2.e13 + m1.e12 * m2.e23 + m1.e13 * m2.e33;

    out.e20 = m1.e20 * m2.e00 + m1.e21 * m2.e10 + m1.e22 * m2.e20 + m1.e23 * m2.e30;
    out.e21 = m1.e20 * m2.e01 + m1.e21 * m2.e11 + m1.e22 * m2.e21 + m1.e23 * m2.e31;
    out.e22 = m1.e20 * m2.e02 + m1.e21 * m2.e12 + m1.e22 * m2.e22 + m1.e23 * m2.e32;
    out.e23 = m1.e20 * m2.e03 + m1.e21 * m2.e13 + m1.e22 * m2.e23 + m1.e23 * m2.e33;

    out.e30 = m1.e30 * m2.e00 + m1.e31 * m2.e10 + m1.e32 * m2.e20 + m1.e33 * m2.e30;
    out.e31 = m1.e30 * m2.e01 + m1.e31 * m2.e11 + m1.e32 * m2.e21 + m1.e33 * m2.e31;
    out.e32 = m1.e30 * m2.e02 + m1.e31 * m2.e12 + m1.e32 * m2.e22 + m1.e33 * m2.e32;
    out.e33 = m1.e30 * m2.e03 + m1.e31 * m2.e13 + m1.e32 * m2.e23 + m1.e33 * m2.e33;

    return out;
}

[[nodiscard]] export constexpr CMatrix4x4 MatrixIdentity()
{
    return { 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1 };
}

[[nodiscard]] export constexpr CMatrix4x4 MatrixTranslation(const CVector3& t)
{
    return { 1,   0,   0, 0,
               0,   1,   0, 0,
               0,   0,   1, 0,
             t.x, t.y, t.z, 1 };
}

[[nodiscard]] export CMatrix4x4 MatrixRotationX(float x)
{
    const float sX = std::sin(x);
    const float cX = std::cos(x);

    return { 1,   0,   0, 0,
             0,  cX,  sX, 0,
             0, -sX,  cX, 0,
             0,   0,   0, 1 };
}

[[nodiscard]] export CMatrix4x4 MatrixRotationY(float y)
{
    const float sY = std::sin(y);
    const float cY = std::cos(y);

    return { cY, 0, -sY, 0,
               0, 1,   0, 0,
              sY, 0,  cY, 0,
               0, 0,   0, 1 };
}

[[nodiscard]] export CMatrix4x4 MatrixRotationZ(float z)
{
    const float sZ = std::sin(z);
    const float cZ = std::cos(z);

    return { cZ, sZ, 0, 0,
             -sZ, cZ, 0, 0,
               0,  0, 1, 0,
               0,  0, 0, 1 };
}

[[nodiscard]] export constexpr CMatrix4x4 MatrixScaling(const CVector3& s)
{
    return { s.x,   0,   0, 0,
               0, s.y,   0, 0,
               0,   0, s.z, 0,
               0,   0,   0, 1 };
}

[[nodiscard]] export constexpr CMatrix4x4 MatrixScaling(float s)
{
    return { s, 0, 0, 0,
             0, s, 0, 0,
             0, 0, s, 0,
             0, 0, 0, 1 };
}

[[nodiscard]] export constexpr CMatrix4x4 InverseAffine(const CMatrix4x4& m)
{
    CMatrix4x4 out;

    const float det0 = m.e11 * m.e22 - m.e12 * m.e21;
    const float det1 = m.e12 * m.e20 - m.e10 * m.e22;
    const float det2 = m.e10 * m.e21 - m.e11 * m.e20;
    const float invDet = 1.0f / (m.e00 * det0 + m.e01 * det1 + m.e02 * det2);

    out.e00 = invDet * det0;
    out.e10 = invDet * det1;
    out.e20 = invDet * det2;

    out.e01 = invDet * (m.e21 * m.e02 - m.e22 * m.e01);
    out.e11 = invDet * (m.e22 * m.e00 - m.e20 * m.e02);
    out.e21 = invDet * (m.e20 * m.e01 - m.e21 * m.e00);

    out.e02 = invDet * (m.e01 * m.e12 - m.e02 * m.e11);
    out.e12 = invDet * (m.e02 * m.e10 - m.e00 * m.e12);
    out.e22 = invDet * (m.e00 * m.e11 - m.e01 * m.e10);

    out.e30 = -m.e30 * out.e00 - m.e31 * out.e10 - m.e32 * out.e20;
    out.e31 = -m.e30 * out.e01 - m.e31 * out.e11 - m.e32 * out.e21;
    out.e32 = -m.e30 * out.e02 - m.e31 * out.e12 - m.e32 * out.e22;

    out.e03 = 0.0f;
    out.e13 = 0.0f;
    out.e23 = 0.0f;
    out.e33 = 1.0f;

    return out;
}

export constexpr CMatrix4x4 MATRIX_IDENTITY = MatrixIdentity();
