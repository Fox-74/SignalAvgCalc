#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include <functional>
#include <iomanip>

// Структура параметров сигнала
struct SignalParams {
    double U;    // Амплитуда
    double f0;   // Частота
    double phi;  // Фазовый сдвиг
    double C;    // Постоянная составляющая
};

// Типы сигналов
enum class SignalType { Sine, Square, Triangle };

// Карта сигналов для параметризации формы
const std::map<SignalType, std::function<double(double)>> signal_map = {
    {SignalType::Sine, [](double x) { return std::sin(x); }},
    {SignalType::Square, [](double x) { return (std::sin(x) > 0 ? 1.0 : -1.0); }},
    {SignalType::Triangle, [](double x) { return 2.0 / M_PI * std::asin(std::sin(x)); }}
};

// Полный сигнал
double f(double t, const SignalParams& params, SignalType type) {
    return params.U * signal_map.at(type)(2 * M_PI * params.f0 * t + params.phi) + params.C;
}

// Численный расчёт среднего значения
double calculate_avg(const SignalParams& params, SignalType type, double T, int N = 1000) {
    double dt = T / N;
    double sum = 0.0;

    for (int i = 0; i < N - 1; ++i) {
        double t1 = i * dt;
        double t2 = (i + 1) * dt;
        sum += (f(t1, params, type) + f(t2, params, type)) * 0.5 * dt;
    }

    return sum / T;
}

// Функция для выбора типа сигнала
SignalType select_signal_type() {
    const std::map<int, SignalType> signal_types = {
        {1, SignalType::Sine},
        {2, SignalType::Square},
        {3, SignalType::Triangle}
    };

    std::cout << "Выберите форму сигнала:\n";
    std::cout << "1 - Синусоидальный\n";
    std::cout << "2 - Прямоугольный\n";
    std::cout << "3 - Треугольный\n";
    int choice;
    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    if (signal_types.find(choice) != signal_types.end()) {
        return signal_types.at(choice);
    } else {
        std::cout << "Неверный выбор, установлен сигнал по умолчанию (синусоидальный).\n";
        return SignalType::Sine;
    }
}

// Функция нормализации ввода (замена запятой на точку)
std::string normalize_input(const std::string& input) {
    std::string normalized = input;
    for (auto& ch : normalized) {
        if (ch == ',') {
            ch = '.';
        }
    }
    return normalized;
}

// Функция для безопасного ввода чисел
double safe_input(const std::string& prompt) {
    std::string input;
    double value;

    while (true) {
        std::cout << prompt;
        std::cin >> input;

        // Проверка на команду выхода
        if (input == "exit") {
            std::cout << "Выход из программы.\n";
            exit(0);
        }

        try {
            value = std::stod(normalize_input(input));
            break;
        } catch (const std::invalid_argument&) {
            std::cerr << "Ошибка: Введите корректное число!\n";
        } catch (const std::out_of_range&) {
            std::cerr << "Ошибка: Число вне допустимого диапазона!\n";
        }
    }

    return value;
}

// Функция для получения параметров сигнала
SignalParams get_signal_params() {
    SignalParams params;
    params.U = safe_input("Введите амплитуду сигнала (U): ");
    params.f0 = safe_input("Введите частоту сигнала (f0): ");
    if (params.f0 <= 0) {
        throw std::runtime_error("Ошибка: частота должна быть больше 0.");
    }
    params.phi = safe_input("Введите фазовый сдвиг (phi): ");
    params.C = safe_input("Введите напряжение смещения (C): ");
    return params;
}

int main() {
    try {
        while (true) { // Бесконечный цикл для повтора работы программы
            SignalParams params = get_signal_params();

            // Выбор типа сигнала
            SignalType type = select_signal_type();

            // Расчёт периода сигнала
            double T = 1.0 / params.f0;

            // Расчёт среднего значения
            double avg = calculate_avg(params, type, T);

            // Вывод результата
            std::cout << std::fixed << std::setprecision(5);
            std::cout << "Среднее значение сигнала: " << avg << " В\n\n";
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}