# Convolution (2d kernel)

2d kernel convolution capabilities in GIL are demonstrated by the program `convolve2d`, compiled from the sources `example/convolve2d.cpp`.

## Synopsis

`convolve2d`

The program doesn't take any argument on the command line.

`convolve2d` expects to find an image called `src_view.png` in the current directory, and produces two images in return, where the filters have been applied: `out-convolve2d.png` and `out-convolve_option_extend_zero.png`

Note that the user is expected to press a key to end the program.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured
- The PNG library installed and configured.

### Execution requirements

- `convolve2d` expects to find an image called `src_view.png` in the current directory.
