# Thresholding

Thresholding capabilities in GIL are demonstrated by the program `threshold`, compiled from the sources `example/threshold.cpp`.

## Synopsis

`threshold`

The program expects to find an image in the JPEG format in the current directory named `input.jpg`, and produces two images in JPEG format in the current directory in return, named `out-threshold-binary.jpg` and `out-threshold-binary-inv.jpg`, where a thresholding and an inverse thresholding, respectively, has been applied.

The program doesn't take any command line arguments.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`threshold` expects to find an image in the JPEG format in the current directory.
