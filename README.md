# CSE 455 Homework 1 #

Welcome friends,

It's time for assignment 1! This one may be a little harder than the last one so remember to start early and start often!

## 1. Image resizing ##

We've been talking a lot about resizing and interpolation in class, now's your time to do it! To resize we'll need some interpolation methods and a function to create a new image and fill it in with our interpolation methods.

- Fill in `float nn_interpolate(image im, float x, float y, int c);` in `src/resize_image.c`
    - It should perform nearest neighbor interpolation. Remember to use the closest `int`, not just type-cast because in C that will truncate towards zero.
- Fill in `image nn_resize(image im, int w, int h);`. It should:
    - Create a new image that is `w x h` and the same number of channels as `im`
    - Loop over the pixels and map back to the old coordinates
    - Use nearest-neighbor interpolate to fill in the image

Now you should be able to run the following `python` command:

    from uwimg import *
    im = load_image("dogsmall.jpg")
    a = nn_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-nn")

Your image should look something like:

![blocky dog](figs/dog4x-nn.png)

Finally, fill in the similar functions `bilinear_interpolate` and `bilinear_resize` to perform bilinear interpolation. Try it out again in `python`:

    from uwimg import *
    im = load_image("dogsmall.jpg")
    a = bilinear_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-bl")

![smooth dog](figs/dog4x-bl.png)

