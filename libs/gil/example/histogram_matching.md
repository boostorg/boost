# Histogram Matching

Histogram matching capabilities in GIL are demonstrated by the program `histogram_matching`, compiled from the sources `example/histogram_matching.cpp`.

## Synopsis
`histogram_matching`

The program doesn't take any argument on the command line.

`histogram_matching` expects to find an image called `test_adaptive.png` in the current directory, and produces an image in return, where the equalization have been applied: `histogram_gray_matching.png`.

## Specific requirements

### Build requirements
- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements
- `histogram_matching` expects to find an image called `test_adaptive.png` in the current directory.
