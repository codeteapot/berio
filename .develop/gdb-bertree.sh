#!/bin/sh

# The package `libtool-bin` must be installed if you are using Debian.
#
# ./configure --prefix=$PWD/stage --enable-debug

libtool --mode=execute gdb testsuite/lib/bertree/bertree
