#pragma once
#include <random>

class Random final
{
    std::mt19937 generator_;
    std::uniform_real_distribution<float> float_dist_;
    std::uniform_int_distribution<int> bool_dist_;

    Random();

public:
    // シングルトンインスタンス取得
    static Random& GetInstance();

    // コピー・ムーブ禁止
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;
    Random(Random&&) = delete;
    Random& operator=(Random&&) = delete;
    ~Random() = default;

    // 0から1のfloatを返す
    float Float();

    // boolを返す（50%の確率）
    bool Bool();
};
