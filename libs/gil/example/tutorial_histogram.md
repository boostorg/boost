# Histogram computation

Histogram computation capabilities in GIL are demonstrated by the program `tutorial_histogram`, compiled from the sources `example/tutorial_histogram.cpp`.

## Synopsis

`tutorial_histogram`

The program expects to find an image in the JPEG format in the current directory named `input.jpg`, and produces a text file named `out-histogram.txt`, in the current direction, where the histogram has been calculated..

The program doesn't take any command line arguments.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`tutorial_histogram` expects to find an image in the JPEG format in the current directory.
