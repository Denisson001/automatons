#!/bin/sh

g++ ../src/main_determine.cpp -std=c++17 -DLOGGING -o determine

./determine < input.txt

rm determine
