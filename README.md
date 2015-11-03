Sunfish
========

Sunfish is computer Shogi program.

Requirement
-----------

- CMake
- C++ Compiler(GCC4.9/Clang/MSVC2013)

Build
-----

### GNU Make

```
make release

# parallel
make release -j

# clean
make clean

# unit test
make test
```

### Xcode

```
mkdir -p build/xcode
cd build/xcode
cmake -G Xcode ../../src
```

### Visual Studio (not maitained)

Set parameters into CMake as follows.

| Name                        | Value                  |
|:----------------------------|-----------------------:|
| Where is the source code    | path/to/sunfish3/src   |
| Where to build the binaries | path/to/sunfish3/build |

Usage
-----

### Execute

```
# CUI application
./sunfish

# unit test
./sunfish_test
```

License
-------

MIT License
