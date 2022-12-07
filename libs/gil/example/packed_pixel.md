# Packed Pixel

Packed pixel formats capabilities in GIL are demonstrated by the program `packed_pixel`, compiled from the sources `example/packed_pixel.cpp`.

## Synopsis

`packed_pixel`

The program doesn't take any argument on the command line.

`packed_pixel` expects to find an image called `input.jpg` in the current directory in JPEG format, and produces two images in the current directory in return, `out-packed_pixel_bgr772.jpg` and `out-packed_pixel_gray1.jpg`.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`packed_pixel` expects to find an image called `input.jpg` in the current directory in JPEG format.
