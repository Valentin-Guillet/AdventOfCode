#!/bin/bash
# Execute this file in a year directory (e.g. in ../2017/) to test every day

for i in {1..25}
do
    echo "Day $i"
    cd ./Day$i
    [ -f main ] || ln -s ~/Documents/project-transporter/AdventOfCode/Day0/main main
    sed -i 's/bool first_half = false;/bool first_half = true;/' Day$i.cpp
    ./main
    sed -i 's/bool first_half = true;/bool first_half = false;/' Day$i.cpp
    ./main
    echo
    cd ..
done
