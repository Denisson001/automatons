#!/bin/sh

g++ ../src/minimal_automaton/main.cpp -std=c++17 -DLOGGING -o minimal_automaton

./minimal_automaton < input.txt

rm minimal_automaton
