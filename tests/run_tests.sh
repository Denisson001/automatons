for script in 'determine' 'minimal_automaton' 'search_distinguishing_word'
do

    g++ ../src/$script/main.cpp -std=c++17 -o main

    echo $script tests:
    for ((i = 1; i < 2; i++))
    do
        ./main < $script/0$i.in > out
        diff $script/0$i.a out || break
        echo test №$i: OK
    done

done

rm main
rm out
