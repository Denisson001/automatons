# Formal langs - Automaton scripts

# Project

- Исходники лежат в директории `src`, тесты в `tests`.
- В директории `scripts` лежат два скрипта:
  - `determine.sh`
    - На вход получает НКА с однобуквенными переходами. На выходе выдает ДКА.
  - `minimal_automaton.sh`
    - На вход получает ПДКА. На выходе выдает МПДКА.

- Оба скрипта читают из файла `input.txt` в той же директории.

# Usage example

- Положить в input.txt описание автомата. Формат можно посмотреть в тестах или в `src/main_*.cpp`- Запустить скрипт

```
cd scripts
bash determine.sh
```

# Tests

- Запустить тесты

```
cd tests
bash run_tests.sh
```

