#pragma once

class KeyInstance
{
    int key_;
    bool pressing_ = false;
    bool pressed_ = false;

public:
    explicit KeyInstance(const int key)
        : key_(key)
    {
    }

    void Update();
    bool Pressing() const;
    bool Pressed() const;
};
