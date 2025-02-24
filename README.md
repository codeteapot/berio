# BER I/O Library for C++

## Contributing

### Building

The build is tested on a Debian "bookworm" with the following packages,

* autoconf
* autoconf-archive
* automake1.11
* pkg-config
* libtool
* g++
* dejagnu

in order to build it, and

* doxygen
* texlive-latex-extra
* texlive-font-utils
* dot2tex

in order to generate the documentation.

If we want to build it on a `stage` directory, we may do the following

```sh
autoreconf --install
```
```sh
./configure --prefix=$PWD/stage
```

Then, we can execute

* `make` to build it,
* `make check` to run tests,
* `make install` to put result files on stage directory,
* `make doxygen-doc` to generate documentation

and so on.
