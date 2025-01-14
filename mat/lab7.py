# Описание задачи:
# Программа выполняет анализ данных при помощи различных методов регрессии (линейной, квадратичной и экспоненциальной).
# Она вычисляет коэффициенты каждого типа регрессии, оценивает точность моделей (сумма квадратов отклонений) 
# и определяет наилучшее приближение.

import numpy as np  # Импортируем библиотеку numpy для работы с массивами и решения систем линейных уравнений
import math  # Импортируем библиотеку math для выполнения математических операций

# Функция для вывода таблицы сравнения исходных и восстановленных значений
def print_table(x, y, calculate_y):
    # Печатаем заголовок таблицы
    print(f"{'x':<8}{'y (исх)':<15}{'y (восст)':<15}{'|y - восст|':<15}")
    print('-' * 53)  # Разделитель строки
    for i in range(len(x)):
        restored_y = calculate_y(i)  # Вычисляем восстановленное значение y
        error = abs(y[i] - restored_y)  # Считаем модуль разницы между исходным и восстановленным значением
        print(f"{x[i]:<8.2f}{y[i]:<15.4f}{restored_y:<15.4f}{error:<15.4f}")  # Печатаем строку таблицы

# Функция для вычисления коэффициентов линейной регрессии
def linear_regression(x, y):
    n = len(x)  # Количество точек данных
    sum_x = sum(x)  # Сумма всех x
    sum_y = sum(y)  # Сумма всех y
    sum_xy = sum(x[i] * y[i] for i in range(n))  # Сумма произведений x и y
    sum_x2 = sum(x[i] ** 2 for i in range(n))  # Сумма квадратов x

    # Вычисляем коэффициенты a и b линейного уравнения y = a * x + b
    a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x ** 2)  # Формула для a
    b = (sum_y - a * sum_x) / n  # Формула для b

    # Вычисляем сумму квадратов отклонений для оценки точности модели
    error = sum((a * x[i] + b - y[i]) ** 2 for i in range(n))
    return a, b, error  # Возвращаем коэффициенты и ошибку

# Функция для вычисления коэффициентов квадратичной регрессии
def quadratic_regression(x, y):
    n = len(x)  # Количество точек данных
    sum_x = sum(x)  # Сумма всех x
    sum_x2 = sum(x[i] ** 2 for i in range(n))  # Сумма квадратов x
    sum_x3 = sum(x[i] ** 3 for i in range(n))  # Сумма кубов x
    sum_x4 = sum(x[i] ** 4 for i in range(n))  # Сумма x в четвёртой степени
    sum_y = sum(y)  # Сумма всех y
    sum_xy = sum(x[i] * y[i] for i in range(n))  # Сумма произведений x и y
    sum_x2y = sum((x[i] ** 2) * y[i] for i in range(n))  # Сумма произведений x^2 и y

    # Составляем матрицу коэффициентов для решения системы уравнений
    matrix = np.array([
        [n, sum_x, sum_x2],
        [sum_x, sum_x2, sum_x3],
        [sum_x2, sum_x3, sum_x4]
    ])
    vector = np.array([sum_y, sum_xy, sum_x2y])  # Вектор правых частей

    coefficients = np.linalg.solve(matrix, vector)  # Решаем систему линейных уравнений
    a, b, c = coefficients  # Извлекаем коэффициенты a, b и c

    # Вычисляем сумму квадратов отклонений для оценки точности модели
    error = sum((a + b * x[i] + c * x[i] ** 2 - y[i]) ** 2 for i in range(n))
    return a, b, c, error  # Возвращаем коэффициенты и ошибку

# Функция для вычисления коэффициентов экспоненциальной регрессии
def exponential_regression(x, y):
    n = len(x)  # Количество точек данных
    log_y = [math.log(yi) for yi in y]  # Вычисляем натуральный логарифм каждого y

    sum_x = sum(x)  # Сумма всех x
    sum_log_y = sum(log_y)  # Сумма логарифмов y
    sum_x_log_y = sum(x[i] * log_y[i] for i in range(n))  # Сумма произведений x и log(y)
    sum_x2 = sum(x[i] ** 2 for i in range(n))  # Сумма квадратов x

    # Вычисляем коэффициенты b и ln(a) для экспоненциального уравнения y = a * e^(b * x)
    b = (n * sum_x_log_y - sum_x * sum_log_y) / (n * sum_x2 - sum_x ** 2)  # Формула для b
    log_a = (sum_log_y - b * sum_x) / n  # Формула для ln(a)
    a = math.exp(log_a)  # Вычисляем a через экспоненту

    # Вычисляем сумму квадратов отклонений для оценки точности модели
    error = sum((a * math.exp(b * x[i]) - y[i]) ** 2 for i in range(n))
    return a, b, error  # Возвращаем коэффициенты и ошибку


# Основная функция программы
def main():
    # Исходные данные (массивы значений x и y)
    x = [2, 2.3, 2.6, 2.9, 3.2, 3.5, 3.8, 4.1, 4.4, 4.7, 5]  # Данные по оси x
    y = [20.25, 26.639, 33.948, 42.169, 51.298, 61.332, 72.269, 84.109, 96.852, 110.495, 125.04]  # Данные по оси y

    # Линейная регрессия
    linear = linear_regression(x, y)  # Вызываем функцию линейной регрессии
    print("Линейная регрессия:")
    print(f"Коэффициенты: a = {linear[0]:.4f}, b = {linear[1]:.4f}")  # Печатаем коэффициенты
    print(f"Уравнение: y = {linear[0]:.4f} * x + {linear[1]:.4f}")  # Печатаем уравнение
    print_table(x, y, lambda i: linear[0] * x[i] + linear[1])  # Выводим таблицу значений

    # Квадратичная регрессия
    quadratic = quadratic_regression(x, y)  # Вызываем функцию квадратичной регрессии
    print("\nКвадратичная регрессия:")
    print(f"Коэффициенты: a = {quadratic[0]:.4f}, b = {quadratic[1]:.4f}, c = {quadratic[2]:.4f}")  # Печатаем коэффициенты
    print(f"Уравнение: y = {quadratic[0]:.4f} + {quadratic[1]:.4f} * x + {quadratic[2]:.4f} * x^2")  # Печатаем уравнение
    print_table(x, y, lambda i: quadratic[0] + quadratic[1] * x[i] + quadratic[2] * x[i] ** 2)  # Выводим таблицу значений

    # Экспоненциальная регрессия
    exponential = exponential_regression(x, y)  # Вызываем функцию экспоненциальной регрессии
    print("\nЭкспоненциальная регрессия:")
    print(f"Коэффициенты: a = {exponential[0]:.4f}, b = {exponential[1]:.4f}")  # Печатаем коэффициенты
    print(f"Уравнение: y = {exponential[0]:.4f} * e^({exponential[1]:.4f} * x)")  # Печатаем уравнение
    print_table(x, y, lambda i: exponential[0] * math.exp(exponential[1] * x[i]))  # Выводим таблицу значений

    # Сравнение моделей
    print("\nСравнение моделей:")
    print(f"Линейная регрессия: сумма квадратов отклонений = {linear[2]:.4f}")  # Печатаем ошибку линейной модели
    print(f"Квадратичная регрессия: сумма квадратов отклонений = {quadratic[3]:.4f}")  # Печатаем ошибку квадратичной модели
    print(f"Экспоненциальная регрессия: сумма квадратов отклонений = {exponential[2]:.4f}")  # Печатаем ошибку экспоненциальной модели

    # Определение наилучшей модели
    print("\nНаилучшее приближение:")
    if linear[2] < quadratic[3] and linear[2] < exponential[2]:  # Сравнение ошибок моделей
        print("Линейная регрессия")
    elif quadratic[3] < exponential[2]:
        print("Квадратичная регрессия")
    else:
        print("Экспоненциальная регрессия")

if __name__ == "__main__":
    main()  # Запуск основной функции

# Описание методов регрессии и их принципов работы:

#     Линейная регрессия:
#         Цель: Найти линейное уравнение y=ax+by=ax+b, которое лучше всего описывает зависимость между переменными xx и yy.
#         Подход: Метод наименьших квадратов минимизирует сумму квадратов отклонений между реальными значениями yy и значениями, предсказанными моделью.
#         Применимость: Подходит, если данные примерно линейно зависимы.

#     Квадратичная регрессия:
#         Цель: Найти квадратичное уравнение y=ax2+bx+cy=ax2+bx+c, которое лучше всего описывает зависимость.
#         Подход: Решение системы уравнений с использованием матрицы коэффициентов, построенной на основе суммы степеней xx.      
#         Применимость: Используется, если зависимость имеет ярко выраженный нелинейный характер (например, параболический).

#     Экспоненциальная регрессия:
#         Цель: Найти уравнение вида y=acdotebcdotxy=acdotebcdotx, описывающее экспоненциальный рост или убывание.
#         Подход: Логарифмирование исходных данных преобразует экспоненциальное уравнение в линейное:
#             ln⁡(y)=ln⁡(a)+bcdotxln(y)=ln(a)+bcdotx
#         Применимость: Используется, если данные имеют экспоненциальный тренд.
# Общие моменты:

#     Все методы используют метод наименьших квадратов для минимизации суммы квадратов отклонений между реальными и предсказанными значениями.
#     Каждый метод имеет свою область применения:
#         Линейная регрессия — для линейных зависимостей.
#         Квадратичная регрессия — для зависимостей с параболическим характером.
#         Экспоненциальная регрессия — для экспоненциального роста или убывания.

# Сравнение:

#     Линейная регрессия: Быстрая и простая, но может не учитывать сложные зависимости.
#     Квадратичная регрессия: Более точная при параболических зависимостях, но сложнее в вычислениях.
#     Экспоненциальная регрессия: Подходит для данных с экспоненциальным трендом, требует предварительного логарифмирования.