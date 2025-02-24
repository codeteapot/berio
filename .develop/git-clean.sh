#!/bin/sh

currentdir="$(pwd)"
scriptdir="$(dirname $(realpath $0))"
rootdir="$scriptdir/.."
doxygenawesomecssdir="$rootdir/doc/doxygen-awesome-css"

cd $rootdir
rm -rf * && git reset && git checkout -- .

# https://jothepro.github.io/doxygen-awesome-css/
cd $doxygenawesomecssdir
git submodule update
git checkout v2.3.4

cd $currentdir
