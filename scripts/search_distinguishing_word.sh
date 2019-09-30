#!/bin/sh

g++ ../src/search_distinguishing_word/main.cpp -std=c++17 -DLOGGING -o search

./search < input.txt

rm search

