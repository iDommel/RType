#! /bin/bash

sudo cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
sudo cmake --build build