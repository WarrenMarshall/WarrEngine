#pragma once

struct w_random
{
    static void seed();
    [[nodiscard]] static int get();
    [[nodiscard]] static float getf();
    [[nodiscard]] static bool getb();
    [[nodiscard]] static float getf_range( float min, float max );
    [[nodiscard]] static int geti_range( int min, int max );
};
