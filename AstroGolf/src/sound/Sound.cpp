#include "Sound.h"

#include <spdlog/spdlog.h>

#include "../Game.h"

Sound::Sound(const std::string& path)
{
    handle_ = LoadSoundMem(std::format("assets/sound/{}", path).c_str());
    if (handle_ == -1)
    {
        spdlog::warn("Failed to load sound file '{}'", path);
        return;
    }
    spdlog::info("Successfully loaded sound file '{}' (handle: {})", path, handle_);
}

Sound::~Sound()
{
    if (handle_)
    {
        DeleteSoundMem(handle_);
    }
}

Sound::Sound(Sound&& other) noexcept : handle_(other.handle_)
{
    other.handle_ = 0;
}

Sound& Sound::operator=(Sound&& other) noexcept
{
    if (this != &other)
    {
        if (handle_)
        {
            DeleteSoundMem(handle_);
        }
        handle_ = other.handle_;
        other.handle_ = 0;
    }
    return *this;
}

void Sound::Play()
{
    if (handle_)
    {
        ChangeNextPlayVolumeSoundMem(GetFinalVolume(), handle_);
        PlaySoundMem(handle_, DX_PLAYTYPE_BACK);
        next_volume_multiplier_ = 1.0f;
    }
}

void Sound::PlayLoop() const
{
    if (handle_)
    {
        ChangeNextPlayVolumeSoundMem(GetFinalVolume(), handle_);
        PlaySoundMem(handle_, DX_PLAYTYPE_LOOP);
    }
}

void Sound::Stop() const
{
    if (handle_)
    {
        StopSoundMem(handle_);
    }
}

void Sound::SetNextVolumeMultiplier(const float multiplier)
{
    next_volume_multiplier_ = multiplier;
}

int Sound::GetHandle() const
{
    return handle_;
}

bool Sound::IsValid() const
{
    return handle_ != 0;
}

int Sound::GetFinalVolume() const
{
    return static_cast<int>(255.0f * Game::soundVolume * volume * next_volume_multiplier_);
}
