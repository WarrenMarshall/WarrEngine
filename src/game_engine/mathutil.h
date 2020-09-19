#pragma once

#define deg2rad(d) ( W_PI * d / 180.0f )
#define rad2deg(r) ( 180.0f * r / W_PI )

#define sin_from_angle(a) ( std::sinf( deg2rad( a ) ) )

constexpr float b2d_scale_factor = 100.0f;
#define to_b2d(value) ((value) / b2d_scale_factor)
#define from_b2d(value) ((value) * b2d_scale_factor)

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct w_accum_value
{
    float value;

    void init();
    void update_value( int steps );
    void update_value();
    void accum( float value );
    void inc();

private:
    float _accumulator;
};
