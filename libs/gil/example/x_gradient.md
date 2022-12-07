# Gradients calculations

Gradients calculations capabilities in GIL are demonstrated by the program `x_gradient`, compiled from the sources `example/x_gradient.cpp`.

## Synopsis

`x_gradient`

The program expects to find an image in the JPEG format in the current directory named `input.jpg`, and produces an image in JPEG format in the current directory in return, named `out-x_gradient.jpg`, where the gradients have been captured.

The program doesn't take any command line arguments.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`x_gradient` expects to find an image in the JPEG format in the current directory.
