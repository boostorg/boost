# Resize

Resizing capabilities in GIL are demonstrated by the program `resize`, compiled from the sources `example/resize.cpp`.

## Synopsis

`resize`

This program expects to find an image called `test.jpg`, in JPEG format, in the current directory, and produces a scaled down version of this image, called `out-resize.jpg`, in JPEG format in the current directory.

The program doesn't take any argument on the command line.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`resize` expects to find an image called `test.jpg`, in JPEG format, in the current directory.
