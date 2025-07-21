#include "Math.h"

#include <valarray>

#include "CrossPointResult.h"

float Math::Lerp(const float& start, const float& end, const float& delta)
{
    return start + (end - start) * delta;
}

Vec2 Math::Lerp(const Vec2& start, const Vec2& end, const float& delta)
{
    return {
        Lerp(start.x, end.x, delta),
        Lerp(start.y, end.y, delta)
    };
}

float Math::EaseOutQuart(const float delta)
{
    return 1.0f - pow(1.0f - delta, 4.0f);
}

float Math::EaseOutQuad(const float x)
{
    return 1.0f - (1.0f - x) * (1.0f - x);
}

CrossPointResult Math::GetCrossPoint(const Vec2& start0, const Vec2& end0, const Vec2& start1, const Vec2& end1)
{
    CrossPointResult result;
    result.crossed = false;
    result.point = Vec2(0, 0);

    // 線分0の方向ベクトル
    const Vec2 dir0 = end0 - start0;
    // 線分1の方向ベクトル
    const Vec2 dir1 = end1 - start1;

    // 2つの線分の方向ベクトルの外積
    const float cross = dir0.Cross(dir1);

    // 平行な場合（外積が0）
    if (std::abs(cross) < 1e-6f)
    {
        return result;
    }

    // 線分0の始点から線分1の始点へのベクトル
    const Vec2 startDiff = start1 - start0;

    // パラメータtとuを計算
    const float t = startDiff.Cross(dir1) / cross;
    const float u = startDiff.Cross(dir0) / cross;

    // 両方のパラメータが0以上1以下の場合、線分が交差している
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
    {
        result.crossed = true;
        result.point = start0 + dir0 * t;
    }

    return result;
}

Vec2 Math::GetCenter(const Vec2& a, const Vec2& b)
{
    const Vec2 min(std::min(a.x, b.x), std::min(a.y, b.y));
    const Vec2 max(std::max(a.x, b.x), std::max(a.y, b.y));
    const auto width = max.x - min.x;
    const auto height = max.y - min.y;
    return {min.x + width * 0.5f, min.y + height * 0.5f};
}
