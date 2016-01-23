Sunfish
========

Sunfish is computer Shogi(Jpanese Chess) program. This is 4th version of Sunfish.

##### Older Versions

- [Sunfish](https://github.com/sunfish-shogi/sunfish)
 + 2008 ~ 2012
 + Participated in [WCSC(World Computer Shogi Championship)2011](http://www.computer-shogi.org/wcsc21/) and [WCSC2012](http://www.computer-shogi.org/wcsc22/).
 + Rookie Award in WCSC2012
- [Sunfish2](https://github.com/sunfish-shogi/sunfish2)
 + 2012 ~ 2013
 + Participated in [WCSC2013](http://www.computer-shogi.org/wcsc23/)
- [Sunfish3](https://github.com/sunfish-shogi/sunfish3)
 + 2014 ~ 2015
 + Participated in [WCSC2015](http://www.computer-shogi.org/wcsc25/)

Requirement
-----------

- CMake
- C++ Compiler(GCC4.9/Clang/MSVC2013)

Build
-----

### GNU Make

```
make TARGET
```

### Xcode

```
mkdir -p out/xcode_test
cd out/xcode_test
cmake -G Xcode ../../src/test
```

### Visual Studio (not maitained)

Set parameters into CMake as follows.

| Name                        | Value                   |
|:----------------------------|------------------------:|
| Where is the source code    | path/to/sunfish4/src    |
| Where to build the binaries | path/to/build_directory |

Usage
-----

### CSA Client

This is a client program of CSA network protocol.
See ["CSA network Protocol"](http://www.computer-shogi.org/protocol/).

```
make csa
vi config/csa.ini
./sunfish_csa
```

### USI Engine

USI(Universal Shogi Interface) is a protocol used for communication between Shogi engine and GUI application.
See ["About USI Protocol"](http://www.geocities.jp/shogidokoro/usi.html).

Build `src/usi`.
If you use `make`, execute with `usi` target as follows:

```
make usi

# debugging mode
make usi-debug
```

Register `sunfish_usi.exe` or `sunfish_usi` into the GUI application.

### Unit Tests

```
make test
```

### Experiments

```
make expt
./sunfish_expt --help
```

### Profiling

```
make prof
```

### Benchmark Tests

```
make bm
./sunfish_bm
```

### Machine Learning

```
make ln
vi config/batch_learn.ini
./sunfish_ln
```

### Development Tool

```
make dev
./sunfish_dev --help
```

License
-------

MIT License
