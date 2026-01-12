====================================================================
Weighted Bundle Index Calculator
====================================================================

Программа для вычисления взвешенных бундельных индексов.
Автоматически конвертирует XLSX файлы в CSV, вычисляет индексы,
сохраняет результаты в XLSX.

====================================================================
Раздел для Ubuntu/Linux
====================================================================

Быстрый запуск (уже откомпилированный main):
Требования: LibreOffice ИЛИ Python с pandas
1. Установите зависимости:
   - Вариант A: sudo apt install libreoffice
   - Вариант B: sudo apt install python3 python3-pip; pip3 install pandas openpyxl
2. Поместите main в папку с quotas.xlsx и network.xlsx
3. Дайте права: chmod +x main
4. Запустите: ./main
5. Результат в results.xlsx

Компиляция и запуск (если нужно скомпилировать у себя):
Требования: g++, OpenMP, LibreOffice/Python
1. Установите: sudo apt install g++ libomp-dev
2. Скомпилируйте: g++ -fopenmp -O3 main.cpp -o main
3. Установите LibreOffice или Python с pandas
4. Запустите: ./main

Возможные проблемы и советы:
- "Error converting...": установите LibreOffice или Python с pandas
- "Permission denied": chmod +x main
- Нет XLSX файлов: нужны quotas.xlsx и network.xlsx

====================================================================
Раздел для Windows
====================================================================

Быстрый запуск (уже откомпилированный main.exe):
Требования: Python с pandas (ОБЯЗАТЕЛЬНО!)
1. Установите Python с python.org (отметьте "Add Python to PATH")
2. Установите pandas: pip install pandas openpyxl
3. Поместите main.exe в папку с quotas.xlsx и network.xlsx
4. Двойной клик по main.exe
5. Результат в results.xlsx

Компиляция и запуск (если нужно скомпилировать у себя):
Требования: MinGW-w64, Python с pandas
1. Установите MinGW-w64 (winlibs.com) и Python с pandas
2. Скомпилируйте: g++ -fopenmp -static -O3 main.cpp -o main.exe
3. Запустите: main.exe

Возможные проблемы и советы:
- "Python not available": установите Python и pandas
- Антивирус блокирует: разрешите запуск
- Ошибка DLL: используйте -static при компиляции

====================================================================
Формат входных файлов:
====================================================================

quotas.xlsx: таблица со столбцами "страна" и "квота" (НЕТ заголовка!)
network.xlsx: таблица со столбцами "от_страны", "к_стране", "вес" (ЕСТЬ заголовок!)

results.xlsx: таблица со столбцами:
- Country (страна)
- Weighted Bundle Index 1 (взвешенный бундельный индекс 1)
- Weighted Bundle Index 2 (взвешенный бундельный индекс 2)
