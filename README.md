# BER I/O Library for C++

## Contributing

### Building

The build is tested on a Debian "bookworm" with the following packages,

* autoconf
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

### Using Doxygen Awesome (Git only)

Documentation is ready to be rendered using
[Doxygen Awesome](https://jothepro.github.io/doxygen-awesome-css/).

Do the following on `doc/config/html/doxygen-awesome-css` directory before generating documentation,

```sh
git submodule update
```
```sh
git checkout v2.3.4
```

### Trick to remove generated files (Git only)

The following command removes all generated files,

```sh
rm -rf * && git reset && git checkout -- .
```

It will restore the original directory content.
