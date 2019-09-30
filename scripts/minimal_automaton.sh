#!/bin/sh

g++ ../src/main_minimal_automaton.cpp -std=c++17 -DLOGGING -o minimal_automaton

./minimal_automaton < input.txt

rm minimal_automaton
