#  Tree Search (AVL)

Поисковое дерево с балансировкой (AVL) и реализация аналогичных функций через `std::set`
---

##  Требования

- **CMake ≥ 3.16**
- **C++17** (g++ или clang++)

---

##  Структура проекта

```
.
├─ CMakeLists.txt
├─ include/
│  └─ Trees/
│     └─ Tree.hpp              # шаблонный класс AVL-дерева
├─ src/
│  ├─ runner.hpp
│  ├─ runner.cpp               # раннер для дерева (парсер k/q, вызов Tree)
│  ├─ func_tree.cpp            # main для функционального режима дерева (печатает ответы)
│  ├─ bench_tree.cpp           # main для бенча дерева (печатает только время)
│  ├─ runner_set.hpp
│  ├─ runner_set.cpp           # раннер для std::set
│  ├─ func_set.cpp             # main для функционального режима std::set
│  └─ bench_set.cpp            # main для бенча std::set
├─ tests/
│  ├─ CMakeLists.txt
│  ├─ e2e_runner.cpp           # e2e-раннер: находит .in/.out и сравнивает
│  ├─ unit/
│  │  └─ tree_test.cpp         # GoogleTest-юниты
│  └─ e2e/
│     ├─ in/                   # входные .in
│     └─ out/                  # ожидаемые .out
└─ build/                      # создаётся CMake (в .gitignore)
```

---

## Установка
```
git clone git@github.com:treet144pi/tree_search.git
```
---
##  Сборка проекта

```bash
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

После сборки появятся бинарники:

| Бинарник                    | Назначение                                      |
|----------------------------|--------------------------------------------------|
| `./build/func_tree`        | функциональный режим для AVL-дерева             |
| `./build/bench_tree`       | бенчмарк дерева (печатает время)                |
| `./build/func_set`         | функциональный режим для `std::set`             |
| `./build/bench_set`        | бенчмарк `std::set` (печатает время)            |
| `./build/tests/unit_tests` | GoogleTest юниты                                |
| `./build/tests/e2e`        | e2e-раннер (.in/.out)                           |

---

##  Протокол команд (stdin)

- `k X` — вставить ключ `X` (дубликаты игнорируются)
- `q A B` — посчитать количество ключей в **диапазоне [A, B)**
  (если `B <= A`, результат `0`)

В функциональном режиме программа печатает ответы через пробел.
В бенч-режиме — только итоговое время выполнения в наносекундах.

---

##  Примеры запуска

### 1) Функциональный режим (дерево)
Интерактивно:
```bash
./build/func_tree
k 10
k 20
q 5 10
q 15 40
# Ctrl+D (Linux/macOS) или Ctrl+Z Enter (Windows) — завершить
```


###  Бенчмарк (время выполнения)
```bash
./build/bench_tree
./build/bench_set
```

Вывод:
```
42 ns
```

---

##  Юнит-тесты (GoogleTest)

```bash
#зайти в build
cd build
ctest --test-dir build -R unit -V
# или напрямую:
./build/tests/unit_tests
# фильтр:
./build/tests/unit_tests --gtest_filter=Bounds.*
```

---

##  E2E-тесты

Раннер автоматически находит пары `.in/.out` и сверяет вывод:

```
tests/e2e/in/1.in   ↔   tests/e2e/out/1.out
```

Запуск:
```bash
./build/tests/e2e
```

Пример вывода:
```
[CASE] 1.in : OK
Summary: 5 / 5 passed
```

### Как добавить новый e2e-кейс

1. Создайте вход: `tests/e2e/in/N.in`
2. Создайте ожидаемый вывод: `tests/e2e/out/N.out`
3. Запустите раннер:
   ```bash
   ./build/tests/e2e
   ```

Пример `.in`:
```
k 10
k 20
q 5 10
q 20 40
```

Пример `.out`:
```
1 1
```

---

## Производительность (benchmark)

- Время измеряется только на выполнении команд
- Используется `std::chrono::steady_clock`


---

## Ссылки
- [GoogleTest](https://github.com/google/googletest)
---

##  Полезные команды

Полная пересборка:
```bash
rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
```

Запуск юнитов:
```bash
./build/tests/unit_tests
```

Запуск e2e:
```bash
./build/tests/e2e
```

Сравнение производительности:
```bash
./build/bench_tree
./build/bench_set
```

---
```
