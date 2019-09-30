#!/bin/sh

echo " ***** "
echo ! Некоторые тесты search_distinguishing_word работают долго из-за большого перебора !
echo " ***** "

dirs=('determine' 'minimal_automaton' 'search_distinguishing_word')
tests_cnt=(3 3 3)

for i in ${!dirs[@]}
do
    script=${dirs[$i]}

    g++ ../src/$script/main.cpp -std=c++17 -o main

    echo $script tests:
    for ((j = 1; j < ${tests_cnt[$i]} + 1; j++))
    do
        ./main < $script/0$j.in > out
        diff $script/0$j.a out || break
        echo test №$j: OK
    done

done

rm main
rm out
