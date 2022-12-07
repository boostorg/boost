# Convolution

Convolution capabilities in GIL are demonstrated by the program `convolution`, compiled from the sources `example/convolution.cpp`.

## Synopsis

`convolution`

The program doesn't take any argument on the command line.

`convolution` expects to find an image called `test.jpg` in the current directory, and produces two images in return, where the filters have been applied: `out-convolution.jpg` and `out-convolution2.jpg`

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

- `convolution` expects to find an image called `test.jpg` in the current directory.
