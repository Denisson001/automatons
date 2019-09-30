# Formal langs - Automaton scripts

# Project

- Исходники лежат в директории `src`, тесты в `tests`.
- В директории `scripts` лежат три скрипта:
  - `determine.sh`
    - На вход получает НКА с однобуквенными переходами. На выходе выдает ДКА.
  - `minimal_automaton.sh`
    - На вход получает ПДКА. На выходе выдает МПДКА.
  - `search_distinguishing_word.sh`
    - На вход получает два ДКА. Пытается найти различающее слово небольшой длины.

- Оба скрипта читают из файла `input.txt` в той же директории.

# Usage example

- Положить в input.txt описание автомата. Формат можно посмотреть в тестах или в `src/utils/read_automaton.cpp`
- Запустить скрипт

```
cd scripts
bash determine.sh
```

- Пример вывода скрипта `determine.sh`
  - Сначала лог работы алгоритма
    - Переходы между подмножествами состояний
    - Завершающие состояния в новом автомате
  - Затем итоговый автомат

```
Finish : (1)
Edge   : (0) (1) 0
Edge   : (1) (2) 0
Edge   : (1) (1) 1
Finish : (3,4)
Edge   : (2) (3,4) 0
Edge   : (3,4) (2,5) 0
Finish : (1,2)
Edge   : (3,4) (1,2) 1
Finish : (2,3,4)
Edge   : (1,2) (2,3,4) 0
Edge   : (1,2) (1) 1
Finish : (2,3,4,5)
Edge   : (2,3,4) (2,3,4,5) 0
Edge   : (2,3,4) (1,2) 1
Edge   : (2,3,4,5) (2,3,4,5) 0
Finish : (1,2,4)
Edge   : (2,3,4,5) (1,2,4) 1
Edge   : (1,2,4) (2,3,4,5) 0
Edge   : (1,2,4) (1) 1
Edge   : (2,5) (3,4) 0
Finish : (4)
Edge   : (2,5) (4) 1
Edge   : (4) (2,5) 0
Edge   : (4) (1) 1

Vertex count  : 10
Alphabet size : 2
Finishes      : 1 3 5 6 7 8 9
Edges
Vertex Finish Alpha :
0 1 0
1 2 0
1 1 1
2 3 0
3 4 0
3 5 1
4 3 0
4 9 1
5 6 0
5 1 1
6 7 0
6 5 1
7 7 0
7 8 1
8 7 0
8 1 1
9 4 0
9 1 1
```

- Пример вывода скрипта `minimal_automaton.sh`
  - Сначала классы эквивалентности на каждой итерации
    - Номер класса эквивалентности `->` номера классов эквивалентности, в которые ведут переходы по каждой из букв
  - Затем итоговый автомат

```
iter : 0
0 : 0 -> 1 0
1 : 1 -> 1 0
2 : 0 -> 1 0
3 : 1 -> 1 0
4 : 0 -> 1 1
5 : 0 -> 1 1
iter : 1
0 : 0 -> 2 0
1 : 2 -> 2 1
2 : 0 -> 2 1
3 : 2 -> 2 1
4 : 1 -> 2 2
5 : 1 -> 2 2
iter : 2
0 : 0 -> 3 1
1 : 3 -> 3 2
2 : 1 -> 3 2
3 : 3 -> 3 2
4 : 2 -> 3 3
5 : 2 -> 3 3

Vertex count  : 4
Alphabet size : 2
Finishes      : 3
Edges
Vertex Finish Alpha :
0 3 0
0 1 1
1 3 0
1 2 1
2 3 0
2 3 1
3 3 0
3 2 1
```

# Tests

- Запустить тесты

```
cd tests
bash run_tests.sh
```

