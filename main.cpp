#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <stdexcept>

// Структура параметров сигнала
struct SignalParams {
    double U;    // Амплитуда
    double f0;   // Частота
    double phi;  // Фазовый сдвиг
    double C;    // Постоянная составляющая
};

// Типы сигналов
enum class SignalType { Sine, Square, Triangle };

// Форма сигнала
double g(double x, SignalType type) {
    switch (type) {
        case SignalType::Sine:
            return std::sin(x);
        case SignalType::Square:
            return (std::sin(x) > 0 ? 1.0 : -1.0);
        case SignalType::Triangle:
            return 2.0 / M_PI * std::asin(std::sin(x));
        default:
            return 0.0;
    }
}

// Полный сигнал
double f(double t, const SignalParams& params, SignalType type) {
    return params.U * g(2 * M_PI * params.f0 * t + params.phi, type) + params.C;
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
    int choice;
    std::cout << "Выберите форму сигнала:\n";
    std::cout << "1 - Синусоидальный\n";
    std::cout << "2 - Прямоугольный\n";
    std::cout << "3 - Треугольный\n";
    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            return SignalType::Sine;
        case 2:
            return SignalType::Square;
        case 3:
            return SignalType::Triangle;
        default:
            std::cout << "Неверный выбор, установлен сигнал по умолчанию (синусоидальный).\n";
            return SignalType::Sine;
    }
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
            throw std::runtime_error("Выход из программы.");
        }

        // Замена запятой на точку
        for (auto& ch : input) {
            if (ch == ',') {
                ch = '.';
            }
        }

        // Попытка преобразовать строку в число
        try {
            value = std::stod(input);
            break;
        } catch (const std::invalid_argument&) {
            std::cerr << "Ошибка: Введите корректное число!\n";
        } catch (const std::out_of_range&) {
            std::cerr << "Ошибка: Число вне допустимого диапазона!\n";
        }
    }

    return value;
}

int main() {
    try {
        while (true) { // Бесконечный цикл для повтора работы программы
            SignalParams params;
            params.U = safe_input("Введите амплитуду сигнала (U): ");
            params.f0 = safe_input("Введите частоту сигнала (f0): ");
            params.phi = safe_input("Введите фазовый сдвиг (phi): ");
            params.C = safe_input("Введите напряжение смещения (C): ");

            // Выбор типа сигнала
            SignalType type = select_signal_type();

            // Расчёт периода сигнала
            if (params.f0 <= 0) {
                std::cerr << "Ошибка: частота должна быть больше 0.\n";
                continue; // Вернуться к началу цикла
            }
            double T = 1.0 / params.f0;

            // Расчёт среднего значения
            double avg = calculate_avg(params, type, T);

            // Вывод результата
            std::cout << "Среднее значение сигнала: " << avg << " В\n\n";
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}