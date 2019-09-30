#!/bin/sh

g++ ../src/determine/main.cpp -std=c++17 -DLOGGING -o determine

./determine < input.txt

rm determine
