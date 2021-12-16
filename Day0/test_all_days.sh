#!/bin/bash
# Execute this file in a year directory (e.g. in ../2017/) to test every day

for i in {1..25}
do
    echo "Day $i"
    cd ./Day$i
    [ -h Makefile ] || ln -s ../../Day0/Makefile Makefile
    make
    ./main
    ./main -s
    echo
    cd ..
done
