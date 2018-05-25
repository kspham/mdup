#!/bin/bash

git submodule init && git submodule update

dir="$(pwd)/build/"
htsdir="$(pwd)/build/lib"

if echo $LD_LIBRARY_PATH | grep -q $htsdir; then
	echo "htslib local path found!"
else
	export LD_LIBRARY_PATH=$htsdir:$LD_LIBRARY_PATH
fi

cd htslib && autoheader && autoconf && ./configure --prefix=$dir && make && make install && cd ../

make