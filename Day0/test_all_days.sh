#!/bin/bash
# Execute this file in a year directory (e.g. in ../2017/) to test every day

echo "Making all targets..."
for i in {1..25}
do
    cd ./Day$i 2>/dev/null || continue
    [ -h Makefile ] || ln -s ../../Day0/Makefile Makefile
    make --question && cd .. && continue
    echo -n "Day $i "
    make
    echo
    cd ..
done

echo "Executing !"
for i in {1..25}
do
    cd ./Day$i 2> /dev/null || continue
    echo "Day $i"
    ./main
    ./main -s
    echo
    cd ..
done
