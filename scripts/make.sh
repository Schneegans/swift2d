#!/bin/sh

# get directory of script and cd to its parent
DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $DIR/..

# run initial cmake if neccessary
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=release ..
cd ..

# buil it!
cd build

export LANG=en_US.utf8
export LANGUAGE=en
make -j16
