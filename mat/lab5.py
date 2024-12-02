import numpy as np

def power_method(A, tol, max_iterations=1000):
    """
    Находит наибольшее собственное значение и соответствующий собственный вектор методом степенной итерации.

    Параметры:
    A : ndarray
        Входная квадратная матрица.
    tol : float
        Точность (порог сходимости).
    max_iterations : int
        Максимальное количество итераций.

    Возвращает:
    eigenvalue : float
        Наибольшее собственное значение.
    eigenvector : ndarray
        Соответствующий собственный вектор.
    iterations : int
        Число выполненных итераций.
    """
    n = A.shape[0]
    x = np.random.rand(n)
    x = x / np.linalg.norm(x)

    for i in range(max_iterations):
        Ax = np.dot(A, x)
        new_eigenvalue = np.linalg.norm(Ax)
        new_x = Ax / new_eigenvalue

        if np.linalg.norm(new_x - x) < tol:
            return new_eigenvalue, new_x, i + 1

        x = new_x

    raise ValueError("Метод степенной итерации не сошелся за максимальное количество итераций.")


def print_results(method_name, iterations, eigenvalue, eigenvector, eps):
    """
    Функция для форматированного вывода результатов.
    """
    print(f"{'*' * 50}")
    print(f"               {method_name}               ")
    print(f"{'*' * 50}")
    print(f"Количество итераций: {iterations}")
    print("Собственный вектор:")
    for val in eigenvector:
        print(f"{val:.5f}")
    print(
        f"\nМаксимальное значение спектрального радиуса с точностью {eps} "
        f"методом {method_name.lower()} равно {eigenvalue:.5f}\n"
    )


# Определяем матрицу (вариант 16)
A = np.array([
    [4.48, 0.04, 0.14, 0.3, 1],
    [0.04, 3.43, 0.98, 0.6, 0.65],
    [0.14, 0.98, 2, 0.4, 0.41],
    [0.3, 0.6, 0.4, 3.43, 0.44],
    [1, 0.65, 0.41, 0.44, 4.48]
])

# Уровни точности
epsilons = [1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9]
results = []

for eps in epsilons:
    largest_eigenvalue, eigenvector, iterations = power_method(A, eps)
    results.append((eps, largest_eigenvalue, eigenvector, iterations))

# Вывод результатов для всех уровней точности
for eps, largest_eigenvalue, eigenvector, iterations in results:
    print_results("СТЕПЕННОЙ МЕТОД", iterations, largest_eigenvalue, eigenvector, eps)
