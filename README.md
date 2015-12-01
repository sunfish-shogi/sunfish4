Sunfish
========

Sunfish is computer Shogi program. This is 4th version of Sunfish.

##### Older Versions

- [Sunfish](https://github.com/sunfish-shogi/sunfish)
 + 2008 ~ 2012
 + Participated in WCSC2011 and WCSC2012(Rookie Award).
- [Sunfish2](https://github.com/sunfish-shogi/sunfish2)
 + 2012 ~ 2013
 + Participated in WCSC2013
- [Sunfish3](https://github.com/sunfish-shogi/sunfish3)
 + 2014 ~ 2015
 + Participated in WCSC2015

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
make release -j 2

# clean
make clean
```

### Xcode

```
mkdir -p build/xcode
cd build/xcode
cmake -G Xcode ../../src/app
```

### Visual Studio (not maitained)

Set parameters into CMake as follows.

| Name                        | Value                   |
|:----------------------------|------------------------:|
| Where is the source code    | path/to/sunfish4/src    |
| Where to build the binaries | path/to/build_directory |

Usage
-----

### Run unit tests

```
make test
```

### Run benchmark tests

```
make benchmark
./sunfish_bm
```

License
-------

MIT License
