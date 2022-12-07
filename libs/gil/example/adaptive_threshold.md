# Adaptive Thresholding

Adaptive Thresholding capabilities in GIL are demonstrated by the program `adaptive_threshold`, compiled from the sources `example/adaptive_threshold.cpp`.

## Synopsis

`adaptive_threshold`

The program doesn't take any argument on the command line.

`adaptive_threshold` expects to find an image called `test_adaptive.png` in the current directory, and produces one image for each thresholding method: 
- `out-threshold-adaptive-mean.png`
- `out-threshold-adaptive-mean-inv.png`
- `out-threshold-adaptive-gaussian.png`
- `out-threshold-adaptive-gaussian-inv.png`.

## Specific requirements

### Build requirements

- A C++ compiler compliant with C++14 or above
- The PNG library installed and configured.

### Execution requirements

- `adaptive_threshold` expects to find an image called `test_adaptive.png` in the current directory.
