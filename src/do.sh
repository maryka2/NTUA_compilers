#!/bin/sh

if [ "$1" != "" ]; then
    make
    echo "Compiling $1..."
    ./pcl < $1 > a.ll || exit 1
    llc-6.0 a.ll -o a.asm
    clang a.asm lib.a -o a.out
else
    echo "ERROR (do.sh): Provide the PCL script name."
fi
