export module math.helpers;

import math.matrix;
import <cmath>;
import <numbers>; // c++ 20

export constexpr float PI = std::numbers::pi_v<float>;

[[nodiscard]] export constexpr float ToRadians(float degrees)
{
    return degrees * PI / 180.0f;
}
[[nodiscard]] export constexpr float ToDegrees(float radians)
{
    return radians * 180.0f / PI;
}

[[nodiscard]] export CMatrix4x4 MakeProjectionMatrix(
    float aspectRatio = 4.0f / 3.0f,
    float FOVx = ToRadians(90.0f),
    float nearClip = 0.1f,
    float farClip = 10000.0f)
{
    const float tanFOVx = std::tan(FOVx * 0.5f);
    const float scaleX = 1.0f / tanFOVx;
    const float scaleY = aspectRatio / tanFOVx;
    const float scaleZa = farClip / (farClip - nearClip);
    const float scaleZb = -nearClip * scaleZa;

    return { scaleX,    0.0f,     0.0f, 0.0f,
              0.0f,   scaleY,     0.0f, 0.0f,
              0.0f,     0.0f,  scaleZa, 1.0f,
              0.0f,     0.0f,  scaleZb, 0.0f };
}
