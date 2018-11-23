# Hash Maps

## Installing doctest
Run the following commands in a directory appropriate for temporarily saving source code and build configurations.

    git clone https://github.com/onqtam/doctest.git
    cd doctest
    mkdir build
    cd build
    cmake ..
    cmake --build .
    sudo cmake --build . --target install

## Building
Run the following commands in the source directory.

    mkdir build
    cd build
    cmake ..
    cmake --build .

## Testing
In the directory `build`, that was created for building the actual binaries, run the following command.

    ctest --verbose

## Installation
In the directory `build`, that was created for building the actual binaries, run the following command to install the compiled libraries.
Make sure that you have the permissions to install system libraries.
Otherwise change the CMake installation prefix or use `sudo`.

    cmake --build . --target install

## Execution
In the directory `build`, that was created for building the actual binaries, run the following commands.
    
    ./main

## Usage in other CMake projects

One can find the installed package by the following command.
```cmake
    find_package(hash_map CONFIG REQUIRED)
```
For linking against it, one has to use the namespace `stroupo`.
```cmake
    target_link_libraries(${TARGET_NAME} PRIVATE/PUBLIC/INTERFACE stroupo::hash_map)
```

## Todo
- Create a CMake package out of the generated library. So there should be three different ways for including another project in your own one.
- Benchmark everything!

## Resources
### Hashtables
- https://probablydance.com/2014/05/03/i-wrote-a-fast-hash-table/
- http://bannalia.blogspot.com/2014/01/a-better-hash-table-clang.html
- https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
- https://stackoverflow.com/questions/34563574/how-to-build-hash-function-for-various-template-types

### CMake
- Daniel Pfeifer: *Effective CMake* [2017]
    - https://www.youtube.com/watch?v=bsXLMQ6WgIk
    - https://de.slideshare.net/DanielPfeifer1/effective-cmake

### doctest
- https://github.com/onqtam/doctest


### packaging
- https://conan.io/