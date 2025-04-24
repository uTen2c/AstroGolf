#include "Pos.h"

Vector2d Pos::GetPoint() const
{
    return point_;
}

double Pos::GetRotation() const
{
    return rotation_;
}

Pos Pos::SetPoint(const Vector2d& point) const
{
    return {point, this->GetRotation()};
}

Pos Pos::SetRotation(const double& rotation) const
{
    return {this->GetPoint(), rotation};
}
