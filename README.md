# Tree Search (AVL)


## Требования

- CMake >= 3.16
- C++17 (g++/clang++)

---

## Структура проекта
```
.
├─ CMakeLists.txt
├─ include/
│  └─ Trees/
│     └─ Tree.hpp              # шаблонный класс AVL-дерева
├─ src/
│  ├─ runner.hpp
│  ├─ runner.cpp               # протокол команд
│  └─ main.cpp                 # int main(){ return launcher(cin, cout); }
├─ tests/
│  ├─ CMakeLists.txt
│  ├─ e2e_runner.cpp           # e2e-раннер: находит .in/.out и сравнивает
│  ├─ unit/
│  │  └─ tree_test.cpp         # GoogleTest
│  ├─ perf/
│  │  └─ avl_bench.cpp         # Google Benchmark
│  └─ e2e/
│     ├─ in/                   # входные .in
│     └─ out/                  # ожидаемые .out
└─ build/
```

---
## Установка
  ```
git clone git@github.com:treet144pi/tree_search.git
  ```

---
## Сборка
  ```
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
  ```
После сборки появятся бинарники:
  ```
- приложение: ./build/task
- юнит-тесты: ./build/tests/unit_tests
- e2e-раннер: ./build/tests/e2e
- бенчмарки: ./build/tests/avl_bench
  ```

---

## Запуск приложения

Интерактивно:
```bash
./build/task
# затем ввод:
# k 10
# k 20
# q 5 10
# Ctrl+D (Linux/macOS) или Ctrl+Z Enter (Windows) — завершить
```

С файлом:
```bash
./build/task < tests/e2e/in/1.in
```

**Протокол команд:**
- `k X` — вставить ключ `X` (дубликаты игнорируются)
- `q A B` — вывести количество ключей


---
## Юнит-тесты (GoogleTest)


```bash
./build/tests/unit_tests
# фильтр по имени тестов:
./build/tests/unit_tests --gtest_filter=Bounds.*
```

---


## E2E-тесты (через раннер)

Раннер автоматически находит пары `tests/e2e/in/*.in` ↔ `tests/e2e/out/*.out`
(совпадающий stem: `1.in` ↔ `1.out`) и сверяет вывод.

Запуск (из корня проекта):
```bash
./build/tests/e2e
```

### Как добавить новый e2e-кейс

1. Создайте вход: `tests/e2e/in/N.in`
2. Создайте ожидаемый вывод: `tests/e2e/out/N.out`
3. Запустите раннер:
   ```bash
   ./build/tests/e2e
   ```
   Пример вывода:
   ```
   [CASE] N.in : OK
   ...
   Summary: X / Y passed
   ```

**Формат .in/.out**

- `.in` — команды можно писать построчно или в одну строку:
  ```
  k 10
  k 20
  q 5 10
  q 20 40
  ```
  или
  ```
  k 10 k 20 q 5 10 q 20 40
  ```

- `.out` — ответы в том же формате, что печатает программа (обычно построчно):
  ```
  1
  1
  ```

---
## Бенчмарки (Google Benchmark)

Запуск:
  ```
./build/tests/avl_bench
  ```
Что меряется
  ```
- Insert — вставка 10 000 и 100 000 случайных ключей в пустое дерево
- LowerBound b UpperBound — 10 000 и 100 000 запросов поиска
- RangeQuery — 10 000 и 100 000  диапазонных запросов [a, b)
  ```
---

## Частые проблемы

- No such file or directory [tests/e2e/in] — запускайте e2e-раннер из корня и проверьте наличие tests/e2e/in и tests/e2e/out

---

## Ссылки на используемые библиотеки
  ```
- Google Benchmark — https://github.com/google/benchmark
- GoogleTest —  https://github.com/google/googletest
  ```
---

## Полезные команды

Полная пересборка (Release)
rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j


Юниты
  ```
./build/tests/unit_tests
  ```
E2E
  ```
./build/tests/e2e
  ```
Бенчи (5 повторов, агрегаты)
  ```
./build/tests/avl_bench --benchmark_repetitions=5 --benchmark_report_aggregates_only=true
  ```
