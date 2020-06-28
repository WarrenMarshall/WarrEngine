#pragma once

struct w_random
{
    static void seed();
    static int get();
    static float getf();
    static bool getb();
    static float getf_range( float min, float max );
    static int geti_range( int min, int max );
};
