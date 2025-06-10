#pragma once
#include <string>

#include "World.h"

class StageWorld : public World
{
public:
    [[nodiscard]] virtual std::string GetStageId() const;

    void Reload() const;
};
