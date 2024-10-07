import math

# Уравнение f(x) = 0
def f(x):
    return x**2 - 10*math.sin(x)

# Производная для метода Ньютона
def df(x):
    return 2*x - 10*math.cos(x)

# Метод бисекции
def bisection(a, b, epsilon):
    if f(a) * f(b) > 0:
        print("Нет корней на данном интервале")
        return None, None
    
    iterations = 0
    while (b - a) / 2 > epsilon:
        c = (a + b) / 2
        if f(c) == 0:
            return c, iterations
        elif f(a) * f(c) < 0:
            b = c
        else:
            a = c
        iterations += 1
    return (a + b) / 2, iterations

# Метод Ньютона
def newton(x0, epsilon):
    iterations = 0
    x = x0
    while abs(f(x)) > epsilon:
        x = x - f(x) / df(x)
        iterations += 1
    return x, iterations

# Метод хорд
def secant(a, b, epsilon):
    iterations = 0
    while abs(b - a) > epsilon:
        a, b = b, b - f(b) * (b - a) / (f(b) - f(a))
        iterations += 1
    return b, iterations

# Метод простых итераций
def simple_iteration(x0, epsilon, max_iterations=1000):
    iterations = 0
    lambda_val = -1.0 / 10  # Подбираем подходящую лямбду для метода
    x = x0
    while abs(f(x)) > epsilon and iterations < max_iterations:
        x_new = x + lambda_val * f(x)
        if abs(x_new - x) < epsilon:
            return x_new, iterations
        x = x_new
        iterations += 1
    return x, iterations

# Основная программа
a = -1
b = 3
epsilon_values = [1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8]

for epsilon in epsilon_values:
    print(f"Точность: {epsilon}")
    
    # Бисекция
    root_bisection, iters_bisection = bisection(a, b, epsilon)
    if root_bisection is not None:
        print(f"Метод бисекции: Корень = {root_bisection}, Количество итераций = {iters_bisection}")
    else:
        print("Метод бисекции: корни не найдены на данном интервале.")
    
    # Ньютон
    try:
        root_newton, iters_newton = newton((a + b) / 2, epsilon)
        print(f"Метод Ньютона: Корень = {root_newton}, Количество итераций = {iters_newton}")
    except ZeroDivisionError:
        print("Метод Ньютона: ошибка деления на ноль.")
    
    # Хорд
    root_secant, iters_secant = secant(a, b, epsilon)
    print(f"Метод хорд: Корень = {root_secant}, Количество итераций = {iters_secant}")
    
    # Простые итерации
    root_simple_iter, iters_simple_iter = simple_iteration((a + b) / 2, epsilon)
    print(f"Метод простых итераций: Корень = {root_simple_iter}, Количество итераций = {iters_simple_iter}")
    print("-" * 50)
