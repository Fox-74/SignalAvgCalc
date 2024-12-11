#include <iostream>
#include <cmath>
#include <vector>
#include <functional>

// Параметры сигнала
double A = 1.0;     // Амплитуда
double f0 = 50.0;   // Частота сигнала (в Гц)
double phi = 0.0;   // Фазовый сдвиг (в радианах)
double C = 0.0;     // Постоянная составляющая
double T = 1.0 / f0; // Период сигнала

// Определение произвольной формы сигнала g(x)
double g(double x) {
    return std::sin(x); // Пример: синусоидальная форма
}

// Полный сигнал
double f(double t) {
    return A * g(2 * M_PI * f0 * t + phi) + C;
}

// Численный расчёт среднего значения
double calculate_avg(std::function<double(double)> signal, double T, int N = 1000) {
    double dt = T / N; // Шаг дискретизации
    double sum = 0.0;

    for (int i = 0; i < N; ++i) {
        double t = i * dt;
        sum += signal(t) * dt; // Интегрирование методом трапеций
    }

    return sum / T; // Усреднение
}

int main() {
    // Вычисление среднего значения
    double avg = calculate_avg(f, T);
    
    // Вывод результата
    std::cout << "Среднее значение сигнала: " << avg << " В" << std::endl;

    return 0;
}
