#include <math.h>
#include <string.h>
#include "image.h"
#include "test.h"
#include "args.h"

int tests_total = 0;
int tests_fail = 0;

int within_eps(float a, float b){
    return a-EPS<b && b<a+EPS;
}

int same_image(image a, image b){
    int i;
    if(a.w != b.w || a.h != b.h || a.c != b.c) return 0;
    for(i = 0; i < a.w*a.h*a.c; ++i){
        if(!within_eps(a.data[i], b.data[i])) 
        {
            printf("The value should be %f, but it is %f! \n", b.data[i], a.data[i]);
            return 0;
        }
    }
    return 1;
}

void test_get_pixel(){
    image im = load_image("data/dots.png");
    // Test within image
    TEST(within_eps(0, get_pixel(im, 0,0,0)));
    TEST(within_eps(1, get_pixel(im, 1,0,1)));
    TEST(within_eps(0, get_pixel(im, 2,0,1)));

    // Test padding
    TEST(within_eps(1, get_pixel(im, 0,3,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,0)));
    TEST(within_eps(0, get_pixel(im, 7,8,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,2)));
}

void test_set_pixel(){
    image im = load_image("data/dots.png");
    image d = make_image(4,2,3);
    set_pixel(d, 0,0,0,0); set_pixel(d, 0,0,1,0); set_pixel(d, 0,0,2,0); 
    set_pixel(d, 1,0,0,1); set_pixel(d, 1,0,1,1); set_pixel(d, 1,0,2,1); 
    set_pixel(d, 2,0,0,1); set_pixel(d, 2,0,1,0); set_pixel(d, 2,0,2,0); 
    set_pixel(d, 3,0,0,1); set_pixel(d, 3,0,1,1); set_pixel(d, 3,0,2,0); 

    set_pixel(d, 0,1,0,0); set_pixel(d, 0,1,1,1); set_pixel(d, 0,1,2,0); 
    set_pixel(d, 1,1,0,0); set_pixel(d, 1,1,1,1); set_pixel(d, 1,1,2,1); 
    set_pixel(d, 2,1,0,0); set_pixel(d, 2,1,1,0); set_pixel(d, 2,1,2,1); 
    set_pixel(d, 3,1,0,1); set_pixel(d, 3,1,1,0); set_pixel(d, 3,1,2,1); 

    // Test images are same
    TEST(same_image(im, d));
}

void test_grayscale()
{
    image im = load_image("data/colorbar.png");
    image gray = rgb_to_grayscale(im);
    image g = load_image("figs/gray.png");
    TEST(same_image(gray, g));
}

void test_copy()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    TEST(same_image(im, c));
}

void test_shift()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    shift_image(c, 1, .1);
    TEST(within_eps(im.data[0], c.data[0]));
    TEST(within_eps(im.data[im.w*im.h+13] + .1,  c.data[im.w*im.h + 13]));
    TEST(within_eps(im.data[2*im.w*im.h+72],  c.data[2*im.w*im.h + 72]));
    TEST(within_eps(im.data[im.w*im.h+47] + .1,  c.data[im.w*im.h + 47]));
}

void test_rgb_to_hsv()
{
    image im = load_image("data/dog.jpg");
    rgb_to_hsv(im);
    image hsv = load_image("figs/dog.hsv.png");
    TEST(same_image(im, hsv));
}

void test_hsv_to_rgb()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    rgb_to_hsv(im);
    hsv_to_rgb(im);
    TEST(same_image(im, c));
}

void test_nn_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = nn_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-nn-for-test.png");
    TEST(same_image(resized, gt));
}

void test_bl_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = bilinear_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-bl.png");
    TEST(same_image(resized, gt));
    
}

void test_multiple_resize()
{
    image im = load_image("data/dog.jpg");
    for (int i = 0; i < 10; i++){
        im = bilinear_resize(im, im.w*4, im.h*4);
        im = bilinear_resize(im, im.w/4, im.h/4);
    }
    image gt = load_image("figs/dog-multipleresize.png");
    TEST(same_image(im, gt));
}


void test_highpass_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_highpass_filter();
    image blur = convolve_image(im, f, 0);
    clamp_image(blur);

    
    image gt = load_image("figs/dog-highpass.png");
    TEST(same_image(blur, gt));
}

void test_emboss_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_emboss_filter();
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    
    image gt = load_image("figs/dog-emboss.png");
    TEST(same_image(blur, gt));
}

void test_sharpen_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_sharpen_filter();
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);


    image gt = load_image("figs/dog-sharpen.png");
    TEST(same_image(blur, gt));
}

void test_convolution(){
    image im = load_image("data/dog.jpg");
    image f = make_box_filter(7);
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    image gt = load_image("figs/dog-box7.png");
    TEST(same_image(blur, gt));

}

void test_gaussian_filter(){
    image f = make_gaussian_filter(7);

    image gt = load_image("figs/gaussian_filter_7.png");
    TEST(same_image(f, gt));    
}

void test_gaussian_blur(){
    image im = load_image("data/dog.jpg");
    image f = make_gaussian_filter(2);
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    image gt = load_image("figs/dog-gauss2.png");
    TEST(same_image(blur, gt));    
}

void test_hybrid_image(){
    image man = load_image("data/melisa.png");
    image woman = load_image("data/aria.png");
    image f = make_gaussian_filter(2);
    image lfreq_man = convolve_image(man, f, 1);
    image lfreq_w = convolve_image(woman, f, 1);
    image hfreq_w = sub_image(woman , lfreq_w);
    image reconstruct = add_image(lfreq_man , hfreq_w);
    image gt = load_image("figs/hybrid.png");
    clamp_image(reconstruct);
    TEST(same_image(reconstruct, gt));
}

void test_frequency_image(){
    image im = load_image("data/dog.jpg");
    image f = make_gaussian_filter(2);
    image lfreq = convolve_image(im, f, 1);
    image hfreq = sub_image(im, lfreq);
    image reconstruct = add_image(lfreq , hfreq);

    image low_freq = load_image("figs/low-frequency.png");
    image high_freq = load_image("figs/high-frequency-clamp.png");

    clamp_image(lfreq);
    clamp_image(hfreq);
    TEST(same_image(lfreq, low_freq));
    TEST(same_image(hfreq, high_freq));
    TEST(same_image(reconstruct, im));
}

void test_sobel(){
    image im = load_image("data/dog.jpg");
    image *res = sobel_image(im);
    image mag = res[0];
    image theta = res[1];
    feature_normalize(mag);
    feature_normalize(theta);

    image gt_mag = load_image("figs/magnitude.png");
    image gt_theta = load_image("figs/theta.png");

    TEST(same_image(gt_mag, mag));
    TEST(same_image(gt_theta, theta));

}



int do_test()
{
    TEST('1' == '1');
    TEST('0' == '1');
    return 0;
}

void run_tests()
{
    test_get_pixel();
    test_set_pixel();
    test_copy();
    test_shift();
    test_grayscale();
    test_rgb_to_hsv();
    test_hsv_to_rgb();
    test_nn_resize();
    test_bl_resize();
    test_multiple_resize();
    test_gaussian_filter();
    test_sharpen_filter();
    test_emboss_filter();
    test_highpass_filter();
    test_convolution();
    test_gaussian_blur();
    test_hybrid_image();
    test_frequency_image();
    test_sobel();
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}

