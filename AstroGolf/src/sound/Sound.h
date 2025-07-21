#pragma once
#include <string>

class Sound final
{
    int handle_;
    float next_volume_multiplier_ = 1.0f;

public:
    float volume = 1.0f;

    explicit Sound(const std::string& path);
    ~Sound();

    // コピー禁止
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    // ムーブ許可
    Sound(Sound&& other) noexcept;
    Sound& operator=(Sound&& other) noexcept;

    void Play();

    void PlayLoop() const;

    void Stop() const;

    void SetNextVolumeMultiplier(float multiplier);

    [[nodiscard]] int GetHandle() const;

    [[nodiscard]] bool IsValid() const;

private:
    int GetFinalVolume() const;
};
