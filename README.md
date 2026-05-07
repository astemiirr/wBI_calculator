# Weighted Bundle Index Calculator

## Что это за программа

Weighted Bundle Index Calculator — программа для вычисления взвешенных бандельных индексов графа.

Проект состоит из двух частей:

- **C++** — вычислительное ядро, считает индексы по CSV-файлам.
- **Python UI** — графическая оболочка, которая выбирает файлы, конвертирует `.xlsx` в `.csv`, запускает C++ и сохраняет результат обратно в `.xlsx`.

---

## Требования

Для сборки C++:

- CMake >= 3.15
- C++17
- OpenMP
- компилятор C++: MinGW / MSVC / g++
- Ninja или другой генератор сборки

Для Python UI:

- Python >= 3.10
- openpyxl
- pandas
- pyinstaller

Установка Python-зависимостей:

```bash
cd ui
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
```

---

## Как собрать C++ часть

### Windows

```bash
cd cpp
mkdir build
cd build
cmake .. -G "Ninja"
cmake --build .
```

После сборки скопируйте `wbi_cpp.exe` в папку:

```text
ui/bin/wbi_cpp.exe
```

---

## Как запустить из исходников

```bash
cd ui
.venv\Scripts\activate
python main.py
```

В окне программы нужно выбрать:

- C++ executable;
- `quotas.xlsx`;
- `network.xlsx`;
- путь для сохранения результата + имя сохраняемого файла;
- параметр `K`;
- режим вычисления;
- меру центральности.

---

## Как собрать релиз

Из папки `ui`:

```bash
pyinstaller --noconsole --onedir --name WBI-UI --add-binary "bin\wbi_cpp.exe;bin" main.py
```

Готовая программа появится в папке:

```text
ui/dist/WBI-UI/
```

---

## Возможности

Программа умеет:

- загружать два входных файла:
  - `quotas.xlsx`;
  - `network.xlsx`;
- автоматически конвертировать `.xlsx` в `.csv`;
- запускать C++ вычисления;
- получать результат в `.csv`;
- сохранять итоговый результат в `.xlsx`;
- удалять временные `.csv` файлы;
- выбирать режим вычисления:
  - `linear`;
  - `parallel`;
  - `compare`;
- выбирать меру центральности:
  - `wbi1`;
  - `wbi2`;
  - `both`.

---

## Формат входных файлов

### `quotas.xlsx`

| Country | Quota |
|---|---|
| USA | 100 |
| UK | 50 |

### `network.xlsx`

| from | to | weight |
|---|---|---|
| USA | UK | 10 |
| UK | Germany | 15 |

---

## Результат

На выходе создаётся `.xlsx` файл с результатами вычислений.

Пример колонок:

```text
Country
wBI_1
wBI_2
```