#include "color.h"

rgb_t hsv2rgb(float h, float s, float v)
{
    float hh, p, q, t, ff;
    uint32_t i;
    rgb_t rgb;

    v *= 255.0;

    if (s <= 0.0) 
    {
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
    }
    else
    {
        hh = h;
        if(hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = (uint32_t)hh;
        ff = hh - i;
        p = v * (1.0 - s);
        q = v * (1.0 - (s * ff));
        t = v * (1.0 - (s * (1.0 - ff)));

        switch(i) 
        {
            case 0:
                rgb.r = v;
                rgb.g = t;
                rgb.b = p;
            break;
            case 1:
                rgb.r = q;
                rgb.g = v;
                rgb.b = p;
            break;
            case 2:
                rgb.r = p;
                rgb.g = v;
                rgb.b = t;
            break;

            case 3:
                rgb.r = p;
                rgb.g = q;
                rgb.b = v;
            break;
            case 4:
                rgb.r = t;
                rgb.g = p;
                rgb.b = v;
            break;
            case 5:
            default:
                rgb.r = v;
                rgb.g = p;
                rgb.b = q;
            break;
        }
    }

    return rgb;     
}

// h in degrees, s and v between 0 and 1
rgb_t hsv2rgb(hsv_t hsv)
{
    return hsv2rgb(hsv.h, hsv.s, hsv.v);
}