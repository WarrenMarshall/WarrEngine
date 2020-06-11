#pragma once

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
