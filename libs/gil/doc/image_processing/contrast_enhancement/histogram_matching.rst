.. _hm:

##################
Histogram Matching 
##################

Description
-----------

Histogram Matching is a technique to match the histograms of two images.

One use case of this would be when two images of the same location have been taken 
under the same local illumination but with different sensors, bringing out different 
features in either image.

The famous histogram equalization is a special case of this algorithm when the reference image
is expected to have a uniform histogram.


Algorithm
---------

#. Calculate the histogram corresponding to input image and reference image.
#. If it is a multi channeled image (e.g. RGB), convert both to an independent color space
   (like YCbCr, HSV etc.).
#. Then calculate the cumulative histogram over the input image and reference image.
#. Normalize both the histogram to bring bin values between 0-1. For multi-channeled images
   normalize each channel independently (by the number of pixels in image).
#. If the cumulative histogram of input image is H(p\ :sub:`x`\) and of reference image is R(p\ :sub:`x'`\) 
   p\ :sub:`x`\  &  p\ :sub:`x'`\  in  [0, 255], then apply the transformation 
   p\ :sub:`x'`\  =  R\ :sup:`-1`\ (H(p\ :sub:`x`\ ))

**Explanation**

Since we will be transforming the image to match a reference image, we match
the cumulative histogram of the image to the cumulative histogram of the reference histogram.

Hence,

        =>  R(p\ :sub:`x'`\)  =  H(p\ :sub:`x`\ )

        =>  p\ :sub:`x'`\     =  R\ :sup:`-1`\ (H(p\ :sub:`x`\ ))

Results
-------
The algorithm is applied on a few standard images. One of the transformations in shown below:

**Original Image(left) & Reference Image(right)**

.. figure:: https://github.com/boost-gil/test-images/blob/master/jpeg/suite/matching.jpg
    :width: 600px
    :align: center
    :height: 300px
    :alt: Could not load image.
    :figclass: align-center

**Histogram matched Image**

.. figure:: https://github.com/boost-gil/test-images/blob/master/jpeg/suite/matching_out.jpg
    :width: 300px
    :align: center
    :height: 300px
    :alt: Could not load image.
    :figclass: align-center


Demo
----

Usage Syntax:

    .. code-block:: cpp

        gray8_image_t inp_img, ref_img;
        read_image("your_image.png", inp_img, png_tag{});
        read_image("your_ref_image.png", ref_img, png_tag{});
        gray8_image_t dst_img(inp_img.dimensions());
        histogram_matching(view(inp_img), view(ref_image), view(dst_img));

        // To specify mask over input image

        vector<vector<bool>> mask(inp_img.height(), vector<bool>(inp_img.width(), true));
        histogram_matching(view(inp_img), view(ref_image), view(dst_img), true, mask);
            
    .. tip::  Convert an RGB image to a channel independent color space
              before trying the histogram matching algorithm.

