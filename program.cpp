#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <QtCore/QString>
#include <chrono>
#include <atomic>
#include <tuple>
#include <numeric>
#include <string>

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}
template <class D>
inline long long to_us(const D &d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}
void readNumbers(std::vector<double> &numbers, std::ifstream &file)
{
    if (!file)
        std::cout << "Can not open such file\n";

    std::string value;
    while (file >> value)
        numbers.push_back(std::stod(value));
    file.close();
}
void writeRes(std::tuple<int, int> &result, std::ofstream &file)
{
    (file) << std::get<0>(result) << " " << std::setprecision(10) << (double)std::get<0>(result) / std::get<1>(result) << "\n";
    (file).close();
}
std::tuple<int, int> strStream(std::vector<double> &numbers)
{
    int sum = 0, count = 0;
    std::ostringstream ss;
    ss << std::setprecision(10);
    auto start = get_current_time_fenced();
    for (auto &x : numbers)
    {
        ss << x;
        count += 1;
    }
    auto end = get_current_time_fenced();
    std::cout << "Time of \"stringstream\" method is: " << to_us(end - start) << "mcs\n";
    sum += ss.str().size();

    return std::make_tuple(sum, count);
}
std::tuple<int, int> strToString(std::vector<double> &numbers)
{
    int count = 0, sum = 0;
    auto start = get_current_time_fenced();
    for (auto &x : numbers)
    {
        sum += std::to_string(x).size();
        count += 1;
    }
    auto end = get_current_time_fenced();
    std::cout << "Time of \"to_string\" method is: " << to_us(end - start) << "mcs\n";
    return std::make_tuple(sum, count);
}
std::tuple<int, int> strSprintf(std::vector<double> &numbers)
{
    int sum = 0, count = 0;
    char converted[50];
    auto start = get_current_time_fenced();
    for (auto &x : numbers)
    {
        sprintf(converted, "%lf", x);
        sum += strlen(converted);
        count += 1;
    }
    auto end = get_current_time_fenced();
    std::cout << "Time of \"sprintf\" method is: " << to_us(end - start) << "mcs\n";
    return std::make_tuple(sum, count);
}

int convert(double x, std::string &s)
{
    (x > 0) ? s = "" : s = "-";
    x = abs(x);
    size_t zil = (size_t)x;
    double frac = x - zil;
    size_t modulo;
    for (modulo = 10; zil % modulo != zil; modulo *= 10)
        ;
    modulo /= 10;
    for (; modulo >= 1; zil %= modulo, modulo /= 10)
        // {
        // std::cout << "Dim " << zil / modulo << "\n";
        // s.push_back((zil % modulo) + '0');
        s.push_back((zil / modulo) + '0');
    // }

    while (ceil(frac) != frac)
        frac *= 10;
    if (frac != 0)
        s.push_back('.');

    zil= (size_t)floor(frac);
    // std::cout << "Zil = " << am << "\n";
    for (modulo = 10; zil % modulo != zil; modulo *= 10)
        ;
    modulo /= 10;
    for (; modulo >= 1; zil %= modulo, modulo /= 10)
    {
        // s.push_back((am % modulo) + '0');
        // std::cout << "Smth " << am / modulo_t << "\n";
        s.push_back((zil / modulo) + '0');
    }

    // std::cout << s << "\n";

    return s.length();
}

std::tuple<int, int> strCustom(std::vector<double> &numbers)
{
    int sum = 0, count = 0;
    auto start = get_current_time_fenced();
    std::string s;

    for (auto x : numbers)
    {
        s = "";
        sum += convert(x, s);
        count += 1;
    }

    auto end = get_current_time_fenced();
    std::cout << "Time of \"custom\" method is: " << to_us(end - start) << "mcs\n";
    return std::make_tuple(sum, count);
}
std::tuple<int, int> strBoost(std::vector<double> &numbers)
{
    int sum = 0, count = 0;
    auto start = get_current_time_fenced();
    for (auto &x : numbers)
    {
        sum += boost::lexical_cast<std::string>(x).size();
        count += 1;
    }
    auto end = get_current_time_fenced();
    std::cout << "Time of \"lexical_cast\" method is: " << to_us(end - start) << "mcs\n";
    return std::make_tuple(sum, count);
}
std::tuple<int, int> strQString(std::vector<double> &numbers)
{
    int sum = 0, count = 0;
    auto start = get_current_time_fenced();
    for (auto &x : numbers)
    {
        sum += QString::number(x, 'g', 10).toStdString().size();
        count += 1;
    }
    auto end = get_current_time_fenced();
    std::cout << "Time of \"QString\" method is: " << to_us(end - start) << "mcs\n";
    return std::make_tuple(sum, count);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Invalid number of parameters\n";
        return 1;
    }

    std::vector<double> numbers;
    std::ofstream outputFile(argv[3], std::ios_base::app);
    std::ifstream inputFile(argv[2]);

    readNumbers(numbers, inputFile);
    std::tuple<int, int> result;

    typedef std::tuple<int, int> (*fn)(std::vector<double> &);
    fn funcs[] = {strStream, strToString, strSprintf, strCustom, strBoost, strQString};
    result = funcs[std::stoi(argv[1]) - 1](numbers);

    writeRes(result, outputFile);
    return 0;
}
