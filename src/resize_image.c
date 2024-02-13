#include <math.h>
#include "image.h"


float nn_interpolate(image im, float x, float y, int c)
{
    // f(x,y,z) = Im(round(x), round(y), z)
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    /*
    * a = im.w / w
    * a*-.5 + b = -.5
    */
    image resized = make_image(w, h, im.c);

    float ax = resized.w / w;
    float ay = resized.h / h;

    float bx = -0.5 + 0.5 * ax;
    float by = -0.5 + 0.5 * ay;

    for (int i = 0; i < resized.w; i++) {
        for (int j = 0; j < resized.h; j++) {
            for (int k = 0; k < resized.c; k++) {
                float old_x = ax * i + bx;
                float old_y = ay * j + by;

                float interpolated_area = nn_interpolate(resized, old_x, old_y, k);
                set_pixel(resized, old_x, old_y, k, interpolated_area);
            }
        }
    }
    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float x_floor = floor(x);
    float x_ceil = ceil(x);
    float y_floor = floor(y);
    float y_ceil = ceil(y);

    float q1 = (y - y_floor) * get_pixel(im, x_floor, y_ceil, c) + (y_ceil - y) * get_pixel(im, x_floor, y_floor, c);
    float q2 = (y - y_floor) * get_pixel(im, x_ceil, y_ceil, c) + (y_ceil - y) * get_pixel(im, x_ceil, y_floor, c);
    float vl = (x_ceil - x) * q1 + (x - x_floor) * q2;
    return vl;
}

image bilinear_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);

    float ax = resized.w / w;
    float ay = resized.h / h;

    float bx = -0.5 + 0.5 * ax;
    float by = -0.5 + 0.5 * ay;

    for (int i = 0; i < resized.w; i++) {
        for (int j = 0; j < resized.h; j++) {
            for (int k = 0; k < resized.c; k++) {
                float old_x = ax * i + bx;
                float old_y = ay * j + by;

                float interpolated_area = nn_interpolate(resized, old_x, old_y, k);
                set_pixel(resized, old_x, old_y, k, interpolated_area);
            }
        }
    }
    return resized;
}

