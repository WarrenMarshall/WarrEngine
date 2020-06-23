#pragma once

#define deg2rad(d) ( W_PI * d / 180.0f )
#define rad2deg(r) ( 180.0f * r / W_PI )
#define sin_from_angle(a) ( std::sinf( deg2rad( a ) ) )

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
