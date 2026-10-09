[![Configure and build](https://github.com/alexen/using_boost/actions/workflows/build.yml/badge.svg)](https://github.com/alexen/using_boost/actions/workflows/build.yml)
[![Generate Doxygen](https://github.com/alexen/using_boost/actions/workflows/gendoc.yml/badge.svg)](https://github.com/alexen/using_boost/actions/workflows/gendoc.yml)

# using_boost

Учебно-демонстрационный C++-проект для изучения библиотек фреймворка Boost.

- **Язык:** C++17
- **Система сборки:** CMake (>= 3.15)
- **Boost:** 1.85 (скачивается через FetchContent из GitHub-релиза)
- **Лицензия:** MIT
- **Автор:** alexen (Enakaev Aleksey)

[Doxygen]-документация проекта опубликована на [GitHub Pages][mydoc].

---

## Содержание

- [Структура проекта](#структура-проекта)
- [Требования](#требования)
- [Сборка](#сборка)
- [CMake Presets](#cmake-presets)
- [Запуск](#запуск)
- [Тестирование](#тестирование)
- [Бенчмарки](#бенчмарки)
- [Пакетирование](#пакетирование)
- [Документация](#документация)
- [CI/CD](#cicd)
- [Компоненты](#компоненты)

---

## Структура проекта

```
using_boost/
├── CMakeLists.txt          # Корневой CMake-файл
├── CMakePresets.json        # Пресеты сборки (7 конфигураций)
├── Doxyfile                 # Конфигурация Doxygen
├── requirements.txt         # Системные зависимости (apt)
├── 3rdparty/                # Внешние зависимости (Celero)
│   └── CMakeLists.txt
├── cmake/                   # CMake-модули пакетирования
│   ├── MakePackage.cmake    # DEB-пакет (CPack)
│   ├── MakeSfx.cmake        # Self-extracting archive
│   ├── MakeSfxDeb.cmake     # SFX + DEB гибрид
│   └── MakeSrcTgz.cmake     # Архив исходников
├── package/                 # Скрипты пакетирования
│   ├── deb/                 # preinst/postinst шаблоны
│   └── sfxdeb/              # Установщик .deb.sh
└── src/                     # Исходные коды (24 компонента)
    ├── CMakeLists.txt       # Werror + подключение компонентов
    ├── algorithm/
    ├── beast/
    ├── core/                # Только тесты (глобальный test runner)
    ├── coroutine2/
    ├── datetime/
    ├── exception/
    ├── filesystem/
    ├── interprocess/
    ├── iostreams/           # Демо + тесты + бенчмарки
    ├── locale/              # Библиотека + тесты
    ├── log/                 # Демо + бенчмарки + модули
    ├── multiindex/
    ├── process/
    ├── program_options/
    ├── property_tree/
    ├── random/
    ├── range/
    ├── regex/
    ├── serialization/
    ├── stacktrace/
    ├── testing/             # Демонстрация API Boost.Test
    ├── thread/
    ├── tribool/
    └── variant/
```

Каждый каталог в `src/` — отдельный компонент, собираемый в исполняемый файл
(или библиотеку). Имя компонента совпадает с именем каталога и именем
исполняемого файла.

---

## Требования

### Системные пакеты

```bash
sudo apt-get install --yes $(cat requirements.txt)
```

Основные зависимости:
- Компилятор с поддержкой C++17 (GCC >= 7, Clang >= 5)
- CMake >= 3.15
- OpenSSL (разработка)
- `makeself` — для создания SFX-установщика
- `libbz2-dev`, `liblzma-dev`, `libzstd-dev` — для Boost.Iostreams

### Boost

Boost **не требуется** устанавливать в систему. Версия 1.85 скачивается
автоматически через `FetchContent` при конфигурации CMake.

Если нужна предустановленная система: `libboost-*-dev` пакеты перечислены
в `requirements.txt`, но проект всё равно использует свежий Boost из
исходников.

---

## Сборка

### Быстрый старт (через пресеты)

```bash
# Конфигурация
cmake --preset debug

# Сборка
cmake --build --preset debug

# Тесты
ctest --preset debug
```

### Без пресетов

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=On
cmake --build build --parallel "$(nproc)"
```

### Доступные опции CMake

| Опция | По умолч. | Описание |
|---|---|---|
| `CMAKE_BUILD_TYPE` | Debug | `Debug` или `Release` |
| `BUILD_SHARED_LIBS` | ON | Разделяемые (`ON`) или статические (`OFF`) библиотеки |
| `BUILD_BENCHMARKS` | OFF | Сборка бенчмарков на Celero |
| `BUILD_TESTING` | ON | Сборка тестов (CTest) |
| `CMAKE_INSTALL_PREFIX` | `/opt/using_boost` | Путь установки |

### Примеры сборки

```bash
# Debug, статические библиотеки
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=Off
cmake --build build --parallel "$(nproc)"

# Release с бенчмарками
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKS=On
cmake --build build --parallel "$(nproc)"

# Быстрая проверка (без тестов)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=Off
cmake --build build --parallel "$(nproc)"
```

### Компиляция отдельных компонентов

```bash
cmake --build build --target filesystem      # Только filesystem
cmake --build build --target filesystem-test # Только тест filesystem
cmake --build build --target iostreams-benchmark  # Бенчмарк iostreams
```

---

## CMake Presets

Проект включает файл `CMakePresets.json` для стандартизации сборок.

### Configure presets

| Пресет | Build type | Shared | Benchmarks | Tests | Компилятор | Каталог сборки |
|---|---|---|---|---|---|---|
| `debug` | Debug | ON | OFF | ON | системный | `build/debug` |
| `debug-static` | Debug | OFF | OFF | ON | системный | `build/debug-static` |
| `release` | Release | ON | OFF | ON | системный | `build/release` |
| `release-static` | Release | OFF | OFF | ON | системный | `build/release-static` |
| `bench` | Release | ON | ON | ON | системный | `build/bench` |
| `minimal` | Debug | ON | OFF | OFF | системный | `build/minimal` |
| `clang-debug` | Debug | ON | OFF | ON | Clang | `build/clang-debug` |

### Build presets

Маппинг на configure presets (одноимённые):

```bash
cmake --build --preset debug
cmake --build --preset release
cmake --build --preset bench         # Сборка с бенчмарками
```

### Test presets

```bash
ctest --preset debug    # 3 теста, с выводом лога упавших
ctest --preset release  # То же для Release
```

---

## Запуск

Все собранные исполняемые файлы находятся в `build/*/stage/bin/`:

```bash
# Сборка через пресет
cmake --preset debug && cmake --build --preset debug

# Запуск демо
./build/debug/stage/bin/filesystem
./build/debug/stage/bin/process
./build/debug/stage/bin/interprocess
./build/debug/stage/bin/log
./build/debug/stage/bin/beast
```

Некоторые компоненты принимают аргументы командной строки:

```bash
# program_options — демо разбора аргументов
./build/debug/stage/bin/program_options --help
./build/debug/stage/bin/program_options --implicit-value Hello --pos arg1 arg2

# stacktrace — генерация и печать стека вызовов
./build/debug/stage/bin/stacktrace
```

---

## Тестирование

```bash
# Все тесты
ctest --preset debug

# С указанием количества потоков
ctest --preset debug --parallel "$(nproc)"

# Только конкретный компонент
ctest -R core
ctest -R iostreams
ctest -R locale
```

Тесты есть в трёх компонентах:

| Компонент | Исполняемый файл | Описание |
|---|---|---|
| **core** | `core-test` | Глобальный `BOOST_TEST_MAIN` test runner |
| **iostreams** | `iostreams-test` | Юнит-тесты Base64 и фильтров |
| **locale** | `locale-test` | Тесты конвертации кодировок |

Отдельно: **testing** — исполняемый файл `testing`, который демонстрирует
различные возможности Boost.Test (data-driven тесты, фикстуры, таймауты,
шаблонные тесты и т.д.), но не подключён к CTest.

---

## Бенчмарки

Для сборки бенчмарков требуется флаг `-DBUILD_BENCHMARKS=ON` (или пресет `bench`).

```bash
cmake --preset bench
cmake --build --preset bench
```

Запуск:

```bash
# Бенчмарк iostreams
./build/bench/stage/bin/iostreams-benchmark

# Бенчмарк логгера
./build/bench/stage/bin/using_boost-benchmark
```

Бенчмарки используют библиотеку [Celero](https://github.com/DigitalInBlue/Celero)
(версия 2.8.5), которая скачивается через FetchContent.

---

## Пакетирование

### DEB-пакет

```bash
cmake --build build --target package
```

Результат: `build/<preset>/using_boost-1.1.0-Linux.deb`

### Self-extracting archive (SFX)

Требует установленного `makeself`:

```bash
cmake --build build --target sfx
```

Результат: `build/<preset>/using_boost-1.1.0-Linux.sh`

### SFX + DEB

```bash
cmake --build build --target sfxdeb
```

Результат: `build/<preset>/using_boost-1.1.0-Linux.deb.sh`

### Архив исходников

```bash
cmake --build build --target srctgz
```

Результат: `using_boost.tar.gz`

---

## Документация

Генерация Doxygen-документации:

```bash
cmake --build build --target doxygen
```

Результат: `build/<preset>/doc/html/index.html`

Опубликованная версия: [alexen.github.io/using_boost][mydoc]

---

## CI/CD

### build.yml

Матричная сборка на `ubuntu-24.04`:

| Build type | Shared libs |
|---|---|
| Debug | ON |
| Debug | OFF |
| Release | ON |
| Release | OFF |

Этапы:
1. Установка системных пакетов
2. Конфигурация CMake
3. Сборка
4. Прогон тестов (CTest)

### gendoc.yml

Генерация и публикация Doxygen-документации на GitHub Pages при каждом push.

---

## Компоненты

| Компонент | Boost библиотеки | Тесты | Бенчмарки | Описание |
|---|---|---|---|---|
| algorithm | (header-only) | — | — | Boost.Algorithm |
| beast | Boost.Beast, Asio | — | — | HTTP/WebSocket (Boost.Beast + OpenSSL) |
| core | unit_test_framework | ✓ | — | Глобальный test runner |
| coroutine2 | Coroutine, Context | — | — | Асинхронные корутины |
| datetime | Date_Time | — | — | Работа с датой/временем |
| exception | (header-only) | — | — | Boost.Exception, диагностика |
| filesystem | Filesystem | — | — | Работа с файловой системой |
| interprocess | Thread | — | — | Разделяемая память (Interprocess) |
| iostreams | Iostreams | ✓ | ✓ | Фильтры потоков, Base64 |
| locale | Locale | ✓ | — | Локализация, кодировки |
| log | Log, Log_setup | — | ✓ | Логирование, модули |
| multiindex | (header-only) | — | — | Multi-index контейнеры |
| process | Filesystem | — | — | Запуск процессов |
| program_options | Program_options | — | — | Разбор CLI-аргументов |
| property_tree | (header-only) | — | — | Boost.PropertyTree |
| random | Random | — | — | Генерация случайных чисел |
| range | (header-only) | — | — | Range-based алгоритмы |
| regex | Regex | — | — | Регулярные выражения |
| serialization | Serialization, Date_Time, Iostreams | — | — | Сериализация |
| stacktrace | Stacktrace | — | — | Стектрейсы |
| testing | unit_test_framework | — | — | Демо API Boost.Test (11 тестов) |
| thread | Thread, Log | — | — | Многопоточность |
| tribool | (header-only) | — | — | Троичная логика |
| variant | (header-only) | — | — | Type-safe union |

---

## Зависимости (внутренние)

```
log (исполняемый файл)
  ├── logger (статическая библиотека) → Boost::log, Boost::log_setup
  ├── dynlib (динамическая библиотека)
  ├── alice (динамическая библиотека) → Boost::log
  └── clark (динамическая библиотека) → Boost::log

stacktrace (исполняемый файл)
  └── funcs (статическая библиотека) → Boost::stacktrace_basic

exception (исполняемый файл)
  └── tools (статическая библиотека)

locale (библиотека) → Boost::locale, ICU
  └── locale-test → + Boost::unit_test_framework
```

---

## Ссылки

- [Документация Boost 1.85](https://www.boost.org/doc/libs/1_85_0/)
- [Документация Celero](https://github.com/DigitalInBlue/Celero)
- [Doxygen][doxygen]
- [GitHub Pages проекта][mydoc]
- [Markdown guide][md-basic], [Extended syntax][md-ext]

[doxygen]: https://www.doxygen.nl/index.html
[mydoc]: https://alexen.github.io/using_boost
[md-basic]: https://www.markdownguide.org/basic-syntax/
[md-ext]: https://www.markdownguide.org/extended-syntax/