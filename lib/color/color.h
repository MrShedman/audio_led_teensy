#include <Arduino.h>

uint32_t rgb (uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}

uint32_t rgb(float r, float g, float b)
{
    return rgb(uint8_t(r * 255.0), uint8_t(g * 255.0), uint8_t(b * 255.0));
}

struct rgb_u8
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct hsv_converter
{
    uint32_t i;
    float p;
    float q;
    float t;
};

void hsv_pre_compute(hsv_converter* conv, float h, float s)
{
    float hh;
    float ff;

    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    conv->i = (uint32_t)hh;
    ff = hh - conv->i;
    conv->p = (1.0 - s);
    conv->q = (1.0 - (s * ff));
    conv->t = (1.0 - (s * (1.0 - ff)));
}

rgb_u8 hsv2rgb(hsv_converter* conv, float v)
{
    float r, g, b;
    switch(conv->i) 
    {
        case 0:
            r = v;
            g = conv->t;
            b = conv->p;
        break;
        case 1:
            r = conv->q;
            g = v;
            b = conv->p;
        break;
        case 2:
            r = conv->p;
            g = v;
            b = conv->t;
        break;

        case 3:
            r = conv->p;
            g = conv->q;
            b = v;
        break;
        case 4:
            r = conv->t;
            g = conv->p;
            b = v;
        break;
        case 5:
        default:
            r = v;
            g = conv->p;
            b = conv->q;
        break;
    }

    return rgb_u8{r*255.0, g*255.0, b*255.0};
}

// h in degrees, s and v between 0 and 1
uint32_t hsv2rgb(float h, float s, float v)
{
    float hh, p, q, t, ff;
    uint32_t i;
    float r, g, b;

    if(s <= 0.0) 
    {
        r = v;
        g = v;
        b = v;
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
                r = v;
                g = t;
                b = p;
            break;
            case 1:
                r = q;
                g = v;
                b = p;
            break;
            case 2:
                r = p;
                g = v;
                b = t;
            break;

            case 3:
                r = p;
                g = q;
                b = v;
            break;
            case 4:
                r = t;
                g = p;
                b = v;
            break;
            case 5:
            default:
                r = v;
                g = p;
                b = q;
            break;
        }
    }

    return rgb(r, g, b);     
}