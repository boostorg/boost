# Affine transformation

Affine transformation capabilities in GIL are demonstrated by the program `affine`, compiled from the sources `example/affine.cpp`.

## Synopsis

`affine`

The program doesn't take any argument on the command line.

`affine` expects to find an image called `test.jpg` in the current directory, and produces an image in return, where the transformations have been applied: `out-affine.jpg`

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

- `affine` expects to find an image called `test.jpg` in the current directory.
