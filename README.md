# Tree Search (AVL)
---
## Требования

- CMake ≥ 3.16
- Компилятор с поддержкой C++17 (g++/clang++)
- Интернет для загрузки GoogleTest (FetchContent)

---

## Структура проекта

```
.
├─ CMakeLists.txt
├─ include/
│  └─ Trees/
│     └─ Tree.hpp              # шаблонный класс AVL-дерева
├─ src/
│  ├─ runner.hpp               # int launcher(std::istream&, std::ostream&)
│  ├─ runner.cpp               # реализация протокола k / q поверх дерева
│  └─ main.cpp                 # int main(){ return launcher(cin, cout); }
├─ tests/
│  ├─ CMakeLists.txt
│  ├─ e2e_runner.cpp           # e2e-раннер: ищет пары .in/.out и сравнивает
│  ├─ unit/
│  │  └─ tree_test.cpp         # GoogleTest-юниты
│  └─ e2e/
│     ├─ in/                   # входные .in
│     └─ out/                  # ожидаемые .out
└─ build/                      # генерируется CMake (в .gitignore)
```


---

## Сборка

```bash
rm -rf build
cmake -S . -B build
cmake --build build -j
```

После сборки будут бинарники:

- приложение: `./build/task`
- юнит-тесты: `./build/tests/unit_tests`
- e2e-раннер: `./build/tests/e2e`

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
- `q A B` — вывести количество ключей в полуинтервале **(A, B]**

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
  2
  0
  ```

---

## Полезные команды

```bash
# пересобрать после правок
cmake --build build -j

# только юниты
./build/tests/unit_tests

# только e2e
./build/tests/e2e
```

---

## Проблемы

- **`No such file or directory [tests/e2e/in]`**
  Запускайте раннер из корня (`./build/tests/e2e`).
---

