# Описание задачи:
# Программа вычисляет первую и вторую производные функции на заданном интервале
# с использованием численных методов, включая разности вперед, назад и симметричные разности.
# Программа также оценивает погрешности вычислений и сравнивает их с теоретически ожидаемыми.

import math

# Исходная функция
def function(x):
    return math.log(x + 1) + 1 / (x + 1)

# Первая производная функции
def first_derivative(x):
    return 1 / (x + 1) - 1 / (x + 1) ** 2

# Вторая производная функции
def second_derivative(x):
    return -1 / (x + 1) ** 2 + 2 / (x + 1) ** 3

# Третья производная функции
def third_derivative(x):
    return 2 / (x + 1) ** 3 - 6 / (x + 1) ** 4

# Четвертая производная функции
def fourth_derivative(x):
    return -6 / (x + 1) ** 4 + 24 / (x + 1) ** 5

# Нахождение максимального значения производной на интервале
def find_max_derivative(derivative, a, b, h):
    max_value = float('-inf')
    x = a
    while x <= b:
        value = abs(derivative(x))
        if value > max_value:
            max_value = value
        x += h
    return max_value

# Разности вперед для первой производной
def forward_difference(y, h):
    return [(y[i + 1] - y[i]) / h if i < len(y) - 1 else '-' for i in range(len(y))]

# Разности назад для первой производной
def backward_difference(y, h):
    return [(y[i] - y[i - 1]) / h if i > 0 else '-' for i in range(len(y))]

# Симметричные разности для первой производной
def central_difference(y, h):
    return [(y[i + 1] - y[i - 1]) / (2 * h) if 0 < i < len(y) - 1 else '-' for i in range(len(y))]

# Численный метод для второй производной
def second_derivative_numeric(y, h):
    return [(y[i - 1] - 2 * y[i] + y[i + 1]) / h ** 2 if 0 < i < len(y) - 1 else '-' for i in range(len(y))]

# Оценка максимальной разности между численным и реальным значением
def find_max_difference(calculated, real):
    differences = [abs(calculated[i] - real[i]) if calculated[i] != '-' else '-' for i in range(len(real))]
    return max([d for d in differences if d != '-'])

# Основная программа
def main():
    a, b = 0, 10  # Интервал вычислений
    h = 1  # Шаг

    x = [a + i * h for i in range(int((b - a) / h) + 1)]  # Массив значений x
    y = [function(xi) for xi in x]  # Значения функции
    y_real_1 = [first_derivative(xi) for xi in x]  # Реальные значения первой производной
    y_real_2 = [second_derivative(xi) for xi in x]  # Реальные значения второй производной

    # Численные методы для первой производной
    forward = forward_difference(y, h)
    backward = backward_difference(y, h)
    central = central_difference(y, h)

    # Численный метод для второй производной
    second_numeric = second_derivative_numeric(y, h)

    # Погрешности
    max_second = find_max_derivative(second_derivative, a, b, h)
    max_third = find_max_derivative(third_derivative, a, b, h)
    max_fourth = find_max_derivative(fourth_derivative, a, b, h)

    # Печать заголовка таблицы
    print(f"{'x':<8}{'y':<15}{'y (реальная)':<20}{'y (вперед)':<20}{'y (назад)':<20}{'y (центр)':<20}{'y'' (реальная)':<20}{'y'' (численная)':<20}")
    print('-' * 150)

    # Печать строк таблицы
    for i in range(len(x)):
        y_forward = forward[i] if forward[i] != '-' else '-'
        y_backward = backward[i] if backward[i] != '-' else '-'
        y_central = central[i] if central[i] != '-' else '-'
        y_second_numeric = second_numeric[i] if second_numeric[i] != '-' else '-'

        print(f"{x[i]:<8.2f}{y[i]:<15.4f}{y_real_1[i]:<20.4f}{y_forward:<20}{y_backward:<20}{y_central:<20}{y_real_2[i]:<20.4f}{y_second_numeric:<20}")

    print("\nМаксимальные погрешности:")
    print(f"Первая производная (вперед): {find_max_difference(forward, y_real_1):.4f}")
    print(f"Первая производная (назад): {find_max_difference(backward, y_real_1):.4f}")
    print(f"Первая производная (центр): {find_max_difference(central, y_real_1):.4f}")
    print(f"Вторая производная: {find_max_difference(second_numeric, y_real_2):.4f}")

    print("\nТеоретические погрешности:")
    print(f"Первая производная (вперед/назад): {max_second * h / 2:.4f}")
    print(f"Первая производная (центр): {max_third * h ** 2 / 6:.4f}")
    print(f"Вторая производная: {max_fourth * h ** 2 / 12:.4f}")

if __name__ == "__main__":
    main()

# Описание методов:
# 1. Разности вперед: Использует значения текущей и следующей точки для оценки производной. Подходит для первого узла.
# 2. Разности назад: Использует значения текущей и предыдущей точки. Применяется для последнего узла.
# 3. Симметричные разности: Учитывают значения соседних точек и обеспечивают более точное приближение.
# 4. Численное вычисление второй производной: Использует разности второго порядка для вычисления второй производной.
# 5. Максимальные производные: Оценивают максимальную величину производной для теоретического расчета погрешностей.
