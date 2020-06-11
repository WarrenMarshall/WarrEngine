#pragma once

struct w_random
{
    void seed();
    int get();
    float getf();
    bool getb();
    float getf_range( float min, float max );
    int geti_range( int min, int max );
};
