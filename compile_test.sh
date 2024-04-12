#!/bin/bash

if test -f ./test; then
    rm ./test
fi

gcc ./test.c ./bmp.c -o test

./test
