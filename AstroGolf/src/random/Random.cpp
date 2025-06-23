#include "Random.h"
#include <chrono>

Random::Random()
    : generator_(static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count())),
      float_dist_(0.0f, 1.0f),
      bool_dist_(0, 1)
{
}

Random& Random::GetInstance()
{
    static Random instance;
    return instance;
}

float Random::Float()
{
    return float_dist_(generator_);
}

bool Random::Bool()
{
    return bool_dist_(generator_) == 1;
}
