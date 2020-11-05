#pragma once

struct w_random
{
    static void seed();
    _NODISCARD static int get();
    _NODISCARD static float getf();
    _NODISCARD static bool getb();
    _NODISCARD static float getf_range( float min, float max );
    _NODISCARD static int geti_range( int min, int max );
};
