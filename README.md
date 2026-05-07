# Weighted Bundle Index Calculator

Кроссплатформенная программа для вычисления взвешенных бандельных индексов.

Поддерживает:

* Windows
* Linux / Ubuntu

Автоматически:

* конвертирует `.xlsx` <--> `.csv`
* вычисляет индексы
* сохраняет результат в `.xlsx`

---

# Требования

## Общие

* CMake >= 3.15
* C++ компилятор с поддержкой C++17
* OpenMP

## Для работы с Excel

**Windows:**

* Python + pandas + openpyxl
  или
* Microsoft Excel (через PowerShell)

**Linux:**

* LibreOffice
  или
* Python + pandas + openpyxl

---

# Сборка проекта

## Windows (MinGW + Ninja)

### 1. Установить:

* GCC (например через WinLibs)
* CMake
* Ninja

### 2. Добавить в PATH:

```
C:\...\gcc\bin
C:\...\ninja
C:\...\cmake\bin
```

### 3. Сборка:

```
mkdir build
cd build
cmake .. -G "Ninja"
cmake --build .
```

---

## Linux / Ubuntu

### 1. Установить зависимости:

```
sudo apt update
sudo apt install g++ cmake ninja-build libomp-dev
```

### 2. Сборка:

```
mkdir build
cd build
cmake ..
cmake --build .
```

---

# Запуск

Перейдите в папку `build`:

```
./wbi 3
```

или на Windows:

```
wbi.exe 3
```

где:

* `3` — параметр K

---

# Формат входных файлов

## quotas.xlsx

| Country | Quota |
| ------- | ----- |
| USA     | 100   |
| UK      | 50    |

---

## network.xlsx

| from | to | weight |
| ---- | -- | ------ |
| USA  | UK | 10     |

---

# Результат

Создаётся файл:

```
results.xlsx
```

С колонками:

* Country
* Weighted Bundle Index 1
* Weighted Bundle Index 2