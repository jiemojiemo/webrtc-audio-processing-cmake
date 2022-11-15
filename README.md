# How to build
```shell
git submodule update --init --recursive
cmake -S . -B build 
cmake --build build -- -j 8
```