g++ ../src/main_determine.cpp -std=c++17 -o main

echo Determine tests:
for ((i = 1; i < 2; i++))
do
    ./main < determine/0$i.in > out
    diff determine/0$i.a out || break
    echo test №$i: OK
done

g++ ../src/main_minimal_automaton.cpp -std=c++17 -o main

echo Minimal automaton tests:
for ((i = 1; i < 2; i++))
do
    ./main < minimal_automaton/0$i.in > out
    diff minimal_automaton/0$i.a out || break
    echo test №$i: OK
done

rm main
rm out
