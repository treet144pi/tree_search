# Tree Search (AVL)
Требования

CMake ≥ 3.16

C++17 компилятор (g++/clang++)

Доступ к интернету для загрузки GoogleTest (FetchContent)

Структура проекта
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
│  ├─ unit/
│  │  └─ tree_test.cpp         # GoogleTest-юниты
│  └─ e2e/
│     ├─ in/                   # входные .in
│     └─ out/                  # ожидаемые .out
└─ build/                      # генерируется CMake (в .gitignore)

Сборка
rm -rf build
cmake -S . -B build
cmake --build build -j


После сборки появятся бинарники:

приложение: ./build/task

юнит-тесты: ./build/tests/unit_tests

e2e-раннер: ./build/tests/e2e (если цель e2e добавлена в tests/CMakeLists.txt)

Запуск приложения

Интерактивно:

./build/task
# затем ввод:
# k 10
# k 20
# q 5 10
# Ctrl+D (Linux/macOS) или Ctrl+Z Enter (Windows) — завершить ввод


С файлом:

./build/task < tests/e2e/in/1.in


Протокол команд:

k X — вставить ключ X (дубликаты игнорируются)

q A B — вывести количество ключей в полуинтервале (A, B]

Как добавить e2e-кейс

Создай вход: tests/e2e/in/N.in

Создай ожидаемый вывод: tests/e2e/out/N.out

Запусти раннер:

./build/tests/e2e


Вывод:

[CASE] N.in : OK
...
Summary: X / Y passed


Формат .in/.out

.in — команды можно писать построчно или в одну строку (оператор >> пропускает пробелы/переводы):

k 10
k 20
q 5 10
q 20 40


или

k 10 k 20 q 5 10 q 20 40


.out — ответы в том формате, который печатает runner.cpp. По умолчанию — по строкам:

1
2
0


(В поставляемом раннере включена «мягкая» нормализация: игнор различий CRLF/LF и лишних пробелов в конце.)
