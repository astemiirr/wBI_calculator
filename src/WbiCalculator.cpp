#include "WbiCalculator.h"

// Функция для вычисления условной суммы группы
ll WbiCalculator::group_cond_sum(ll group_sum, ll node)
{
    // ll group_size = group.size();
    // ll sum = 0;
    // for (ll i = 0; i < group_size; ++i) {
    //     sum += in_weight[node][group[i]];
    // }

    if (group_sum >= quota[node])
    {
        return group_sum;
    }

    return 0;
}

// Рекурсивная функция для генерации комбинаций
void WbiCalculator::combination(ll curr, ll last_ind, ll count, ll group_sum, ll node, ll option)
{
    if (curr == count)
    {
        if (option == 1)
        {
            wBI_1[node] += double(group_cond_sum(group_sum, node)) / in_edges_sum[node];
            wBI_2[node] += double(group_cond_sum(group_sum, node)) / TOTAL_SUM;
        }
        else
        {
            wBI_1[node] += double(group_cond_sum(group_sum, node)) / in_edges_sum[node];
            wBI_2[node] += double(group_cond_sum(group_sum, node)) / TOTAL_SUM;
        }

        return;
    }

    ll in_count = in_nodes[node].size();
    if (count - curr > in_count - 1 - last_ind)
    {
        return;
    }

    for (ll i = last_ind + 1; i < in_count; ++i)
    {
        group_sum += in_weight[node][in_nodes[node][i]];
        combination(curr + 1, i, count, group_sum, node, option);
        group_sum -= in_weight[node][in_nodes[node][i]];
    }

    return;
}

// Функция для вычисления wBI_1 и wBI_2 для одного узла
void WbiCalculator::calculate_wBI(ll node)
{
    for (ll i = 1; i <= K; ++i)
    {
        ll group_sum = 0;
        combination(0, -1, i, group_sum, node, 1);
    }
}

// Функция для чтения списка стран из квот
void WbiCalculator::read_countries_from_quotas_csv(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    country_names.clear();

    // // Читаем заголовок (пропускаем)
    // if (std::getline(file, line)) {
    //     // Пропускаем заголовок
    // }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string country_name, quota_str;

        // Читаем два поля, разделенные запятой
        if (std::getline(ss, country_name, ';') && std::getline(ss, quota_str, ';'))
        {
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
    for (ll i = 0; i < country_names.size(); ++i)
    {
        country_to_index[country_names[i]] = i;
    }

    N = country_names.size();
    std::cout << std::endl;
    std::cout << "Found " << N << " countries" << std::endl;
    std::cout << "-------------------------" << std::endl
              << std::endl;
}

// Функция для чтения рёбер из CSV
void WbiCalculator::read_edges_csv(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    // Инициализируем структуры данных
    in_weight.assign(N, std::vector<ll>(N, 0));
    in_nodes.resize(N);

    // Читаем заголовок (пропускаем)
    if (std::getline(file, line))
    {
        // Пропускаем заголовок
    }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string from_country, to_country, weight_str;

        // Читаем три столбца: from_country, to_country, weight
        // Читаем три поля, разделенные ЗАПЯТОЙ
        if (std::getline(ss, from_country, ';') &&
            std::getline(ss, to_country, ';') &&
            std::getline(ss, weight_str, ';'))
        {

            // Убираем возможные пробелы
            from_country.erase(0, from_country.find_first_not_of(" \t"));
            from_country.erase(from_country.find_last_not_of(" \t") + 1);
            to_country.erase(0, to_country.find_first_not_of(" \t"));
            to_country.erase(to_country.find_last_not_of(" \t") + 1);
            weight_str.erase(0, weight_str.find_first_not_of(" \t"));
            weight_str.erase(weight_str.find_last_not_of(" \t") + 1);

            // Проверяем, что страны есть в нашем списке
            if (country_to_index.count(from_country) && country_to_index.count(to_country))
            {
                ll from_idx = country_to_index[from_country];
                ll to_idx = country_to_index[to_country];
                ll weight = std::stoll(weight_str);

                in_weight[to_idx][from_idx] = weight;
                in_nodes[to_idx].push_back(from_idx);

                // std::cout << "Edge: " << from_country << "(" << from_idx << ") -> "
                //     << to_country << "(" << to_idx << ") = " << weight << std::endl;
            }
            else
            {
                std::cout << "Warning: Unknown country in edge: " << from_country << " -> " << to_country << std::endl;
            }
        }
        else
        {
            std::cout << "Warning: Could not parse line: " << line << std::endl;
        }
    }

    // Нахождение узла с максимальным количеством входящих рёбер
    ll mx_in_nodes = 0;
    for (auto &el : in_nodes)
    {
        mx_in_nodes = fmax(mx_in_nodes, el.size()); // ll -> double
    }
    std::cout << std::endl
              << "Maximum number of incoming edges = " << mx_in_nodes << std::endl;
    // (Фрагмент кода для оценки времени вычисления)
    std::cout << "-------------------------" << std::endl
              << std::endl;

    file.close();
}

// Функция для чтения квот из CSV
void WbiCalculator::read_quotas_csv(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    quota.assign(N, 0);

    // // Читаем заголовок (пропускаем)
    // if (std::getline(file, line)) {
    //     std::cout << "Quotas header: " << line << std::endl;
    // }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string country_name, quota_str;

        // Читаем два поля, разделенные ЗАПЯТОЙ
        if (std::getline(ss, country_name, ';') && std::getline(ss, quota_str, ';'))
        {
            // Убираем возможные пробелы
            country_name.erase(0, country_name.find_first_not_of(" \t"));
            country_name.erase(country_name.find_last_not_of(" \t") + 1);
            quota_str.erase(0, quota_str.find_first_not_of(" \t"));
            quota_str.erase(quota_str.find_last_not_of(" \t") + 1);

            if (country_to_index.count(country_name))
            {
                ll node_idx = country_to_index[country_name];
                quota[node_idx] = std::stoll(quota_str);
                // std::cout << "Quota: " << country_name << "(" << node_idx << ") = " << quota[node_idx] << std::endl;
            }
            else
            {
                std::cout << "Warning: Unknown country in quotas: " << country_name << std::endl;
            }
        }
        else
        {
            std::cout << "Warning: Could not parse quotas line: " << line << std::endl;
        }
    }

    std::cout << "-------------------------" << std::endl
              << std::endl;

    file.close();
}

// Функция для вычисления сумм
void WbiCalculator::calculate_sums()
{
    TOTAL_SUM = 0;
    in_edges_sum.assign(N, 0);

    for (ll i = 0; i < N; ++i)
    {
        for (ll j = 0; j < N; ++j)
        {
            in_edges_sum[i] += in_weight[i][j];
            TOTAL_SUM += in_weight[i][j];
        }
    }
}

// Функция для записи результатов в CSV
void WbiCalculator::write_results_csv(const std::string &filename)
{
    std::ofstream file(filename);

    // Заголовки
    file << "Country,wBI_1,wBI_2\n";

    // Данные - используем исходные названия стран
    for (ll i = 0; i < N; ++i)
    {
        file << country_names[i] << ";"
             << std::fixed << std::setprecision(6) << wBI_1[i] << ";"
             << std::fixed << std::setprecision(6) << wBI_2[i] << "\n";
    }

    file.close();
}

// Функция для выполнения вычислений
void WbiCalculator::perform_calculations()
{
    // Шаг 2.1: Читаем список стран и создаем mapping
    std::cout << "Reading countries from quotas.csv..." << std::endl
              << std::endl;
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
    std::cout << "Calculating wBI_1 and wBI_2...\n"
              << std::endl;
#pragma omp parallel for
    for (ll i = 0; i < N; ++i)
    {
        // std::cout << "Thread " << omp_get_thread_num() << " processing vertex " << i << std::endl;
        // std::cout << "Calculating for " << country_names[i] << "..." << std::endl;
        calculate_wBI(i);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "ПАРАЛЛЕЛЬНОЕ ВЫЧИСЛЕНИЕ:" << "\n";
    std::cout << "Время: " << parallel_duration.count() << " мс" << "\n";
    std::cout << "\n";

    // Шаг 2.5: Записываем результаты в CSV
    std::cout << "Writing results to results.csv..." << std::endl;
    write_results_csv("results.csv");
}