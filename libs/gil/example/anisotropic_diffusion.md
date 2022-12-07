# Anisotropic diffusion

Anisotropic diffusion capabilities in GIL are demonstrated by the program `anisotropic_diffusion`, compiled from the sources `example/anisotropic_diffusion.cpp`.

## Synopsis

`anisoptropic_diffusion input.png output.png gray|rgb iterations kappa`
- The first parameter must be the full path to an existing image in the JPEG format for `anisoptropic_diffusion` to process
- The second parameter is the full path to the output image of `anisotropic_diffusion`. The directory will *not* be created and must exist.
- The third parameter is the colour space: either `gray` or `rgb`
- The fourth parameter is the number of iterations, which *must* be a positive integer
- The fifth and last parameter is the value of the kappa constant

Note that both the input and the ouput images must be in the PNG format.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements

`anisotropic_diffusion` has no specific execution requirements.
