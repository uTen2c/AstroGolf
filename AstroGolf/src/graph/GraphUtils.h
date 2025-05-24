#pragma once
#include <string>

class GraphUtils
{
    GraphUtils() = default;
    ~GraphUtils() = default;
    
public:
    static int Load(const std::string& graphName);
};
