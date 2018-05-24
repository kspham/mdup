#!/bin/bash

git submodule init && git submodule update

cd htslib && autoheader && autoconf && ./configure && make && cd ../

make