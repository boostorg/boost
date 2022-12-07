# Morphology

Morphological operations capabilities in GIL are demonstrated by the program `morphology`, compiled from the sources `example/morphology.cpp`.

## Synopsis

`morphology input.png output-image-template operation1 [operation2 ... operationN]`

- The first parameter must be the full path to an existing image in the PNG format for `morphology` to process
- The second parameter is the pattern to use to name the output files. For example, a template of `out-` will generate files like `out-erosion.png`. Note that a full path can be given here, but that the directory must exist, as `morphology` will *not* create it.
- The rest of the parameters are operation names, separated by a space. Each operation triggers the output of a file, whose name follows the pattern: `_output-image-template_-_operation_-.png`. For example, the line `morphology input.png out- erosion` will produce the image `out-erosion.png`.

The morphological operations available are the following:
- black_hat
- top_hat
- morphological_gradient
- dilation
- erosion
- opening
- closing
- binary

The operations can be provided in any order, only note that if `binary` is supplied, it will be applied first.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements

`morphology` has no specific execution requirements.
