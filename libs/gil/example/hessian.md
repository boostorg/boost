# Hessian Feature Detection

Hessian feature detection capabilities in GIL are demonstrated by the program `hessian`, compiled from the sources `example/hessian.cpp`.

## Synopsis
`hessian input.png window-size hessian-threshold output.png`

- The first parameter must be the full path to an existing image in the PNG format for `hessian` to process
- The second parameter is the size of the window containing the pixels to analyse and must be an odd number (e.g. 9 for a 3x3 matrix)
- The third parameter is the hessian threshold used to identify the optimal values
- The fourth and last parameter is the full path to the output image of `hessian`. The directory will *not* be created and must exist.

## Specific requirements

### Build requirements
- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured

### Execution requirements
- `hessian` has no specific execution requirements.
