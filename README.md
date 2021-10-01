[![Build](https://github.com/subski/kmeans_c/workflows/CMake/badge.svg)](https://github.com/subski/kmeans_c/actions)
## How to build (Linux & Windows)

### Requirements
   - OpenCL
   - SDL2
     - (Linux install) : `sudo apt-get install ocl-icd-opencl-dev ocl-icd-libopencl1 opencl-headers ocl-icd-dev libsdl2-dev`

1. Build the CMake project with the command: `cmake -B build/`
2. Build the binaries with the command:      `cmake --build build/`

## How to run

Execute the `kmeans_c` binary in the `build/bin/` folder.

## How to use
Press 'Space' when the window open to run a step of the KMeans algorithm.