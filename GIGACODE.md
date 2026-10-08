# Project Context: using_boost

## Overview

Учебно-демонстрационный C++-проект для изучения различных библиотек фреймворка Boost (версия 1.69).

- **Язык:** C++17
- **Система сборки:** CMake (>= 3.15)
- **Лицензия:** MIT
- **Автор:** alexen (Enakaev Aleksey)

### Используемые библиотеки Boost

Подключаются через `find_package(Boost 1.69 REQUIRED COMPONENTS ...)`:
`unit_test_framework`, `serialization`, `iostreams`, `date_time`, `coroutine`, `thread`, `locale`, `regex`, `filesystem`, `stacktrace_basic`, `log`, `log_setup`, `program_options`, `random`.

Дополнительно: **OpenSSL** (для `iostreams`/base64), **Celero** (бенчмарки, v2.8.5, скачивается через FetchContent).

## Архитектура

Монолитная структура: каждый каталог в `src/` — отдельная `add_subdirectory`, собирающая исполняемый файл (или библиотеку).

```
src/
├── core/         — базовые макросы/утилиты (использует Boost.Test)
├── serialization — сериализация (Boost.Serialization + Boost.DateTime + Boost.Iostreams)
├── iostreams/    — работа с потоками (Boost.Iostreams, base64)
├── coroutine2/   — корутины (Boost.Coroutine2)
├── property_tree/— дерево свойств (Boost.PropertyTree)
├── beast/        — HTTP/WebSocket (Boost.Beast)
├── locale/       — локализация (Boost.Locale)
├── testing/      — демонстрация возможностей Boost.Test
├── datetime/     — работа с датой/временем (Boost.DateTime)
├── regex/        — регулярные выражения (Boost.Regex)
├── variant/      — type-safe union (Boost.Variant)
├── range/        — range-based алгоритмы (Boost.Range)
├── multiindex/   — multi-index контейнеры (Boost.MultiIndex)
├── filesystem/   — работа с ФС (Boost.Filesystem)
├── tribool/      — троичная логика (Boost.Tribool)
├── stacktrace/   — стектрейсы (Boost.Stacktrace)
├── log/          — логирование (Boost.Log, собственная библиотека `logger`)
├── algorithm/    — алгоритмы (Boost.Algorithm)
├── exception/    — работа с исключениями (Boost.Exception)
├── process/      — запуск процессов (Boost.Process)
├── interprocess/ — межпроцессное взаимодействие (Boost.Interprocess)
├── program_options/ — разбор аргументов CLI (Boost.ProgramOptions)
└── random/       — генерация случайных чисел (Boost.Random)
```

### Зависимости

- **`src/log/logger/`** — статическая библиотека, обёртка над Boost.Log. Используется в `src/log/`.
- **`src/log/modules/dynlib/`** — подкачка динамических модулей логирования.
- **`3rdparty/`** — Celero (FetchContent) для бенчмарков.

## Сборка и запуск

### Зависимости для сборки

Устанавливаются через `requirements.txt`:
```bash
sudo apt-get install --yes $(cat requirements.txt)
```

### Конфигурация CMake

```bash
# Стандартная сборка
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=On

# Статическая линковка
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off

# Сборка с бенчмарками
cmake -B build -DBUILD_BENCHMARKS=On

# Без тестов
cmake -B build -DBUILD_TESTING=Off
```

### Сборка

```bash
cmake --build build --parallel $(nproc)
```

### Тестирование

```bash
cd build && ctest --parallel $(nproc)
```

Библиотека `Boost::unit_test_framework` используется в большинстве компонентов. Точка входа глобального test runner — `BOOST_TEST_MAIN` (в `core/test/core_test.cpp`). Отдельный test runner для демонстрации API Boost.Test — `src/testing/`.

### Документация

Doxygen:
```bash
cmake --build build --target doxygen
```

Опубликована на GitHub Pages: https://alexen.github.io/using_boost

### Пакетирование

- **DEB-пакет:** `make package`
- **SFX-установщик (makeself):** `cmake --build build --target sfx`
- **SFX + DEB:** `cmake --build build --target sfxdeb`
- **Архив с исходниками:** `cmake --build build --target srctgz`

## CI/CD (GitHub Actions)

Два workflow:
- **build.yml** — матричная сборка (Debug/Release × Shared/Static), прогон тестов на push/PR в master.
- **gendoc.yml** — генерация Doxygen-документации на каждый push, публикация в gh-pages.

## Разработка

### C++ Standard

Используется C++17. Компиляция со строгими флагами: `-Wall -Werror -Wpedantic -Wextra`.

### Отладка

Флаги отладки: `-ggdb3 -O0 -fno-inline`. RPATH установлен в `$ORIGIN/../lib` для подхвата общих библиотек.

### Тестирование

- Каждый компонент может иметь свой test target (имя `<component>-test`), подключаемый через `add_test()`.
- Единый test runner `core-test` с `BOOST_TEST_MAIN`.
- Отдельный демонстрационный target `testing` с набором тестов для изучения API Boost.Test.

### Конвенции

- **Файловая структура:** `src/<component>/` содержит `CMakeLists.txt`, и иногда `test/` и `benchmark/` подкаталоги.
- **Именование таргетов:** исполняемый файл = имя компонента, тестовый = `<component>-test`, бенчмарк = `<component>-benchmark`.
- **Исключения:** `3rdparty/` для внешних зависимостей, `cmake/` для модулей CMake, `package/` для скриптов пакетирования.

### Примечания

- CMake policy `CMP0167 OLD` — отключает новое поведение для `FetchContent`.
- `INSTALL_RPATH` настроен на `$ORIGIN/../lib` для корректной работы инсталлированных бинарников с общими библиотеками.
- Префикс установки по умолчанию: `/opt/using_boost` (меняется через `CMAKE_INSTALL_PREFIX`).