#!/bin/sh


curdir="${PWD##*/}"
rm bin/$curdir
make -j$(nproc)
make run
