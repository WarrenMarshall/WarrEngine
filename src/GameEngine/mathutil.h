#pragma once

constexpr float W_PI = 3.14159265358979323846f;

namespace
{
    inline float deg2rad( double d )
    {
        return static_cast<float>( W_PI * d / 180.0);
    }

    inline float rad2deg( double r )
    {
        return static_cast<float>(180.0 * r / W_PI);
    }

    inline float sin_from_angle( double angle )
    {
        return std::sinf( deg2rad( angle ) );
    }
}

// ----------------------------------------------------------------------------

struct w_fibonacci
{
    int v1 = 0;
    int v2 = 0;
    int sum = 0;

    w_fibonacci();

    int step();
};

// ----------------------------------------------------------------------------

struct w_accum_value
{
    float accumulator;
    float value;

    void init();
    void update_value( int steps );
    void update_value();
    void accum( float value );
    void inc();
};
