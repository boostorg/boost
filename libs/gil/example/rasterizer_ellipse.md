# Ellipse rasterizeration

Ellipse rasterization capabilities in GIL are demonstrated by the program `rasterizer_ellipse`, compiled from the sources `example/rasterizer_ellipse.cpp`.

## Synopsis

`rasterizer_ellipse`

The program doesn't take any argument on the command line.

`rasterizer_ellipse` produces three images in the current directory:
- `rasterized_ellipse_gray.jpg`, in the JPEG format, wich is a greyscale image of an ellipse
- `rasterized_ellipse_rgb.jpg`, in the JPEG format, wich is an RGB image of an ellipse
- `rasterized_ellipse_rgb_out_of_bound.jpg`, in the JPEG format, wich is an RGB image of an ellipse bigger than the containinig view.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`rasterizer_ellipse` doesn't have any specific execution requirements.
