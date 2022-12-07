# Line rasterizeration

Line rasterization capabilities in GIL are demonstrated by the program `rasterizer_line`, compiled from the sources `example/rasterizer_line.cpp`.

## Synopsis

`rasterizer_line`

The program doesn't take any argument on the command line.

`rasterizer_line` produces four images of different lines in the current directory:
- `line-bresenham-256-256.png`, in the PNG format
- `line-bresenham-256-128.png`, in the PNG format
- `line-bresenham-256-1.png`, in the PNG format
- `line-bresenham-1-256.png`, in the PNG format.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements

`rasterizer_line` doesn't have any specific execution requirements.
