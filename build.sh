#!/bin/bash

RED="\033[0;31m\033[1m"
NC="\033[0m" # No Color

if [ "$BASH_VERSION" = '' ]; then
    printf "${RED}Please run by command: bash build.sh${NC}\n"
    exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${DIR}
git submodule update --init --recursive || exit 1

# build htslib
DIR="$(pwd)/htslib/"
cd htslib && autoheader && autoconf
./configure --prefix=${DIR} --disable-lzma --disable-libcurl --disable-bz2 || exit 1
make || exit 1
make install || exit 1
cd ../

# build mdup
make || exit 1