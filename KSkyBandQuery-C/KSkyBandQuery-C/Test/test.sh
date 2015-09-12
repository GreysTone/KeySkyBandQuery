#!/usr/bin/env bash

gcc data_gen.c -o data_gen
gcc brute_force.c -o brute_force
cd ..
make clean
make
cd Test

for (( i = 0; i < 10; i++ )); do
    ./data_gen $1 $2 $3
    ./brute_force
    ../sky_query
    python check.py
done
