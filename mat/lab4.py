import numpy as np

def seidel_method(A, b, eps, max_iterations=1000):
    """
    Решение системы линейных алгебраических уравнений методом Зейделя.

    Параметры:
    A : ndarray
        Коэффициентная матрица
    b : ndarray
        Вектор свободных членов
    eps : float
        Заданная точность
    max_iterations : int
        Максимальное количество итераций

    Возвращает:
    x : ndarray
        Найденный вектор решения
    iterations : int
        Количество итераций
    """
    n = len(b)
    x = np.zeros(n)
    for iterations in range(max_iterations):
        x_new = np.copy(x)
        for i in range(n):
            sum1 = sum(A[i][j] * x_new[j] for j in range(i))
            sum2 = sum(A[i][j] * x[j] for j in range(i + 1, n))
            x_new[i] = (b[i] - sum1 - sum2) / A[i][i]
        
        # Проверка на точность
        if np.linalg.norm(x_new - x, ord=np.inf) < eps:
            return x_new, iterations + 1
        x = x_new
    
    raise ValueError("Метод Зейделя не сошелся за заданное количество итераций.")

# Исходные данные
A = np.array([
    [2000, 150, -500, -1000],
    [200, 3100, -1000, -1000],
    [650, 450, 1411, 440],
    [-1900, 50, 800, 1240]
])

b = np.array([9710, 21010, 7823.3, -7362.4])

# Значения точности
epsilons = [1e-2, 1e-3, 1e-4, 1e-5, 1e-6]

# Вычисления
for eps in epsilons:
    try:
        solution, iterations = seidel_method(A, b, eps)
        print(f"Точность: {eps}")
        print(f"Решение: {solution}")
        print(f"Количество итераций: {iterations}\n")
    except ValueError as e:
        print(f"При точности {eps} метод не сошелся: {e}")
