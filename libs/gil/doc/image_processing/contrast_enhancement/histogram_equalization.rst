.. _he:

######################
Histogram Equalization 
######################

Description
-----------

Histogram equalization also known as histogram flattening, is a non-linear image enhancement
algorithm that follows the idea that not only should an image cover the entire grayscale space
but also be uniformly distributed over that range. 

An ideal image would be the one having a flat histogram.

Although care should be taken before applying a non-linear transformation on the image
histogram, there are good mathematical reasons why a flat histogram is the desired goal.

A simple scenario would be an image with pixels concentrated in an interval, in which case
histogram equalization transforms pixels to achieve a flat histogram image. Thus enhancing
the image contrast.

.. figure:: he_chart.png
    :width: 200px
    :align: center
    :height: 100px
    :alt: Could not load image.
    :figclass: align-center

    Pixels concentrated in an interval spread out.

Algorithm
---------

#. First calculate the histogram corresponding to input image.
#. If it is a multi channeled image (e.g. RGB), convert it to a independent color space
   (like YCbCr, HSV etc.).
#. Then calculate the cumulative histogram over the input image.
#. Normalize the histogram to bring bin values between 0-1. For multi-channeled images 
   normalize each channel independently (by the number of pixels in image).
#. If the histogram of image is H(p\ :sub:`x`\)  p\ :sub:`x`\  in  [0, 255], then apply 
   the transformation p\ :sub:`x'`\  = H(p\ :sub:`x`\),  p\ :sub:`x'`\  is pixel in output
   image.

**Explanation**

Since we will be transforming the image to match a flat histogram, we match
the cumulative histogram of the image to the cumulative histogram of a flat histogram.

Cumulative histogram of flat image is  H(p\ :sub:`x'`\)  =  p\ :sub:`x'` .

Hence,

        =>  H(p\ :sub:`x'`\)  =  H(p\ :sub:`x`\)

        =>  p\ :sub:`x'`\     =  H(p\ :sub:`x`\)

Results
-------
The algorithm is applied on a few standard images. One of the transformations in shown below:

**Grayscale Image**

.. figure:: https://github.com/boost-gil/test-images/blob/master/jpeg/suite/barbara.jpg
    :width: 512px
    :align: center
    :height: 256px
    :alt: Could not load image.
    :figclass: align-center

**RGB**

.. figure:: https://github.com/boost-gil/test-images/blob/master/jpeg/suite/church.jpg
    :width: 900px
    :align: center
    :height: 300px
    :alt: Could not load image.
    :figclass: align-center


Demo
----

Usage Syntax:

    .. code-block:: cpp

            gray8_image_t inp_img;
            read_image("your_image.png", inp_img, png_tag{});
            gray8_image_t dst_img(inp_img.dimensions());
            histogram_equalization(view(inp_img), view(dst_img));

            // To specify mask over input image

            vector<vector<bool>> mask(inp_img.height(), vector<bool>(inp_img.width(), true));
            histogram_equalization(view(inp_img), view(dst_img), true, mask);
            
    .. tip::  Convert an RGB image to a channel independent color space
              before trying the histogram equalization algorithm.

