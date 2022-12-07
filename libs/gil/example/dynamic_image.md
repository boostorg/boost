# Dynamic Image

Dynamic image manipulation capabilities in GIL are demonstrated by the program `dynamic_image`, compiled from the sources `example/dynamic_image.cpp`.

## Synopsis

`dynamic_image`

The program doesn't take any argument on the command line.

`dynamic_image` expects to find an image called `test.jpg` in the current directory, and produces an image in return, where the a flip has been applied: `out-dynamic_image.jpg`.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured

### Execution requirements

- `dynamic_image` expects to find an image called `test.jpg` in the current directory.
