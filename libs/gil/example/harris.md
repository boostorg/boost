# Harris Corner Detection

Harris corner detection capabilities in GIL are demonstrated by the program `harris`, compiled from the sources `example/harris.cpp` and `hvstack.hpp`.

## Synopsis

`harris input.png window-size discriminant harris-threshold output.png`

- The first parameter must be the full path to an existing image in the PNG format for `harris` to process
- The second parameter is the size of the window containing the pixels to analyse and must be an odd number (e.g. 9 for a 3x3 matrix)
- The third parameter is the empirically-defined discriminant constant, usually between 0.04 and 0.06
- The fourth parameter is the harris threshold used to identify the optimal values
- The fifth and last parameter is the full path to the output image of `harris`. The directory will *not* be created and must exist.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured

### Execution requirements

- `harris` has no specific execution requirements.
