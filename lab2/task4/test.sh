#/bin/bash

make clean
make optimized
mkdir data

for i in {1..8}; do
    for j in {1..9}; do
        ./queens $i $j 10 2 >> data/data_$i
    done
done