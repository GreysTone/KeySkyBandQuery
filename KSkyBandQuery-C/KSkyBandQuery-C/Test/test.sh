#!/usr/bin/env bash

gcc DataGen.c -o DataGen
gcc Brute.c -o Brute
cd ..
make clean
make
cd Test

for (( i = 0; i < 100; i++ )); do
    ./DataGen $1 $2 $3
    ./Brute
    ../main
    python Check.py
done
