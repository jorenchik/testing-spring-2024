# Overview

[![Tests](https://github.com/jorenchik/testing-spring-2024/actions/workflows/test.yml/badge.svg)](https://github.com/jorenchik/testing-spring-2024/actions/workflows/test.yml)

[Latvian Description](./README-lv.md)

## Project

This is a simple C++ project that includes the function for our colleagues to
test. The function (i.e., the subfunctions that are used in main function) is
located in `validation.cc`.

## Additional libraries and executable

Directory `modules` is concerned with preparation of data prior to the function
call. `test_validation.cc` is a single test file that contain the tests.
[GTest](https://github.com/google/googletest) is used for tests.

## Compilation on Linux and MacOS

Compilation on Linux and MacOS requires cmake and CXX compiler (e.g., g++).

### MacOS

MacOS require a separate installation of `gcc13`.

#### Using [Homebrew](https://brew.sh/)

```bash
brew install cmake gcc@13
brew install ninja # optional
```

### Linux

You know what to do.

### ninja

```bash
cmake -G Ninja -B buld
# or
# cmake -G Ninja -B build -D CMAKE_CXX_COMPILER=g++-13 # MacOS
cd build
ninja
./main
```

### Make

```bash
cmake -B build
# or
# cmake -B build -D CMAKE_CXX_COMPILER=g++-13 # for MacOS
cd build
make
./main
```

### Windows and/or VSCode

Install [CMake](https://cmake.org/download/) and VSCode
[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
extension.

- Open project as root in VSCode.
- Press **View->Command palette...** or `Ctrl+Shift+P` and run `CMake: Build`.

Resulting binaries are `build/main` -- the program and `build/test_validation`
to run tests specified in `test_validation.cc`.
