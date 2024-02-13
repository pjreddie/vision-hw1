#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

/*
In our data array we store the image in CHW format
*/

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int imW = im.w;
    int imH = im.h;
    int imC = im.c;
    x = (x < 0) ? 0 : x;
    x = (x >= imW) ? imW - 1 : x;
    y = (y < 0) ? 0 : y;
    y = (y >= imH) ? imH - 1 : y;
    c = (c < 0) ? 0 : c;
    c = (c >= imC) ? imC - 1 : c;

    int idx = c * imW * imH + y * imW + x;
    return im.data[idx];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int imW = im.w;
    int imH = im.h;
    int imC = im.c;
    if (x < 0 || x >= imW || y < 0 || y >= imH || c < 0 || c >= imC)
        return;

    int idx = c * imW * imH + y * imW + x;
    im.data[idx] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, sizeof(float)*im.w*im.h*im.c);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int y = 0; y < im.h; ++y)
        for (int x = 0; x < im.w; ++x)
            gray.data[y * im.w + x] = 0.299 * im.data[y * im.w + x] +
                    0.587 * im.data[im.w * im.h + y * im.w + x] +
                    0.114 * im.data[2 * im.w * im.h + y * im.w + x];
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    if (c < 0 || c >= im.c)
        return;

    for (int y = 0; y < im.h; ++y)
        for (int x = 0; x < im.w; ++x)
            im.data[c * im.w * im.h + y * im.w + x] += v;
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int c = 0; c < im.c; ++c)
        for (int y = 0; y < im.h; ++y)
            for (int x = 0; x < im.w; ++x) {
                int idx = c * im.w * im.h + y * im.w + x;
                im.data[idx] = fmax(im.data[idx], 0.0);
                im.data[idx] = fmin(im.data[idx], 1.0);
            }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float r, g, b, min, max, c, h, s, v;
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            r = im.data[x + y*im.w];
            g = im.data[x + y*im.w + im.w*im.h];
            b = im.data[x + y*im.w + 2*im.w*im.h];
            min = three_way_min(r, g, b);
            max = three_way_max(r, g, b);
            c = max - min;
            v = max;
            if (v != 0) {
                s = c / v;
            } else {
                s = 0;
            }

            if (c != 0) {
                if (v == r) {
                    h = (g - b) / c;
                } else if (v == g) {
                    h = ((b - r) / c) + 2;
                } else {
                    h = ((r - g) / c) + 4;
                }
            } else {
                h = 0;
            }

            h /= 6;
            if (h < 0) {
                h++;
            }
            im.data[x + y*im.w] = h;
            im.data[x + y*im.w + im.w*im.h] = s;
            im.data[x + y*im.w + 2*im.w*im.h] = v;
        }
    }
}

// https://github.com/chuanenlin/cse455-hw0/blob/dfece5eedab7cf264f776205bc877d49c24a3adf/src/process_image.c#L133
void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float h, s, v, c, max, min, r, g, b, h_temp;
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            h = im.data[x + y*im.w];
            s = im.data[x + y*im.w + im.w*im.h];
            v = im.data[x + y*im.w + 2*im.w*im.h];
            c = s * v;
            max = v;

            if (v != c) {
            min = v - c;
            } else {
                min = 0.0;
            }
            h_temp = h * 6;
            if (c == 0) {
                r = v;
                g = v;
                b = v;
            } else if (h_temp > 5 && h_temp < 6) {
                r = max;
                g = min;
                b = ((((h_temp /  6) - 1) * 6 * c) - g) * -1;
            } else if (h_temp == 5) {
                r = max;
                g = min;
                b = max;
            } else if (h_temp < 5 && h_temp > 4) {
                g = min;
                r = (h_temp - 4) * c + g;
                b = max;
            } else if (h_temp == 4) {
                r = min;
                g = min;
                b = max;
            } else if (h_temp < 4 && h_temp > 3) {
                r = min;
                g = (((h_temp - 4) * c) - r) * -1;
                b = max;
            } else if (h_temp == 3) {
                r = min;
                g = max;
                b = max;
            } else if (h_temp < 3 && h_temp > 2) {
                r = min;
                g = max;
                b = ((h_temp - 2) * c) + r;
            } else if (h_temp == 2) {
                r = min;
                g = max;
                b = min;
            } else if (h_temp < 2 && h_temp > 1) {
                g = max;
                b = min;
                r = (((h_temp - 2) * c) - b) * -1;
            } else if (h_temp == 1) {
                r = max;
                g = max;
                b = min;
            } else if (h_temp < 1 && h_temp > 0) {
                r = max;
                b = min;
                g = (h_temp * c) + b;
            } else {
                r = max;
                g = min;
                b = min;
            }
            im.data[x + y*im.w] = r;
            im.data[x + y*im.w + im.w*im.h] = g;
            im.data[x + y*im.w + 2*im.w*im.h] = b;
        }
    }
}


void scale_image(image im, int c, float v) {
    if (c < 0 || c >= im.c)
        return;


    for (int y = 0; y < im.h; ++y)
        for (int x = 0; x < im.w; ++x)
            im.data[c * im.w * im.h + y * im.w + x] *= v;
}