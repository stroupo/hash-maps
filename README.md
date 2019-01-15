# Hash Maps
![](https://img.shields.io/github/languages/top/stroupo/hash-maps.svg?style=for-the-badge)
![](https://img.shields.io/github/languages/code-size/stroupo/hash-maps.svg?style=for-the-badge)
![](https://img.shields.io/github/repo-size/stroupo/hash-maps.svg?style=for-the-badge)
![](https://img.shields.io/github/license/stroupo/hash-maps.svg?style=for-the-badge)

<b>
<table>
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/stroupo/hash-maps">
                <img src="https://img.shields.io/github/last-commit/stroupo/hash-maps/master.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/stroupo/hash-maps/tree/master"><img src="https://circleci.com/gh/stroupo/hash-maps/tree/master.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/stroupo/hash-maps">
              <img src="https://codecov.io/gh/stroupo/hash-maps/branch/master/graph/badge.svg" />
            </a>
        </td>
        <td>
            <a class="badge-align" href="https://www.codacy.com/app/lyrahgames/hash-maps?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=stroupo/hash-maps&amp;utm_campaign=Badge_Grade">
                <img src="https://api.codacy.com/project/badge/Grade/6b9b79cea57f453988149c64d17dfccb"/>
            </a>
        </td>
        <td>
            <a href="https://lgtm.com/projects/g/stroupo/hash-maps/context:cpp">
                <img alt="Language grade: C/C++" src="https://img.shields.io/lgtm/grade/cpp/g/stroupo/hash-maps.svg?logo=lgtm&logoWidth=18"/>
            </a>
        </td>
    </tr>
    <tr>
        <td>
            development
        </td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/stroupo/hash-maps">
                <img src="https://img.shields.io/github/commit-activity/y/stroupo/hash-maps.svg?logo=github&logoColor=white">
            </a>
        </td>
        <td>
            <img src="https://img.shields.io/github/release/stroupo/hash-maps.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/stroupo/hash-maps.svg?label=pre-release&logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag/stroupo/hash-maps.svg?colorB=blue&logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/stroupo/hash-maps.svg?label=latest%20tag&logo=github&logoColor=white">
        </td>
    </tr>
</table>
</b>

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
In the directory `build`, that was created for building the actual binaries, run the following command to install the compiled libraries together with the CMake package.
Make sure that you have the permissions to install system libraries.
Otherwise change the CMake installation prefix or use `sudo`.

    cmake --build . --target install

## Uninstallation
In the directory `build`, that was created for building and installing the actual binaries, run the following command to uninstall the compiled libraries.
Make sure that you have the permissions to remove system libraries.
    
    cmake --build . --target uninstall

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
Note that the installation of the library is not required to find and use the CMake package.
After building the source code of the project CMake exports a local package configuration which refers to the current build directory.
As long as this build directory exists the commands written above can be used to find the library.

## Todo
- Create a CMake package out of the generated library. So there should be three different ways for including another project in your own one.
- Benchmark everything!

## Resources
- [I wrote a fast hash table](https://probablydance.com/2014/05/03/i-wrote-a-fast-hash-table/)
- [A better hash table](http://bannalia.blogspot.com/2014/01/a-better-hash-table-clang.html)
- [A fast alternative to the modulo reduction](https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/)
- [How to build a hash function for various template types](https://stackoverflow.com/questions/34563574/how-to-build-hash-function-for-various-template-types)