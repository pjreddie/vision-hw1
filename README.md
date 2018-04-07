# CSE 455 Homework 1 #

Welcome friends,

It's time for assignment 1! This one may be a little harder than the last one so remember to start early and start often! In order to make grading easier, please only edit the files we mention. You should be able to submit `resize_image.c` and `filter_image.c` and we should be able to compile and run them with a fresh clone of the repo without any other files.

To start out this homework, copy over your `process_image.c` file from hw0 to the `src` file in this homework. We will be continuing to build out your image library.

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
    im = load_image("data/dogsmall.jpg")
    a = nn_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-nn")

Your image should look something like:

![blocky dog](figs/dog4x-nn.png)

Finally, fill in the similar functions `bilinear_interpolate` and `bilinear_resize` to perform bilinear interpolation. Try it out again in `python`:

    from uwimg import *
    im = load_image("data/dogsmall.jpg")
    a = bilinear_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-bl")

![smooth dog](figs/dog4x-bl.png)

These functions will work fine for small changes in size, but when we try to make our image smaller, say a thumbnail, we get very noisy results:

    from uwimg import *
    im = load_image("data/dog.jpg")
    a = nn_resize(im, im.w//7, im.h//7)
    save_image(a, "dog7th-bl")

![jagged dog thumbnail](figs/dog7th-nn.png)

As we discussed, we need to filter before we do this extreme resize operation!

## 2. Image filtering with convolutions ##

We'll start out by filtering the image with a box filter. There are very fast ways of performing this operation but instead, we'll do the naive thing and implement it as a convolution because it will generalize to other filters as well!
    
### 2.1 Create your box filter ###

Ok, bear with me. We want to create a box filter, which as discussed in class looks like this:

![box filter](figs/boxfilter.png)

One way to do this is make an image, fill it in with all 1s, and then normalize it. That's what we'll do because the normalization function may be useful in the future!

First fill in `void l1_normalize(image im)`. This should normalize an image to sum to 1.

Next fill in `image make_box_filter(int w)`. We will only use square box filters so just make your filter `w x w`. It should be a square image with one channel with uniform entries that sum to 1.

### 2.2 Write a convolution function ###

Now it's time to fill in `image convolve_image(image im, image filter, int preserve)`. For this function we have a few scenarios. With normal convolutions we do a weighted sum over an area of the image. With multiple channels in the input image there are a few possible cases we want to handle:

- If `filter` and `im` have the same number of channels then it's just a normal convolution. We sum over spatial and channel dimensions and produce a 1 channel image. UNLESS:
- If `preserve` is set to 1 we should produce an image with the same number of channels as the input. This is useful if, for example, we want to run a box filter over an RGB image and get out an RGB image. This means each channel in the image will be filtered by the corresponding channel in the filter. UNLESS:
- If the `filter` only has one channel but `im` has multiple channels we want to apply the filter to each of those channels. Then we either sum between channels or not depending on if `preserve` is set.

Also, `filter` better have either the same number of channels as `im` or have 1 channel. I check this with an `assert`.

We are calling this a convolution but you don't need to rotate the filter or anything. Just apply it to the image as we discussed in class:

![covolution](figs/convolution.png)

Once you are done, test out your convolution by filtering our image! We need to use `preserve` because we want to produce an image that is still RGB.

    from uwimg import *
    im = load_image("data/dog.jpg")
    f = make_box_filter(7)
    blur = convolve_image(im, f, 1)
    save_image(blur, "dog-box7")

We'll get some output that looks like this:

![covolution](figs/dog-box7.png)

Now we can use this to perform our thumbnail operation:

    from uwimg import *
    im = load_image("data/dog.jpg")
    f = make_box_filter(7)
    blur = convolve_image(im, f, 1)
    thumb = nn_resize(blur, blur.w//7, blur.h//7)
    save_image(thumb, "dogthumb")

![covolution](figs/dogthumb.png)

Look at how much better our new resized thumbnail is!

Resize             |  Blur and Resize
:-------------------------:|:-------------------------:
![](figs/dog7th-nn.png)  |  ![](figs/dogthumb.png)


