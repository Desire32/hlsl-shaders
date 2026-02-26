export module math.vector3;

import <cmath>;
import <span>;

export struct CVector3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    CVector3() = default;
    constexpr CVector3(float xIn, float yIn, float zIn)
        : x(xIn), y(yIn), z(zIn) {}

    constexpr void Set(std::span<const float, 3> elts)
    {
        x = elts[0];
        y = elts[1];
        z = elts[2];
    }

    [[nodiscard]] constexpr float Dot(const CVector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] constexpr CVector3 operator+(const CVector3& v) const
    {
        return { x + v.x, y + v.y, z + v.z };
    }

    [[nodiscard]] constexpr CVector3 operator-(const CVector3& v) const
    {
        return { x - v.x, y - v.y, z - v.z };
    }

    [[nodiscard]] constexpr CVector3 operator*(float s) const
    {
        return { x * s, y * s, z * s };
    }

    [[nodiscard]] constexpr CVector3 operator/(float s) const
    {
        return { x / s, y / s, z / s };
    }

    constexpr CVector3& operator+=(const CVector3& v)
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    constexpr CVector3& operator-=(const CVector3& v)
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    constexpr CVector3& operator*=(float s)
    {
        x *= s; y *= s; z *= s;
        return *this;
    }
};


export constexpr float kfEpsilon = 0.5e-6f;

[[nodiscard]] export constexpr bool IsZero(float x)
{
    return std::abs(x) < kfEpsilon;
}

// 1 / sqrt
[[nodiscard]] export inline float InvSqrt(float x)
{
    return 1.0f / std::sqrt(x);
}

// scalar product
[[nodiscard]] export constexpr float Dot(const CVector3& v1, const CVector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// cross product
[[nodiscard]] export constexpr CVector3 Cross(const CVector3& v1, const CVector3& v2)
{
    return { v1.y * v2.z - v1.z * v2.y,
             v1.z * v2.x - v1.x * v2.z,
             v1.x * v2.y - v1.y * v2.x };
}

// TODO
[[nodiscard]] export constexpr CVector3 Subtract(const CVector3& v, const CVector3& w)
{
    return v - w;
}

[[nodiscard]] export CVector3 Normalise(const CVector3& v)
{
    const float lengthSq = v.x * v.x + v.y * v.y + v.z * v.z;

    if (IsZero(lengthSq))
        return { 0.0f, 0.0f, 0.0f };

    const float invLength = InvSqrt(lengthSq);
    return { v.x * invLength, v.y * invLength, v.z * invLength };
}

export constexpr CVector3 kXAxis{ 1.0f, 0.0f, 0.0f };
export constexpr CVector3 kYAxis{ 0.0f, 1.0f, 0.0f };
export constexpr CVector3 kZAxis{ 0.0f, 0.0f, 1.0f };
