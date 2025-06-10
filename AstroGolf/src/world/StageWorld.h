#pragma once
#include <string>

#include "World.h"

class StageWorld : public World
{
public:
    virtual std::string GetStageId() const;
};
