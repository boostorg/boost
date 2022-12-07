# Custom pixel reference and iteraror: Interleaved Pointer

Definition of custom pixel reference and iterator capabilities in GIL are demonstrated by the program `interleaved_ptr`, compiled from the sources `example/interleaved_ptr.cpp`, `interleaved_ptr.hpp` and `interleaved_ref.hpp`.

## Synopsis

`interleaved_ptr`

The program doesn't take any argument on the command line.

The program expects to find an image, `test.jpg` in the current directory, and produces the image `out-interleaved_ptr.jpg` in return, that has been generated using the custom pixel reference and iterator objects.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The JPEG library installed and configured.

### Execution requirements

`interleaved_ptr` expects to find an image, `test.jpg` in the current directory.
