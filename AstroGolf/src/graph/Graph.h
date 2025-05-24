#pragma once
#include <string>

#include "../math/DrawStack.h"

class Graph final
{
public:
    const int handle;
    const int width;
    const int height;

    Graph(const std::string& path, int width, int height);
    ~Graph();

    void DrawCenter(const float& x, const float& y) const;
    void Draw(const DrawStack& stack) const;
};
