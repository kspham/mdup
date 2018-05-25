#!/bin/bash

git submodule init && git submodule update

dir="$(pwd)/htslib/"

cd htslib && autoheader && autoconf

./configure --prefix=$dir --disable-lzma --disable-libcurl --disable-bz2

make -j 4 && make install && cd ../

make