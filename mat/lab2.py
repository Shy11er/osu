import numpy as np

# Функция для красивого вывода матрицы
def print_matrix(matrix, name):
    print(f"\n{name}:")
    for row in matrix:
        print("  ".join(f"{elem:10.4f}" for elem in row))

# Решение системы методом Гаусса
def gaussian_elimination_with_matrix_output(A, b):
    n = len(b)
    A_extended = np.hstack([A, b.reshape(-1, 1)])  # объединение матрицы с вектором b
    for i in range(n):
        max_row = np.argmax(np.abs(A_extended[i:n, i])) + i
        if i != max_row:
            A_extended[[i, max_row]] = A_extended[[max_row, i]]
        for j in range(i+1, n):
            factor = A_extended[j, i] / A_extended[i, i]
            A_extended[j, i:] -= factor * A_extended[i, i:]
    
    # Обратный ход
    x = np.zeros(n)
    for i in range(n-1, -1, -1):
        x[i] = (A_extended[i, -1] - np.dot(A_extended[i, i+1:n], x[i+1:n])) / A_extended[i, i]
    
    return x, A_extended[:, :-1]  # Возвращаем решения и матрицу C

# Функция для расчета погрешности вручную
def calculate_manual_error(A, x, b):
    n = len(b)
    Ax = np.dot(A, x)
    error_vector = Ax - b
    error = 0
    for i in range(n):
        error += error_vector[i] ** 2
    return np.sqrt(error)

# Обратная матрица методом Гаусса
def gaussian_inverse(A):
    n = A.shape[0]
    A_inv_extended = np.hstack([A, np.eye(n)])
    for i in range(n):
        max_row = np.argmax(np.abs(A_inv_extended[i:, i])) + i
        if i != max_row:
            A_inv_extended[[i, max_row]] = A_inv_extended[[max_row, i]]
        A_inv_extended[i] /= A_inv_extended[i, i]
        for j in range(i+1, n):
            factor = A_inv_extended[j, i]
            A_inv_extended[j] -= factor * A_inv_extended[i]
    
    # Обратный ход
    for i in range(n-1, -1, -1):
        for j in range(i):
            A_inv_extended[j] -= A_inv_extended[j, i] * A_inv_extended[i]
    
    return A_inv_extended[:, n:]

# LU-разложение вручную
def lu_decomposition_manual(A):
    n = A.shape[0]
    L = np.eye(n)
    U = np.zeros((n, n))
    
    for i in range(n):
        for j in range(i, n):
            U[i, j] = A[i, j] - np.dot(L[i, :i], U[:i, j])
        for j in range(i+1, n):
            L[j, i] = (A[j, i] - np.dot(L[j, :i], U[:i, i])) / U[i, i]
    
    return L, U

# Основная часть программы
A = np.array([
    [3.2, 1.7, 1.5, 2],
    [3, 2, 3.2, 1],
    [2.4, 4.3, 2, 1.17],
    [4, 4, -5, -5]
])
b = np.array([1.08, 4.2, 4.696, 16.1])

# Решение методом Гаусса
x_gauss, C_matrix = gaussian_elimination_with_matrix_output(A.copy(), b.copy())

# Погрешность
error_gauss_manual = calculate_manual_error(A, x_gauss, b)

# Обратная матрица
A_inv_gauss = gaussian_inverse(A.copy())

# Произведение матриц A и обратной
A_mult_A_inv = np.dot(A, A_inv_gauss)

# Определитель методом Гаусса
det_gauss = np.prod(np.diag(C_matrix))

# LU-разложение
L, U = lu_decomposition_manual(A.copy())

# Вектор y
y = np.linalg.solve(L, b)

# Решение методом LU
x_lu = np.linalg.solve(U, y)

# Определитель методом LU
det_lu = np.prod(np.diag(U))

# Вывод всех результатов
print("1. Корни уравнения методом Гаусса:", x_gauss)
print_matrix(C_matrix, "2. Матрица C, полученная методом Гаусса")
print("\n3. Погрешность решения методом Гаусса:", error_gauss_manual)
print_matrix(C_matrix, "4. Матрица после прямого хода для нахождения обратной")
print_matrix(A_inv_gauss, "5. Обратная матрица, полученная методом Гаусса")
print_matrix(A_mult_A_inv, "6. Произведение исходной матрицы на обратную")
print("\n7. Определитель, вычисленный методом Гаусса:", det_gauss)
print_matrix(L, "8. L-матрица")
print_matrix(U, "8. U-матрица")
print("\n9. Вектор y:", y)
print("\n10. Ответы, полученные методом LU-разложения:", x_lu)
print("\n11. Определитель, вычисленный методом LU:", det_lu)
