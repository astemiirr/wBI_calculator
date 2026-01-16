#ifndef WBI_INDICES_H
#define WBI_INDICES_H

#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>

typedef long long ll;

// Класс для вычисления индексов wBI
class WbiCalculator {
private:
    // Переменные для вычислений
    std::vector<std::vector<ll>> in_weight;
    std::vector<std::vector<ll>> in_nodes;
    std::vector<ll> quota;
    std::vector<ll> in_edges_sum;
    std::vector<double> wBI_1;
    std::vector<double> wBI_2;
    
    // Векторы для работы со странами
    std::vector<std::string> country_names; // country_names[i] = название страны с индексом i
    std::map<std::string, ll> country_to_index; // country_to_index["Russia"] = индекс
    
    // Функция для вычисления условной суммы группы
    ll group_cond_sum(std::vector<ll>& group, ll node) {
        ll group_size = group.size();
        ll sum = 0;
        for (ll i = 0; i < group_size; ++i) {
            sum += in_weight[node][group[i]];
        }

        if (sum >= quota[node]) {
            return sum;
        }
        return 0;
    }
    
    // Рекурсивная функция для генерации комбинаций
    void combination(ll curr, ll count, std::vector<ll> v, ll node, ll option) {
        if (curr == count) {
            if (option == 1) {
                wBI_1[node] += double(group_cond_sum(v, node)) / in_edges_sum[node];
            }
            else {
                wBI_2[node] += double(group_cond_sum(v, node)) / TOTAL_SUM;
            }

            return;
        }

        ll in_count = in_nodes[node].size();

        for (ll i = 0; i < in_count; ++i) {
            bool flag = 1;
            for (ll j = 0; j < curr; ++j) {
                if (in_nodes[node][i] <= v[j]) {
                    flag = 0;
                }
            }
            if (flag) {
                v[curr] = in_nodes[node][i];
                combination(curr + 1, count, v, node, option);
            }
        }

        return;
    }
    
    // Функция для вычисления wBI_1 для одного узла
    void calculate_wBI_1(ll node) {
        for (ll i = 1; i <= K; ++i) {
            std::vector<ll> v(i, 0);
            combination(0, i, v, node, 1);
        }
    }
    
    // Функция для вычисления wBI_2 для одного узла
    void calculate_wBI_2(ll node) {
        for (ll i = 1; i <= K; ++i) {
            std::vector<ll> v(i, 0);
            combination(0, i, v, node, 2);
        }
    }
    
    // Функция для чтения списка стран из квот
    void read_countries_from_quotas_csv(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        
        country_names.clear();
        
        // // Читаем заголовок (пропускаем)
        // if (std::getline(file, line)) {
        //     // Пропускаем заголовок
        // }
        
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string country_name, quota_str;
            
            // Читаем два поля, разделенные запятой
            if (std::getline(ss, country_name, ',') && std::getline(ss, quota_str, ',')) {
                // Убираем возможные пробелы в начале и конце
                country_name.erase(0, country_name.find_first_not_of(" \t"));
                country_name.erase(country_name.find_last_not_of(" \t") + 1);
                
                // std::cout << "Country: '" << country_name << "'" << std::endl;
                country_names.push_back(country_name);
            }
        }
        
        file.close();
        
        // Сортируем страны лексикографически
        std::sort(country_names.begin(), country_names.end());
        
        // Создаем mapping: название страны -> индекс
        country_to_index.clear();
        for (ll i = 0; i < country_names.size(); ++i) {
            country_to_index[country_names[i]] = i;
        }
        
        N = country_names.size();
        std::cout << std::endl;
        std::cout << "Found " << N << " countries" << std::endl;
        std::cout << "-------------------------" << std::endl << std::endl;
    }
    
    // Функция для чтения рёбер из CSV
    void read_edges_csv(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        
        // Инициализируем структуры данных
        in_weight.assign(N, std::vector<ll>(N, 0));
        in_nodes.resize(N);
        
        // Читаем заголовок (пропускаем)
        if (std::getline(file, line)) {
            // Пропускаем заголовок
        }
        
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string from_country, to_country, weight_str;
            
            // Читаем три столбца: from_country, to_country, weight
            // Читаем три поля, разделенные ЗАПЯТОЙ
            if (std::getline(ss, from_country, ',') &&
                std::getline(ss, to_country, ',') &&
                std::getline(ss, weight_str, ',')) {
                
                // Убираем возможные пробелы
                from_country.erase(0, from_country.find_first_not_of(" \t"));
                from_country.erase(from_country.find_last_not_of(" \t") + 1);
                to_country.erase(0, to_country.find_first_not_of(" \t"));
                to_country.erase(to_country.find_last_not_of(" \t") + 1);
                weight_str.erase(0, weight_str.find_first_not_of(" \t"));
                weight_str.erase(weight_str.find_last_not_of(" \t") + 1);
                
                // Проверяем, что страны есть в нашем списке
                if (country_to_index.count(from_country) && country_to_index.count(to_country)) {
                    ll from_idx = country_to_index[from_country];
                    ll to_idx = country_to_index[to_country];
                    ll weight = std::stoll(weight_str);
                    
                    in_weight[to_idx][from_idx] = weight;
                    in_nodes[to_idx].push_back(from_idx);
                    
                    // std::cout << "Edge: " << from_country << "(" << from_idx << ") -> "
                    //     << to_country << "(" << to_idx << ") = " << weight << std::endl;
                }
                else {
                    // std::cout << "Warning: Unknown country in edge: " << from_country << " -> " << to_country << std::endl;
                }
            }
            else {
                std::cout << "Warning: Could not parse line: " << line << std::endl;
            }
        }
        
        std::cout << "-------------------------" << std::endl << std::endl;
        
        file.close();
    }
    
    // Функция для чтения квот из CSV
    void read_quotas_csv(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        
        quota.assign(N, 0);
        
        // // Читаем заголовок (пропускаем)
        // if (std::getline(file, line)) {
        //     std::cout << "Quotas header: " << line << std::endl;
        // }
        
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string country_name, quota_str;
            
            // Читаем два поля, разделенные ЗАПЯТОЙ
            if (std::getline(ss, country_name, ',') && std::getline(ss, quota_str, ',')) {
                // Убираем возможные пробелы
                country_name.erase(0, country_name.find_first_not_of(" \t"));
                country_name.erase(country_name.find_last_not_of(" \t") + 1);
                quota_str.erase(0, quota_str.find_first_not_of(" \t"));
                quota_str.erase(quota_str.find_last_not_of(" \t") + 1);
                
                if (country_to_index.count(country_name)) {
                    ll node_idx = country_to_index[country_name];
                    quota[node_idx] = std::stoll(quota_str);
                    // std::cout << "Quota: " << country_name << "(" << node_idx << ") = " << quota[node_idx] << std::endl;
                }
                else {
                    std::cout << "Warning: Unknown country in quotas: " << country_name << std::endl;
                }
            }
            else {
                std::cout << "Warning: Could not parse quotas line: " << line << std::endl;
            }
        }
        
        std::cout << "-------------------------" << std::endl << std::endl;
        
        file.close();
    }
    
    // Функция для вычисления сумм
    void calculate_sums() {
        TOTAL_SUM = 0;
        in_edges_sum.assign(N, 0);
        
        for (ll i = 0; i < N; ++i) {
            for (ll j = 0; j < N; ++j) {
                in_edges_sum[i] += in_weight[i][j];
                TOTAL_SUM += in_weight[i][j];
            }
        }
    }
    
    // Функция для записи результатов в CSV
    void write_results_csv(const std::string& filename) {
        std::ofstream file(filename);
        
        // Заголовки
        file << "Country,wBI_1,wBI_2\n";
        
        // Данные - используем исходные названия стран
        for (ll i = 0; i < N; ++i) {
            file << country_names[i] << ","
                << std::fixed << std::setprecision(6) << wBI_1[i] << ","
                << std::fixed << std::setprecision(6) << wBI_2[i] << "\n";
        }
        
        file.close();
    }
    
public:
    ll K;
    ll N;
    ll TOTAL_SUM;

    // Функция для выполнения вычислений
    void perform_calculations() {
        // Шаг 2.1: Читаем список стран и создаем mapping
        std::cout << "Reading countries from quotas.csv..." << std::endl << std::endl;
        read_countries_from_quotas_csv("quotas.csv");
        
        // Шаг 2.2: Читаем данные из CSV
        std::cout << "Reading edges from network.csv..." << std::endl;
        read_edges_csv("network.csv");
        
        std::cout << "Reading quotas from quotas.csv..." << std::endl;
        read_quotas_csv("quotas.csv");
        
        // Шаг 2.3: Вычисляем суммы
        std::cout << "Calculating sums..." << std::endl;
        calculate_sums();

        // Инициализируем результаты
        wBI_1.assign(N, 0.0);
        wBI_2.assign(N, 0.0);
        
        // Шаг 2.4 Параллельные вычисления
        auto start_time = std::chrono::high_resolution_clock::now();
        std::cout << "Calculating wBI_1..." << std::endl;
        #pragma omp parallel for
        for (ll i = 0; i < N; ++i) {
            // std::cout << "Thread " << omp_get_thread_num() << " processing vertex " << i << std::endl;
            // std::cout << "Calculating for " << country_names[i] << "..." << std::endl;
            calculate_wBI_1(i);
        }
        
        std::cout << "Calculating wBI_2..." << std::endl << std::endl;
        #pragma omp parallel for
        for (ll i = 0; i < N; ++i) {
            // std::cout << "Thread " << omp_get_thread_num() << " processing vertex " << i << std::endl;
            // std::cout << "Calculating for " << country_names[i] << "..." << std::endl;
            calculate_wBI_2(i);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << "PARALLEL CALCULATING:" << "\n";
        std::cout << "TIME: " << parallel_duration.count() << " ms" << "\n";
        std::cout << "\n";

        // Шаг 2.5: Записываем результаты в CSV
        std::cout << "Writing results to results.csv..." << std::endl;
        write_results_csv("results.csv");
    }
};

// Класс для работы с файлами
class FileProcessor {
private:
    // Проверка доступности команды в системе
    bool command_exists(const std::string& command) {
        #ifdef _WIN32
            std::string test_cmd = "where " + command + " > nul 2>&1";
        #else
            std::string test_cmd = "command -v " + command + " > /dev/null 2>&1";
        #endif
        return (system(test_cmd.c_str()) == 0);
    }
    
    // Проверка доступности Python
    bool python_available() {
        return command_exists("python") || command_exists("python3");
    }
    
    // Проверка доступности LibreOffice
    bool libreoffice_available() {
        return command_exists("libreoffice");
    }
    
    // Конвертация XLSX в CSV через Python (кросс-платформенная)
    bool convert_xlsx_to_csv_python(const std::string& xlsx_file) {
        std::string base_name = xlsx_file.substr(0, xlsx_file.find_last_of('.'));
        std::string csv_file = base_name + ".csv";
        
        // Пробуем сначала python, потом python3
        std::vector<std::string> python_cmds = {"python", "python3"};
        
        for (const auto& python_cmd : python_cmds) {
            if (command_exists(python_cmd)) {
                // Создаем временный Python файл с правильным синтаксисом
                std::string python_script = 
                    "import sys\n"
                    "try:\n"
                    "    import pandas as pd\n"
                    "    pd.read_excel('" + xlsx_file + "').to_csv('" + csv_file + "', index=False)\n"
                    "    sys.exit(0)\n"
                    "except ImportError:\n"
                    "    print('ERROR: pandas not installed. Install with: pip install pandas openpyxl')\n"
                    "    sys.exit(1)\n"
                    "except Exception as e:\n"
                    "    print('ERROR:', e)\n"
                    "    sys.exit(1)";
                
                // Записываем скрипт во временный файл
                std::ofstream script_file("temp_xlsx_to_csv.py");
                if (!script_file) return false;
                script_file << python_script;
                script_file.close();
                
                // Запускаем Python скрипт
                std::string command = python_cmd + " temp_xlsx_to_csv.py";
                int result = system(command.c_str());
                
                // Удаляем временный файл
                std::remove("temp_xlsx_to_csv.py");
                
                if (result == 0) {
                    return true;
                }
            }
        }
        return false;
    }
    
    // Конвертация CSV в XLSX через Python
    bool convert_csv_to_xlsx_python(const std::string& csv_file) {
        std::string base_name = csv_file.substr(0, csv_file.find_last_of('.'));
        std::string xlsx_file = base_name + ".xlsx";
        
        // Пробуем сначала python, потом python3
        std::vector<std::string> python_cmds = {"python", "python3"};
        
        for (const auto& python_cmd : python_cmds) {
            if (command_exists(python_cmd)) {
                // Создаем временный Python файл с правильным синтаксисом
                std::string python_script = 
                    "import sys\n"
                    "try:\n"
                    "    import pandas as pd\n"
                    "    pd.read_csv('" + csv_file + "').to_excel('" + xlsx_file + "', index=False)\n"
                    "    sys.exit(0)\n"
                    "except ImportError:\n"
                    "    print('ERROR: pandas not installed. Install with: pip install pandas openpyxl')\n"
                    "    sys.exit(1)\n"
                    "except Exception as e:\n"
                    "    print('ERROR:', e)\n"
                    "    sys.exit(1)";
                
                // Записываем скрипт во временный файл
                std::ofstream script_file("temp_csv_to_xlsx.py");
                if (!script_file) return false;
                script_file << python_script;
                script_file.close();
                
                // Запускаем Python скрипт
                std::string command = python_cmd + " temp_csv_to_xlsx.py";
                int result = system(command.c_str());
                
                // Удаляем временный файл
                std::remove("temp_csv_to_xlsx.py");
                
                if (result == 0) {
                    return true;
                }
            }
        }
        return false;
    }
    
    // Конвертация XLSX в CSV через LibreOffice
    bool convert_xlsx_to_csv_libreoffice(const std::string& xlsx_file) {
        std::string command = "libreoffice --headless --convert-to csv " + xlsx_file + " --outdir . > /dev/null 2>&1";
        int result = system(command.c_str());
        return (result == 0);
    }
    
    // Конвертация CSV в XLSX через LibreOffice
    bool convert_csv_to_xlsx_libreoffice(const std::string& csv_file) {
        std::string command = "libreoffice --headless --convert-to xlsx " + csv_file + " --outdir . > /dev/null 2>&1";
        int result = system(command.c_str());
        return (result == 0);
    }
    
public:
    // Функция для конвертации XLSX в CSV (кросс-платформенная)
    bool convert_xlsx_to_csv(const std::string& xlsx_file) {
        std::cout << "Converting " << xlsx_file << " to CSV..." << std::endl;
        
        #ifdef _WIN32
            // Windows: используем Python
            std::cout << "Windows detected, using Python method..." << std::endl;
            if (python_available()) {
                if (convert_xlsx_to_csv_python(xlsx_file)) {
                    std::cout << "Successfully converted using Python" << std::endl;
                    return true;
                }
            }
            
            std::cerr << "Error: Python not available for conversion!" << std::endl;
            std::cerr << "Please install Python with pandas: pip install pandas openpyxl" << std::endl;
            return false;
        #else
            // Linux/Unix: сначала пробуем LibreOffice, потом Python
            std::cout << "Linux/Unix detected, trying LibreOffice..." << std::endl;
            if (libreoffice_available()) {
                if (convert_xlsx_to_csv_libreoffice(xlsx_file)) {
                    std::cout << "Successfully converted using LibreOffice" << std::endl;
                    return true;
                }
            }
            
            std::cout << "LibreOffice failed or not available, trying Python..." << std::endl;
            if (python_available()) {
                if (convert_xlsx_to_csv_python(xlsx_file)) {
                    std::cout << "Successfully converted using Python" << std::endl;
                    return true;
                }
            }
            
            std::cerr << "Error: Neither LibreOffice nor Python available for conversion!" << std::endl;
            return false;
        #endif
    }
    
    // Функция для конвертации CSV в XLSX (кросс-платформенная)
    bool convert_csv_to_xlsx(const std::string& csv_file) {
        std::cout << "Converting " << csv_file << " to XLSX..." << std::endl;
        
        #ifdef _WIN32
            // Windows: используем Python
            std::cout << "Windows detected, using Python method..." << std::endl;
            if (python_available()) {
                if (convert_csv_to_xlsx_python(csv_file)) {
                    std::cout << "Successfully converted using Python" << std::endl;
                    return true;
                }
            }
            
            std::cerr << "Error: Python not available for conversion!" << std::endl;
            std::cerr << "Please install Python with pandas: pip install pandas openpyxl" << std::endl;
            return false;
        #else
            // Linux/Unix: сначала пробуем LibreOffice, потом Python
            std::cout << "Linux/Unix detected, trying LibreOffice..." << std::endl;
            if (libreoffice_available()) {
                if (convert_csv_to_xlsx_libreoffice(csv_file)) {
                    std::cout << "Successfully converted using LibreOffice" << std::endl;
                    return true;
                }
            }
            
            std::cout << "LibreOffice failed or not available, trying Python..." << std::endl;
            if (python_available()) {
                if (convert_csv_to_xlsx_python(csv_file)) {
                    std::cout << "Successfully converted using Python" << std::endl;
                    return true;
                }
            }
            
            std::cerr << "Error: Neither LibreOffice nor Python available for conversion!" << std::endl;
            return false;
        #endif
    }
    
    // Функция для удаления файлов
    void delete_file(const std::string& filename) {
        std::remove(filename.c_str());
    }
};

#endif