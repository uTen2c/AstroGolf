#pragma once
#include <DxLib.h>
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

    template <class T>
    void DrawCenter(const T& x, const T& y, const int& tileX = 0, const int& tileY = 0) const
    {
        const auto destX = static_cast<float>(x) - static_cast<float>(width) * 0.5f;
        const auto destY = static_cast<float>(y) - static_cast<float>(height) * 0.5f;
        DrawRectGraphF(destX, destY, tileX * width, tileY * height, width, height, handle, true);
    }
    
    template <class T>
    void Draw(const T& x, const T& y, const int& tileX = 0, const int& tileY = 0) const
    {
        DrawRectGraphF(static_cast<float>(x), static_cast<float>(y), tileX * width, tileY * height, width, height, handle, true);
    }

    void Draw(const DrawStack& stack, const int& tileX = 0, const int& tileY = 0) const;
};
