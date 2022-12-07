# Sobel and Scharr filters for edge detection

Edge detection via Sobel and Scharr filters capabilities in GIL are demonstrated by the program `sobel_scharr`, compiled from the sources `example/sobel_scharr.cpp`.

## Synopsis

`sobel_scharr input.png sobel|scharr output-x.png output-y.png`

- The first argument must be the full path to an existing image in PNG format
- The second argument is the type of the filter to apply, either `sobel` or `scharr`
- The third argument is the full path to an image of the result of applying the filter in the x direction. The directory will *not* be created and must exist
- The fourth and last argument is the full path to an image of the result of applying the filter in the y direction. The directory will *not* be created and must exist.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements

`sobek_scharr` doesn't have any specific execution requirements.
